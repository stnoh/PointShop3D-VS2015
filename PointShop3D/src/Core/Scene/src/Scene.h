// Title:   Scene.h
// Created: Thu Sep 25 14:28:14 2003
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

#ifndef __SCENE_H_
#define __SCENE_H_

#include <qobject.h>

#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../Interfaces/src/SurfelInterface.h"
#include "../../src/CoreDLL.h"
#include "Object.h"

/**
 * The <code>Scene</code> contains all the <code>Object</code>s to be rendered. It makes sure that
 * all <code>SurfelInterface</code>s within this <code>Scene</code> have the same type, that is that they
 * all support the same properties such as diffuse color, splat radius and position and provides
 * methods to convert all <code>SurfelInterface</code>s into the requested type.
 *
 * All properties of the <code>SurfelInterface</code>s are manipulated via this <code>Scene</code>, as to make
 * sure that signal listeners are properly informed on changing a <code>SurfelCollection</code>. Signal listeners
 * need only connect to the signals as sent by this <code>Scene</code>:
 *
 * <ul>
 *   <li><code>sceneMoved</code></li>
 *   <li><code>objectMoved</code></li>
 *   <li><code>objectModified</code></li>
 *   <li><code>activeObjectAboutToChange</code></li>
 *   <li><code>activeObjectChanged</code></li>
 *   <li><code>objectRenamed</code></li>
 *   <li><code>objectAdded</code></li>
 *   <li><code>objectRemoved</code></li>
 *   <li><code>renderFrame</code></li>
 * </ul>
 * 
 * The <code>RendererManager</code> connects to the signals <code>sceneMoved</code> and <code>objectMoved</code>
 * and updates the scene view matrices in the current <em>main</em> or <em>preview</em> renderer 
 * (whichever is active) and updates the scene <code>Transformer</code> as well. It also connects to the
 * <code>renderFrame</code> signal and lets the current <em>main</em> or <em>preview</em> renderer render the frame,
 * so renderers should only connect to these signals as to update internal data structures such as GL display lists,
 * <em>not</em> as to render the frame themselves since this is handled by the <code>RendererManager</code>.
 * As for the <em>auxiliary</em> renderer though it is up to whoever uses it to update it as needed.
 * 
 * <b>Note:</b>
 * All transformation matrices are <em>column</em> major order and the coordinate system is a <em>right</em>-handed
 * system (like in OpenGL).
 *
 * Airliner:  And from the pilot during his welcome message: "Foo airlines is pleased to have some of the best
 *            flight attendants in the industry. Unfortunately, none of them are on this flight!" 
 *
 * @author Oliver Knoll
 * @version 1.2
 * @see Object
 * @see SurfelCollection
 * @see SurfelInterface
 */
class Scene : public QObject {

	Q_OBJECT

public:

	/**
	 * Creates an instance of this <code>Scene</code>, if necessary, and returns it.
	 *
	 * @return an instance of this <code>RendererManager</code>
	 */
	CORE_API static Scene *getInstance();

	/**
	 * Destroys the instance of this <code>Scene</code>, if there is any.
	 */
	CORE_API static void destroyInstance();

	/**
	 * Enables <code>autoDelete</code>, that is, if <code>enable</code> is set
	 * to <code>true</code>, the <code>Object</code>s are <code>delete</code>d when they
	 * are removed from this <code>Scene</code> or this <code>Scene</code> itself
	 * is <code>delete</code>d. By default <code>autoDelete<code> is turned on.
	 *
	 * @param enable
	 *        set to <code>true</code> if <code>Object</code>s are to be <code>delete</code>d when
	 *        they are removed from this <code>Scene</code> or this <code>Scene</code>
	 *        itself is <code>delete</code>d
	 */
	CORE_API void setAutoDelete (const bool enable);

	/**
	 * Returns the state of <code>autoDelete</code>.
	 * 
	 * @return <code>true</code>, if <code>Object</code>s are <code>delete</code>d when removed; <code>false</code> else
	 */
	CORE_API bool isAutoDelete() const;

	/**
	 * Returns the number of <code>Object</code>s in this <code>Scene</code>.
	 *
	 * @return the number of <code>Object</code>s in this <code>Scene</code>
	 */
	CORE_API uint getNofObjects() const;


	/**
	 * Returns the number of enabled <code>Object</code>s in this <code>Scene</code>.
	 *
	 * @return the number of enabled <code>Object</code>s in this <code>Scene</code>
	 */
	CORE_API uint getNofEnabledObjects();


	/**
	 * Returns the number of <code>SurfelInterface</code>s in this <code>Scene</code>, that is the sum of
	 * all <code>SurfelInterface</code>s in all <code>Object</code>s in this <code>Scene</code>.
	 *
	 * @return the number of <code>SurfelInterface</code>s in this <code>Scene</code>
	 */
	CORE_API uint getNofSurfels();

