// Title:   Parameterization.cpp
// Created: Thu Sep 25 14:58:23 2003
// Authors: Richard Keiser, Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003 Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#include <qstringlist.h>
#include <stdio.h>
#include <time.h>

#include "../../../../Core/Interfaces/src/FrameBufferInterface.h"
#include "../../../../Core/Interfaces/src/SurfelInterface.h"
#include "../../../../Core/Configuration/src/Configuration.h"
#include "../../../../Core/RendererManager/src/RendererManager.h"
#include "../../../../Core/CoreTools/src/CoreTools.h"
#include "../../../../Core/DataStructures/src/SurfelCollection.h"
#include "../../../../Core/DataStructures/src/NeighbourHood.h"
#include "../../../../Core/DataStructures/src/Cluster.h"
#include "../../../../Core/CoreResourceTools/src/CoreResourceTools.h"
#include "../../../../Core/Brush/src/Brush.h"
#include "../../../../Core/Scene/src/Scene.h"
#include "../../../../Core/Painter/src/Painter.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "../../../../Core/MarkerManager/src/Marker2D.h"
#include "../../../../Core/MarkerManager/src/Marker3D.h"
#include "../../../../Utilities/src/ImageOps.h"
#include "../../../../Utilities/src/Common.h"
#include "../../../../Utilities/UserMessages/src/StatusBar.h"
#include "Parameterization.h"
#include "SplatBuffer.h"

// #define PERFMEASURING

#ifdef PERFMEASURING
static FILE *_f; 
static clock_t _start, _finish; 
#endif

#ifdef PERFMEASURING 
#define INIT_PERFMEASURING(_name) \
_f = fopen(_name,"wb");
#else
#define INIT_PERFMEASURING
#endif

#ifdef PERFMEASURING
#define COMMENT_PERFMEASURING(_text,_nr) fprintf(_f, _text); fprintf(_f," %i\n",_nr);
#else
#define COMMENT_PERFMEASURING
#endif

#ifdef PERFMEASURING
#define START_PERFMEASURING _start = clock();
#else
#define START_PERFMEASURING
#endif

#ifdef PERFMEASURING
#define STOP_PERFMEASURING(_text) \
_finish = clock(); fprintf(_f, _text); fprintf(_f, " %3.3f seconds\n", (double)(_finish - _start) / CLOCKS_PER_SEC);
#else
#define STOP_PERFMEASURING
#endif

#ifdef PERFMEASURING
#define CLOSE_PERFMEASURING fclose(_f);
#else
#define CLOSE_PERFMEASURING
#endif


// **************
// public methods
// **************

Parameterization::Parameterization() {

	// set number of multigrid levels
	nofLevels           = 4;
	// set cluster size for generating multigrid levels
	clusterSize         = 3;

	precision           = 0.001f;
	coverageThreshold   = 0.3f;
	displacementScaling = 10.0f;
	lowPassFilter       = 0.f;
	fittingConstrWeights = 1.f;

	applyTexture           = true;
	applyTextureAlpha      = false;
	applyDisplacement      = false;
	applyDisplacementAlpha = false;

	levelSizes                = 0;
	nofFittingConstraints     = 0;
	multiGridLevels           = 0;
	positions                 = 0;
	normals                   = 0;
	selectedSurfels           = 0;
	uvCoordinates             = 0;                
	neighbourHoods            = 0;
	clusters                  = 0;
	fittingConstraintsU       = 0;
	fittingConstraintsV       = 0;
	fittingConstraintIndices  = 0;

	filterBrush = false;

	isMultiGridValid = false;

	// get informed whenever the selection has changed
	this->connect (CoreTools::getInstance()->getSelectionTool(), SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));

INIT_PERFMEASURING("g:\\test.txt");
}

Parameterization::~Parameterization() {
	this->clearMultiGrid();

CLOSE_PERFMEASURING;
}

void Parameterization::setPrecision (const float newPrecision) {
	precision = newPrecision;
}

float Parameterization::getPrecision() const {
	return precision;
}

void Parameterization::setFittingConstrWeights(const float newWeigths) {
    fittingConstrWeights = newWeigths;
}

float Parameterization::getFittingConstrWeights() const {
    return fittingConstrWeights;
}

void Parameterization::setLowPassFilter(const float newLowPassFilter) {
    lowPassFilter = newLowPassFilter;
}

float Parameterization::getLowPassFilter() const {
    return lowPassFilter;
}

void Parameterization::setCoverageThreshold(const float newCoverageThreshold) {
	coverageThreshold = newCoverageThreshold;
}

float Parameterization::getCoverageThreshold() const {
	return coverageThreshold;
}

void Parameterization::setFilterBrushEnabled(const bool enable) {
    filterBrush = enable;
}

bool Parameterization::isFilterBrushEnabled() {
    return filterBrush;
}

