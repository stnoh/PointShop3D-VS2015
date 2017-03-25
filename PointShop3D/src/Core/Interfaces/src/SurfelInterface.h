// Title:   SurfelInterface.h
// Created: Thu Sep 25 14:29:01 2003
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

#ifndef __SURFELINTERFACE_H_
#define __SURFELINTERFACE_H_

#include <qcolor.h>
#include <qstring.h>
#include <qstringlist.h>
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../DataStructures/src/AssociatedSurfels.h"

#include "../../src/CoreDLL.h"

////////////////// linux portability ////////////////

#ifndef __max
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef __min
#define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

/////////////////////////////////////////////////////

class SurfelCollection;

/**
 * This interface defines the API for surfels, surface elements, which are the atomic elements of
 * which <code>Object</code>s consist of (instead of triangles). Think of them as points
 * with a normal, which defines the orientation of the circle (or ellipse) which is drawn
 * as soon as the point (surfel) is rendered.
 *
 * This interface provides some static methods for converting between user- and system readable
 * surfel properties.
 *
 * <b>Note:</b> only the <code>SurfelCollection</code> class is allowed to create and <code>delete</code>
 *              individual surfels (<code>addSurfel</code> and <code>removeSurfel</code>) so that the 
 *				<code>SurfelCollection</code> can maintain a consistent state.
 *              Additionally each property can only be set via
 *              the <code>Scene</code> so renderers can be informed accordingly.
 *
 * @author Oliver Knoll, Richard Keiser
 * @version 2.0
 */
class SurfelInterface {

public:

	/**
	 * Flags which can be OR'ed together - add new flag values here. Up to 32 flags are supported.
	 */
	typedef enum flag {

		NO_FLAGS			= 0x00000000,   // no flags set
		SELECTED1			= 0x00000001,   // set when this surfel is part of selection 1 - does NOT render the surfel differently
									        // set the flag EMPHASISE as well in this case
		SELECTED2			= 0x00000002,   // set when this surfel is part of selection 2 - does NOT render the surfel differently
									        // set the flag EMPHASISE as well in this case
		SELECTED3			= 0x00000004,   // set when this surfel is part of selection 3 - does NOT render the surfel differently
								            // set the flag EMPHASISE as well in this case
		EMPHASISE			= 0x00000008,   // set when the surfel should be rendered differently, e.g. when selected
		COVERED				= 0x00000010,   // used internally (only for temporary computations!)
		INSIDE				= 0x00000020,   // used for CSG calculations, true if point is inside other model
		FEATURE				= 0x00000040,   // determines if surfel is feature surfel
		CLIP_ORIENTATION	= 0x00000080,   // set if union csg (positive clipping) see renderer
		CLIP_BOUNDARY_A	    = 0x00000100,	// surfel close to boundery A that needs to be clipped
		CLIP_BOUNDARY_B     = 0x00000200,	// surfel close to boundery B that needs to be clipped
		INVISIBLE			= 0x00000400,	// set the flag if the surfel should not be rendered
		ALL_FLAGS			= 0xffffffff	// all flags set

	} Flag;

	/**
	 * Consists of up to 32 OR'ed <code>Flag</code> values.
	 *
	 * @see #Flag
	 */
	typedef int Flags;

	/**
	 * Properties which can be OR'ed together. Up to 32 properties are supported.
	 */
	typedef enum property {

		NO_PROPERTIES        = 0x00000000,  // no properties
		POSITION			 = 0x00000001,  // the position of the surfel
		NORMAL				 = 0x00000002,	// the normal of the surfel
		RADIUS				 = 0x00000004,  // the radius of the surfel
		DIFFUSE_COLOR		 = 0x00000008,	// the diffuse color of the surfel
		SPECULAR_COLOR		 = 0x00000010,	// the specular color of the surfel
		FLAGS                = 0x00000020,	// the flags of the surfel
		AMBIENT_COEFFICIENT  = 0x00000040,  // the ambient coefficient 
		DIFFUSE_COEFFICIENT  = 0x00000080,  // the diffuse coefficient
		SPECULAR_COEFFICIENT = 0x00000100,  // the specular coefficient
		SHININESS            = 0x00000200,  // the shininess
		TEXTURE_COORDINATE   = 0x00000400,  // the texture UV coordinates
		TANGENT_AXES         = 0x00000800,  // the tangent axes
		ASSOCIATED_SURFELS	 = 0x00001000,	// associated surfels
		DETAIL				 = 0x00002000,	// detail vectors and coefficients
		ALL_PROPERTIES		 = 0xffffffff	// all properties

	} Property;