	/**
	 * A convenience method which returns a name which is guaranteed to be unique within this <code>Scene</code>,
	 * given the <code>name</code> to be tested:
	 * <ul>
	 *   <li>if <code>name</code> doesn't exist yet in this <code>Scene</code>, <code>name</code> is simply returned.</li>
	 *   <li>if <code>name</code> is a null string, then <code>object_</code><em>n</em> is returned, where
	 *       <em>n</em> is a sequence number such as 0001 which is guaranteed to be unique.</li>
	 *   <li>if <code>name</code> exists already, a sequence number of the form <em>_n</em> is appended, where
	 *       <em>n</em> is a sequence number such as 0001 which is guaranteed to be unique.</li>
	 * </ul>
	 *
	 * @param name
	 *        a <code>QString</code> containing the name to be tested for uniqueness; may be a null string
	 * @return a <code>QString</code> containing the unique name, based on <code>name</code>
	 */
	CORE_API QString getUniqueName (const QString name);

	// ****************************
	// object manipulation / access
	// ****************************

	/**
	 * Adds the <code>newObject</code> to this <code>Scene</code>. This method returns the <code>Object</code>'s
	 * name which is guaranteed to be unique within this <code>Scene</code> (the same value which
	 * the convenience method <code>getUniqueName</code> would return). Usually this is
	 * the name of the <code>Object</code> itself, but if the object is inserted twice, the second name will
	 * become <em>object</em>_002, the third name <em>object</em>_003 and so on.
	 *
	 * <b>Note 1:</b> if the <code>newObject</code> is the first <code>Object</code> to be inserted it will
	 *              automatically become the <em>active</em> <code>Object</code>, <em>regardless</em> of the
	 *              settings of <code>makeActive</code>! This guarantees that there's always an <em>active</em>
	 *              <code>Object</code>, except if there's no <code>Object</code> at all.
	 * <b>Note 2:</b> The signals <code>activeObjectAboutToChange</code> and <code>activeObjectChanged</code>
	 *                are sent regardless of <code>emitSignal</em>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>activeObjectAboutToChange</code></li>
	 *   <li><code>activeObjectChanged</code></li>
	 *   <li><code>objectAdded</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param newObject
	 *        a pointer to a new <code>Object</code> to be inserted
	 * @param makeActive
	 *        set to <code>true</code> if the newly inserted <code>newObject</code> should be the new
	 *        <em>active</em> <code>object</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectAdded</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>; note that the signals <code>activeObjectAboutToChange</code>
	 *        and <code>activeObjectChanged</code> are sent regardless of <code>emitSignal</code>, if
	 *        <code>makeActive</code> is set to <code>true</code>
	 * @return a <code>QString</code> specifying the unique name of the <code>newObject</code>
	 * @see #getActiveObject
	 * @see #activeObjectAboutToChange
	 * @see #activeObjectChanged
	 * @see #objectAdded
	 * @see #renderFrame
	 */
	CORE_API QString addObject (Object *newObject, const bool makeActive = true, const bool emitSignal = true);

	/**
	 * Removes the <code>object</code> from this <code>Scene</code>. The <code>object</code> is <code>delete</code>d
	 * if <code>autoDelete</code> is turned on. If the <code>object</code> was the <em>active</em> <code>Object</code>
	 * the new <em>active</em> <code>Object</code> will become the first available <code>Object</code> in this
	 * <code>Scene</code> (or 0 if there are no more <code>Object</code>s).
	 *
	 * <b>Note:</b> The signals <code>activeObjectAboutToChange</code> and <code>activeObjectChanged</code>
	 *                are sent regardless of <code>emitSignal</em>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>activeObjectAboutToChange</code></li>
	 *   <li><code>activeObjectChanged</code></li>
	 *   <li><code>objectRemoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param object
	 *        a pointer to an <code>Object</code> to be removed; if <code>autoDelete</code> is turned
	 *        on the pointer is invalid after this operation
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectRemoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>; note that the signals <code>activeObjectAboutToChange</code>
	 *        and <code>activeObjectChanged</code> are sent regardless of <code>emitSignal</code>, if needed
	 * @see #setAutoDelete
	 * @see #activeObjectAboutToChange
	 * @see #activeObjectChanged
	 * @see #objectRemoved
	 * @see #renderFrame
	 */
	CORE_API void removeObject (Object *object, const bool emitSignal = true);