void Parameterization::setNofLevels (const uint newNofLevels) {

	if (nofLevels != newNofLevels) {
		this->clearMultiGrid();
		nofLevels = newNofLevels;
	};

}

uint Parameterization::getNofLevels() const {
	return nofLevels;
}

void Parameterization::setClusterSize(const uint newClusterSize) {
	
	if (clusterSize != newClusterSize) {
		this->clearMultiGrid();
		clusterSize = newClusterSize;
	}
	
}

uint Parameterization::getClusterSize() const {
	return clusterSize;
}


void Parameterization::setDisplacementScaling (const float scaling) {
	displacementScaling = scaling;
}

float Parameterization::getDisplacementScaling() const {
	return displacementScaling;
}

void Parameterization::generateUVCoordinates(bool resampleAtTextureResolutionFlag) {

	StatusBar *statusBar;
	uint i;
	
COMMENT_PERFMEASURING("Multigrid_levels",nofLevels);
COMMENT_PERFMEASURING("Clustersize_levels",clusterSize);

	if (CoreTools::getInstance()->getSelectionTool()->getSelectionSize() == 0) {
		// do nothing if no surfels are selected
		return;
	}

	statusBar = StatusBar::getInstance();

	statusBar->showMessage ("Initializing multi grid solver...", -1);
	this->initializeMultiGrid();

	// add fitting constraints to all levels
	this->initializeFittingConstraints();

	statusBar->showMessage ("Adding fitting constraints...", -1);

	for (i = 0; i < nofLevels; i++) {
		multiGridLevels[i]->addFittingConstraints (fittingConstraintsU[i], fittingConstraintsV[i], fittingConstraintIndices[i], nofFittingConstraints, fittingConstrWeights);
	}

	// solve levels, start at lowest level and propagate solution to higher levels
	statusBar->showMessage ("Solving equations...", -1);

START_PERFMEASURING;
	for (i = 0; i < nofLevels - 1; i++) {
		multiGridLevels[i]->setPrecision (precision * (float)levelSizes[i] / (float)levelSizes[nofLevels - 1]);
		multiGridLevels[i]->generateUVCoordinates();
		this->initSolutionFromLowerLevel (i + 1);
	}
	multiGridLevels[nofLevels-1]->setPrecision (precision);
	multiGridLevels[nofLevels-1]->generateUVCoordinates();
STOP_PERFMEASURING("Multigrid_solution");
	
	if (resampleAtTextureResolutionFlag == false) {
		statusBar->showMessage ("Assigning texture to selection...", -1);

		this->assignTexture ();
		for (i = 0; i < nofLevels; i++) {
			multiGridLevels[i]->removeFittingConstraints();
		}
	}
	else {
		// this also invalidates the multigrid levels since we deselect everything in the selection
		statusBar->showMessage ("Resampling selection...", -1);

		this->resampleAtTextureResolution();
	}

	// we have to force the emission of the signal
	// NOTE: if the surface is not resampled, do not emit the object modified signal. this signal indirectly
	// causes the selection tool to emit a selection changed signal, which in turn invalidates the multigrid
	// structure. however, we want to avoid this!
	if (resampleAtTextureResolutionFlag == true) {
		Scene::getInstance()->emitObjectModifiedSignal (true);
	}
	else {
		Scene::getInstance()->emitObjectModifiedSignal (false);
	}
	Scene::getInstance()->emitRenderFrameSignal ();

	statusBar->showMessage ("Parameterization done.");

}

// ***************
// private methods
// ***************

