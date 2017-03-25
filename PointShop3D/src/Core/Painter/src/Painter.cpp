// Title:   Painter.cpp
// Created: Thu Sep 25 14:15:11 2003
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

#include "../../Scene/src/Scene.h"
#include "../../../Utilities/src/Common.h"
#include "../../../Utilities/src/MyMathTools.h"
#include "../../DataStructures/src/SurfelCollection.h"

#include "Painter.h"




Painter::Painter (const Brush *brush) {

	scene = Scene::getInstance();
	filterBlendingGrid = 0;
	this->setBrush (brush);

	cutoffRadius = 2.0f;
}


Painter::~Painter() {

	if (filterBlendingGrid) {
		delete[] filterBlendingGrid;
	}
	
}

bool Painter::paint (std::vector<SurfelInterface*>* surfels,
					 int numSurfels,
					 const std::vector<MyDataTypes::TextureCoordinate>* texCoords,
					 const float scale,
					 const Vector3D *carveDirection,
					 const float maxDeviation,
					 const bool gaps,
					 float* jacobians, const Vector3D* localX, const Vector3D* localY,
					 const bool useEllipticalSurfels, float lowPassFilter) {
	

	// assumes that splat coordinates are brush coordinates!!
	
	int              i; 
	float            alpha;
	
	Vector3D         oldPosition,
		             displacementVector,
		             newPosition;	
	float            displacementScalar;
		
	QRgb             oldColor,
		             newColor;		
	float            red,
		             green,
		             blue;
	
	float            oldCoeff,
		             newCoeff, brushCoeff;

	if (!surfels) return false;

	BrushSample *brushSamples = 0;

	(void)maxDeviation;

	// compute property descriptor for all surfels
	SurfelInterface::PropertyDescriptor newPropertyDescriptor = SurfelInterface::NO_PROPERTIES;
	if (displacementChannel) {
		newPropertyDescriptor |= SurfelInterface::POSITION;
	}
	if (ambCoeffChannel) {
		newPropertyDescriptor |= SurfelInterface::AMBIENT_COEFFICIENT;
	}
	if (diffColorChannel) {
		newPropertyDescriptor |= SurfelInterface::DIFFUSE_COLOR;
	}
	if (specColorChannel) {
		newPropertyDescriptor |= SurfelInterface::SPECULAR_COLOR;
	}
	if (diffCoeffChannel) {
		newPropertyDescriptor |= SurfelInterface::DIFFUSE_COEFFICIENT;
	}
	if (specCoeffChannel) {
		newPropertyDescriptor |= SurfelInterface::SPECULAR_COEFFICIENT;
	}
	if (shininessChannel) {
		newPropertyDescriptor |= SurfelInterface::SHININESS;
	}

	/////////////////////////////////////////////////////////////////////////////
	// with resampling, surfels are arranged on regular grid
	//

	if (!texCoords || texCoords->size() == 0) {

		if(!gaps) {
			// compute the expected average radius by 
			float topRowLength = ((*surfels)[0]->getPosition() - (*surfels)[sizeX - 1]->getPosition()).getLength();
			float bottomRowLength = ((*surfels)[(sizeY - 1) * sizeX]->getPosition() - (*surfels)[sizeY * sizeX - 1]->getPosition()).getLength();
			float leftColumnLength = ((*surfels)[0]->getPosition() - (*surfels)[(sizeY - 1) * sizeX]->getPosition()).getLength();
			float rightColumnLength = ((*surfels)[sizeX - 1]->getPosition() - (*surfels)[sizeY * sizeX - 1]->getPosition()).getLength();
			
			expectedRadius = MyMathTools::maxf (MyMathTools::maxf (topRowLength, bottomRowLength) / (float) sizeX, MyMathTools::maxf (leftColumnLength, rightColumnLength) / (float) sizeY);
		}
		surfelRadiusTooBig = false;
		

		// assumes that splats are arranged in grid according to brush resolution!
		int x, y;

		// set property descriptor first
		SurfelCollection *sc = NULL;
		for (x = 0; x < sizeX; x++) {
			for (y = 0; y < sizeY; y++) {
				if((*surfels)[y * sizeX + x] && sc != (*surfels)[y * sizeX + x]->getSurfelCollection()) {
					sc = (*surfels)[y * sizeX + x]->getSurfelCollection();
					// flag has to be false (no signal emitted) otherwise the pointers are invalidated!
					sc->setPropertyDescriptor(sc->getPropertyDescriptor() | newPropertyDescriptor, false);
				}
				
			}
		}

		// positions
		if (displacementChannel) {			
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {
					
					if((*surfels)[y * sizeX + x]) {
						oldPosition = (*surfels)[y * sizeX + x]->getPosition();

						if (displacementAlpha) {
							displacementScalar = displacementAlpha->getValue (x, y);
						}
						else {
							displacementScalar = 1.0f;
						}					
						if (carveDirection) {
							// assumes that carving component stores distance of surface point to tool reference plane!
							if (scale > 0.0f) {
								displacementScalar *= MyMathTools::maxf (0.0f, scale * displacementNormal->getValue (x, y) - displacementCarving->getValue (x, y));						
							}
							else {
								displacementScalar *= -MyMathTools::maxf (0.0f, -(scale * displacementNormal->getValue (x, y) - displacementCarving->getValue (x, y)));						
							}
							displacementVector = displacementScalar * (*carveDirection);

						}
						else {
							displacementVector = (scale * displacementScalar * displacementNormal->getValue (x, y)) * (*surfels)[y * sizeX + x]->getNormal();
						}

						scene->setSurfelPosition ((*surfels)[y * sizeX + x], oldPosition + displacementVector, false);
					}
				}
			}
		}			

		if(!gaps) {
			if (!computeRadiiAndNormals (surfels)) {
				return false;
			}
		} 
		else {
			if (!computeRadiiAndNormalsWithGaps (surfels)) {
				return false;
			}
		}

		
		// diffuse color
		if (diffColorChannel) {
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {

					if((*surfels)[y * sizeX + x]) {
						red = 255.0f * diffRedComponent->getValue (x, y);
						green = 255.0f * diffGreenComponent->getValue (x, y);
						blue = 255.0f * diffBlueComponent->getValue (x, y);
						
						if (diffColorAlpha) {
							oldColor = (*surfels)[y * sizeX + x]->getDiffuseColor ();
							alpha = diffColorAlpha->getValue (x, y);
							red   = alpha * red   + (1 - alpha) * qRed (oldColor);
							green = alpha * green + (1 - alpha) * qGreen (oldColor);
							blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
						}					
						newColor = qRgb ((int)red, (int)green, (int)blue);

						scene->setSurfelDiffuseColor ((*surfels)[y * sizeX + x], newColor, false);
					}
				}
			}
		}

		
		// specular color
		if (specColorChannel) {
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {
															
					if((*surfels)[y * sizeX + x]) {
						red = 255.0f * specRedComponent->getValue (x, y);
						green = 255.0f * specGreenComponent->getValue (x, y);
						blue = 255.0f * specBlueComponent->getValue (x, y);
						
						if (specColorAlpha) {
							oldColor = (*surfels)[y * sizeX + x]->getSpecularColor ();
							alpha = specColorAlpha->getValue (x, y);
							red   = alpha * red   + (1 - alpha) * qRed (oldColor);
							green = alpha * green + (1 - alpha) * qGreen (oldColor);
							blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
						}					
						newColor = qRgb ((int)red, (int)green, (int)blue);					

						scene->setSurfelSpecularColor ((*surfels)[y * sizeX + x], newColor, false);
					}
				}
			}
		}


		// diffuse coefficient
		if (diffCoeffChannel) {
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {

					if((*surfels)[y * sizeX + x]) {															
						if (diffuseCoefficientAlpha) {
							alpha = diffuseCoefficientAlpha->getValue (x, y);
							oldCoeff = (*surfels)[y * sizeX + x]->getDiffuseCoefficient ();
							newCoeff = alpha * diffuseCoefficient->getValue (x, y) + (1 - alpha) * oldCoeff;	
						}
						else {
							newCoeff = diffuseCoefficient->getValue (x, y);
						}

						scene->setSurfelDiffuseCoefficient ((*surfels)[y * sizeX + x], newCoeff, false);					
					}
				}
			}
		}


		// specular coefficient
		if (specCoeffChannel) {
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {

					if((*surfels)[y * sizeX + x]) {															
						if (specularCoefficientAlpha) {
							alpha = specularCoefficientAlpha->getValue (x, y);
							oldCoeff = (*surfels)[y * sizeX + x]->getSpecularCoefficient ();
							newCoeff = alpha * specularCoefficient->getValue (x, y) + (1 - alpha) * oldCoeff;	
						}
						else {
							newCoeff = specularCoefficient->getValue (x, y);
						}

						scene->setSurfelSpecularCoefficient ((*surfels)[y * sizeX + x], newCoeff, false);
					}
				}
			}
		}


		// shininess
		if (shininessChannel) {
			for (x = 0; x < sizeX; x++) {
				for (y = 0; y < sizeY; y++) {
					
					if((*surfels)[y * sizeX + x]) {
						if (shininessAlpha) {
							alpha = shininessAlpha->getValue (x, y);
							oldCoeff = (*surfels)[y * sizeX + x]->getShininess ();
							newCoeff = alpha * shininess->getValue (x, y) + (1 - alpha) * oldCoeff;	
						}
						else {
							newCoeff = shininess->getValue (x, y);
						}
					
						scene->setSurfelShininess ((*surfels)[y * sizeX + x], newCoeff, false);
					}
				}
			}
		}

	}

	/////////////////////////////////////////////////////////////////////////////
	// sampling on old positions, texture coordinates are provided
	// 
	else {

		SurfelCollection *sc = NULL;
		// set property descriptor first
		for(i=0; i<numSurfels; i++) {

			if(sc != (*surfels)[i]->getSurfelCollection()) {
				sc = (*surfels)[i]->getSurfelCollection();
			// flag has to be false (no signal emitted) otherwise the pointers are invalidated!
				sc->setPropertyDescriptor(sc->getPropertyDescriptor() | newPropertyDescriptor, false);
			}
		}

	    // resample the brush using EWA filtering
	    if(jacobians != 0) {

			brushSamples = new BrushSample[numSurfels];

			for(i=0; i<numSurfels; i++) {

				float localKernel[4];
				Vector3D axis0, axis1;

				if (useEllipticalSurfels) {
					// get ellipse axes in object space coordinates
					axis0 = (*surfels)[i]->getAxisOne();
					axis1 = (*surfels)[i]->getAxisTwo();
					// express ellipse axes in local tangent coordinates
					localKernel[0] = Vector3D::dotProduct(axis0, localX[i]);
					localKernel[1] = Vector3D::dotProduct(axis1, localX[i]);
					localKernel[2] = Vector3D::dotProduct(axis0, localY[i]);
					localKernel[3] = Vector3D::dotProduct(axis1, localY[i]);
				}
				else {
					// circular surfels
					// [u,v]' = J * [[radius 0]' [0 radius]'] * [x,y]'
					localKernel[0] = (*surfels)[i]->getRadius();;
					localKernel[1] = 0.f;
					localKernel[2] = 0.f;
					localKernel[3] = (*surfels)[i]->getRadius();;
				}

				// reads (uses) J, localX, localY
				brushSamples[i] = rasterizeSurfel ((*surfels)[i], (*texCoords)[i], &(jacobians[i*4]), localKernel, localX[i], localY[i], lowPassFilter);
			}
	    }
		
		// diffuse color		
		if (diffColorChannel) {
			for (i = 0; i < numSurfels; i++) {
				
			    if(brushSamples == 0) {
				red = 255.0f * diffRedComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				green = 255.0f * diffGreenComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				blue = 255.0f * diffBlueComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
			    } else {
				red = brushSamples[i].redDiffColor;
				green = brushSamples[i].greenDiffColor;
				blue = brushSamples[i].blueDiffColor;
			    }
				
				if (diffColorAlpha) {
					oldColor = (*surfels)[i]->getDiffuseColor ();
					if(brushSamples == 0) {
						alpha = diffColorAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    alpha = brushSamples[i]. diffColorAlpha;
					}

					red   = alpha * red   + (1 - alpha) * qRed (oldColor);
					green = alpha * green + (1 - alpha) * qGreen (oldColor);
					blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
				}
				
				newColor = qRgb ((int)red, (int)green, (int)blue);
				
				scene->setSurfelDiffuseColor ((*surfels)[i], newColor, false);
			}
		}


		// specular color		
		if (specColorChannel) {
			for (i = 0; i < numSurfels; i++) {				
				
			    if(brushSamples == 0) {
				red = 255.0f * specRedComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				green = 255.0f * specGreenComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				blue = 255.0f * specBlueComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
			    } else {
				red = brushSamples[i].redDiffColor;
				green = brushSamples[i].greenDiffColor;
				blue = brushSamples[i].blueDiffColor;
			    }
				
				if (specColorAlpha) {
					oldColor = (*surfels)[i]->getSpecularColor ();
					if(brushSamples == 0) {
						alpha = specColorAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					     alpha = brushSamples[i].specColorAlpha;
					}

					red   = alpha * red   + (1 - alpha) * qRed (oldColor);
					green = alpha * green + (1 - alpha) * qGreen (oldColor);
					blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
				}
				
				newColor = qRgb ((int)red, (int)green, (int)blue);
				
				scene->setSurfelSpecularColor ((*surfels)[i], newColor, false);
			}
		}

		// ambient Coefficient
		if (ambCoeffChannel) {
			for (i = 0; i < numSurfels; i++) {
					
				if (ambientCoefficientAlpha) {
					oldCoeff = (*surfels)[i]->getAmbientCoefficient ();

					if(brushSamples == 0) {
    					    alpha = ambientCoefficientAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					    brushCoeff = ambientCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    alpha = brushSamples[i].ambientCoeffAlpha;
					    brushCoeff = brushSamples[i].ambientCoeff;
					}

					newCoeff = alpha * brushCoeff + (1 - alpha) * oldCoeff;	
				}
				else {

				    if(brushSamples == 0) {
					brushCoeff = ambientCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				    } else {
					brushCoeff = brushSamples[i].ambientCoeff;
				    }

					newCoeff = brushCoeff;
				}
				scene->setSurfelAmbientCoefficient ((*surfels)[i], newCoeff, false);
			}
		}

		// diffuse Coefficient
		if (diffCoeffChannel) {
			for (i = 0; i < numSurfels; i++) {
					
				if (diffuseCoefficientAlpha) {
					oldCoeff = (*surfels)[i]->getDiffuseCoefficient ();

					if(brushSamples == 0) {
						alpha = diffuseCoefficientAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
						brushCoeff = diffuseCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    alpha = brushSamples[i].diffCoeffAlpha;
					    brushCoeff = brushSamples[i].diffCoeff;
					}

					newCoeff = alpha * brushCoeff + (1 - alpha) * oldCoeff;	
				}
				else {
				    
					if(brushSamples == 0) {
						brushCoeff = diffuseCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    brushCoeff = brushSamples[i].diffCoeff;
					}

					newCoeff = brushCoeff;
				}
				scene->setSurfelDiffuseCoefficient ((*surfels)[i], newCoeff, false);
			}
		}

		// specular Coefficient
		if (specCoeffChannel) {
			for (i = 0; i < numSurfels; i++) {					
				if (specularCoefficientAlpha) {
					oldCoeff = (*surfels)[i]->getSpecularCoefficient ();

					if(brushSamples == 0) {
						alpha = specularCoefficientAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
						brushCoeff = specularCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    alpha = brushSamples[i].specCoeffAlpha;
					    brushCoeff = brushSamples[i].specCoeff;
					}
						
					newCoeff = alpha * brushCoeff + (1 - alpha) * oldCoeff;	
				}
				else {
					
				    if(brushSamples == 0) {
					brushCoeff = specularCoefficient->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				    } else {
				        brushCoeff = brushSamples[i].specCoeff;
				    }

				    newCoeff = brushCoeff;
				}
				scene->setSurfelSpecularCoefficient ((*surfels)[i], newCoeff, false);
			}
		}

		// shininess
		if (shininessChannel) {
			for (i = 0; i < numSurfels; i++) {					
				if (shininessAlpha) {
					oldCoeff = (*surfels)[i]->getShininess ();

					if(brushSamples == 0) {
						alpha = specularCoefficientAlpha->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
						brushCoeff = shininess->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
					} else {
					    alpha = brushSamples[i].specCoeffAlpha;
					    brushCoeff = brushSamples[i].shininess;
					}

					newCoeff = alpha * 100.0f * brushCoeff + (1 - alpha) * oldCoeff;	
				}
				else {
				    if(brushSamples == 0) {
					brushCoeff = shininess->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
				    } else {
					brushCoeff = brushSamples[i].shininess;
				    }

					newCoeff = 100.0f * brushCoeff;
				}

				scene->setSurfelShininess ((*surfels)[i], newCoeff, false);
			}
		}

		if(brushSamples != 0) {
		    delete[] brushSamples;
		}

	}
	return true;
}