	/**
	 * Removes all <code>Object</code>s from this <code>Scene</code>. The <code>Object</code>s are <code>delete</code>d
	 * if <code>autoDelete</code> is turned on. Additionally the camera is positioned into the <em>home</code> position.
	 *
	 * <b>Note 1:</b> The signals <code>sceneMoved</code> and <code>objectModified</code> are <em>not</em> emitted.
	 * <b>Note 2:</b> The signals <code>activeObjectAboutToChange</code> and <code>activeObjectChanged</code>
	 *                are sent regardless of <code>emitSignal</em>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>activeObjectAboutToChange</code></li>
	 *   <li><code>activeObjectChanged</code></li>
	 *   <li><code>objectRemoved</code></li>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signals <code>objectRemoved</code> and <code>sceneMoved</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @param renderFrame
	 *        set to <code>false</code> if the signal <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see Configuration#getHomePosition
	 * @see #setAutoDelete
	 * @see #objectRemoved
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void reset (const bool emitSignal = true, const bool renderFrame = true);

	/**
	 * Returns the first <code>Object</code> in this <code>Scene</code> and
	 * sets the internal iterator to it. Returns 0 if there are no <code>Object</code>s in this <code>Scene</code>.
	 *
	 * @return a pointer to the first <code>Object</code> in this <code>Scene</code>; 0 if there are no
	 *         <code>Object</code>s
	 *
	 * @see #getNextObject
	 */
	CORE_API Object *getFirstObject();

	/**
	 * Returns the next <code>Object</code> in this <code>Scene</code> and sets the internal iterator to it.
	 * May be 0.
	 *
	 * <b>Attention:</b> If new <code>Object</code>s are added or removed, the internal iterator is not
	 * invalidated - however no guarantee can be made about the <em>order</em> in which the <em>remaining</em>
	 * <code>Object</code>s are traversed or that these newly added <code>Object</code>s are
	 * returned at all, unless <code>getFirstObject</code> is called and the whole <code>Scene</code>
	 * is traversed from the beginning again with succesive calls to <code>getNextObject</code>.
	 *
	 * @return a pointer to the next <code>Object</code>; 0 is returned if there are no more
	 *         <code>)Object</code>s to be returned
	 * @see #getFirstObject
	 */
	CORE_API Object *getNextObject();
	
	/**
	 * Sets the <code>Object</code> identified by its <code>objectName</code> <em>active</em>. All
	 * <code>Object</code> manipulations such as adding or removing <code>SurfelInterface</code>s
	 * should affect the <em>active</em> <code>Object</code>. It's up to the
	 * <code>RendererInterface</code> to render all other inactive <code>Object</code>s visually
	 * differently.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>activeObjectAboutToChange</code></li>
	 *   <li><code>activeObjectChanged</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param objectName
	 *        a <code>QString</code> which identifies the <code>Object</code>
	 * @param renderFrame
	 *        set to <code>false</code> if the signal <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #activeObjectAboutToChange
	 * @see #activeObjectChanged
	 * @see #renderFrame
	 */
	CORE_API void setActiveObject (const QString objectName, const bool renderFrame = true);

	/**
	 * Returns the <em>active</code> <code>Object</code>, that is the <code>Object</code> which is
	 * affected by all manipulations such as adding or removing of <code>SurfelInterface</code>s. Is only
	 * 0 if there are no <code>Object</code>s in this <code>Scene</code>
	 *
	 * @return a pointer to the <em>active</code> <code>Object</code>; is 0 if there are no <code>Object</code>s
	 *         in this <code>Scene</code>
	 * @see #getActiveObjectID
	 */
	CORE_API Object *getActiveObject() const;

	/**
	 * Gets the ID of the <em>active</em> <code>Object</code>. Each <code>Object</code> has an ID (and index), the first
	 * <code>Object</code> has ID = 0, the second ID = 1 and so on. This is useful if a <code>QComboBox</code>
	 * containing all <code>Object</code>s has to be updated.
	 *
	 * @return the ID of the <em>active</em> <code>Object</code>; -1 if there are no <code>Object</code>s at all
	 * @see #getActiveObject
	 */
	CORE_API int getActiveObjectID() const;

	// ***************
	// transformations
	// ***************