	/**
	 * Consists of up to 32 OR'ed <code>Property</code> values, describing which properties
	 * of a <code>SurfelInterface</code> are supported/changed.
	 *
	 * @see #Property
	 */
	typedef int PropertyDescriptor;

	
	/**
	 * Properties of a base surfel (for consistency with Pointshop3D Version 1.2)
	 */
	CORE_API static const SurfelInterface::PropertyDescriptor BaseSurfel_PropertyDescriptor;
	/**
	 * Properties of an el surfel (for consistency with Pointshop3D Version 1.2)
	 */
	CORE_API static const SurfelInterface::PropertyDescriptor ElSurfel_PropertyDescriptor;
	/**
	 * Properties of an enhanced el surfel
	 */
	CORE_API static const SurfelInterface::PropertyDescriptor EnhElSurfel_PropertyDescriptor;
	/**
	 * Properties of a surfel1 surfel (for consistency with Pointshop3D Version 1.2)
	 */
	CORE_API static const SurfelInterface::PropertyDescriptor Surfel1_PropertyDescriptor;

	/**
	 * Returns a user readable <code>QString</code> for the <code>property</code>.
	 *
	 * @param property
	 *        the <code>Property</code> to be translated into a user-readable <code>QString</code>
	 * @return a <code>QString</code> containing a user-readable text for the <code>property</code>;
	 *         a default value is returned in case the <code>property</code> isn't supported yet
	 * @see Property
	 */
	CORE_API static QString getUserReadableProperty (const Property property);

	/**
	 * Returns a "system" readable <code>Property</code> for the user-readable <code>QString</code>.
	 *
	 * @param userReadableProperty
	 *        a <code>QString</code> containing a user-readable text for a <code>Property</code>
	 * @return a "system" readable <code>Property</code>; in case the <code>userReadableProperty</code>
	 *         cannot be mapped to one of the system <code>Properties</code> <code>NO_PROPERTIES</code> is
	 *         returned
	 * @see Property
	 */
	CORE_API static SurfelInterface::Property getSystemProperty (const QString userReadableProperty);

	/**
	 * Returns a <code>QStringList</code> which contains user readable <code>QString</code>s for
	 * all editable <code>Properties</code> which is useful for <code>QComboBox</code>es, for example.
	 * The entries in this <code>QStringList</code> can be mapped to system <code>Properties</code>
	 * with <code>getSystemProperty</code>.
	 *
	 * @return a <code>QStringList</code> containing user readable <code>QString</code>s for all
	 *         editable <code>Properties</code>
	 * @see #getSystemProperty
	 */
	CORE_API static QStringList getEditableProperties();

	/**
	 * Returns a property descriptor that only lists the editable properties.
	 *
	 * @return a property descriptor that only lists the editable properties
	 */
	CORE_API static PropertyDescriptor getEditablePropertyDescriptor();

	/**
	 * Returns the number of editable properties.
	 *
	 * @return the number of editable properties
	 */
	CORE_API static int getNumOfEditableProperties();

	/**
	 * Returns the number of data components that should be provided by a brush to paint given
	 * a surfel property (e.g., 1 for scalar attributes,3 for RGB color).
	 * 
	 * <b>Note:</b> this method is only useful for properties that can be painted by a brush!
	 *
	 * @return the number of data components that should be provided by a brush to paint
	 */
	CORE_API static int getNumOfPropertyBrushComponents(Property property);

	/**
     * Returns the number of supported selections.
	 *
	 * @return the number of supported selections
	 */
	CORE_API static int getNumOfSelections();
	
	/**
     * Returns all selection flags OR'ed together.
	 *
	 * @return all selection flags OR'ed together
	 */
	CORE_API static Flags getSelectionFlags();

	/**
	 * Returns the position.
	 *
	 * @return a <code>Vector3D</code> containing the position
	 * @see Scene#setSurfelPosition
	 */

    inline Vector3D getPosition() const;

	/**
	 * Returns the normal.
	 *
	 * @return a <code>Vector3D</code> containing the normal
	 * @see Scene#setSurfelNormal
	 */
	inline Vector3D getNormal() const;

	/**
	 * Returns the axis 1.
	 *
	 * @return a <code>Vector3D</code> containing the axis 1
	 * @see Scene#setSurfelTangentAxes
	 */
	inline Vector3D getAxisOne() const;

