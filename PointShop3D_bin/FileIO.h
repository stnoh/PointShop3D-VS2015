///////////////////////////////////////////////////////////////////////////////
// written by Seung-Tak Noh ( seungtak.noh [at] gmail.com )
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <fstream>
#include <vector>

#include <glm/glm.hpp>

// it just supports PLY format only ******************************************* [TODO] support other extensions
namespace cgvl {

// write ply file
void WritePointCloudPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::vec3> N, std::vector<glm::u8vec3> C);
void WritePointCloudPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::vec3> N);
void WritePointCloudPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::u8vec3> C);
void WritePointCloudPly(const char* filepath, std::vector<glm::vec3> P);

void WriteTriMeshPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::vec3> N, std::vector<glm::u8vec3> C, std::vector<glm::uint> F);
void WriteTriMeshPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::vec3> N, std::vector<glm::uint> F);
void WriteTriMeshPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::u8vec3> C, std::vector<glm::uint> F);
void WriteTriMeshPly(const char* filepath, std::vector<glm::vec3> P, std::vector<glm::uint> F);

// read ply file ************************************************************** [TODO] check the header reading, etc.
void ReadPointCloudPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::vec3>& N, std::vector<glm::u8vec3>& C);
void ReadPointCloudPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::vec3>& N);
void ReadPointCloudPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::u8vec3>& C);
void ReadPointCloudPly(const char* filepath, std::vector<glm::vec3>& P);

void ReadTriMeshPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::vec3>& N, std::vector<glm::u8vec3>& C, std::vector<glm::uint>& F);
void ReadTriMeshPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::vec3>& N, std::vector<glm::uint>& F);
void ReadTriMeshPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::u8vec3>& C, std::vector<glm::uint>& F);
void ReadTriMeshPly(const char* filepath, std::vector<glm::vec3>& P, std::vector<glm::uint>& F);

}