	/**
	 * Rotates this <code>Scene</code> absolutely counter-clockwise <code>angle</code> radians around 
	 * the rotation axis [<code>x</code>, <code>y</code>, <code>z</code>], which goes through
	 * the <code>Scene</code>'s center of gravity.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
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
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void setRotation (const float angle, const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Rotates this <code>Scene</code> relatively counter-clockwise <code>angle</code> radians around 
	 * the rotation axis [<code>x</code>, <code>y</code>, <code>z</code>], which goes through
	 * the <code>Scene</code>'s center of gravity.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
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
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void rotate (const float dAngle, const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Translates this <code>Scene</code> absolutely by the vector [<code>x</code>, <code>y</code>, <code>z</code>],
	 * that is the new position of the <code>Scene</code> is defined by this vector.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param dx
	 *        the relative x-component of the translation vector
	 * @param dy
	 *        the relative y-component of the translation vector
	 * @param dz
	 *        the relative z-component of the translation vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void setTranslation (const float x, const float y, const float z, const bool emitSignal = true);

	/**
	 * Translates this <code>Scene</code> relatively by the vector [<code>dx</code>, <code>dy</code>, <code>dz</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param dx
	 *        the relative x-component of the translation vector
	 * @param dy
	 *        the relative y-component of the translation vector
	 * @param dz
	 *        the relative z-component of the translation vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void translate (const float dx, const float dy, const float dz, const bool emitSignal = true);

	/**
	 * Scales this <code>Scene</code> absolutely by the vector
	 * [<code>scaleX</code>, <code>scaleY</code>, <code>scaleZ</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param scaleX
	 *        the absolute x-component of the scale vector
	 * @param scaleY
	 *        the absolute y-component of the scale vector
	 * @param scaleZ
	 *        the absolute z-component of the scale vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void setScale (const float scaleX, const float scaleY, const float scaleZ, const bool emitSignal = true);

	/**
	 * Scales this <code>Scene</code> relatively by the vector
	 * [<code>dScaleX</code>, <code>dScaleY</code>, <code>dScaleZ</code>].
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param dScaleX
	 *        the relative x-component of the scale vector
	 * @param dScaleY
	 *        the relative y-component of the scale vector
	 * @param dScaleZ
	 *        the relative z-component of the scale vector
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void scale (const float dScaleX, const float dScaleY, const float dScaleZ, const bool emitSignal = true);

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Scene</code>'s scale 
	 * in the <code>Scene</code> (world) coordinate system and stores the result in <code>scaleMatrix</code>.
	 *
	 * @param scaleMatrix
	 *        a <code>TransformationMatrix16f</code> where the scale matrix is stored into
	 */ 
	CORE_API void getScaleMatrix (MyDataTypes::TransformationMatrix16f scaleMatrix) const;

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Scene</code>'s translation 
	 * in the <code>Scene</code> (world) coordinate system and stores the result in <code>translationMatrix</code>.
	 *
	 * @param translationMatrix
	 *        a <code>TransformationMatrix16f</code> where the translation matrix is stored into
	 */ 
	CORE_API void getTranslationMatrix (MyDataTypes::TransformationMatrix16f translationMatrix) const;

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Scene</code>'s rotation 
	 * in the <code>Scene</code> (world) coordinate system and stores the result in <code>rotationMatrix</code>.
	 *
	 * @param rotationMatrix
	 *        a <code>TransformationMatrix16f</code> where the rotation matrix is stored into
	 */ 
	CORE_API void getRotationMatrix (MyDataTypes::TransformationMatrix16f rotationMatrix) const;

	/**
	 * Sets the absolute camera position. The camera can also be positioned with <code>setTransformation</code>,
	 * <code>setRotation</code> and <code>setScale</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>sceneMoved</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 * 
	 * @param cameraPosition
	 *        the <code>CameraPosition</code> which defines the absolute position of the camera
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>sceneMoved</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #setTransformation
	 * @see #setRotation
	 * @see #setScale
	 * @see #sceneMoved
	 * @see #renderFrame
	 */
	CORE_API void setCameraPosition (const MyDataTypes::CameraPosition cameraPosition, const bool emitSignal = true);

	/**
	 * Gets the camera position. Note that <code>getTransformationMatrix</code> returns the final
	 * transformation matrix, which is <code>F = T * S * R</code>, where <code>ST</code> is the combined scale and
	 * translation matrix and <code>R</code> the rotation matrix, both which describe the <code>CameraPosition</code>.
	 * 
	 * @param cameraPosition
	 *        the <code>CameraPosition</code> where the camera position is stored into
	 * @see #getTransformationMatrix
	 * @see #getScaleMatrix
	 * @see #getTranslationMatrix
	 * @see #getRotationMatrix
	 */
	CORE_API MyDataTypes::CameraPosition getCameraPosition () const;

	/**
	 * Gets a <code>TransformationMatrix16f</code> which describes the <code>Scene</code>'s final transformation <code>F</code>
	 * in the <code>Scene</code> (world) coordinate system and stores the result in <code>transformationMatrix</code>:
	 * <em>F = T * S * R</em>, where <em>S</em> is the scale matrix, <em>T</em> the translation matrix
	 * and <em>R</em> the rotation matrix. Both the combined scale and transformation matrix <em>ST</code> and
	 * the rotation matrix <em>R</em> can also be queried with <code>getCameraPosition</code>.
	 *
	 * @param transformationMatrix
	 *        a <code>TransformationMatrix16f</code> where the final transformation matrix is stored into
	 * @see #getScaleMatrix
	 * @see #getTranslationMatrix
	 * @see #getRotationMatrix
	 * @see #getCameraPosition
	 */
	CORE_API void getTransformationMatrix (MyDataTypes::TransformationMatrix16f transformationMatrix);

	// *******************
	// Surfel manipulation
	// *******************
	// the surfels can only be manipulated by this Scene, so it can maintain a consistent state
	// for e.g. search structures and inform the renderers properly whenever the image needs to be
	// re-rendered

