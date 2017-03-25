// Title:   Object.h
// Created: Thu Sep 25 14:28:17 2003
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

#ifndef __OBJECT_H_
#define __OBJECT_H_

#include <qobject.h>
#include <qdatastream.h>
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
#include <vector>
// hack around MSVC 6.0 compiler bug (release mode only)
#if defined (_WIN32) && defined (NDEBUG)
// the include path as to be added in _every_ project which depends on this header file
#include <SurfelCollection.h>
#else
#include "../../DataStructures/src/SurfelCollection.h"
#endif
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../src/CoreDLL.h"

/**
 * The <code>Object</code> contains the <code>SurfelCollection</code> and the transformation matrices for
 * rotation (around the object's center of gravity), scale and translation. <code>Object</code> are identified
 * by their name, so each <code>Object</code> in the <code>Scene</code> must have an unique name.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class Object : public QObject {

	Q_OBJECT

public:



	typedef enum flag {

		NO_FLAGS			= 0x00000000,   // no flags set
		VISIBLE				= 0x00000001,   // set when is visible, i.e. should be rendered by the renderer
		ENABLED				= 0x00000002,   // set when object is enabled, i.e. not locked
		ALL_FLAGS			= 0xffffffff

	} Flag;

	
	typedef int Flags;


	/**
	 * Creates this <code>Object</code> which is identified by its <code>name</code>.
	 *
	 * @param newName
	 *        a <code>QString</code> which contains the name of this <code>Object</code>
	 * @param propertyDescriptor
	 *		  a <code>SurfelInterface::PropertyDescriptor</code> which is used to set the
	 *		  available attributes of the surfels
	 *
	 * <b>Note:</b> the <code>name</code> defaults to a "null string", but in order that this object can be
	 *              uniquely identified within the <code>Scene</code> use <code>setName</code> to assign it
	 *              a unique name.
	 * @see #setName
	 */
	CORE_API Object (const QString name = QString(), const SurfelInterface::PropertyDescriptor propertyDescriptor = SurfelInterface::BaseSurfel_PropertyDescriptor);

	/**
	 * Destructor
	 */
	CORE_API virtual ~Object();

	/**
	 * Sets the name of this <code>Object</code> which also serves as an identifier in the <code>Scene</code>
	 * and must therefore be unique for each <code>Object</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectRenamed</code></li>
	 * </ul>
	 *
	 * @param newName
	 *        a <code>QString</code> which contains the name of this <code>Object</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectRenamed</code> should
	 *        be supressed.
	 * @see #objectRenamed
	 */
	CORE_API void setName (const QString newName, const bool emitSignal = true);

	/**
	 * Returns the name of this <code>Object</code>.
	 *
	 * @return a <code>QString</code> which contains the name of this <code>Object</code>
	 */
	CORE_API QString getName() const;

	/**
	 * Copies the elements from <code>newSurfelCollection</code> into the <code>SurfelCollection</code> of
	 * this <code>Object</code>. The previous elements are removed.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 * </ul>
	 *
	 * @param newSurfelCollection
	 *        a pointer to a <code>std::vector<SurfelInterface*></code> from which the elements are copied into the
	 *        <code>SurfelCollection</code> of this <code>Object</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 */
	CORE_API void setSurfelCollection (std::vector<SurfelInterface*> *surfels, const bool emitSignal = true);


	/**
	 * Extracts the <code>SurfelCollection</code> of this <code>Object</code>. The <code>SurfelCollection</code>
	 * is removed from the object, but not deleted from memory. This means that when the object is deleted, the
	 * <code>SurfelCollection</code> will still be valid, e.g. for merging with another object.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 * </ul>
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @return the objects surfel collection
	 * @see #objectModified
	 */
	CORE_API SurfelCollection* extractSurfelCollection (const bool emitSignal = true);

	/**
	 * Returns the <code>SurfelCollection</code> which contains all the <code>SurfelInterface</code>s
	 * this <code>Object</code> consists of.
	 *
	 * @return a pointer to a <code>SurfelCollection</code>
	 */
	CORE_API SurfelCollection *getSurfelCollection();
	
	/**
	 * Sets the flags specified by <code>flags</code> of this object to the value <code>on</code>.
	 *
	 * @param flags
	 *        the <code>Flag</code>s to be set
	 * @param on
	 *        the new value of the <code>flags</code>
	 */
	CORE_API void setFlag (const Object::Flags flags, const bool on);

	/*
	 * Returns <code>true</code> if <em>all</em> flags specified in <code>flags</code>
	 * are set to <code>true</code>.
     *
     * @param flags
	 *        OR'ed <code>Flag</code> values which specify the <code>Flags</code> to be tested
	 * @return <code>true</code> if <em>all</em> flags specified in <code>flags</code>
	 *         are set to <code>true</code>; <code>false</code> else
	 * @see Object#setFlag
	 */
	CORE_API bool isFlagOn (const Object::Flags flags);

	/**
	 * Makes a deep copy of the <code>sourceObject</code>.
	 *
	 * <b>Note:</b> The name of <code>sourceObject</code> is also copied - in order to identify
	 *              both <code>Object</code>s in the <code>Scene</code> later on make sure that one
	 *              of these <code>Object</code>s get an unique name again!
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 * </ul>
	 *
	 * @param sourceObject
	 *        a pointer to the source <code>Object</code> which is to be copied
	 *        into this <code>Object</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 */
	CORE_API void copy (Object *sourceObject, const bool emitSignal = true);

	/**
	 * Returns a deep copy of this <code>Object</code>. The copy must be <code>delete</code>d after use.
	 *
	 * * <b>Note:</b> The name of this <code>Object</code> is also copied - in order to identify
	 *              both <code>Object</code>s in the <code>Scene</code> later on make sure that one
	 *              of these <code>Object</code>s get an unique name again!
	 *
	 * @return a deep copy of this <code>Object</code>; must be <code>delete</code>d after use
	 */
	CORE_API Object *copy();

	// ***************
	// transformations
	// ***************

	/**
	 * Rotates this <code>Object</code> absolutely counter-clockwise <code>angle</code> radians around 
	 * the rotation axis [<code>x</code>, <code>y</code>, <code>z</code>], which goes through
	 * the <code>Object</code>'s center of gravity.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param angle
	 *        the angle in <em>radians</em>, which defines the absolute counter-clockwise rotation
	 * @param x
	 *        the x-component of the rotation axis
	 * @param y
	 *        the y-component of the rotation axis
	 * @param z
	 *        the z-component of the rotation axis
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void setRotation (const float angle, const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Rotates this <code>Object</code> relatively counter-clockwise <code>angle</code> radians around 
	 * the rotation axis [<code>x</code>, <code>y</code>, <code>z</code>], which goes through
	 * the <code>Object</code>'s center of gravity.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param dAngle
	 *        the angle in <em>radians</em>, which defines the relative counter-clockwise rotation
	 * @param x
	 *        the x-component of the rotation axis
	 * @param y
	 *        the y-component of the rotation axis
	 * @param z
	 *        the z-component of the rotation axis
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void rotate (const float dAngle, const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Positions this <code>Object</code> absolutely to the coordinate [<code>x</code>, <code>y</code>, <code>z</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param x
	 *        the x component of the position
	 * @param y
	 *        the y component of the position
	 * @param z
	 *        the z component of the position
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void setPosition (const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Positions this <code>Object</code> absolutely to the coordinate <code>newPosition</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param newPosition
	 *        a <code>Vector3D</code> specifying the new position
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void setPosition (const Vector3D newPosition, const bool emitSignal = true);

	/**
	 * Gets the position of this <code>Object</code>.
	 *
	 * @param x
	 *       a pointer to a <code>float</code> which will contain the x component of the position
	 * @param y
	 *       a pointer to a <code>float</code> which will contain the x component of the position
	 * @param z
	 *       a pointer to a <code>float</code> which will contain the x component of the position
	 */
	CORE_API void getPosition (float *x, float *y, float *z) const;

	/**
	 * Returns the position of this <code>Object</code>.
	 *
	 * @return a <code>Vector3D</code> containing the position
	 */
	CORE_API Vector3D getPosition() const;

	/**
	 * Translates this <code>Object</code> relatively by the vector [<code>dx</code>, <code>dy</code>, <code>dz</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param dx
	 *        the relative x-component of the translation vector
	 * @param dy
	 *        the relative y-component of the translation vector
	 * @param dz
	 *        the relative z-component of the translation vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void translate (const float dx, const float dy, const float dz, const bool emitSignal = true);

	/**
	 * Translates this <code>Object</code> relatively by the vector <code>translation</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param translation
	 *        a <code>Vector3D</code> specifying the translation in x-, y- and z-direction
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void translate (Vector3D translation, const bool emitSignal = true);

	/**
	 * Scales this <code>Object</code> absolutely by the vector
	 * [<code>scaleX</code>, <code>scaleY</code>, <code>scaleZ</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param scaleX
	 *        the absolute x-component of the scale vector
	 * @param scaleY
	 *        the absolute y-component of the scale vector
	 * @param scaleZ
	 *        the absolute z-component of the scale vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void setScale (const float scaleX, const float scaleY, const float scaleZ, const bool emitSignal = true);

	/**
	 * Scales this <code>Object</code> absolutely by the vector newScale.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param newScale
	 *        a <code>Vector3D</code> containing the x-, y- and z-direction scales
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void setScale (const Vector3D newScale, const bool emitSignal = true);

	/**
	 * Gets the scale of this <code>Object</code>.
	 *
	 * @param x
	 *       a pointer to a <code>float</code> which will contain the x component of the scale
	 * @param y
	 *       a pointer to a <code>float</code> which will contain the x component of the scale
	 * @param z
	 *       a pointer to a <code>float</code> which will contain the x component of the scale
	 */
	CORE_API void getScale (float *scaleX, float *scaleY, float *scaleZ) const;

	/**
	 * Returns the scale of this <code>Object</code>.
	 *
	 * @return a <code>Vector3D</code> containing the scale
	 */
	CORE_API Vector3D getScale() const;

	/**
	 * Scales this <code>Object</code> relatively by the vector
	 * [<code>dScaleX</code>, <code>dScaleY</code>, <code>dScaleZ</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param dScaleX
	 *        the relative x-component of the scale vector
	 * @param dScaleY
	 *        the relative y-component of the scale vector
	 * @param dScaleZ
	 *        the relative z-component of the scale vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void scale (const float dScaleX, const float dScaleY, const float dScaleZ, const bool emitSignal = true);

	/**
	 * Scales this <code>Object</code> relatively by the vector [<code>scale</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param scale
	 *        a <code>Vector3D</code> specifying the scale in x-, y- and z-direction
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */
	CORE_API void scale (const Vector3D scale, const bool emitSignal = true);

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Object</code>'s scale 
	 * in its local coordinate system and stores the result in <code>scaleMatrix</code>.
	 *
	 * @param scaleMatrix
	 *        a <code>TransformationMatrix16f</code> where the scale matrix is stored into
	 */ 
	CORE_API void getScaleMatrix (MyDataTypes::TransformationMatrix16f scaleMatrix) const;

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Object</code>'s translation 
	 * in its local coordinate system and stores the result in <code>translationMatrix</code>.
	 *
	 * @param translationMatrix
	 *        a <code>TransformationMatrix16f</code> where the translation matrix is stored into
	 */ 
	CORE_API void getTranslationMatrix (MyDataTypes::TransformationMatrix16f translationMatrix) const;

	/**
	 * Sets a <code>TransformationMatrix16f</code> which describes the <code>Object</code>'s rotation 
	 * in its local coordinate system.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectMoved</code></li>
	 * </ul>
	 *
	 * @param newRotationMatrix
	 *        a <code>TransformationMatrix16f</code> which contains the new rotation
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectMoved
	 */ 
	CORE_API void setRotationMatrix (MyDataTypes::TransformationMatrix16f newRotationMatrix, const bool emitSignal = true);

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Object</code>'s rotation 
	 * in its local coordinate system and stores the result in <code>rotationMatrix</code>.
	 *
	 * @param rotationMatrix
	 *        a <code>TransformationMatrix16f</code> where the rotation matrix is stored into
	 */ 
	CORE_API void getRotationMatrix (MyDataTypes::TransformationMatrix16f rotationMatrix) const;

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Object</code>'s final transformation <code>F</code>
	 * in its local coordinate system and stores the result in <code>transformationMatrix</code>:
	 * <code>F = T * S * R</code>, where <code>S</code> is the scale, <code>T</code> is the translation
	 * and <code>R</code> the rotation matrix.
	 *
	 * @param transformationMatrix
	 *        a <code>TransformationMatrix16f</code> where the final transformation matrix is stored into
	 * @see #getScaleMatrix
	 * @see #getTranslationMatrix
	 * @see #getRotationMatrix
	 */
	CORE_API void getTransformationMatrix (MyDataTypes::TransformationMatrix16f transformationMatrix) const;


