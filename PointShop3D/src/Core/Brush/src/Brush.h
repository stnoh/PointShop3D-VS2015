// Title:   Brush.h
// Created: Thu Sep 25 14:31:38 2003
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

#ifndef __BRUSH_H_
#define __BRUSH_H_

#include <qsize.h>
#include <qpopupmenu.h>

#include "../../src/CoreDLL.h"
#include "../../Interfaces/src/SurfelInterface.h"
#include "BrushChannel.h"


/**
 * The brush class is a central component for surface editing in Pointshop3D. Brushes store a set
 * brush channels, one channel for each editable surface propertey. The channels define how the 
 * surface should be modified when the brush is applied.
 *
 * Access to brush channels is provided via their respective surface property, i.e., 
 * <code>SurfelInterface::Property</code>. Data is passed to brush channels as <code>QImage</code>s
 * and stored in an internal format.
 *
 * @see BrushChannel
 * @see SurfelInterface
 *
 * @author Matthias Zwicker
 * @version 1.2
 */
class Brush {

public:


	/**
	 * File identification string for Pointshop3D brush description files.
	 */
	static const char PBD_FILE_ID[9];

	/**
	 * Construct a brush with a given name and size.
	 */
	CORE_API Brush (const QString name, const QSize brushSize);

	/**
	 * Default constructor.
	 */
	CORE_API Brush();

	/**
	 * Construct a brush by copying the data from a given brush.
	 */
	CORE_API Brush (const Brush *src);

	CORE_API virtual ~Brush();

	/**
	 * Load a brush channel from a .pbd (Pointshop3D brush descriptor) file.
	 */
	CORE_API int load (const QString currentDirectory, const QString currentFileName);

	/**
	 * Save a brush channel to a .pbd (Pointshop3D brush descriptor) file.
	 * NOTE: if the brush stores its own directory, the <code>currentDirectory</code> parameter
	 * is ignored. If the brush does not store its own directory, it is saved to the
	 * <code>currentDirectory</code>.
	 */
	CORE_API int save (const QString currentDirectory);

	// access to brush channels

	/**
	 * Generates a brush channel from the given bitmap. 
	 *
	 * - This method gets the number of required channel components using 
	 * <code>SurfelInterface::getNumOfPropertyBrushComponents()</code>.
	 * - All values contained in the brush channel components will be in
	 * the range [0..1].
	 * - The <code>scale</code> parameter is used to scale each value in the 
	 * bitmap when it is inserted into the brush
	 *
	 * @see SurfelInterface
	 * @see BrushChannel
	 */ 
	CORE_API void setChannel (const SurfelInterface::Property surfelProperty, const float scale, const QImage *bitmap);

	/**
	 * Remove a channel for a given surfel property from the brush.
	 * @see SurfelInterface
	 */
	CORE_API void removeChannel (const SurfelInterface::Property surfelProperty);

	/**
	 * Remove all channels from the brush.
	 */
	CORE_API void removeAllChannels();

	/**
	 * Check whether the brush has data for a certain channel, i.e., surfel property.
	 * @see SurfelInterface
	 */
	CORE_API bool hasChannel (const SurfelInterface::Property surfelProperty) const;

	/**
	 * Get the brush channel for a given surfel property.
	 * @see SurfelInterface
	 */
	CORE_API BrushChannel* getChannel (const SurfelInterface::Property surfelProperty) const;

	// access to alpha channels
	
	/** 
	 * Generates a brush alpha channel from the given bitmap. Alpha channels always have one brush
	 * channel component. The brush channel component will contain values in the range [0..1].
	 * @see SurfelInterface
	 */
	CORE_API void setAlphaChannel (const SurfelInterface::Property surfelProperty, const QImage *bitmap);

	/**
	 * Remove the alpha channel for a given surfel property from the brush.
	 * @see SurfelInterface
	 */
	CORE_API void removeAlphaChannel (const SurfelInterface::Property surfelProperty);

	/**
	 * Remove all alpha channels from the brush.
	 */
	CORE_API void removeAllAlphaChannels();

	/**
	 * Check whether the brush has an alpha channel for a given property.
	 * @see SurfelInterface
	 */
	CORE_API bool hasAlphaChannel (const SurfelInterface::Property surfelProperty) const;

	/**
	 * Get the alpha channel of the brush for a given surfel property.
	 * @see SurfelInterface
	 */
	CORE_API BrushChannel* getAlphaChannel (const SurfelInterface::Property surfelProperty) const;

	/**
	 * Get the name of the brush.
	 */
	CORE_API QString getName() const;

	/**
	 * Set the name of the brush.
	 */
	CORE_API void setName (const QString name);

	/**
	 * Get the size of the brush.
	 */
	CORE_API QSize getSize() const;

	/**
	 * Set the size of the brush.
	 */
	CORE_API void setSize (const QSize size);

	/**
	 * Return a preview image of the brush. The preview image is used e.g. for display in the <code>
	 * BrushChooserTool</code>.
	 *
	 * @param width
	 *        width of single bitmap in pixels
	 * @param height
	 *        height of single bitmap in pixels
	 * @param useAlphaMasks
	 *        true if alpha masks are incorporated into bitmaps
	 *
	 * @see BrushChooserTool
	 */
	CORE_API QImage *createPreviewImage (unsigned int width, unsigned int height, bool useAlphaMasks);

	/**
	 * Return the number of used channels in the brush.
	 */
	CORE_API int getNumOfUsedChannels() const;

	/**
	 * Get the menu id of the brush. This id is used in the <code>BrushChooserTool</code>.
	 * @see BrushChooserTool
	 */
	CORE_API int getMenuId() const;

	/**
	 * Set the menu id of the brush. This id is used in the <code>BrushChooserTool</code>.
	 * @see BrushChooserTool
	 */
	CORE_API void setMenuId (const int newId);

	/**
	 * Set a pointer to the popup menu of the <code>BrushChooserTool</code> that displays
	 * this brush.
	 */
	CORE_API void setPopupMenu(QPopupMenu *popupMenu);

	/**
	 * Return a pointer to the popup menu of the <code>BrushChooserTool</code> that displays
	 * this brush. The return value might be zero in case the brush has not been added to 
	 * the brush registry which is managed by the <code>BrushChooserTool</code>.
	 * @see BrushChooserTool
	 */
	CORE_API QPopupMenu* getPopupMenu();


private:
 
	/**
	 * Given a surfel property, return the index of the corresponding channel in the internal
	 * channels array. If the property does not have a corresponding channel, return -1.
	 **/
	int getChannelIndex (const SurfelInterface::Property surfelProperty) const;
	void initBrush (const QString name, const QSize size);

	QString name;								// name of the brush
	QString directory;							// directory relative to the Pointshop3D brush directory
												// where this brush should be saved

	QSize size;									// size of the brush

	unsigned int nChannels;						// number of channels in the brush

	BrushChannel **channels;					// array of pointers to brush channels
	BrushChannel **alphaChannels;				// array of pointers to alpha channels used for blending
	SurfelInterface::Property *properties;		// the surfel properties corresponding to the brush channels

	int menuId;					            	// id for brush chooser menu, used for updates
	QPopupMenu *popupMenu;						// popup menu in the brush chooser tool that contains this brush
};


#endif  // __BRUSH_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
