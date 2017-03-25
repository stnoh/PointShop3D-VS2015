// Title:   Marker2D.cpp
// Created: Thu Sep 25 14:16:16 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
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

#include <qwidget.h>
#include <qpoint.h>
#include <qpainter.h>
#include <qfont.h>
#include <qrect.h>
#include <qpen.h>
#include <qcolor.h>

#include "../../../Utilities/src/MyMathTools.h"
#include "../../../DataTypes/src/Vector2D.h"
#include "../../../DataTypes/src/MyDataTypes.h"
#include "Marker2D.h"

static const float THRESHOLD = 8.0f;

Marker2D::Marker2D (const MyDataTypes::TextureCoordinate textureCoordinate, const bool selected, const uint id, const Style style) {

	this->textureCoordinate = textureCoordinate;
	this->selected          = selected;
	this->id                = id;
	this->style             = style;

	eraseOldPosition = true;

}

Marker2D::~Marker2D() {}

float Marker2D::getNormalX() const {
	return textureCoordinate.u;
}

float Marker2D::getNormalY() const {
	return textureCoordinate.v;
}

bool Marker2D::isClicked (const QPoint mousePosition, const QWidget *widget) {

	Vector2D mousePos,
		     markerPos;
	QPoint   position;

	position = this->getPosition (widget);
	markerPos[0] = position.x();
	markerPos[1] = position.y();
	mousePos[0]  = mousePosition.x();
	mousePos[1]  = mousePosition.y();

	return (MyMathTools::squaredEuklideanDistance2D (markerPos, mousePos) < THRESHOLD);

}

void Marker2D::setID (const uint newID) {
	id = newID;
}

uint Marker2D::getID() const {
	return id;
}

QPoint Marker2D::getPosition (const QWidget *widget) const {
	
	QPoint position;

	position.setX ((int)((float)widget->size().width()  * textureCoordinate.u + 0.5f));	
	position.setY ((int)((float)widget->size().height() * textureCoordinate.v + 0.5f));
	return position;

}

void Marker2D::setSelected (const bool selected) {
	this->selected = selected;
}

bool Marker2D::isSelected() const {
	return selected;
}

void Marker2D::setStyle (const Style newStyle) {
	style = newStyle;
}

MarkerInterface::Style Marker2D::getStyle() const {
	return style;
}

void Marker2D::togglePainting (QWidget *widget) {

	QPainter painter;
	QFont    font;
	QPoint   position;

	position = this->getPosition (widget);

	painter.begin (widget);
	painter.setPen (QPen (Qt::white));
	// XOR mode
	painter.setRasterOp (Qt::XorROP);

	switch (style) {

	case BOUNDARY_CONDITION_STYLE:
		{

			font = painter.font();
			font.setPointSize (12);
			font.setBold (true);
			

			if (selected == true) {
				painter.drawRect (position.x() - 5, position.y() - 5, 11, 11);
				font.setUnderline (true);
			}

			// draw a rectangle, 5 pixel border
			painter.setPen (QPen (QColor (255, 255, 255), 5));
			painter.drawRect (position.x() - 5, position.y() - 5, 11, 11);	

			painter.setFont (font);
			// draw the id
			painter.drawText (position.x() + 7, position.y() - 7, QString::number (id));

		}
		break;

	case FITTING_CONSTRAINT_STYLE:
		{

			font = painter.font();
			font.setPointSize (12);
			font.setBold (true);
			

			if (selected == true) {
				painter.drawEllipse (position.x() - 5, position.y() - 5, 11, 11);
				font.setUnderline (true);
			}

			// draw a circle, 5 pixel border
			painter.setPen (QPen (QColor (255, 255, 255), 5));
			painter.drawEllipse (position.x() - 5, position.y() - 5, 11, 11);	

			painter.setFont (font);
			// draw the id
			painter.drawText (position.x() + 7, position.y() - 7, QString::number (id));

		}
		break;

	default:
		break;

	}

	painter.end();	

}

bool Marker2D::moveTo (const QPoint newPosition, QWidget *widget) {

	QRect area;

	area = QRect (QPoint (0,0), widget->size());

	// erase the old position
	if (eraseOldPosition == true) {
		this->togglePainting (widget);
		eraseOldPosition = false;
	}

	// update normalized position
	this->updateNormalPosition (newPosition, widget);

	if (area.contains (newPosition) == true) {

		// redraw at new position
		this->togglePainting (widget);
		eraseOldPosition = true;
		return true;

	}
	else {
		return false;
	}

}

// ***************
// private methods
// ***************

void Marker2D::updateNormalPosition (const QPoint position, const QWidget *widget) {
	textureCoordinate.u = (float)position.x() / (float)(widget->size().width());
	textureCoordinate.v = (float)position.y() / (float)(widget->size().height());
}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