void Painter::select (std::vector<SurfelInterface *>* surfels, 
					  int numSurfels,
					  const std::vector<MyDataTypes::TextureCoordinate>* texCoords,
					  int *selectionCounters) {


	int                   i;
	float                 red,
		                  green,
						  blue;

	if (!surfels || !texCoords) {
		return;
	}
	
	if (diffColorChannel) {
		for (i = 0; i < numSurfels; i++) {

			red = diffRedComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
			green = diffGreenComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
			blue = diffBlueComponent->getValue ((int)(*texCoords)[i].u, (int)(*texCoords)[i].v);
	

			if (blue > 0.66) {				
				// if not selected yet...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED1) == false) {					
					// ...mark this surfel as selected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED1 | SurfelInterface::EMPHASISE), true, false);
					selectionCounters[0]++;
				}
			}
			else if (blue < 0.33) {
				// if selected...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED1) == true) {
					
					// ...mark this surfel as deselected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED1 | SurfelInterface::EMPHASISE), false, false);
					selectionCounters[0]--;
					if ((*surfels)[i]->isSelected()) {
						scene->setSurfelFlagOn ((*surfels)[i], SurfelInterface::EMPHASISE, true, false);
					}
				}
			}

			if (red > 0.66) {				
				// if not selected yet...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED2) == false) {					
					// ...mark this surfel as selected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED2 | SurfelInterface::EMPHASISE), true, false);	
					selectionCounters[1]++;
				}
			}
			else if (red < 0.33) {
				// if selected...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED2) == true) {
					
					// ...mark this surfel as deselected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED2 | SurfelInterface::EMPHASISE), false, false);					
					selectionCounters[1]--;

					if ((*surfels)[i]->isSelected()) {
						scene->setSurfelFlagOn ((*surfels)[i], SurfelInterface::EMPHASISE, true, false);
					}
				}
			}


			if (green > 0.66) {				
				// if not selected yet...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED3) == false) {					
					// ...mark this surfel as selected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED3 | SurfelInterface::EMPHASISE), true, false);					
					selectionCounters[2]++;
				}
			}
			else if (green < 0.33) {
				// if selected...
				if ((*surfels)[i]->isFlagOn (SurfelInterface::SELECTED3) == true) {
					
					// ...mark this surfel as deselected, but DON'T emit the signal here
					scene->setSurfelFlagOn ((*surfels)[i], (SurfelInterface::Flag)(SurfelInterface::SELECTED3 | SurfelInterface::EMPHASISE), false, false);					
					selectionCounters[2]--;

					if ((*surfels)[i]->isSelected()) {
						scene->setSurfelFlagOn ((*surfels)[i], SurfelInterface::EMPHASISE, true, false);
					}
				}
			}




		}
	}

}



