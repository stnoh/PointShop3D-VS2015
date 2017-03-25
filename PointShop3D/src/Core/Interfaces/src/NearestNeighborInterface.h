#ifndef _NEAREST_NEIGHBOR_INTERFACE_H_
#define _NEAREST_NEIGHBOR_INTERFACE_H_

#include "../../../DataTypes/src/Vector3D.h"
#include "../../src/CoreDLL.h"

//------------------------------------------------------------------------------------
class NearestNeighborInterface
//------------------------------------------------------------------------------------
{
public: 

	CORE_API virtual void setNofNeighbors(const int nofNeighbors) = 0;

    CORE_API virtual void queryNeighbors(const Vector3D &pos, const float maxSqrDist) = 0;

	CORE_API virtual Vector3D getNeighborPosition(unsigned int index) = 0;
    
    CORE_API virtual float getSquaredDistance(unsigned int index) = 0;

    CORE_API virtual unsigned int getNofFoundNeighbors() = 0;
	CORE_API virtual unsigned int getNofNeighbors() = 0;

};
#endif