void Parameterization::initializeFittingConstraints() {

	const QList<Marker3D>   *markers3D;
	const QList<Marker2D>   *markers2D;
	Marker3D                *currentMarker3D;	
	Marker2D                *currentMarker2D;
	uint                    nofSelectedSurfels,
							index;
	bool                    indexFound;
	Vector3D                surfelPosition,
		                    position;

	uint                    i, l;
	
	SurfelInterface         *surfel;
	MarkerManager           *markerManager;

	markerManager = MarkerManager::getInstance();
	markers3D = markerManager->getFittingConstraints3D();
	markers2D = markerManager->getFittingConstraints2D();

	QListIterator<Marker3D> markers3DIterator (*markers3D);
	QListIterator<Marker2D> markers2DIterator (*markers2D);
	nofFittingConstraints = markerManager->getMinimumFittingConstraintListCount();

	// remove the old fittingConstraint coordinates and indices, if there are any
	if (fittingConstraintsU[nofLevels -1] != 0) {
		delete[] fittingConstraintsU[nofLevels -1];
	}

	if (fittingConstraintsV[nofLevels -1] != 0) {
		delete[] fittingConstraintsV[nofLevels -1];
	}

	if (fittingConstraintIndices[nofLevels - 1] != 0) {
		delete[] fittingConstraintIndices[nofLevels - 1];
	}

	fittingConstraintsU[nofLevels - 1]      = new float[nofFittingConstraints];
	fittingConstraintsV[nofLevels - 1]      = new float[nofFittingConstraints];
	fittingConstraintIndices[nofLevels - 1] = new  uint[nofFittingConstraints];
	
	nofSelectedSurfels = 0;
	for (i = 0; i < nofFittingConstraints; i++) {

		currentMarker3D = markers3DIterator.current();
		currentMarker2D = markers2DIterator.current();

		surfel = currentMarker3D->getSurfel();

		if (surfel->isFlagOn (SurfelInterface::SELECTED1) == true ) {

			// store all fitting constraints in an internal table
			fittingConstraintsU[nofLevels - 1][nofSelectedSurfels] = currentMarker2D->getNormalX();
			fittingConstraintsV[nofLevels - 1][nofSelectedSurfels] = currentMarker2D->getNormalY();
			surfelPosition = surfel->getPosition();
			// search the position in the 'positions' array, which yields the index
			indexFound = false;
			index      = 0;
			while (indexFound == false) {

				position = positions[nofLevels - 1][index];
				if (position == surfelPosition) {
					indexFound = true;
				}
				else {
					index++;
				}

			}

			fittingConstraintIndices[nofLevels - 1][nofSelectedSurfels] = index;
			nofSelectedSurfels++;

		}

		++markers3DIterator;
		++markers2DIterator;

	}

	// update the number of fitting constraints, just the constraints which denote
	// a selected surfel count
	nofFittingConstraints = nofSelectedSurfels;

	// propagate fitting constraints to lower levels
	for (l = nofLevels - 1; l > 0; l--) {

		// remove the old fittingConstraint coordinates and indices, if there are any
		if (fittingConstraintsU[l - 1] != 0) {
			delete[] fittingConstraintsU[l - 1];
		}

		if (fittingConstraintsV[l - 1] != 0) {
			delete[] fittingConstraintsV[l - 1];
		}

		if (fittingConstraintIndices[l - 1] != 0) {
			delete[] fittingConstraintIndices[l - 1];
		}
		fittingConstraintsU[l - 1]      = new float[nofFittingConstraints];
		fittingConstraintsV[l - 1]      = new float[nofFittingConstraints];
		fittingConstraintIndices[l - 1] = new  uint[nofFittingConstraints];

		for (i = 0; i < nofFittingConstraints; i++) {
			
			// copy U and V values
			fittingConstraintsU[l - 1][i] = fittingConstraintsU[l][i];
			fittingConstraintsV[l - 1][i] = fittingConstraintsV[l][i];
			// assign the constraint to the parent surfel of the higher level surfel
			index = clusters[l]->getParentPositionIndex (fittingConstraintIndices[l][i]);
			fittingConstraintIndices[l - 1][i] = index;

		}

	}
	
}

void Parameterization::assignTexture () {

	std::vector<SurfelInterface *> texturedSurfels;
	std::vector<MyDataTypes::TextureCoordinate> textureCoordinates;
	int numOfTexturedSurfels;
	Scene *scene;
	float             u,
		              v;
	uint              nofPositions,
		              i;

	Brush             *brush;
	MyDataTypes::TextureCoordinate texCoord;

	float *jacobians, tmp[4];
	Vector3D *localX, *localY;

	brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();

	if (brush == 0) {
		return;
	}

	int brushSizeX = brush->getSize().width();
	int brushSizeY = brush->getSize().height();

	scene = Scene::getInstance();

	Painter *painter = new Painter (brush);

	nofPositions = levelSizes[nofLevels - 1];

	textureCoordinates.resize (nofPositions);
	texturedSurfels.resize (nofPositions);

	if(filterBrush) {
    	    jacobians = new float[nofPositions*4];
    	    localX = new Vector3D[nofPositions];
    	    localY = new Vector3D[nofPositions];
	}

	numOfTexturedSurfels = 0;
	for (i = 0; i < nofPositions; i++) {

		u = uvCoordinates[nofLevels - 1][i];
		v = uvCoordinates[nofLevels - 1][i + nofPositions];

		texCoord.u = u;
		texCoord.v = v;

		if (u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f) {

			textureCoordinates[numOfTexturedSurfels].u = brushSizeX * u;
			textureCoordinates[numOfTexturedSurfels].v = brushSizeY * v;
			texturedSurfels[numOfTexturedSurfels] = selectedSurfels[i];		

			if(filterBrush) {
			    // computes J, localX, localY
			    computeJacobian (i, tmp, localX[i], localY[i]);
			    jacobians[numOfTexturedSurfels*4] = tmp[0];
			    jacobians[numOfTexturedSurfels*4+1] = tmp[1];
			    jacobians[numOfTexturedSurfels*4+2] = tmp[2];
			    jacobians[numOfTexturedSurfels*4+3] = tmp[3];
			}

			numOfTexturedSurfels++;	
		}
		scene->setSurfelTextureCoordinate (selectedSurfels[i], texCoord, false);
	}

	if(filterBrush) {
	    painter->paint (&texturedSurfels, numOfTexturedSurfels, &textureCoordinates,
		1.0f, 0, 2.0f, false, jacobians, localX, localY, useEllipticalSurfels, lowPassFilter);

	    delete []localX;
	    delete []localY;
	    delete []jacobians;
	} else {
	    painter->paint (&texturedSurfels, numOfTexturedSurfels, &textureCoordinates);
	}

	delete painter;
}

