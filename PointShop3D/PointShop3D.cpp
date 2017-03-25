///////////////////////////////////////////////////////////////////////////////
// Written by Seung-Tak Noh ( seungtak.noh [at] gmail.com )
// wrapping class for parameterization part in PointShop3D
///////////////////////////////////////////////////////////////////////////////
#include "PointShop3D.h"


///////////////////////////////////////////////////////////////////////////////
// cons
///////////////////////////////////////////////////////////////////////////////
PointShop3D::PointShop3D()
{
	parameterization = new Parameterization();
}

PointShop3D::~PointShop3D()
{
	if (NULL != parameterization) delete parameterization;
}


///////////////////////////////////////////////////////////////////////////////
// public methods
///////////////////////////////////////////////////////////////////////////////
void PointShop3D::SetPointCloud
(const std::vector<glm::vec3> P, const std::vector<glm::vec3> N)
{
	// check size error
	if (P.size() != N.size()) {
		fprintf(stderr, "ERROR: point cloud and normal size does not match.\n");
		return ;
	}

	// check point cloud size to warn the speed issue ************************* [TODO]

	// copy data
	parameterization->targetCloudPoints = std::vector<glm::vec3>(P);
	parameterization->targetCloudNormals = std::vector<glm::vec3>(N);
}

void PointShop3D::SetConstraints
(const std::vector<int> indices, const std::vector<glm::vec2> tex2D)
{
	// check size error
	if (indices.size() != tex2D.size()) {
		fprintf(stderr, "ERROR: constraint size does not match.\n");
		return ;
	}

	// check size error
	int sz = parameterization->targetCloudPoints.size();
	for (const int i : indices)
	{
		if ( sz < i ) {
			fprintf(stderr, "ERROR: 2D marker information has invalid index.\n");
			return ;
		}
	}

	// copy data
	parameterization->markers3Dindex = std::vector<int>(indices);
	parameterization->markers2D = std::vector<glm::vec2>(tex2D);
}

void PointShop3D::Parameterize()
{
	parameterization->generateUVCoordinates(false);
}

std::vector<glm::vec3> PointShop3D::GetTexCoord()
{
	// get UV & reserve size
	std::vector<glm::vec2> uv = parameterization->finalUV;
	std::vector<glm::vec3> TC(uv.size());

	// convert UV data to RGB
	for (int i = 0; i < uv.size(); i++) {
		float u = uv[i].x;
		float v = uv[i].y;
		glm::vec2 uv = glm::vec2(u, v);
		float b = 0.0f;

		// default filter: out of [0:1] - add BLUE channel
		const float out_of_bound = 1.0f;
		if (u < 0.0f || 1.0f < u || v < 0.0f || 1.0f < v) b = out_of_bound;

		// insert UV in RG channel
		TC[i].r = u;
		TC[i].g = v;
		TC[i].b = b;
	}

	return TC;
}
