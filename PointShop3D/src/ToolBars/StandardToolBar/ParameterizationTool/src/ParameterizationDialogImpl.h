// Title:   ParameterizationDialogImpl.h
// Created: Thu Sep 25 14:58:19 2003
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

#ifndef __PARAMETERIZATIONDIALOGIMPL_H_
#define __PARAMETERIZATIONDIALOGIMPL_H_

#include <qstring.h>
#if QT_VERSION >= 0x030000
# include <qptrlist.h>
# ifndef QList
#  define QList QPtrList
# endif
# ifndef QListIterator
#  define QListIterator QPtrListIterator
# endif
#else
# include <qlist.h>
#endif

#include "../../../../Core/Brush/src/Brush.h"
#include "../../../../Core/MarkerManager/src/MarkerManager.h"
#include "../UI/ParameterizationDialog.h"
#include "../UI/ExtendedSettings.h"
#include "BitmapMarker.h"

// forward declaration as to implement circular dependency: "ParameterizationTool" needs "ParamterizationDialogImpl"
// and vice versa - since this circular dependency is very local, it's okay for now...
class ParameterizationTool;

/**
 * This class implements a dialog, which allows the user to select the bitmap and mark it
 * with markers, which specify the mapping between these 2D-Markers and the 3D-Markers in the
 * object.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class ParameterizationDialogImpl : public ParameterizationDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>ParameterizationDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @param ParameterizationTool
	 *        a pointer to the <code>ParameterizationTool</code>
	 * @return a pointer to an instance of this <code>ParameterizationDialogImpl</code>
	 */
	static ParameterizationDialogImpl *getInstance (ParameterizationTool *parameterizationTool);

	/**
	 * Destroys the instance of this <code>ParameterizationDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	void handleRemoveSelected2D();
	void handleRemoveAll2D();
	void handleRemoveSelected3D();
	void handleRemoveAll3D();
	
	void handleMarkPropertyBitmap();

	void handlePropertyChosen (const QString &newProperty);

	void handleClearSelection();
	void handleSelectAll();
	void handleUVGeneration();
	void handleExtensions (bool enable);
	void handleShowAlpha (bool enable);	
	void handleParameterizationTypeChanged();

private:

	ExtendedSettings          *extendedSettings;
	ParameterizationTool      *parameterizationTool;
	BitmapMarker              *bitmapMarker;

	const Brush               *currentBrush;
	SurfelInterface::Property currentProperty;

	float                     precision,
		                      coverageThreshold,
							  displacementScaling,
							  lowPassFilter,
							  fittingConstrWeights;
	uint                      nofLevels,
		                      clusterSize;

	// singleton pattern
	ParameterizationDialogImpl (ParameterizationTool *parameterizationTool,
		                        QWidget *parent = 0,
					            const char *name = 0,
					            bool modal = false,
					            WFlags flags = 0);
protected:  // To suppress warnings...
    virtual ~ParameterizationDialogImpl();
private:

	void initDialog();
	// updates the pixmaps according to the current brush
	void updatePixmaps();	
	
	// checks if there are markers, same number of 2D and 3D markers and if there is a selection
	// if yes, the 'Generate UV Coordinates' button is enabled
	void checkConditions();


	void orthoParameterize (unsigned int uIndex, unsigned int vIndex);


private slots:

	void handleBrushChanged (const Brush *newBrush);

	void handleFittingConstraints3DChanged (const QList<Marker3D> *fittingConstraint3DList);
	void handleFittingConstraints2DChanged (const QList<Marker2D> *fittingConstraint2DList);

	void handleSelectionChanged (SurfelInterface::Flag selectionFlag);

};

#endif  // __PARAMETERIZATIONDIALOGIMPL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