void Parameterization::resampleAtTextureResolution() {


	Brush *brush = CoreResourceTools::getInstance()->getBrushChooserTool()->getSelectedBrush();

	//SurfelCollection* originalSurfels = surfels[levels-1];
	SurfelInterface                *surfel;
	float                          J[4];
	Vector3D                       localX,
		                           localY;
	MyDataTypes::TextureCoordinate uv;

	SplatBuffer                    *splatBuffer;
	uint                           nofPositions,
		                           i;
	SurfelCollection               *activeSurfelCollection;
	SelectionTool                  *selectionTool;
	bool                           *deleteFlags;

	Vector3D		       axis0, axis1;
	float                          localKernel[4];

	float			       radius;

	if (brush == 0) {
		return;
	}

	activeSurfelCollection = Scene::getInstance()->getActiveObject()->getSurfelCollection();

	selectionTool = CoreTools::getInstance()->getSelectionTool();
	// temporarily disconnect, we still need the multigrid levels
	selectionTool->disconnect (this);

	// we have to deselect everything before we are going to add/remove surfels
	selectionTool->clearSelection (false);

	// get informed again whenever the selection has changed
	this->connect (selectionTool, SIGNAL (selectionChanged (SurfelInterface::Flag)),
		           this, SLOT (handleSelectionChanged (SurfelInterface::Flag)));


	// the 3D constraints will be invalidated, since the associated surfels will be removed!
	MarkerManager::getInstance()->removeAllFittingConstraints3D (true);
	
	// instantiate splat buffer
	splatBuffer = new SplatBuffer (brush->getSize(), coverageThreshold);

	// rasterize original surfels into splat buffer
	splatBuffer->clearBuffer();

	nofPositions = levelSizes[nofLevels - 1];

	// speciality: we use the alpha component of the position brush channel for 
	// modulating the low pass filter
	ChannelComponent *alphaFilter = 0;
	if (brush->hasAlphaChannel (SurfelInterface::POSITION)) {
		alphaFilter = brush->getAlphaChannel (SurfelInterface::POSITION)->getComponent(0);	
	}
	QSize brushSize = brush->getSize();
	
	// initialize the delete flags (which indicate the surfels to be deleted later on)
	deleteFlags = new bool[nofPositions];
	for (i = 0; i < nofPositions; i++) {
		deleteFlags[i] = false;
	}

	for (i = 0; i < nofPositions; i++) {

		surfel = selectedSurfels[i];
		
		// uv = surfel->getTextureCoordinate();
		uv.u = uvCoordinates[nofLevels - 1][i];
		uv.v = uvCoordinates[nofLevels - 1][i + nofPositions];

		// splat all surfels. clipping to texture domain is performed during rasterization
		// writes J, localX, localY
		computeJacobian (i, J, localX, localY);

		// Concatenate the mapping matrix from the local tangent space to the global
		// parameterization (i.e., the Jacobian J) with the matrix describing the 
		// ellipse in the local tangent space (which is defined by localX, localY).
		if (useEllipticalSurfels) {
			// get ellipse axes in object space coordinates
			axis0 = surfel->getAxisOne();
			axis1 = surfel->getAxisTwo();
			// express ellipse axes in local tangent coordinates
			localKernel[0] = Vector3D::dotProduct(axis0, localX);
			localKernel[1] = Vector3D::dotProduct(axis1, localX);
			localKernel[2] = Vector3D::dotProduct(axis0, localY);
			localKernel[3] = Vector3D::dotProduct(axis1, localY);
		}
		else {
			// circular surfels
			// [u,v]' = J * [[radius 0]' [0 radius]'] * [x,y]'
			radius = surfel->getRadius();
			localKernel[0] = radius;
			localKernel[1] = 0.f;
			localKernel[2] = 0.f;
			localKernel[3] = radius;
		}

		// modulate the low pass filter using an alpha channel of the brush
		int tex_x = (int)(uv.u * (brushSize.width()-1));
		int tex_y = (int)(uv.v * (brushSize.height()-1));
		if(tex_x < 0) tex_x = 0;
		if(tex_x >= brushSize.width()) tex_x = brushSize.width()-1;
		if(tex_y < 0) tex_y = 0;
		if(tex_y >= brushSize.height()) tex_y = brushSize.height()-1;

		float tmp;
		if(alphaFilter) {
			tmp = alphaFilter->getValue (tex_x, tex_y);
		} else {
			tmp = 1.f;
		}

		// reads (uses) J, localX, localY
		splatBuffer->rasterizeSurfel (surfel, uv, J, localKernel, localX, localY, tmp * lowPassFilter);

		// only delete splats that are inside the parameter domain of the texture
		if (uv.u >= 0.0f && uv.u <= 1.0f && uv.v >=0.0f && uv.v <= 1.0f)
			deleteFlags[i] = true;

	}

	// generate new surfels from splat buffer
	
	if (splatBuffer->addTextureToActiveObject (brush, displacementScaling, useEllipticalSurfels)) {
		
		// now delete the old surfels according to the 'deleteFlags'
		for (i = 0; i < nofPositions; i++) {
			
			if (deleteFlags[i] == true) {
				activeSurfelCollection->removeSurfel (selectedSurfels[i], false);
			}			
		}
		Scene::getInstance()->emitObjectModifiedSignal ();
	}

	delete[] deleteFlags;
	delete splatBuffer;

	// since we have deselected everything, we must invalidate the multigrid levels as well
	this->clearMultiGrid();

}