void Painter::paintWholeObject (SurfelCollection *surfels) {


	SurfelInterface *surfel;
	float            alpha;
	
		
	QRgb             oldColor,
		             newColor;		
	float            red,
		             green,
		             blue;
	
	float            oldCoeff,
		             newCoeff;


	for (surfel = surfels->getFirstSurfel(); surfel != 0; surfel = surfels->getNextSurfel()) {

		// diffuse color		
		if (diffColorChannel) {
			red = 255.0f * diffRedComponent->getValue (0, 0);
			green = 255.0f * diffGreenComponent->getValue (0, 0);
			blue = 255.0f * diffBlueComponent->getValue (0, 0);
			
			if (diffColorAlpha) {
				oldColor = surfel->getDiffuseColor ();
				alpha = diffColorAlpha->getValue (0, 0);
				red   = alpha * red   + (1 - alpha) * qRed (oldColor);
				green = alpha * green + (1 - alpha) * qGreen (oldColor);
				blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
			}
			
			newColor = qRgb ((int)red, (int)green, (int)blue);			
			scene->setSurfelDiffuseColor (surfel, newColor, false);
		}
		
		
		// specular color		
		if (specColorChannel) {
			
			
			red = 255.0f * specRedComponent->getValue (0, 0);
			green = 255.0f * specGreenComponent->getValue (0, 0);
			blue = 255.0f * specBlueComponent->getValue (0, 0);
			
			if (specColorAlpha) {
				oldColor = surfel->getSpecularColor ();
				alpha = specColorAlpha->getValue (0, 0);
				red   = alpha * red   + (1 - alpha) * qRed (oldColor);
				green = alpha * green + (1 - alpha) * qGreen (oldColor);
				blue  = alpha * blue  + (1 - alpha) * qBlue (oldColor);
			}
			
			newColor = qRgb ((int)red, (int)green, (int)blue);
			
			scene->setSurfelSpecularColor (surfel, newColor, false);
		}
		
		// ambient Coefficient
		if (ambCoeffChannel) {
			
			if (ambientCoefficientAlpha) {
				oldCoeff = surfel->getAmbientCoefficient ();
				alpha = ambientCoefficientAlpha->getValue (0, 0);
				newCoeff = alpha * ambientCoefficient->getValue (0,
					0) + (1 - alpha) * oldCoeff;	
			}
			else {
				newCoeff = ambientCoefficient->getValue (0, 0);
			}
			scene->setSurfelAmbientCoefficient (surfel, newCoeff, false);
		}
		
		// diffuse Coefficient
		if (diffCoeffChannel) {
			
			if (diffuseCoefficientAlpha) {
				oldCoeff = surfel->getDiffuseCoefficient ();
				alpha = diffuseCoefficientAlpha->getValue (0, 0);
				newCoeff = alpha * diffuseCoefficient->getValue (0,
					0) + (1 - alpha) * oldCoeff;	
			}
			else {
				newCoeff = diffuseCoefficient->getValue (0, 0);
			}
			scene->setSurfelDiffuseCoefficient (surfel, newCoeff, false);
		}
		
		// specular Coefficient
		if (specCoeffChannel) {
			if (specularCoefficientAlpha) {
				oldCoeff = surfel->getSpecularCoefficient ();
				alpha = specularCoefficientAlpha->getValue (0, 0);
				newCoeff = alpha * specularCoefficient->getValue (0,
					0) + (1 - alpha) * oldCoeff;	
			}
			else {
				newCoeff = specularCoefficient->getValue (0, 0);
			}
			scene->setSurfelSpecularCoefficient (surfel, newCoeff, false);
		}
		
		// shininess
		if (shininessChannel) {
			if (shininessAlpha) {
				oldCoeff = surfel->getShininess ();
				alpha = shininessAlpha->getValue (0, 0);
				newCoeff = alpha * 100.0f * shininess->getValue (0,
					0) + (1 - alpha) * oldCoeff;	
			}
			else {
				newCoeff = 100.0f * shininess->getValue (0, 0);
			}
			scene->setSurfelShininess (surfel, newCoeff, false);
		}
		
	}

}