	/**
	 * Returns the axis 2.
	 *
	 * @return a <code>Vector3D</code> containing the axis 2
	 * @see Scene#setSurfelTangentAxes
	 */
	inline Vector3D getAxisTwo() const;

	/**
	 * Returns the radius.
	 *
	 * @return the radius
	 * @see Scene#setSurfelRadius
	 */
	inline float getRadius() const;

	/**
	 * Returns the diffuse color.
	 *
	 * @return the <code>QRgb</code> diffuse color
	 * @see Scene#setSurfelDiffuseColor
	 */
	inline QRgb getDiffuseColor() const;

	/**
	 * Returns the specular color.
	 *
	 * @return the <code>QRgb</code> specular color
	 * @see Scene#setSurfelSpecularColor
	 */
	inline QRgb getSpecularColor() const;

	/**
	 * Returns <code>true</code> if <em>all</em> flags specified in <code>flags</code>
	 * are set to <code>true</code>.
	 *
	 * @param flags
	 *        OR'ed <code>Flag</code> values which specify the <code>Flags</code> to be tested
	 * @return <code>true</code> if <em>all</em> flags specified in <code>flags</code>
	 *         are set to <code>true</code>; <code>false</code> else
	 * @see Scene#setSurfelFlagOn
	 */
	inline bool isFlagOn (const Flags flags) const;

	/**
	 * Returns <code>true</code> if this <code>SurfelInterface</code> is selected.
	 *
	 * @return <code>true</code> if this <code>SurfelInterface</code> is selected; <code>false</code> else
	 * @see Scene#setSurfelSelected
	 */ 
	inline bool isSelected() const;

	/**
	 * Returns the <code>Flags</code>.
	 *
	 * @return the <code>Flags</code>
	 * @see Scene#setSurfelFlags
	 */
	inline Flags getFlags() const;

	/**
	 * Returns the ambient coefficient.
	 *
	 * @return the ambient coefficient
	 * @see Scene#setSurfelAmbientCoefficient
	 */
	inline float getAmbientCoefficient() const;

	/**
	 * Returns the diffuse coefficient.
	 *
	 * @return the diffuse coefficient
	 * @see Scene#setSurfelDiffuseCoefficient
	 */
	inline float getDiffuseCoefficient() const;

	/**
	 * Returns the specular coefficient.
	 *
	 * @return the specular coefficient
	 * @see Scene#setSurfelSpecularCoefficient
	 */
	inline float getSpecularCoefficient() const;

	/**
	 * Returns the shininess.
	 *
	 * @return the shininess
	 * @see Scene#setSurfelShininess
	 */
	inline float getShininess() const;

	/**
	 * Returns the <code>TextureCoordinate</code>.
	 *
	 * @return the <code>TextureCoordinate</code>
	 * @see Scene#setSurfelTextureCoordinate
	 */
	inline MyDataTypes::TextureCoordinate getTextureCoordinate() const;

	/**
	 * Returns the number of associated surfels.
	 *
	 */
	inline unsigned int getNofAssociatedSurfels () const;

	/**
	 * Gets the associated surfels
	 * 
	 * @return the associated <code>SurfelInterface</code>. Returns 0 if the index is out of range
	 */
	inline SurfelInterface* getAssociatedSurfel (unsigned int idx) const;

	/**
	 * Gets the detail vector
	 * <p>Used for multiresolution.</p>
	 *
	 * @return the detail vector
	 */
	inline Vector3D getDetailVector () const;
	
	/**
	 * Gets the detail coefficient
	 * <p>Used for multiresolution.</p>
	 *
	 * @return the detail coefficient
	 */
	inline float getDetailCoefficient () const;

	/**
	 * Sets the detail vector
	 * <p>Used for multiresolution.</p>
	 *
	 * @param detailVector
	 *		the detail vector
	 */
	inline void setDetailVector (const Vector3D detailVector);

	/**
	 * Sets the detail coefficient
	 * <p>Used for multiresolution.</p>
	 *
	 * *param detailCoefficient
	 *		the detail coefficient
	 */
	inline void setDetailCoefficient (const float detailCoefficient);

public:

	/**
	 * Sets the position.
	 *
	 * @param newPosition
	 *        a <code>Vector3D</code> containing the new position
	 */
	inline void setPosition (const Vector3D newPosition);