bool Parameterization::computeJacobian (int positionIndex, float J[4], Vector3D& X, Vector3D& Y) {

	NeighbourHood                  *neighbourHood = neighbourHoods[nofLevels - 1];
	Vector3D                       p,
		                           p0,
					               p0_p,
					               n;
	float                          x,
		                           y;
	uint                           indexS;
	float                          uv[2];
	MyDataTypes::TextureCoordinate uv0;
	float                          invG[4],
		                           G[4],
					               c[4];
	float                          _det;
	int                            i;
	
	int                            nofNeighbours = 7;

	p0 = positions[nofLevels - 1][positionIndex];
	uv0.u = uvCoordinates[nofLevels - 1][positionIndex];
	uv0.v = uvCoordinates[nofLevels - 1][positionIndex + levelSizes[nofLevels - 1]];

	// neighbourHood->computeNearestPoints (p0, nNeighbors);
	neighbourHood->setNofNeighbours (nofNeighbours);
	neighbourHood->setSourcePoint (p0);
	
	// initialize least squares matrix
	for (i = 0; i < 4; i++) {
		G[i] = 0.0f;
	}

	for (i = 0; i < 4; i++) {
		c[i] = 0.0f;
	}

	// compute local coordinate system (i.e. basis vectors X,Y) on the surfel tangent plane
	indexS = neighbourHood->getNeighbourPositionIndex (nofNeighbours - 1);

	// p = s->getPosition();
	p = positions[nofLevels - 1][indexS];
	p0_p = p - p0;
	// n = surfel->getNormal();
	n = normals[nofLevels - 1][indexS];
	X = Vector3D::crossProduct(p0_p, n);
	X.normalize();
	Y = Vector3D::crossProduct(n, X);
	Y.normalize();

	// add the constraints for each neighbor to the least squares matrix
	for (i = 1; i < nofNeighbours; i++) {

		indexS = neighbourHood->getNeighbourPositionIndex (i);

		p = positions[nofLevels - 1][indexS];
		p0_p = p - p0;

		// compute local coordinates of the neighbor position
		x = Vector3D::dotProduct(p0_p, X);
		y = Vector3D::dotProduct(p0_p, Y);

		G[0] += x*x;
		G[1] += x*y;
		G[2] += y*x;
		G[3] += y*y;

		// s->getTextureCoords(uv);
		uv[0] = uvCoordinates[nofLevels - 1][indexS];
		uv[1] = uvCoordinates[nofLevels - 1][indexS + levelSizes[nofLevels - 1]];
		c[0] += x * (uv[0]-uv0.u);
		c[1] += y * (uv[0]-uv0.u);
		c[2] += x * (uv[1]-uv0.v);
		c[3] += y * (uv[1]-uv0.v);
	}

	// solve least squares matrix
	_det = 1.f / (G[0]*G[3] - G[1]*G[2]);
	invG[0] = G[3] * _det;
	invG[1] = -G[1] * _det;
	invG[2] = -G[2] * _det;
	invG[3] = G[0] * _det;

	// the Jacobian represents the mapping from local coordinates on the surfel
	// tangent plane to the global parameterization
	J[0] = invG[0] * c[0] + invG[1] * c[1]; // du_dx
	J[1] = invG[2] * c[0] + invG[3] * c[1]; // du_dy
	J[2] = invG[0] * c[2] + invG[1] * c[3]; // dv_dx
	J[3] = invG[2] * c[2] + invG[3] * c[3]; // dv_dy

	return true;
}