void Painter::setBrush (const Brush *newBrush) {

	sizeX = newBrush->getSize().width();
	sizeY = newBrush->getSize().height();

	/////////////////////////////////////////////////////////////////////////////
	displacementChannel = newBrush->getChannel (SurfelInterface::POSITION);
	if (displacementChannel) {
		
		displacementNormal  = displacementChannel->getComponent (0);	
		displacementCarving = displacementChannel->getComponent (1);	
		
		if (newBrush->hasAlphaChannel (SurfelInterface::POSITION)) {
			displacementAlpha = newBrush->getAlphaChannel (SurfelInterface::POSITION)->getComponent(0);	
		}
		else {
			displacementAlpha = 0;
		}			
	}
	
	/////////////////////////////////////////////////////////////////////////////
	diffColorChannel = newBrush->getChannel (SurfelInterface::DIFFUSE_COLOR);
	if (diffColorChannel) {
		
		if (diffColorChannel->getNOfComponents() == 3) {
			diffRedComponent = diffColorChannel->getComponent (0);
			diffGreenComponent = diffColorChannel->getComponent (1);
			diffBlueComponent = diffColorChannel->getComponent (2);
		}
		else {
			diffRedComponent = diffGreenComponent = diffGreenComponent = diffColorChannel->getComponent (0);
		}
		
		if (newBrush->hasAlphaChannel (SurfelInterface::DIFFUSE_COLOR)) {
			diffColorAlpha = newBrush->getAlphaChannel (SurfelInterface::DIFFUSE_COLOR)->getComponent(0);
		}
		else {
			diffColorAlpha = 0;
		}							
	}	

	/////////////////////////////////////////////////////////////////////////////
	specColorChannel = newBrush->getChannel (SurfelInterface::SPECULAR_COLOR);
	if (specColorChannel) {
		
		if (specColorChannel->getNOfComponents() == 3) {
			specRedComponent = specColorChannel->getComponent (0);
			specGreenComponent = specColorChannel->getComponent (1);
			specBlueComponent = specColorChannel->getComponent (2);
		}
		else {
			specRedComponent = specGreenComponent = specGreenComponent = specColorChannel->getComponent (0);
		}
		
		if (newBrush->hasAlphaChannel (SurfelInterface::SPECULAR_COLOR)) {
			specColorAlpha = newBrush->getAlphaChannel (SurfelInterface::DIFFUSE_COLOR)->getComponent(0);
		}
		else {
			specColorAlpha = 0;
		}							
	}	


	/////////////////////////////////////////////////////////////////////////////
	ambCoeffChannel = newBrush->getChannel (SurfelInterface::AMBIENT_COEFFICIENT);
	if (ambCoeffChannel) {
		
		ambientCoefficient  = ambCoeffChannel->getComponent (0);	
		
		if (newBrush->hasAlphaChannel (SurfelInterface::AMBIENT_COEFFICIENT)) {
			ambientCoefficientAlpha = newBrush->getAlphaChannel (SurfelInterface::AMBIENT_COEFFICIENT)->getComponent(0);	
		}
		else {
			ambientCoefficientAlpha = 0;
		}			
	}


	/////////////////////////////////////////////////////////////////////////////
	diffCoeffChannel = newBrush->getChannel (SurfelInterface::DIFFUSE_COEFFICIENT);
	if (diffCoeffChannel) {
		
		diffuseCoefficient  = diffCoeffChannel->getComponent (0);	
		
		if (newBrush->hasAlphaChannel (SurfelInterface::DIFFUSE_COEFFICIENT)) {
			diffuseCoefficientAlpha = newBrush->getAlphaChannel (SurfelInterface::DIFFUSE_COEFFICIENT)->getComponent(0);	
		}
		else {
			diffuseCoefficientAlpha = 0;
		}			
	}

	/////////////////////////////////////////////////////////////////////////////
	specCoeffChannel = newBrush->getChannel (SurfelInterface::SPECULAR_COEFFICIENT);
	if (specCoeffChannel) {
		
		specularCoefficient  = specCoeffChannel->getComponent (0);	
		
		if (newBrush->hasAlphaChannel (SurfelInterface::SPECULAR_COEFFICIENT)) {
			specularCoefficientAlpha = newBrush->getAlphaChannel (SurfelInterface::SPECULAR_COEFFICIENT)->getComponent(0);	
		}
		else {
			specularCoefficientAlpha = 0;
		}			
	}

	/////////////////////////////////////////////////////////////////////////////
	shininessChannel = newBrush->getChannel (SurfelInterface::SHININESS);
	if (shininessChannel) {
		
		shininess  = shininessChannel->getComponent (0);	
		
		if (newBrush->hasAlphaChannel (SurfelInterface::SHININESS)) {
			shininessAlpha = newBrush->getAlphaChannel (SurfelInterface::SHININESS)->getComponent(0);	
		}
		else {
			shininessAlpha = 0;
		}			
	}


	if (filterBlendingGrid) {
		delete[] filterBlendingGrid;
	}



	// compute the filter weight function for blending with original surface function
	// a boundary ring of size zeroBorder is zero
	// then a ring of size (border - borderZero) is blended towards 1
	// the inner ring is set to one

	filterBlendingGrid = new float [sizeX * sizeY];

	int x, y;
	float scaledX, scaledY;
	float border = 0.3f;
	float zeroBorder = 0.1f;
	float funX, funY;

	for (x = 0; x < sizeX; x++) {

		for (y = 0; y < sizeY; y++) {

			scaledX = (float) x / (float) sizeX;
			scaledY = (float) y / (float) sizeY;

			if ((scaledX <= zeroBorder) || (scaledX >= 1.0f - zeroBorder)) {
				funX = 0.0f;
			}
			else if (scaledX <= border) {				
				funX = 1.0f - (scaledX - zeroBorder) / (border - zeroBorder);
				funX = (funX * funX - 1.0f) * (funX * funX - 1.0f);
			} else if (scaledX >= 1.0f - border) {
				funX = (scaledX - (1.0f - border)) / (border - zeroBorder);
				funX = (funX * funX - 1.0f) * (funX * funX - 1.0f);
			} else {
				funX = 1.0f;
			}

			if ((scaledY <= zeroBorder) || (scaledY >= 1.0f - zeroBorder)) {
				funY = 0.0f;
			}
			else if (scaledY <= border) {				
				funY = 1.0f - (scaledY - zeroBorder) / (border - zeroBorder);
				funY = (funY * funY - 1.0f) * (funY * funY - 1.0f);
			} else if (scaledY >= 1.0f - border) {
				funY = (scaledY - (1.0f - border)) / (border - zeroBorder);
				funY = (funY * funY - 1.0f) * (funY * funY - 1.0f);
			} else {
				funY = 1.0f;
			}


			filterBlendingGrid [y * sizeX + x] = funY * funX;

		}	
		
	}

}