	/**
	 * Sets the normal.
	 *
	 * <b>Note:</b>	If the <code>Property TANGENT_AXES</code> is set the tangent axes are recomputed such
	 *				that they are orthogonal to the normal vector. The length of the axes is computed as
	 *				the average of the previous axes length, therefore the surfels are circular afterwards.
	 *				<p>If you want to set both the normal and the tangent axes use
	 *				<code>setSurfelNormalTangentAxesSystem</code> instead.</p>
	 *
	 * @param newNormal
	 *        a <code>Vector3D</code> containing the new normal
	 * @see #Property
	 */
	inline void setNormal (const Vector3D newNormal);

	/**
	 * Sets the tangent axes.
	 * <b>Note:</b>	If the <code>Property NORMAL</code> is set the normal is recalculated. If you
	 *				want to set both the normal and the tangent axes use
	 *				<code>setSurfelNormalTangentAxesSystem</code> instead.
	 *				<p>If the <code>Property RADIUS</code> is set the radius is recomputed from the
	 *				tangent axes </p>
	 *
	 * @param newAxisOne
	 *        a <code>Vector3D</code> containing the new axis 1
	 *
	 * @param newAxisTwo
	 *        a <code>Vector3D</code> containing the new axis 2
	 * @see #Property
	 */
	inline void setTangentAxes (const Vector3D newAxisOne, const Vector3D newAxisTwo);

	/**
	 * Sets the normal and the tangent axes.
	 *
	 * <b>Note:</b>	If the <code>Property RADIUS</code> is set the radius is recomputed from the
	 *				tangent axes
	 *
	 * @param normal
	 *		  a <code>Vector3D</code> containing the new normal
	 * @param newAxisOne
	 *        a <code>Vector3D</code> containing the new axis 1
	 *
	 * @param newAxisTwo
	 *        a <code>Vector3D</code> containing the new axis 2
	 * @see #Property
	 */
	inline void setNormalTangentAxesSystem (const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);

	/**
	 * Sets the radius.
	 *
	 * <b>Note:</b>	If the <code>Property TANGENT_AXES</code> is set the 
	 *				length of the tangent axes is set to radius.
	 *
	 * @param newRadius
	 *        the new radius
	 */
	inline void setRadius (const float newRadius);

	/**
	 * Computes the color attributes from <code>copySurfel1</code> and <code>copySurfel2</code>.
	 *
	 * <b>Note:</b>	The scalar values are computed as follows: 
	 *				val = (factor * copySurfel1->val) + ((1.0 - factor) * copySurfel2->val);
	 *				The following properties are computed:
	 *				<code>SurfelInterface::DIFFUSE_COLOR</code>,
	 *				<code>SurfelInterface::SPECULAR_COLOR</code>,
	 *				<code>SurfelInterface::DIFFUSE_COEFFICIENT</code>,
	 *				<code>SurfelInterface::SPECULAR_COEFFICIENT</code>,
	 *				<code>SurfelInterface::AMBIENT_COEFFICINET</code>,
	 *				<code>SurfelInterface::SHININESS</code>
	 *
	 * @param copySurfel1
	 *        a pointer to a <code>SurfelInterface</code> to copy the color attributes
	 * @param copySurfel2
	 *        a pointer to a <code>SurfelInterface</code> to copy the color attributes
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see SurfelInterface#Property
	 */
	inline void combineColorProperties(SurfelInterface *copySurfel1, SurfelInterface *copySurfel2, float factor);

	/**
	 * Sets the diffuse color.
	 *
	 * @param newDiffuseColor
	 *        the new <code>QRgb</code> diffuse color
	 */
	inline void setDiffuseColor (const QRgb newDiffuseColor);

	/**
	 * Sets the specular color.
	 *
	 * @param newSpecularColor
	 *        the new <code>QRgb</code> specular color
	 */
	inline void setSpecularColor (const QRgb newSpecularColor);

	/**
	 * Sets <em>all</em> the flags specified in <code>flags</code> to the value <code>on</code>.
	 * 
	 * @param flags
	 *        specifies which flags are to be set/unset
	 * @param on
	 *        the value of each flag to be set/unset
	 */
	inline void setFlagOn (const Flags flags, const bool on);
	
	/**
	 * Sets the flags of this <code>SurfelInterface</code> to <code>newFlags</code>.
	 *
	 * @param newFlags
	 *        the new <code>Flags</code> of this <code>SurfelInterface</code>
	 */
	inline void setFlags (const Flags newFlags);

	/**
	 * Sets the ambient coefficient.
	 *
	 * @param newAmbientCoefficient
	 *        the new ambient coefficient
	 */
	inline void setAmbientCoefficient (const float newAmbientCoefficient);

