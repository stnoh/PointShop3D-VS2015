// Title:   BitmapManagerDialogImpl.h
// Created: Thu Sep 25 14:31:22 2003
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

#ifndef __BITMAPMANAGERDIALOGIMPL_H_
#define __BITMAPMANAGERDIALOGIMPL_H_

#include <qstring.h>
#include <qbuttongroup.h>
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


#include "../../../Configuration/src/Configuration.h"
#include "../../../Brush/src/Brush.h"
#include "../../BitmapResourceTool/src/BitmapResourceTool.h"
#include "../UI/BitmapManagerDialog.h"

/**
 * This class implements a dialog, which allows the user to
 * choose combinations of bitmaps, and resize them to a specified common size.
 *
 * @author Mark Pauly
 * @version 1.2
 */
class BitmapManagerDialogImpl : public BitmapManagerDialogBase {
	
    Q_OBJECT

public:

	/**
	 * This method creates an instance of this <code>BitmapManagerDialogImpl</code>, if necessary,
	 * and returns it. Only one instance at a time exists (singleton pattern).
	 *
	 * @return an instance of this <code>BitmapManagerDialogImpl</code>
	 */
	static BitmapManagerDialogImpl *getInstance ();

	/**
	 * Destroys the instance of this <code>BitmapManagerDialogImpl</code>, if there
	 * is one.
	 */
	static void destroyInstance();

public slots:

	void handleGetAlphaFromProperty();
	void handlePropertySelectionChanged ();
	void handleClearPropertyBitmap ();
	void handleClearAlphaMask ();
	void handleCreateNewBrush();
	void handleUpdateBrush();
	void handleApplySizeFromCurrentBitmap();
	void handleSelectedBrushChanged (const Brush *newBrush);
	void handleConstProperty();
	void handleShapeAlpha();
	void handleColorChooserProperty();
	void handleClearAll();
	void handleSizeChanged();
	void handleFGColorChanged (QRgb newFGColor);


protected slots:

	void showEvent (QShowEvent *se);

private slots:

	void handlePropertyBitmapChanged (const QImage *newPropertyBitmap);
	void handleAlphaBitmapChanged (const QImage *newAlphaBitmap);

private:

	BitmapResourceTool *propertyResourceTool,
		               *alphaResourceTool;


	int                numOfProperties,
		               currentProperty;
	QImage             **propertyBitmaps,		// the bitmap manager dialog keeps its own copy of these bitmaps
	                   **alphaBitmaps;


	// singleton pattern
	BitmapManagerDialogImpl (QWidget* parent = 0, const char* name = 0, bool modal = false, WFlags flags = 0);
protected:	// To suppress warnings...
	virtual ~BitmapManagerDialogImpl();
private:


	void initDialog();
	void setSelectedBrush();
	void updateSelectedBrush();

	QImage* extractAlpha(const QImage *input);
	QImage* convertTo8bppGrayScale (const QImage *input);


	QImage *constBitmap;		// bitmaps generated locally in the brush builder tool
	QImage *constAlpha;
	QImage *gaussianAlpha;


	void setBrushModifiedState (bool brushModified);
};

#endif  // __BITMAPMANAGERDIALOGIMPL_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