bool Painter::computeRadiiAndNormals (std::vector<SurfelInterface *>* surfels) {

	int x, y;
	Vector3D dX, dY;
	
	for (x = 1; x < sizeX - 1; x++) {
		// top row
		dX = (*surfels)[x + 1]->getPosition() - (*surfels)[x - 1]->getPosition();
		dY = 2.0f * ((*surfels)[sizeX + x]->getPosition() - (*surfels)[x]->getPosition());
		setRadiusAndNormal ((*surfels)[x], dX, dY);
		
		// bottom row
		dX = (*surfels)[(sizeY - 1) * sizeX + x + 1]->getPosition() - (*surfels)[(sizeY - 1) * sizeX + x - 1]->getPosition();
		dY = 2.0f * ((*surfels)[(sizeY - 1) * sizeX + x]->getPosition() - (*surfels)[(sizeY - 2) * sizeX + x]->getPosition());
		setRadiusAndNormal ((*surfels)[(sizeY - 1) * sizeX + x], dX, dY);

		// interior grid
		for (y = 1; y < sizeY - 1; y++) {
			dX = (*surfels)[y * sizeX + x + 1]->getPosition() - (*surfels)[y * sizeX + x - 1]->getPosition();
			dY = (*surfels)[(y + 1) * sizeX + x]->getPosition() - (*surfels)[(y - 1) * sizeX + x]->getPosition();
			setRadiusAndNormal ((*surfels)[y * sizeX + x], dX, dY);
		}
	}


	for (y = 1; y < sizeY - 1; y++) {

		// left column
		dX = 2.0f * ((*surfels)[y * sizeX + 1]->getPosition() - (*surfels)[y * sizeX]->getPosition());
		dY = (*surfels)[(y + 1) * sizeX]->getPosition() - (*surfels)[(y - 1) * sizeX]->getPosition();			
		setRadiusAndNormal ((*surfels)[y * sizeX], dX, dY);
		
		// right column
		dX = 2.0f * ((*surfels)[y * sizeX + sizeX - 1]->getPosition() - (*surfels)[y * sizeX + sizeX - 2]->getPosition());
		dY = (*surfels)[(y + 2) * sizeX - 1]->getPosition() - (*surfels)[y * sizeX - 1]->getPosition();
		setRadiusAndNormal ((*surfels)[y * sizeX + sizeX - 1], dX, dY);
	}

	// corners
	// left top
	dX = 2.0f * ((*surfels)[1]->getPosition() - (*surfels)[0]->getPosition());
	dY = 2.0f * ((*surfels)[sizeX]->getPosition() - (*surfels)[0]->getPosition());
	setRadiusAndNormal ((*surfels)[0], dX, dY);
	
	// right top
	dX = 2.0f * ((*surfels)[sizeX - 1]->getPosition() - (*surfels)[sizeX - 2]->getPosition());
	dY = 2.0f * ((*surfels)[sizeX + sizeX - 1]->getPosition() - (*surfels)[sizeX - 1]->getPosition());
	setRadiusAndNormal ((*surfels)[sizeX - 1], dX, dY);
	
	// left bottom
	dX = 2.0f * ((*surfels)[(sizeY - 1) * sizeX + 1]->getPosition() - (*surfels)[(sizeY - 1) * sizeX]->getPosition());
	dY = 2.0f * ((*surfels)[(sizeY - 1) * sizeX]->getPosition() - (*surfels)[(sizeY - 2) * sizeX]->getPosition());
	setRadiusAndNormal ((*surfels)[(sizeY - 1) * sizeX], dX, dY);
	
	// right bottom
	dX = 2.0f * ((*surfels)[sizeY * sizeX - 1]->getPosition() - (*surfels)[sizeY * sizeX - 2]->getPosition());
	dY = 2.0f * ((*surfels)[sizeY * sizeX - 1]->getPosition() - (*surfels)[(sizeY - 1) * sizeX - 1]->getPosition());
	setRadiusAndNormal ((*surfels)[sizeY * sizeX - 1], dX, dY);
	
	
	// abort if surfel radius is too big
	if (surfelRadiusTooBig) {
		return false;
	}

	return true;

}