bool Parameterization::computeJacobianUnstable(SurfelInterface* surfel, float J[4], Vector3D& X, Vector3D& Y) {
	
	NeighbourHood *neighbourHood = neighbourHoods[nofLevels - 1];

	SurfelInterface *S;
	uint            index1,
		            index2;
	Vector3D P[3];						// the respective 3D positions
	Vector3D P0_P1, P0_P2, nP0_P2;		// vectors joining the points
	
	float x[3], y[3];					// local frame (i.e., X,Y) coordinates of the points
	float du_dx, du_dy, dv_dx, dv_dy;	// partial derivatives

	int i;
	int nofNeighbours = 7;

	float cosAngle;								// angle spanned by the three surfels defining the Jacobian
	float cos_3Pi_4 = (float)cos(3.f*M_PI/4.f);	// thresholds for choosing the three surfels defining the Jacobian
	float cos_Pi_8 = (float)cos(M_PI/8.f);

	Vector3D debugP[7];
	Vector3D debugP0_P[7];
	(void)debugP;
	(void)debugP0_P;

	MyDataTypes::TextureCoordinate uv0, uv1, uv2;

	x[0] = 0.f;
	y[0] = 0.f;

	// get 3 surfels defining the Jacobian
	// NOTE: make sure that the 3 surfels properly span a plane, otherwise the Jacobian
	// is not well defined
	S = surfel;
	P[0] = S->getPosition();

	neighbourHood->setNofNeighbours (nofNeighbours);
	neighbourHood->setSourcePoint (P[0]);

	index1 = neighbourHood->getNeighbourPositionIndex(1);

	P[1] = positions[nofLevels - 1][index1];

	P0_P1 = P[1] - P[0];
	X = P0_P1;
	x[1] = X.normalize();

	// find second neighbor, make sure that the 3 points span a plane
	for (i = 2; i < nofNeighbours; i++) {

		index2 = neighbourHood->getNeighbourPositionIndex(i);

		P[2] = positions[nofLevels - 1][index2];

		P0_P2 = P[2] - P[0];
		nP0_P2 = P0_P2;
		nP0_P2.normalize();
		cosAngle = Vector3D::dotProduct(X, nP0_P2);
		if(cosAngle > cos_3Pi_4 && cosAngle < cos_Pi_8) {
			break;
		}

	}
	if(cosAngle <= cos_3Pi_4 || cosAngle >= cos_Pi_8) {

		return false;
	}

	// compute local frame with basis X,Y and local coordinates of P[0],P[1],P[2]
	y[1] = 0.f;
	Y = Vector3D::crossProduct(S->getNormal(), X);
	x[2] = Vector3D::dotProduct(X, P0_P2);
	y[2] = Vector3D::dotProduct(Y, P0_P2);

	// alternative way to compute the Jacobian
	float det  = x[1]*y[2] - y[1]*x[2];
	float inv[4] = {y[2]/det, -y[1]/det, -x[2]/det, x[1]/det};
	
	uv0 = S->getTextureCoordinate();

	uv1.u = uvCoordinates[nofLevels - 1][index1];
	uv1.v = uvCoordinates[nofLevels - 1][index1 + levelSizes[nofLevels - 1]];

	uv2.u = uvCoordinates[nofLevels - 1][index2];
	uv2.v = uvCoordinates[nofLevels - 1][index2 + levelSizes[nofLevels - 1]];

	du_dx = inv[0] * (uv1.u - uv0.u) + inv[1] * (uv2.u - uv0.u);
	du_dy = inv[2] * (uv1.u - uv0.u) + inv[3] * (uv2.u - uv0.u);
	dv_dx = inv[0] * (uv1.v - uv0.v) + inv[1] * (uv2.v - uv0.v);
	dv_dy = inv[2] * (uv1.v - uv0.v) + inv[3] * (uv2.v - uv0.v);

	J[0] = du_dx;
	J[1] = du_dy;
	J[2] = dv_dx;
	J[3] = dv_dy;
	
	return true;

}

void Parameterization::initSolutionFromLowerLevel (const uint levelIndex) {

	uint                           parentIndex;
	uint                           nofPositions,
		                           nofParentPositions,
		                           i;

	// propagate solution from parent surfels to their children
	
	nofPositions       = levelSizes[levelIndex];
	nofParentPositions = levelSizes[levelIndex - 1];
	for (i = 0; i < nofPositions; i++) {

		parentIndex = clusters[levelIndex]->getParentPositionIndex (i);
		uvCoordinates[levelIndex][i]                = uvCoordinates[levelIndex - 1][parentIndex];
		uvCoordinates[levelIndex][i + nofPositions] = uvCoordinates[levelIndex - 1][parentIndex + nofParentPositions];
		
	}

}

