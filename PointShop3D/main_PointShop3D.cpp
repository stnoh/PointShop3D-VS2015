///////////////////////////////////////////////////////////////////////////////
// written by Seung-Tak Noh ( seungtak.noh [at] gmail.com )
// 
// This code is the simple tester of parameterization part in PointShop3D.
// https://graphics.ethz.ch/pointshop3d/index.html
///////////////////////////////////////////////////////////////////////////////
#include "src/ToolBars/StandardToolBar/ParameterizationTool/src/Parameterization.h"
Parameterization *parameterization;

int main(int argc, char** argv)
{
	parameterization = new Parameterization();

	delete parameterization;

    return 0;
}