	/**
	 * This is a convenience method which sets the <code>newPropertyDescriptor</code> for the <code>SurfelCollection</code>s
	 * of all <code>Object</code>s in this <code>Scene</code>, that is if the existing <code>PropertyDescriptor</code>
	 * is different from <code>newPropertyDescriptor</code> then the <code>SurfelInterface</code>s are converted.
	 * It is equivalent with calling <code>setPropertyDescriptor</code> on one of the <code>SurfelCollection</code>s
	 * in this <code>Scene</code>, since all <code>SurfelCollection</code>s in this <code>Scene</code> must
	 * have the same type of <code>SurfelInterface</code>s. This <code>Scene</code> takes care of that.
	 *
	 * <b>Note:</b> As to make sure that the <code>SurfelInterface</code>s in this <code>Scene</code> support
	 *              <em>at least</em> certain properties call <code>requestSurfelPropertyDescriptor</code>
	 *              instead as to save conversion time.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param newPropertyDescriptor
	 *        the <code>PropertyDescriptor</code> which consists of up to 32 OR'ed <code>Properties</code>
	 * @see SurfelInterface#Property
	 * @see SurfelCollection#setPropertyDescriptor
	 * @see #requestSurfelPropertyDescriptor
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelPropertyDescriptor (const SurfelInterface::PropertyDescriptor newPropertyDescriptor);

	/**
	 * This method requests that all <code>SurfelInterface</code>s support <em>at least</em> the properties as given
	 * in <code>requestedPropertyDescriptor</code>. <code>ToolInterface</code>s or <code>PluginInterface</code>s
	 * should call this method as to make sure that the properties they want to modify are supported by the
	 * <code>SurfelInterface</code>s in this <code>Scene</code>. The <code>SurfelInterface</code>s are only
	 * converted if they do not support at least one property given in <code>requestedPropertyDescriptor</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param requestedPropertyDescriptor
	 *        the <code>PropertyDescriptor</code> which consists of up to 32 OR'ed <code>Properties</code> which
	 *        should be at least supported by the <code>SurfelInterface</code>s in this <code>Scene</code>
	 * @see SurfelInterface#Property
	 * @see #setSurfelPropertyDescriptor
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void requestSurfelPropertyDescriptor (const SurfelInterface::PropertyDescriptor requestedPropertyDescriptor);
	
	/**
	 * This is a convencience method which returns the <code>PropertyDescriptor</code> of the <code>SurfelCollection</code>
	 * of the first <code>Object</code> in this <code>Scene</code>. It is equivalent with calling
	 * <code>getPropertyDescriptor</code> on any <code>SurfelCollection</code> in this <code>Scene</code>,
	 * since they have all the same <code>PropertyDescriptor</code>s. If there is no <code>Object</code> in this
	 * <code>Scene</code> the returned value is <code>NO_PROPERTIES</code>. 
	 *
	 * @return the <code>PropertyDescriptor</code> of the <code>SurfelCollection</code> of the first
	 *         <code>Object</code> in this <code>Scene</code> which consists of up to 32 OR'ed <code>Properties</code>;
	 *         <code>NO_PROPERTIES</code> is returned when there are no <code>Object</code>s in this <code>Scene</code>
	 */
	CORE_API SurfelInterface::PropertyDescriptor getSurfelPropertyDescriptor() const;

	/**
	 * Sets the position of the <code>surfel</code> to <code>newSurfelPosition</code>.
	 *
	 * <b>Note:</b> 
	 * If the <code>Property POSITION</code> does not exist yet, it is added to the 
	 * <code>PropertyDescriptor</code> and the position of existing surfels is set to a default value.				
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newPosition
	 *        a <code>Vector3D</code> specifying the new position of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see SurfelInterface#Property
	 */
	CORE_API void setSurfelPosition (SurfelInterface *surfel, const Vector3D newPosition, const bool emitSignal = true);

	/**
	 * Sets the normal of the <code>surfel</code> to <code>newNormal</code>.
	 *
	 * <b>Note:</b>	If the <code>Property TANGENT_AXES</code> is set the tangent axes are recomputed such
	 *				that they are orthogonal to the normal vector. The length of the axes is computed as
	 *				the average of the previous axes length, therefore the surfels are circular afterwards.
	 *				<p>If you want to set both the normal and the tangent axes use
	 *				<code>setSurfelNormalTangentAxesSystem</code> instead.</p>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newNormal
	 *        a <code>Vector3D</code> specifying the new normal of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see #setSurfelNormalTangentAxesSystem
	 * @see SurfelInterface#Property
	 */
	CORE_API void setSurfelNormal (SurfelInterface *surfel, const Vector3D newNormal, const bool emitSignal = true);