bool Painter::computeRadiiAndNormalsWithGaps (std::vector<SurfelInterface *>* surfels) {

	int x, y;
	Vector3D dX, dY;
	
	for (y = 0; y < sizeY; y++) {
		for (x = 0; x < sizeX; x++) {
			if((*surfels)[y * sizeX + x]) {
				if(computeDX(surfels, x, y, dX) && computeDY(surfels, x, y, dY)) {
					setRadiusAndNormal ((*surfels)[y * sizeX + x], dX, dY);
				}
			}
		}
	}

	surfelRadiusTooBig = false;

	return true;
}

inline bool Painter::computeDX(std::vector<SurfelInterface *>* surfels, const int x, const int y, Vector3D& dX) {

	if( x > 0 && (*surfels)[y*sizeX + x - 1] != 0 && x < sizeX-1 && (*surfels)[y*sizeX + x + 1] != 0 ) {
		dX = (*surfels)[y*sizeX + x + 1]->getPosition() - (*surfels)[y*sizeX + x - 1]->getPosition();
		return true;
	}
	else if( x > 0 && (*surfels)[y*sizeX + x - 1] != 0) {
		dX = 2.0f * ((*surfels)[y*sizeX + x]->getPosition() - (*surfels)[y*sizeX + x - 1]->getPosition());
		return true;
	}
	else if(x < sizeX-1 && (*surfels)[y*sizeX + x + 1] != 0 ) {
		dX = 2.0f * ((*surfels)[y*sizeX + x + 1]->getPosition() - (*surfels)[y*sizeX + x]->getPosition());
		return true;
	}
	else {
		return false;
	}
}

