///////////////////////////////////////////////////////////////////////////////
// written by Seung-Tak Noh ( seungtak.noh [at] gmail.com )
// wrapping class for parameterization part in PointShop3D
///////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef POINTSHOP_3D
#define POINTSHOP_3D

#include <glm/glm.hpp>
#include <vector>

#include "src/ToolBars/StandardToolBar/ParameterizationTool/src/Parameterization.h"

class PointShop3D
{
public:
	PointShop3D();
	virtual ~PointShop3D();

	void SetPointCloud(const std::vector<glm::vec3> P, const std::vector<glm::vec3> N);
	void SetConstraints(const std::vector<int> indices, const std::vector<glm::vec2> tex2D);
	void Parameterize();

	std::vector<glm::vec3> GetTexCoord();

private:
	Parameterization *parameterization = NULL;
};

#endif