	/**
	 * Sets the tangent axes of the <code>surfel</code> to <code>newAxisOne</code> and <code>newAxisTwo</code>.
	 *
	 * <b>Note:</b>	If the <code>Property NORMAL</code> is set the normal is recalculated. If you
	 *				want to set both the normal and the tangent axes use
	 *				<code>setSurfelNormalTangentAxesSystem</code> instead.
	 *				<p>If the <code>Property RADIUS</code> is set the radius is recomputed from the
	 *				tangent axes </p>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newAxisOne
	 *        a <code>Vector3D</code> specifying the new tangent axis 1 of the <code>surfel</code>
	 * @param newAxisTwo
	 *        a <code>Vector3D</code> specifying the new tangent axis 2 of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
 	 * @see #setSurfelNormalTangentAxesSystem
	 * @see SurfelInterface#Property
	 */
	CORE_API void setSurfelTangentAxes (SurfelInterface *surfel, const Vector3D newAxisOne, const Vector3D newAxisTwo, const bool emitSignal = true);

	/**
	 * Sets the normal and tangent axes of the <code>surfel</code> to 
	 * <code>newNormal</code>, <code>newAxisOne</code> and <code>newAxisTwo</code>, respectively.
	 *
	 * <b>Note:</b>	If the <code>Property RADIUS</code> is set the radius is recomputed from the
	 *				tangent axes
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newNormal
	 *        a <code>Vector3D</code> specifying the new normal of the <code>surfel</code>
	 * @param newAxisOne
	 *        a <code>Vector3D</code> specifying the new tangent axis 1 of the <code>surfel</code>
	 * @param newAxisTwo
	 *        a <code>Vector3D</code> specifying the new tangent axis 2 of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see #setSurfelNormal
	 * @see #setSurfelTangentAxes
	 * @see SurfelInterface#Property
	 */
	CORE_API void setSurfelNormalTangentAxesSystem (SurfelInterface *surfel, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo, const bool emitSignal = true);


	/**
	 * Sets the radius of the <code>surfel</code> to <code>newRadius</code>.
	 *
	 * <b>Note:</b>	If the <code>Property TANGENT_AXES</code> is set the 
	 *				length of the tangent axes is set to radius.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newRadius
	 *        the new radius of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see SurfelInterface#Property
	 */
	CORE_API void setSurfelRadius (SurfelInterface *surfel, const float newRadius, const bool emitSignal = true);

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
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
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
	CORE_API void combineSurfelColorProperties(SurfelInterface *surfel, SurfelInterface *copySurfel1, SurfelInterface *copySurfel2, float factor, const bool emitSignal = true);

	/**
	 * Sets the diffuse color of the <code>surfel</code> to <code>newDiffuseColor</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newDiffuseColor
	 *        the new <code>QRgb</code> diffuse color of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelDiffuseColor (SurfelInterface *surfel, const QRgb newDiffuseColor, const bool emitSignal = true);

	/**
	 * Sets the specular color of the <code>surfel</code> to <code>newSpecularColor</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newSpecularColor
	 *        the new <code>QRgb</code> specular color of the <code>surfel</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelSpecularColor (SurfelInterface *surfel, const QRgb newDiffuseColor, const bool emitSignal = true);

	/**
	 * Sets the flags specified by <code>flags</code> of the <code>surfel</code> to the value <code>on</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param flags
	 *        the <code>Flag</code>s to be set
	 * @param on
	 *        the new value of the <code>flags</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelFlagOn (SurfelInterface *surfel, const SurfelInterface::Flag flags, const bool on, const bool emitSignal = true);

	/**
	 * Sets the flags of the <code>surfel</code> to <code>newFlags</code>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newFlags
	 *        the <code>Flag</code>s to be set
	 * @param on
	 *        the new value of the <code>flags</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelFlags (SurfelInterface *surfel, const SurfelInterface::Flags newFlags, const bool emitSignal = true);
	
	/**
	 * Sets the flags of all the <code>SurfelInterface</code>s in this <code>Scene</code> to <code>newFlags</code>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param newFlags
	 *        the <code>Flag</code>s to be set
	 * @param on
	 *        the new value of the <code>flags</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setAllSurfelFlagsOn (const SurfelInterface::Flags newFlags, const bool on, const bool emitSignal = true);

	/**
	 * Clears all associated surfels in this <code>Scene</code>
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void clearAllAssociatedSurfels(const bool emitSignal = true);

	/**
	 * Sets the ambient coefficient of the <code>surfel</code> to <code>newAmbientCoefficient</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newAmbientCoefficient
	 *        the new ambient coefficient in [0.0, 1.0]
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelAmbientCoefficient (SurfelInterface *surfel, const float newAmbientCoefficient, const bool emitSignal = true);

	/**
	 * Sets the diffuse coefficient of the <code>surfel</code> to <code>newDiffuseCoefficient</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newDiffuseCoefficient
	 *        the new diffuse coefficient in [0.0, 1.0]
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelDiffuseCoefficient (SurfelInterface *surfel, const float newDiffuseCoefficient, const bool emitSignal = true);

	/**
	 * Sets the specular coefficient of the <code>surfel</code> to <code>newSpecularCoefficient</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newSpecularCoefficient
	 *        the new specular coefficient in [0.0, 1.0]
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelSpecularCoefficient (SurfelInterface *surfel, const float newSpecularCoefficient, const bool emitSignal = true);

	/**
	 * Sets the shininess of the <code>surfel</code> to <code>newShininess</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newShininess
	 *        the new shininess in [0.0, 1.0]
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelShininess (SurfelInterface *surfel, const float newShininess, const bool emitSignal = true);

	/**
	 * Sets the texture coordinate of the <code>surfel</code> to <code>newTextureCoordinate</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be changed
	 * @param newTextureCoordinate
	 *        the <code>TextureCoordinate</code> in [[0.0, 1.0], [0.0, 1.0]]
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void setSurfelTextureCoordinate (SurfelInterface *surfel, const MyDataTypes::TextureCoordinate newTextureCoordinate, const bool emitSignal = true);


	/**
	 * Add an associated surfel to a surfel
	 * <p>Associated surfels which intersect are clipped by the renderer</p>
	 *
  	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to add an associated surfel
	 * @param associatedSurfel
	 *        a pointer to the associated surfel
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see #clearAssociatedSurfels
	 */
	CORE_API bool addAssociatedSurfel (SurfelInterface *surfel, SurfelInterface *associatedSurfel, const bool emitSignal = true);