void Parameterization::initializeMultiGrid() {

	uint                         i,j,
		                         baseLevel,
		                         nofSelectedSurfels,
					             nofClusters;

	std::vector<SurfelInterface*> *selection;
	SurfelInterface              *surfel;

	if (isMultiGridValid == true) {
		return;
	}
	else {
		this->clearMultiGrid();
	}

	levelSizes                = new uint[nofLevels];                      
	multiGridLevels           = new MultiGridLevel*[nofLevels];
	positions                 = new Vector3D*[nofLevels];
	normals                   = new Vector3D*[nofLevels];
	uvCoordinates             = new float*[nofLevels * 2];  // 2 floats per uv-coordinate                
	neighbourHoods            = new NeighbourHood*[nofLevels];
	clusters                  = new Cluster*[nofLevels];
	fittingConstraintsU       = new float*[nofLevels];
	fittingConstraintsV       = new float*[nofLevels];
	fittingConstraintIndices  = new uint*[nofLevels];


	// find out whether we are working with elliptical surfels. note that we cannot 
	// find out from the selection, since the selection is a std::vector<SurfelInterface*> of surfels, 
	// not a surfel collection. so we check the type of the surfels in the active object.
	Scene *scene = Scene::getInstance();
	SurfelCollection *objectSurfels = scene->getActiveObject()->getSurfelCollection();

	if (objectSurfels->getPropertyDescriptor() & SurfelInterface::TANGENT_AXES) {
	    useEllipticalSurfels = true;
	} else {
	    useEllipticalSurfels = false;
	}

	// *************************
	// initialize the base level
	// *************************

	baseLevel = nofLevels - 1;

	// initialise the "pyramid base" level with the positions and normals of the selected surfels,
	// the multiGridLevel, the neighbourHood and the uvCoordinates
	// (the fitting constraints are allocated and initialized later on in this algorithm)
	
	selection = CoreTools::getInstance()->getSelectionTool()->getSelection();

	nofSelectedSurfels = selection->size();
	levelSizes[baseLevel] = nofSelectedSurfels;
	
	// positions and normals from selected surfels
	positions[baseLevel] = new Vector3D[nofSelectedSurfels];
	normals[baseLevel]   = new Vector3D[nofSelectedSurfels];
	selectedSurfels      = new SurfelInterface*[nofSelectedSurfels];

	for (i = 0 ; i < selection->size(); i++) {
		
		surfel = selection->at(i);
		positions[baseLevel][i] = surfel->getPosition();
		normals[baseLevel][i]   = surfel->getNormal();
		selectedSurfels[i]      = surfel;
	
	}

	neighbourHoods[baseLevel]  = new NeighbourHood (positions[baseLevel], nofSelectedSurfels);

	uvCoordinates[baseLevel]   = new float[2 * nofSelectedSurfels];
	for(i = 0; i < 2*nofSelectedSurfels; i++) uvCoordinates[baseLevel][i] = 0.f;
START_PERFMEASURING;
	multiGridLevels[baseLevel] = new MultiGridLevel (neighbourHoods[baseLevel], normals[baseLevel], uvCoordinates[baseLevel]);
	
	// these are allocated and initialized later on in the algorithm
	fittingConstraintsU[baseLevel]      = 0;
	fittingConstraintsV[baseLevel]      = 0;
	fittingConstraintIndices[baseLevel] = 0;

	// ***************************
	// initialize the upper levels
	// ***************************

	i = baseLevel;
	while (i > 0) {
		i--;
		
		// cluster surfels to generate surfels of new level
		// NOTE: the clustering algorithm generates indices for the new surfels, too
		clusters[i + 1] = new Cluster (neighbourHoods[i + 1], normals[i + 1], clusterSize);		
		
		// initialize positions and normals - note again that these arrays are deleted by
		// the cluster itself
		clusters[i + 1]->calculate (&(positions[i]), &(normals[i]), &nofClusters);
		levelSizes[i] = nofClusters;

		neighbourHoods[i]  = new NeighbourHood (positions[i], nofClusters);
		uvCoordinates[i]   = new float[2 * nofClusters];
		for(j = 0; j < 2*nofClusters; j++) uvCoordinates[i][j] = 0.f;
		multiGridLevels[i] = new MultiGridLevel (neighbourHoods[i], normals[i], uvCoordinates[i]);

		// these are allocated and initialized later on in the algorithm
		fittingConstraintsU[i]      = 0;
		fittingConstraintsV[i]      = 0;
		fittingConstraintIndices[i] = 0;
	}
STOP_PERFMEASURING("Multigrid_initialization");

	// we do not need a cluster for the lowest level
	clusters[0] = 0;

	isMultiGridValid = true;

}