inline bool Painter::computeDY(std::vector<SurfelInterface *>* surfels, const int x, const int y, Vector3D& dY) {
	if( y > 0 && (*surfels)[(y-1)*sizeX + x] != 0 && y < sizeY-1 && (*surfels)[(y+1)*sizeX + x] != 0 ) {
		dY = (*surfels)[(y + 1) * sizeX + x]->getPosition() - (*surfels)[(y - 1) * sizeX + x]->getPosition();
		return true;
	}
	else if( y > 0 && (*surfels)[(y-1)*sizeX + x] != 0 ) {
		dY = 2.0f * ((*surfels)[y * sizeX + x]->getPosition() - (*surfels)[(y - 1) * sizeX + x]->getPosition());
		return true;
	}
	else if( y < sizeY-1 && (*surfels)[(y+1)*sizeX + x] != 0 ) {
		dY = 2.0f * ((*surfels)[(y+1) * sizeX + x]->getPosition() - (*surfels)[y * sizeX + x]->getPosition());
		return true;
	}
	else {
		return false;
	}
}

inline void Painter::setRadiusAndNormal (SurfelInterface *surfel, Vector3D dX, Vector3D dY) {

	// surfel radius
	float radius = sqrt(MyMathTools::maxf (dX.getSquaredLength(), dY.getSquaredLength())) * 0.5f;
	if (radius > 4.0f* expectedRadius) {
		surfelRadiusTooBig = true;
	}

	// surfel normal
	Vector3D newNormal = Vector3D::crossProduct (dY, dX);
	newNormal.normalize();

	if (surfel->getSurfelCollection()->getPropertyDescriptor() & SurfelInterface::TANGENT_AXES) {
		// tangent axes
		scene->setSurfelNormalTangentAxesSystem (surfel, newNormal, 0.5 * dY, 0.5 * dX, false);
	}
	else {
		scene->setSurfelRadius (surfel, radius, false);
		scene->setSurfelNormal (surfel, newNormal, false);
	}
}