	/**
	 * clears all associated surfels of a surfel
	 *
  	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to add an associated surfel
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #objectModified
	 * @see #renderFrame
	 * @see #addAssociatedSurfel
	 */	
	CORE_API void clearAssociatedSurfels (SurfelInterface *surfel, const bool emitSignal = true);

	/**
	 * Removes the <code>surfels</code> from the <code>SurfelCollection</code>s of <em>all</em> <code>Object</code>s
	 * in this <code>Scene</code>.
	 *
	 * <b>Note:</b> if you want to remove the <code>surfels</code> from the <em>active</em> <code>Object</code>
	 *              it is far more efficient to call <code>removeSurfels</code> on its <code>SurfelCollection</code>
	 *              instead!        
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a pointer to a <code>std::vector<SurfelInterface*></code> which contains the <code>SurfelInterface</code> to
	 *        be removed from <em>all</em> <code>Object</code>s in this <code>Scene</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see SurfelCollection#removeSurfels
	 * @see #getActiveObject
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void removeSurfels (std::vector<SurfelInterface*> *surfels, const bool emitSignal = true);

	/**
	 * Removes the surfels from the <code>SurfelCollection</code>s of <em>all</em> <code>Object</code>s
	 * in this <code>Scene</code> for which <code>flag</code> is true. 
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param flag
	 *        The <code>SurfelInterface</code> flag that specifies which surfels should be deleted
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see SurfelCollection#removeSurfels
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void removeSurfels (SurfelInterface::Flag flag, const bool emitSignal = true);

	/**
	 * Removes the <code>surfels</code> from the <code>SurfelCollection</code>s of the <code>Object</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param object
	 *        the <code>Object</code> from which the <code>surfels</code> have to be removed
	 * @param surfels
	 *        a pointer to a <code>QList<SurfelInterface></code> which contains the <code>SurfelInterface</code> to
	 *        be removed from the <code>Object</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see SurfelCollection#removeSurfels
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void removeSurfels (Object *object, QList<SurfelInterface> *surfels, const bool emitSignal = true);
	
	/**
	 * Removes the <code>surfels</code> from the <code>SurfelCollection</code>s of <em>all</em> <code>Object</code>s
	 * in this <code>Scene</code>.
	 *
	 * <b>Note:</b> if you want to remove the <code>surfels</code> from the <em>active</em> <code>Object</code>
	 *              it is far more efficient to call <code>removeSurfels</code> on its <code>SurfelCollection</code>
	 *              instead!        
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>objectModified</code></li>
	 *   <li><code>renderFrame</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a <code>std::vector<SurfelInterface></code> which contains pointers to the <code>SurfelInterface</code> to
	 *        be removed from <em>all</em> <code>Object</code>s in this <code>Scene</code>
	 * @param nofSurfels
	 *        the number of <code>SurfelInterface</code>s in <code>surfels</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>objectModified</code> and <code>renderFrame</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see SurfelCollection#removeSurfels
	 * @see #getActiveObject
	 * @see #objectModified
	 * @see #renderFrame
	 */
	CORE_API void removeSurfels (std::vector<SurfelInterface *> surfels, const int nofSurfels, const bool emitSignal = true);

	/**
	 * Forces the signal <code>objectModified</code> to be emitted. Useful if an unknown number of <code>SurfelInterface</code>
	 * are going to be modified and each operation suppresses this signal.
	 *
	 * @param samplingPatternAltered
	 *        true if the sampling pattern has been altered
	 *
	 * <b>Attention:</b> Use with caution! Don't forget to emit the signal <code>objectModified</code>!
	 *
	 * @see #objectModified
	 */
	CORE_API void emitObjectModifiedSignal (bool samplingPatternAltered = false);

	/**
	 * Forces the signal <code>renderFrame</code> to be emitted.
	 *
	 * @see #renderFrame
	 */
	CORE_API void emitRenderFrameSignal();

signals:

	/**
	 * Emitted whenever the <code>Scene</code> has been rotated, moved or scaled, that is if only the
	 * <code>Scene</code> transformation matrix has been changed. A re-rendering of the image
	 * is therefore necessary which is triggered by the <code>RendererManager</code>.
	 *
	 * <b>Note</b>: The <code>RendererManager</code> connects to this signal and makes sure that the proper
	 *              <code>Scene</code> transformation matrix is set in the <code>RendererInterface</code> and
	 *              has the <code>Scene</code> re-rendered, so there is no need for <code>RendererInterace</code>s
	 *              to connect to this signal and re-render the <code>Scene</code>.
	 *
	 * @param transformationMatrix
	 *        the <code>TransformationMatrix16f</code> which describes the <code>Scene</code>'s final transformation
	 * @see #getTransformationMatrix
	 * @see RendererManager	 
	 */
	void sceneMoved(const float *transformationMatrix);

	/**
	 * Emitted whenever the <code>Object</code>s in the <code>Scene</code> have been rotated, translated or
	 * scaled.
	 *
	 * <b>Note</b>: <code>RendererInterace</code>s need only connect to this <code>objectMoved</code> signal,
	 *              <em>not</em> to the signals sent by the individual <code>Object</code>s, since those signals
	 *              are internally re-sent via this <code>Scene</code>, so the <code>RendererInterace</code>s get
	 *              informed only once when they need to re-render the <code>Scene</code>.
	 */
	void objectMoved();

	/**
	 * Emitted whenever one or more <code>Object</code>s in the <code>Scene</code> have been modified. This can happen
	 * for the following reasons:
	 * <ul>
	 *   <li><code>Surfel</code>s have been added or removed to/from its <code>SurfelCollection</code></li>
	 *   <li><code>Surfel</code>s from its <code>SurfelCollection</code> have been changed</code></li>
	 *   <li>the <code>PropertyDescriptor<code> of its <code>SurfelCollection</code> has changed
	 * </ul>
	 *
	 * @param samplingPatternAltered
	 *        true if the sampling pattern has been altered
	 *
	 * <b>Note</b>: <code>RendererInterface</code>s need only connect to this <code>objectModified</code> signal,
	 *              <em>not</em> to the signals sent by the individual <code>Object</code>s, since those signals
	 *              are internally re-sent via this <code>Scene</code>, so the <code>RendererInterface</code>s get
	 *              informed only once when they need to re-render the <code>Scene</code>.
	 */
	void objectModified (bool samplingPatternAltered = false);

	/**
	 * Emitted whenever the active object is about to change, i.e., immediately before the active object
	 * actually changes. This signal is caught e.g. by the <code>MarkerManager</code>, that removes all 3D markers upon
	 * receipt of the signal.
	 */
	void activeObjectAboutToChange();

	/**
	 * Emitted whenever the active <code>Object</code> has changed.
	 */
	void activeObjectChanged();

	/**
	 * Emitted whenever one or more <code>Object</code>s in the <code>Scene</code> have been renamed.
	 */
	void objectRenamed();

	/**
	 * Emitted whenever one or more <code>Object</code>s have been added to this <code>Scene</code>.
	 */
	void objectAdded();

	/**
	 * Emitted whenever one or more <code>Object</code>s have been removed from this <code>Scene</code>.
	 */
	void objectRemoved();

	/**
	 * This signal is emitted whenever this <code>Scene</code> needs to be re-rendered.
	 *
	 * <b>Note:</b> The <code>RendererManager</code> connects to this signal and lets the <em>main</em>
	 *              and <em>preview</em> <code>RendererInterface</code> render the image.
	 */
	void renderFrame();

private:

	QList<Object>                        objects;
	Object                               *activeObject;
	int                                  activeObjectID;
	MyDataTypes::CameraPosition          cameraPosition;
	MyDataTypes::TransformationMatrix16f transformationMatrix;         // contains the Scene's final transformation matrix
	                                                                   // calculated from the 'cameraPosition'
	bool                                 isTransformationMatrixValid;  // true if 'transformationMatrix' holds valid data

	Scene();
protected: // To suppress warnings
	virtual ~Scene();
private:

	// updates the transformation matrix
	void updateTransformationMatrix();

	// sets the 'newPropertyDescriptor' for the SurfelCollections of all Objects in this Scene
	// emits the signal 'objectModified'
	void setAllPropertyDescriptors (const SurfelInterface::PropertyDescriptor newPropertyDescriptor);

private slots:

	// converts the surfel collections of all the other objects in this scene
	void handleSurfelTypeChanged (const SurfelInterface::PropertyDescriptor newPropertyDescriptor);

	// re-emits the signal sent by Object
	void handleObjectModified ();

	// re-emits the signal sent by Object
	void handleObjectRenamed ();

	// re-emits the signal sent by Object
	void handleObjectMoved();

};


#endif  // __SCENE_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