void Parameterization::clearMultiGrid() {

	uint baseLevel,
		 i;

	// **************************************
	// delete all entries in the upper levels
	// **************************************

	baseLevel = nofLevels - 1;

	for (i = 0; i < nofLevels - 1; i++) {

		if (multiGridLevels != 0 && multiGridLevels[i] != 0) {
			delete multiGridLevels[i];
			multiGridLevels[i] = 0;
		}

		// the normals and positions of levels 0 .. nofLevels - 2
		// are deleted by the corresponding clusters!

		if (uvCoordinates != 0 && uvCoordinates[i] != 0) {
			delete[] uvCoordinates[i];
			uvCoordinates[i] = 0;
		}

		if (neighbourHoods != 0 && neighbourHoods[i] != 0) {
			delete neighbourHoods[i];
			neighbourHoods[i] = 0;
		}

		if (clusters != 0 && clusters[i] != 0) {
			delete clusters[i];
			clusters[i] = 0;
		}

		if (fittingConstraintsU != 0 && fittingConstraintsU[i] != 0) {
			delete[] fittingConstraintsU[i];
			fittingConstraintsU[i] = 0;
		}

		if (fittingConstraintsV != 0 && fittingConstraintsV[i] != 0) {
			delete[] fittingConstraintsV[i];
			fittingConstraintsV[i] = 0;
		}

		if (fittingConstraintIndices != 0 && fittingConstraintIndices[i] != 0) {
			delete[] fittingConstraintIndices[i];
			fittingConstraintIndices[i] = 0;
		}

	}

	// ****************************
	// clear the base level entries
	// ****************************

	if (multiGridLevels != 0 && multiGridLevels[baseLevel] != 0) {
		delete multiGridLevels[baseLevel];
		multiGridLevels[baseLevel] = 0;
	}

	// we have to delete the base level positions and normals ourselves
	if (positions != 0 && positions[baseLevel] != 0) {
		delete[] positions[baseLevel];
		positions[baseLevel] = 0;
	}

	if (normals != 0 && normals[baseLevel] != 0) {
		delete[] normals[baseLevel];
		normals[baseLevel] = 0;
	}

	if (uvCoordinates != 0 && uvCoordinates[baseLevel] != 0) {
		delete[] uvCoordinates[baseLevel];
		uvCoordinates[baseLevel] = 0;
	}

	if (neighbourHoods != 0 && neighbourHoods[baseLevel] != 0) {
		delete neighbourHoods[baseLevel];
		neighbourHoods[baseLevel] = 0;
	}

	if (clusters != 0 && clusters[baseLevel] != 0) {
		delete clusters[baseLevel];
		clusters[baseLevel] = 0;
	}

	if (fittingConstraintsU != 0 && fittingConstraintsU[baseLevel] != 0) {
		delete[] fittingConstraintsU[baseLevel];
		fittingConstraintsU[baseLevel] = 0;
	}

	if (fittingConstraintsV != 0 && fittingConstraintsV[baseLevel] != 0) {
		delete[] fittingConstraintsV[baseLevel];
		fittingConstraintsV[baseLevel] = 0;
	}

	if (fittingConstraintIndices != 0 && fittingConstraintIndices[baseLevel] != 0) {
		delete[] fittingConstraintIndices[baseLevel];
		fittingConstraintIndices[baseLevel] = 0;
	}

	// ************************
	// delete each level itself
	// ************************

	if (levelSizes != 0) {
		delete[] levelSizes;
		levelSizes = 0;
	}

	if (multiGridLevels != 0) {
		delete[] multiGridLevels;
		multiGridLevels = 0;
	}

	if (positions != 0) {
		delete[] positions;
		positions = 0;
	}

	if (normals != 0) {
		delete[] normals;
		normals = 0;
	}

	if (selectedSurfels != 0) {
		delete[] selectedSurfels;
		selectedSurfels = 0;
	}

	if (uvCoordinates != 0) {
		delete[] uvCoordinates;
		uvCoordinates = 0;
	}

	if (neighbourHoods != 0) {
		delete[] neighbourHoods;
		neighbourHoods = 0;
	}

	if (clusters != 0) {
		delete[] clusters;
		clusters = 0;
	}

	if (fittingConstraintsU != 0) {
		delete[] fittingConstraintsU;
		fittingConstraintsU = 0;
	}

	if (fittingConstraintsV != 0) {
		delete[] fittingConstraintsV;
		fittingConstraintsV = 0;
	}

	if (fittingConstraintIndices != 0) {
		delete[] fittingConstraintIndices;
		fittingConstraintIndices = 0;
	}

	isMultiGridValid = false;
	
}

// *************
// private slots
// *************

void Parameterization::handleSelectionChanged (SurfelInterface::Flag selectionFlag) {
	
	if (selectionFlag == SurfelInterface::SELECTED1) {
		this->clearMultiGrid();
	}
}







// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
