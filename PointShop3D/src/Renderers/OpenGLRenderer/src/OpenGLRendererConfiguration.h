// Title:   OpenGLRendererConfiguration.h
// Created: Thu Sep 25 15:44:58 2003
// Authors: Oliver Knoll, Mark Pauly, Tim Weyrich, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003, Computer Graphics Lab, ETH Zurich
//
// This file is part of the Pointshop3D system.
// See http://www.pointshop3d.com/ for more information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General
// Public License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330,
// Boston, MA 02111-1307 USA
// 
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#ifndef _OPENGLRENDERERCONFIGURATION_H_
#define _OPENGLRENDERERCONFIGURATION_H_

#include <qwidget.h>

#include "../../../Core/Interfaces/src/RendererConfigurationInterface.h"
#include "../UI/OpenGLRendererConfigurationWidget.h"

/**
 * This class implements the settings for the OpenGL renderer.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class OpenGLRendererConfiguration : public RendererConfigurationInterface {

	Q_OBJECT

public:

	OpenGLRendererConfiguration();
	virtual ~OpenGLRendererConfiguration();

	void setCutOffRadius (const float newCutoffRadius);
	float getCutOffRadius() const;

	void setConstantPointSizeEnabled (const bool enable);
	bool isConstantPointSizeEnabled() const;

	void setSmoothPointEnabled (const bool enable);
	bool isSmoothPointEnabled() const;

	void setUseDisplayListsEnabled (const bool enable);
	bool isUseDisplayListsEnabled() const;

	void setUseVertexShadersEnabled (const bool enable);
	bool isUseVertexShadersEnabled() const;


	// ****************************************
	// interface RendererConfigurationInterface
	// ****************************************

	QWidget *createConfigurationWidget (QWidget *parent);
	void apply();

signals:

	void cutOffRadiusChanged (const float newCutOffRadius);

	/**
	 * Emitted whenever the constant point size has changed.
	 *
	 * @param isEnabled
	 *        <code>true</code> if a constant point size is used; <code>false</code> if the point
	 *        size is calculated dynamically
	 */
	void constantPointSizeChanged (const bool isEnabled);

	void smoothPointChanged (const bool isEnabled);	
	void useDisplayListsChanged (const bool isEnabled);	
	void useVertexShadersChanged (const bool isEnabled);	

private:

	float  cutOffRadius;
	bool   constantPointSize;
	bool   smoothPoint;
    bool   useDisplayLists;
    bool   useVertexShaders;
	OpenGLRendererConfigurationWidget *configurationWidget;

};

#endif  // _OPENGLRENDERERCONFIGURATION_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