	/**
	 * Sets the diffuse coefficient.
	 *
	 * @param newDiffuseCoefficient
	 *        the new diffuse coefficient
	 */
	inline void setDiffuseCoefficient (const float newDiffuseCoefficient);

	/**
	 * Sets the specular coefficient.
	 *
	 * @param newSpecularCoefficient
	 *        the new specular coefficient
	 */
	inline void setSpecularCoefficient (const float newSpecularCoefficient);

	/**
	 * Sets the shininess.
	 *
	 * @param newShininess
	 *        the new shininess
	 */
	inline void setShininess (const float newShininess);

	/**
	 * Sets the texture coordinate.
	 *
	 * @param newTextureCoordinate
	 *        the new <code>TextureCoordinate</code>
	 */
	inline void setTextureCoordinate (const MyDataTypes::TextureCoordinate newTextureCoordinate);

	/**
	 * Copies the attributes of the <code>sourceSurfel</code> to this surfel.
	 *
	 * @param sourceSurfel
	 *        a pointer to the source <code>SurfelInterface</code> which is to be copied
	 *        into this <code>SurfelInterface</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #setAutoDelete
	 */
	inline void copy (const SurfelInterface *sourceSurfel);

	/**
	 * Adds a surfel into the list of associated surfels. Each surfel can store a list of
	 * associated surfels. This mechanism allows grouping of surfels, e.g. for feature surfel
	 * clipping.
	 *
	 * @param surfel
	 *        the surfel to be added into the list of associated surfels.
	 *
	 * @return true if surfel could be added to the list. False if adding failed. This can happen if the
	 * list has a fixed size and no more surfels can be added.
	 */
	inline bool addAssociatedSurfel (SurfelInterface *surfel);
		
   /**
	* Clears the list of associated surfels.
	*/
	inline void clearAssociatedSurfels ();

   /**
    * Gets the surfel collection to which the surfel belongs
	*
	* @return the corresponding surfel collection
	*/
	inline SurfelCollection* getSurfelCollection();

   /**
    * Returns the surfel index
	*
	* @return the surfel index
	*/
	inline int getIndex() const;

protected:

	friend class Scene;
	friend class SurfelCollection;   

	/**
	 * initialize surfel with index and collection to which it belongs
	 * Is called by <code>SurfelCollection</code> only
	 *
	 * @param index
	 *		surfel index of collection
	 * @param sc
	 *		belonging collection
	 */
	inline void init(const int index, SurfelCollection *sc);
	
protected:
	/**
	 * The surfel collection to which this surfel belongs
	 */
	SurfelCollection *sc;
	/**
	 * index in the surfel collection
	 */
	int index;

};

#include "../../DataStructures/src/SurfelCollection.h"


// **************
// INLINE METHODS
// **************
inline int SurfelInterface::getIndex() const {

	return index;
}

inline SurfelCollection* SurfelInterface::getSurfelCollection() {
	return sc;
}

inline void SurfelInterface::init(const int index, SurfelCollection *sc) {
	this->index = index;
	this->sc	= sc;
}


inline Vector3D SurfelInterface::getPosition() const {
	return (sc->*(sc->_getPosition))(index);
}

inline Vector3D SurfelInterface::getNormal() const {
	return (sc->*(sc->_getNormal))(index);
}

inline Vector3D SurfelInterface::getAxisOne() const {
	return (sc->*(sc->_getAxisOne))(index);	
}

inline Vector3D SurfelInterface::getAxisTwo() const {
	return (sc->*(sc->_getAxisTwo))(index);	
}

inline float SurfelInterface::getRadius() const {
	return (sc->*(sc->_getRadius))(index);
}

inline QRgb SurfelInterface::getDiffuseColor() const {
	return (sc->*(sc->_getDiffuseColor))(index);
}

inline QRgb SurfelInterface::getSpecularColor() const {
	return (sc->*(sc->_getSpecularColor))(index);
}

inline bool SurfelInterface::isFlagOn (const Flags flags) const {
	return (sc->*(sc->_isFlagOn))(index, flags);
}

inline bool SurfelInterface::isSelected() const {
	return (sc->*(sc->_isSelected))(index);
}

inline SurfelInterface::Flags SurfelInterface::getFlags() const {
	return (sc->*(sc->_getFlags))(index);
}

inline float SurfelInterface::getAmbientCoefficient() const {
	return (sc->*(sc->_getAmbientCoefficient))(index);
}


