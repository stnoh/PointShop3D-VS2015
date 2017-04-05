#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>


void ReadParamInfo(const char* paramTxt)
{
	// read each line of landmark information text file
	std::ifstream iFile(paramTxt);
	std::string line;
	while (std::getline(iFile, line)) {

		std::stringstream strstr(line);
		std::istream_iterator<std::string> it(strstr);
		std::istream_iterator<std::string> end;
		std::vector<std::string> results(it, end);

		// format: {vertex index} {u} {v} {b}
		int count = 0;
		int p_idx = std::stoi(results[count++]);
		float u = std::stof(results[count++]);
		float v = std::stof(results[count++]);
		float b = std::stof(results[count++]);

		// show as text
		if (0.0f == b) printf("vertex #%d: (%f, %f)\n", p_idx, u, v);
	}
	iFile.close();
}

int main(int argc, char** argv)
{
	// arguments for program execution
	const char *exeFile = "PointShop3D_bin.exe";
	const char *inPly = "../data/eye_left_camera.ply";
	const char *inTxt = "../data/eye_left_landmark.txt";
	const char *outTxt = "../data/eye_left_param.txt";
	const char *outPly = "../data/eye_left_param.ply";

	// to C-style single string
	char argument[2048];
	sprintf_s(argument, "%s %s %s %s %s", exeFile, inPly, inTxt, outTxt, outPly);

	// to Windows 64bit string
	wchar_t wargument[2048];
	mbstowcs(wargument, argument, strlen(argument) + 1);
	LPWSTR ptr_argv = wargument;

	wchar_t wexeFile[2048];
	mbstowcs(wexeFile, exeFile, strlen(exeFile) + 1);
	LPWSTR ptr_exec = wexeFile;

	// execute external binary EXE with arguments
	STARTUPINFO StartupInfo = { 0 };
	PROCESS_INFORMATION ProcessInfo;
	StartupInfo.cb = sizeof(STARTUPINFO);
	BOOL test = CreateProcess(ptr_exec, ptr_argv, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
	if (!test) {
		fprintf(stderr, "ERROR:\n");
	}
	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

	// read executed result:
	ReadParamInfo(outTxt);

	return 0;
}