signals:

	/**
	 * Emitted whenever the <code>SurfelCollection</code> of this <code>Object</code> has been changed
	 * (surfels have been added/removed/changed).
	 *
	 * <b>Note</b>: Renderers should not connect to this <code>objectModified</code> signal directly,
	 *              they should connect to the signal <code>Scene::objectModified</code> instead, since
	 *              internally the <code>Scene</code> already connects to this signal.
	 * @see Scene#objectModified
	 */
	void objectModified();

	/**
	 * Emitted whenever the name of this <code>Object</code> has been changed.
	 *
	 * <b>Note</b>: Renderers should not connect to this <code>objectRenamed</code> signal directly,
	 *              they should connect to the signal <code>Scene::objectRenamed</code> instead, since
	 *              internally the <code>Scene</code> already connects to this signal.
	 * @see Scene#objectModified
	 */
	void objectRenamed();

	/**
	 * Emitted whenever the <code>Object</code> has been rotated, translated or moved.
	 *
	 * <b>Note</b>: Renderers should not connect to this <code>objectMoved</code> signal directly,
	 *              they should connect to the signal <code>Scene::objectMoved</code> instead, since
	 *              internally the <code>Scene</code> already connects to this signal.
	 * @see Scene#objectMoved
	 */
	void objectMoved();

private:

	QString                              name;
	SurfelCollection                     *surfelCollection;
	MyDataTypes::TransformationMatrix16f rotationMatrix,
		                                 scaleTranslationMatrix;   // contains the scale and translation

	Flags								 flags;  // 32 boolean flags

private slots:

	// re-emits the signal objectModified
	void handleSurfelCollectionChanged();

};


#endif  // __OBJECT_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