inline float SurfelInterface::getDiffuseCoefficient() const {
	return (sc->*(sc->_getDiffuseCoefficient))(index);
}

inline float SurfelInterface::getSpecularCoefficient() const {
	return (sc->*(sc->_getSpecularCoefficient))(index);
}

inline float SurfelInterface::getShininess() const {
	return (sc->*(sc->_getShininess))(index);
}

inline MyDataTypes::TextureCoordinate SurfelInterface::getTextureCoordinate() const {
	return (sc->*(sc->_getTextureCoordinate))(index);
}

inline unsigned int SurfelInterface::getNofAssociatedSurfels () const {
	return (sc->*(sc->_getNofAssociatedSurfels))(index);
}


inline SurfelInterface* SurfelInterface::getAssociatedSurfel (unsigned int idx) const {
	return (sc->*(sc->_getAssociatedSurfel))(index, idx);
}

inline Vector3D SurfelInterface::getDetailVector () const {
	return (sc->*(sc->_getDetailVector))(index);
}

inline float SurfelInterface::getDetailCoefficient () const {
	return (sc->*(sc->_getDetailCoefficient))(index);
}

inline void SurfelInterface::setDetailVector (const Vector3D detailVector) {
	(sc->*(sc->_setDetailVector))(index, detailVector);
}

inline void SurfelInterface::setDetailCoefficient (const float detailCoefficient) {
	(sc->*(sc->_setDetailCoefficient))(index, detailCoefficient);
}

inline void SurfelInterface::setPosition (const Vector3D newPosition) {
	(sc->*(sc->_setPosition))(index, newPosition);
}

inline void SurfelInterface::setNormal (const Vector3D newNormal) {
	(sc->*(sc->_setNormal))(index, newNormal);
}

inline void SurfelInterface::setTangentAxes (const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	(sc->*(sc->_setTangentAxes))(index, newAxisOne, newAxisTwo);
}

inline void SurfelInterface::setNormalTangentAxesSystem (const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo) {
	(sc->*(sc->_setNormalTangentAxesSystem))(index, newNormal, newAxisOne, newAxisTwo);
}

inline void SurfelInterface::setRadius (const float newRadius) {
	(sc->*(sc->_setRadius))(index, newRadius);
}

inline void SurfelInterface::combineColorProperties(SurfelInterface *copySurfel1, SurfelInterface *copySurfel2, float factor) {
	sc->combineColorProperties(index, copySurfel1, copySurfel2, factor);
}

inline void SurfelInterface::setDiffuseColor (const QRgb newDiffuseColor) {
	(sc->*(sc->_setDiffuseColor))(index, newDiffuseColor);
}

inline void SurfelInterface::setSpecularColor (const QRgb newSpecularColor) {
	(sc->*(sc->_setSpecularColor))(index, newSpecularColor);
}

inline void SurfelInterface::setFlagOn (const Flags flags, const bool on) {
	(sc->*(sc->_setFlagOn))(index, flags, on);
}

inline void SurfelInterface::setFlags (const Flags newFlags) {
	(sc->*(sc->_setFlags))(index, newFlags);
}

inline void SurfelInterface::setAmbientCoefficient (const float newAmbientCoefficient) {
	(sc->*(sc->_setAmbientCoefficient))(index, newAmbientCoefficient);
}

inline void SurfelInterface::setDiffuseCoefficient (const float newDiffuseCoefficient) {
	(sc->*(sc->_setDiffuseCoefficient))(index, newDiffuseCoefficient);
}

inline void SurfelInterface::setSpecularCoefficient (const float newSpecularCoefficient) {
	(sc->*(sc->_setSpecularCoefficient))(index, newSpecularCoefficient);
}

inline void SurfelInterface::setShininess (const float newShininess) {
	(sc->*(sc->_setShininess))(index, newShininess);
}

inline void SurfelInterface::setTextureCoordinate (const MyDataTypes::TextureCoordinate newTextureCoordinate) {
	(sc->*(sc->_setTextureCoordinate))(index, newTextureCoordinate);
}

inline void SurfelInterface::copy (const SurfelInterface *sourceSurfel) {
	sc->copySurfelProperties(index, sourceSurfel);
}

inline bool SurfelInterface::addAssociatedSurfel (SurfelInterface *surfel) {
	return (sc->*(sc->_addAssociatedSurfel))(index, surfel);
}

inline void SurfelInterface::clearAssociatedSurfels () {
	(sc->*(sc->_clearAssociatedSurfels))(index);
}




#endif  // __SURFELINTERFACE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
