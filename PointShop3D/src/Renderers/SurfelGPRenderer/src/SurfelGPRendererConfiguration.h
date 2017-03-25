// Title:   SurfelGPRendererConfiguration.h
// Created: Thu Sep 25 14:51:18 2003
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

#ifndef _SURFELGPRENDERERCONFIGURATION_H_
#define _SURFELGPRENDERERCONFIGURATION_H_

#include <qwidget.h>

#include "../../../Core/Interfaces/src/RendererConfigurationInterface.h"
#include "../UI/SurfelGPRendererConfigurationWidget.h"

/**
 * This class implements the settings for the <emSurfelGP Renderer</em>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SurfelGPRendererConfiguration : public RendererConfigurationInterface {

	Q_OBJECT

public:

	SurfelGPRendererConfiguration();
	virtual ~SurfelGPRendererConfiguration();

	void setCutOffRadius (const float newSplatCutoffRadius);
	float getCutOffRadius() const;

	void setTwoSidedNormalsEnabled (const bool enabled);
	bool isTwoSidedNormalsEnabled() const;
	void setShadowsEnabled (const bool enabled);
	bool isShadowsEnabled() const;

	// ****************************************
	// interface RendererConfigurationInterface
	// ****************************************

	QWidget *createConfigurationWidget (QWidget *parent);
	void apply();

signals:

	void cutOffRadiusChanged (const float newCutOffRadius);
	void twoSidedNormalsChanged (const bool enable);

private:

	SurfelGPRendererConfigurationWidget *configurationWidget;
	float                               cutOffRadius;
	bool                                twoSidedNormals;
	bool                                shadows;
										 

};

#endif  // _SURFELGPRENDERERCONFIGURATION_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
