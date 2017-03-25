///////////////////////////////////////////////////////////////////////////////
// written by Seung-Tak Noh ( seungtak.noh [at] gmail.com )
// 
// This generates the binary module of parameterization part in PointShop3D
// https://graphics.ethz.ch/pointshop3d/index.html
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

#include "FileIO.h"

#include "PointShop3D.h"
#pragma comment(lib, "PointShop3D")

void ReadLandmarkInfo(const char* landmarkTxt,
	std::vector<int>& indices, std::vector<glm::vec2>& TC)
{
	std::vector<int> _indices;
	std::vector<glm::vec2> _TC;

	// read each line of landmark information text file
	std::ifstream iFile(landmarkTxt);
	std::string line;
	while (std::getline(iFile, line)) {

		std::stringstream strstr(line);
		std::istream_iterator<std::string> it(strstr);
		std::istream_iterator<std::string> end;
		std::vector<std::string> results(it, end);

		// format: {vertex index} {tex U} {tex V}
		int count = 0;
		int p_idx = std::stoi(results[count++]);
		float u = std::stof(results[count++]);
		float v = std::stof(results[count++]);

		// insert to data structure
		_indices.push_back(p_idx);
		_TC.push_back(glm::vec2(u, v));

		// show as text
		printf("landmark - vertex #%d: (%f, %f)\n", p_idx, u, v);
	}
	iFile.close();

	// assign imported data to container
	indices = _indices;
	TC = _TC;
}

void WriteParameterizedInfo(const char* paramTxt,
	std::vector<glm::vec3>& TC)
{
	std::ofstream oFile(paramTxt);

	for (int i = 0; i < TC.size(); i++) {
		glm::vec3 tex2D = TC[i];

//		if (1.0f == tex2D.z) continue;

		std::stringstream ss;
		char line[1024];
//		std::snprintf(line, 1024, "%d %f %f", i, tex2D.x, tex2D.y);
		std::snprintf(line, 1024, "%d %f %f %f", i, tex2D.x, tex2D.y, tex2D.z);

		oFile << line << std::endl;
	}

	oFile.close();
}

void WriteParameterizedMesh(const char* meshPly, std::vector<glm::vec3> P,
	std::vector<glm::vec3> N, std::vector<glm::vec3> T, std::vector<uint> F)
{
	std::vector<glm::u8vec3> C_new;
	for (int i = 0; i < T.size(); i++) {
		glm::vec3 uvb = T[i];

		// check boundary & clamping RG-channel
		if (0.0f < uvb.b)
		{
			if (uvb.r < 0.0f) uvb.r = 0.0f;
			if (uvb.r > 1.0f) uvb.r = 1.0f;
			if (uvb.g < 0.0f) uvb.g = 0.0f;
			if (uvb.g > 1.0f) uvb.g = 1.0f;
		}

		glm::u8vec3 uvb_u8;
		uvb_u8.r = (uint)(255.0f * uvb.r);
		uvb_u8.g = (uint)(255.0f * uvb.g);
		uvb_u8.b = (uint)(255.0f * uvb.b);
		C_new.push_back(uvb_u8);
	}

	cgvl::WriteTriMeshPly(meshPly, P, N, C_new, F);
}


///////////////////////////////////////////////////////////////////////////////
// entry point
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// check the number of arguments
	if (4 > argc) {
		fprintf(stderr, "not enough argument: PointShop3D_bin.exe {pointset.ply} {uv_constraints.txt} {output_uv.txt}\n");
		return 1;
	}

	// read mesh ply file from command line argument (1)
	std::vector<glm::vec3> P, N;
	std::vector<glm::u8vec3> C;
	std::vector<uint> F;
	cgvl::ReadTriMeshPly(argv[1], P, N, C, F);

	// read text uv constraints file from command line argument (2)
	std::vector<int> indices;
	std::vector<glm::vec2> uv_consts;
	ReadLandmarkInfo(argv[2], indices, uv_consts);

	// parameterize
	PointShop3D ps3D;
	ps3D.SetPointCloud(P, N);
	ps3D.SetConstraints(indices, uv_consts);
	ps3D.Parameterize();

	std::vector<glm::vec3> texCoord = ps3D.GetTexCoord();

	// export result to text file from command line argument (3)
	WriteParameterizedInfo(argv[3], texCoord);

	// [TEST] simple conversion & export to ply file
	if (5 == argc) WriteParameterizedMesh(argv[4], P, N, texCoord, F);

    return 0;
}