Painter::BrushSample Painter::rasterizeSurfel (SurfelInterface *surfel, MyDataTypes::TextureCoordinate texCoords, float J[4], float localKernel[4], Vector3D localX, Vector3D localY, float lowPassFilter) {
    
    BrushSample sample, zeroSample;

    zeroSample.redDiffColor = 0.f;
    zeroSample.greenDiffColor = 0.f;
    zeroSample.blueDiffColor = 0.f;
    zeroSample.diffColorAlpha = 0.f;
    zeroSample.redSpecColor = 0.f;
    zeroSample.greenSpecColor = 0.f;
    zeroSample.blueSpecColor = 0.f;
    zeroSample.diffColorAlpha = 0.f;
    zeroSample.ambientCoeff = 0.f;
    zeroSample.ambientCoeffAlpha = 0.f;
    zeroSample.diffCoeff = 0.f;
    zeroSample.diffCoeffAlpha = 0.f;
    zeroSample.specCoeff = 0.f;
    zeroSample.specCoeffAlpha = 0.f;
    zeroSample.shininess = 0.f;
    sample = zeroSample;

    // bounding box of the ellipse to be rasterized
    int uMin, uMax, vMin, vMax;
    float lx, ly;
    // float centerTexture[2];
    MyDataTypes::TextureCoordinate centerTexture;
    
    int U, V;
    float u, v;
    float q, dq, ddq;
    float weight;
    
    float w;
    float conic[4];
    float detConic;
    float splat[4];

    int width = sizeX;
    int height = sizeY;

    // NOTE: 
    // - the Jacobian J[] stores the partial derivatives J[] = [[dudx,dvdx]' [dudy,dvdy]']
    // - since surfel texture coordinates are in the range [0..1]x[0..1], but the texels have
    // coordinates in the range [0.5..width-0.5]x[0.5..height-0.5] ("texel coordinates"), we 
    // have to scale the texture coordinates to texel coordinates. note that this scaling
    // implements the matrix operation 
    // [dTexelU dTexelV] = [[width-1 0]' [0 height-1]] * [[dudx,dvdx]' [dudy,dvdy]'] * [dx dy] 
    J[0] *= (width-1.f);
    J[1] *= (width-1.f);
    J[2] *= (height-1.f);
    J[3] *= (height-1.f);

    // concatenate the Jacobian with the matrix 'localKernel' describing the ellipse
    // in the local tangent space, i.e., we compute splat = J * localKernel. this results in the 
    // "mapping matrix" of the splat in texture space.
    splat[0] = J[0]*localKernel[0] + J[1]*localKernel[2];
    splat[1] = J[0]*localKernel[1] + J[1]*localKernel[3];
    splat[2] = J[2]*localKernel[0] + J[3]*localKernel[2];
    splat[3] = J[2]*localKernel[1] + J[3]*localKernel[3];

    centerTexture = texCoords;

    // step 1: calculate the EWA ellipse coefficients
    // compute bounding box
    // NOTE: also consider the low pass filter!!
    splat[0] < 0.0f ? (lx = -splat[0]) : (lx = splat[0]);
    splat[2] < 0.0f ? (ly = -splat[2]) : (ly = splat[2]);
    splat[1] < 0.0f ? (lx -= splat[1]) : (lx += splat[1]);
    splat[3] < 0.0f ? (ly -= splat[3]) : (ly += splat[3]);
    lx += (float)sqrt(lowPassFilter);
    ly += (float)sqrt(lowPassFilter);
    lx *= cutoffRadius;
    ly *= cutoffRadius;
    uMax = (int)(centerTexture.u + lx) + 1;
    uMin = (int)(centerTexture.u - lx) - 1;
    vMax = (int)(centerTexture.v + ly) + 1;
    vMin = (int)(centerTexture.v - ly) - 1;

    // compute the conic matrix of the splat, where conic = ( splat*transpose(splat) + diag(lowPassfilter) )^(-1)
    // NOTE: the *conic* is used for rasterization (not the variance!)	
    // NOTE: we also add the low pass filter here!
    conic[0] = splat[2]*splat[2] + splat[3]*splat[3] + lowPassFilter;
    conic[1] = conic[2] = -(splat[0]*splat[2] + splat[1]*splat[3]);
    conic[3] = splat[0]*splat[0] + splat[1]*splat[1] + lowPassFilter;

    detConic = 1 / (splat[0]*splat[0]*splat[3]*splat[3] + 
		    splat[1]*splat[1]*splat[2]*splat[2] - 
		    2.0*splat[0]*splat[2]*splat[1]*splat[3] +
		    splat[0]*splat[0]*lowPassFilter + 
		    splat[1]*splat[1]*lowPassFilter + 
		    splat[2]*splat[2]*lowPassFilter + 
		    splat[3]*splat[3]*lowPassFilter + 
		    lowPassFilter*lowPassFilter );

    conic[0] *= detConic;
    conic[1] *= detConic;
    conic[2] *= detConic;
    conic[3] *= detConic;

    // step 2: rasterize the EWA ellipse

    // clipping
    if (uMin < 0) {
	    uMin = 0;
	    if(uMax < 0)
		return zeroSample;
    }
    if (uMax >= width) {
	    uMax = width - 1;
	    if(uMin >= width)
		return zeroSample;
    }
    if (vMin < 0) {
	    vMin = 0;
	    if(vMax < 0)
		return zeroSample;
    }
    if (vMax >= height) {
	    vMax = height - 1;
	    if(vMin >= height)
		return zeroSample;
    }

    u = ((float)uMin + 0.5f) - centerTexture.u;
    ddq = 2.0f * conic[0];
    weight = 0.f;

    // ellipse rasterization
    for (V = vMin; V <= vMax; V++) {

	    // finite differences for ellipse rasterization
	    v  = ((float)V + 0.5f) - centerTexture.v;
	    dq = conic[0] * (2 * u + 1) + conic[1] * 2.0f * v;
	    q  = (conic[3] * v + conic[1] * 2.0f * u) * v + conic[0] * u * u;

	    for (U = uMin; U <= uMax; U++) {

		    if (q < cutoffRadius*cutoffRadius) {

			    w = exp(-0.5*q);

	    		    if (diffColorChannel) {
				sample.redDiffColor += w*diffRedComponent->getValue (U, V);
				sample.greenDiffColor += w*diffGreenComponent->getValue (U, V);
				sample.blueDiffColor += w*diffBlueComponent->getValue (U, V);

				if (diffColorAlpha) {
				    sample.diffColorAlpha += w*diffColorAlpha->getValue(U, V);
				}
			    }

			    if (specColorChannel) {
				sample.redSpecColor += w*specRedComponent->getValue (U, V);
				sample.greenSpecColor += w*specGreenComponent->getValue (U, V);
				sample.blueSpecColor += w*specBlueComponent->getValue (U, V);

				if (specColorAlpha) {
				    sample.specColorAlpha += w*specColorAlpha->getValue(U, V);
				}
			    }

			    if (ambCoeffChannel) {
				sample.ambientCoeff += w*ambientCoefficient->getValue (U, V);

				if (ambientCoefficientAlpha) {
				    sample.ambientCoeffAlpha += w*ambientCoefficientAlpha->getValue(U, V);
				}
			    }
			    
			    if (diffCoeffChannel) {
				sample.diffCoeff += w*diffuseCoefficient->getValue (U, V);

				if (diffuseCoefficientAlpha) {
				    sample.diffCoeffAlpha += w*diffuseCoefficientAlpha->getValue(U, V);
				}
			    }

    			    if (specCoeffChannel) {
				sample.specCoeff += w*specularCoefficient->getValue (U, V);

				if (specularCoefficientAlpha) {
				    sample.specCoeffAlpha += w*specularCoefficientAlpha->getValue(U, V);
				}
			    }
			    
			    weight += w;
		    }

		    q  += dq;
		    dq += ddq;
	    }
    } 

    if(weight > 0) {
	sample.redDiffColor = sample.redDiffColor / weight * 255.f;
	sample.greenDiffColor =sample.greenDiffColor / weight * 255.f;
	sample.blueDiffColor = sample.blueDiffColor / weight * 255.f;
	sample.diffColorAlpha /= weight;
	sample.redSpecColor = sample.redSpecColor / weight * 255.f;
	sample.greenSpecColor = sample.greenSpecColor / weight * 255.f;
	sample.blueSpecColor = sample.blueSpecColor / weight * 255.f;
	sample.diffColorAlpha /= weight;
	sample.ambientCoeff /= weight;
	sample.ambientCoeffAlpha /= weight;
	sample.diffCoeff /= weight;
	sample.diffCoeffAlpha /= weight;
	sample.specCoeff /= weight;
	sample.specCoeffAlpha /= weight;
	sample.shininess /= weight;
    }

    return sample;
}

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
