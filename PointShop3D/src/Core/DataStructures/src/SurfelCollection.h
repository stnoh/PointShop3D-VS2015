// Title:   SurfelCollection.h
// Created: Thu Sep 25 14:29:45 2003
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

// it is important that SurfelInterface is included before defining __SURFELCOLLECTION_H_!
#ifndef __SURFELINTERFACE_H_
	#include "../../Interfaces/src/SurfelInterface.h"
#endif

#ifndef __SURFELCOLLECTION_H_
#define __SURFELCOLLECTION_H_

#include <qobject.h>
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
#include <qdatastream.h>
#include <vector>
#include "../../../DataTypes/src/MyDataTypes.h"
#include "../../../DataTypes/src/Vector3D.h"
#include "../../src/CoreDLL.h"
#include "NeighbourHood.h"
#include "AssociatedSurfels.h"

/**
 * The <code>SurfelCollection</code> contains all the surfels which make up an <code>Object</code>.
 *
 * <b>Note:</b> The surfel collection is value-based. I.e., surfel objects only exist in surfel collections, in 
 * other words, a pointer to a surfel that is not contained in any surfel collection is meaningless. Pointers
 * to surfels should rather be treated as "references" or "indices". Removing a surfel from a surfel collection
 * always means discarding its attribute data and de-allocating its memory.
 *
 * For further information, have a look on the web at www.pointshop3d.com
 *
 * @author Oliver Knoll, Richard Keiser
 * @version 1.2
 */
class SurfelCollection : public QObject {

	Q_OBJECT

public:
	/**
	 * Iterator for the surfel collection
	 */
	class const_iterator	{
		
		const std::vector<SurfelInterface*>		*surfels;
		int										idx;

	  public:
		  const_iterator(const std::vector<SurfelInterface*> *_surfels, int _idx)
		  : surfels(_surfels),
			idx(_idx)
		{ }
	
		const_iterator() { }

		const SurfelInterface* operator*() {
		  return (*surfels)[idx];
		}

		const_iterator  &operator++() {
		  ++idx;
		  return *this;
		}

		const_iterator  &operator--() {
		  --idx;
		  return *this;
		}

		bool  operator==(const const_iterator &it) {
		  return surfels == it.surfels && idx == it.idx;
		}

		bool  operator!=(const const_iterator &it) {
		  return surfels != it.surfels || idx != it.idx;
		}
	 };


public:
	/**
	 * The defaul parameter for texture UV coordiantes with value (0.0, 0.0).
	 */
	CORE_API static const MyDataTypes::TextureCoordinate defaultTextureCoordinate;
	
	/**
	 * Creates this <code>SurfelCollection</code>.
	 *
	 * <b>Note</b>: By setting the default parameter values to reasonable values the performance might be 
	 *				drastically increased. By setting the correct <code>propertyDescriptor</code> the conversion
	 *				between surfel types might be prevented.
	 *				<p>If the number of surfels of a collection is known (or can be estimated) the capacity should always
	 *				be equal or larger to the number of surfels. This prevents re-allocating of the arrays.</p>
	 *				<p>If new surfels are added and the capacity of the collection gets too small, the new capacity of the collection
	 *				will be <code>capacity + (capacity*incrCapacityFactor) + 1</code>. 
	 *				This value should be adapted to the probability of new surfels being insert, i.e.	
	 *				choose a larger value if the probability that many new surfels are inserted is high. If you want to
	 *				save memory choose a small value.
	 *
	 * @params propertyDescriptor
	 *		Sets the properties (@see #setPropertyDescriptor)
	 * @params capacity
	 *		Sets the capacity of the surfel collection arrays
	 * @params incrCapacityFactor
	 *		If the capacity of the surfel collection arrays has to be increased it is set to 
	 *		<code>capacity + (capacity*incrCapacityFactor) + 1</code>. <code>incrCapacityFactor</code> must be equal or larger than 0.0
	 *
	 * @see #setPropertyDescriptor
	 * @see #getPropertyDescriptor
	 * @see #reserve
	 * @see #capacity
	 * @see #setCapacityIncrement
	 *		
	 */
	CORE_API SurfelCollection ( const SurfelInterface::PropertyDescriptor propertyDescriptor = SurfelInterface::BaseSurfel_PropertyDescriptor,
								const int capacity = 0,
								const float incrCapacityFactor = 1.0);

	/**
	 * Destroys this <code>SurfelCollection</code>. All surfels are deleted, too. All references to these
	 * surfels become invalid!
	 */
	CORE_API ~SurfelCollection();

	/** 
	 * Converts all the types of the surfels which implement the <code>SurfelInterface</code>s so they
	 * support the properties requested in <code>newPropertyDescriptor. If less <code>Properties</code>
	 * are requested than there are already supported then the superfluous <code>Properties</code> are simply discarded.
	 * If more <code>Properties</code> are requested the additional <code>Properties</code> are set to
	 * default values.
	 *
	 * <b>Note:</b> If new properties are included the values for the existing surfels are computed if possible
	 *				from other properties, otherwise default values are set. E.g. if the property
	 *				<code>RADIUS</code> is set and the property <code>TANGENT_AXES</code> is deleted,
	 *				the radius is first computed from the tangent axes, then the tangent axes array is deleted.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelTypeChanged</code></li>
	 * </ul>
	 *
	 * @param newPropertyDescriptor
	 *        the <code>PropertyDescriptor</code> which consists of up to 32 OR'ed <code>Properties</code>
	 * @see SurfelInterface#Property
	 * @see #surfelTypeChanged
	 * @see #getComputedPropertyDescriptor
	 */
	CORE_API void setPropertyDescriptor (const SurfelInterface::PropertyDescriptor newPropertyDescriptor, bool emitSignal = true);

	/**
	 * Returns the <code>PropertyDescriptor</code>, which specifies which properties the <code>SurfelInterface</code>s
	 * in this <code> SurfelCollection</code> support.
	 *
	 * @return the <code>PropertyDescriptor</code> which consists of up to 32 OR'ed <code>Properties</code>
	 * @see #Property
	 * @see #getComputedPropertyDescriptor
	 */
	CORE_API SurfelInterface::PropertyDescriptor getPropertyDescriptor() const;

   /** 
    * Returns the properties which are computed encoded as a <code>PropertyDescriptor</code>.
	*
	* A property is computed if it is not stored directly but computed from other properties,
	* e.g. if a surfel has only the tangent axes as properties but not the normal,
	* the normal is computed from the tangent axes. In this case, 
	* <code>getComputedProperties() | SurfelInterface::NORMAL </code> would be 1.
	* The stored properties can be found using #getPropertyDescriptor
	*
	* @return the computed properties
	*
	* @see #setPropertyDescriptor
	* @see #getPropertyDescriptor
	*/
	CORE_API SurfelInterface::PropertyDescriptor getComputedPropertyDescriptor() const;


	/**
	 * Returns the number of <code>SurfelInterface</code>s in this <code>SurfelCollection</code>.
	 *
	 * @return the number of <code>SurfelInterface</code>s in this <code>SurfelCollection</code>
	 *
	 * @see #resize
	 */
	CORE_API inline uint getNofSurfels() const;

	// ***********
	// Data access
	// ***********

	/**
	 * Returns the first <code>SurfelInterface</code> in this <code>SurfelCollection</code> and
	 * sets the internal iterator to it.
	 *
	 * @see #getNextSurfel
	 * @see #getPrevSurfel
	 * @see #getCurrentSurfel
	 */
	CORE_API SurfelInterface *getFirstSurfel();

	/**
	 * Returns the current <code>SurfelInterface</code> in this <code>SurfelCollection</code> according
	 * to the internal iterator.
	 *
	 * @return a pointer to the current <code>SurfelInterface</code>; 0 is returned if there are no
	 *         <code>)SurfelInterface</code>s to be returned
	 * @see #getFirstSurfel
	 * @see #getNextSurfel
	 * @see #getPrevSurfel
	 */
	CORE_API SurfelInterface *getCurrentSurfel() const;

	/**
	 * Returns the next <code>SurfelInterface</code> in this <code>SurfelCollection</code> and
	 * sets the internal iterator to it.
	 *
	 * @return a pointer to the next <code>SurfelInterface</code>; 0 is returned if there are no more
	 *         <code>)SurfelInterface</code>s to be returned
	 * @see #getFirstSurfel
	 * @see #getPrevSurfel
	 * @see #getCurrentSurfel
	 */
	CORE_API SurfelInterface *getNextSurfel();	

	/**
     * Returns the index-th surfel in the collection
	 *
	 * <b>Note</b>: no range control is performed
	 *
	 * @return the index-th surfel
	 */
	CORE_API SurfelInterface *getSurfel(const int index);


    /**
     * returns the surfel vector
	 */
	CORE_API std::vector<SurfelInterface*>* getSurfelVector();

    /**
	 * returns the position vector
	 */
	CORE_API std::vector<Vector3D>* getPositionVector();

	/**
	 * returns the normal vector
	 */
	CORE_API std::vector<Vector3D>* getNormalVector();

	/**
	 * Returns a const_iterator pointing to the beginning of the surfel vector
	 */
	const_iterator begin(void) const { return const_iterator(surfels, 0); 
	}

	/**
	 * Returns a const_iterator pointing to the end of the surfel vector
	 */
	const_iterator end(void) const { return const_iterator(surfels, getNofSurfels()); }
  
	/**
	 * Returns the number of surfels in the surfel collection
	 *
	 * @see #getNofSurfels()
	 */
	uint size() const { return getNofSurfels(); }


	// ********************
	// Surfel manipulations
	// ********************

	/**
	 * Check if the surfel collection contains a given surfel.
	 */
	CORE_API bool contains (const SurfelInterface *surfel);

	/**
	 * Adds a new <code>SurfelInterface</code> with the given properties (all other available properties are
	 * set to default values) to this <code>SurfelCollection</code> and returns a pointer to it.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param position
	 *        a <code>Vector3D</code> specifying the position of the surfel
	 * @param normal
	 *        a <code>Vector3D</code> specifying the normal of the surfel
	 * @param axisOne
	 *        a <code>Vector3D</code> specifying the tangent axisOne of the surfel
	 * @param axisTwo
	 *        a <code>Vector3D</code> specifying the tangent axisTwo of the surfel
	 * @param radius
	 *        the radius of the surfel
	 * @param diffuseColor
	 *        the <code>QRgb</code> value of the diffuse color of the surfel
	 * @param specularColor
	 *        the <code>QRgb</code> value of the specular color of the surfel
	 * @param flags
	 *        the <code>Flags</code> of the surfel
	 * @param ambientCoefficient
	 *        the ambient coefficient of the surfel; must be in [0.0, 1.0]
	 * @param diffuseCoefficient
	 *        the diffuse coefficient of the surfel; must be in [0.0, 1.0]
	 * @param specularCoefficient
	 *        the specular coefficient of the surfel; must be in [0.0, 1.0]
	 * @param shininess
	 *        the shininess of the surfel; must be in [0.0, 1.0]
	 * @param textureCoordinate
	 *        the <code>TextureCoordinate</code> which contains the UV texture coordinates
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @return a pointer to the newly inserted <code>SurfelInterface</code>
	 * @see #surfelCollectionChanged
	 */
	CORE_API SurfelInterface *addSurfel (const Vector3D position, const Vector3D normal,
		                                 const Vector3D axisOne, const Vector3D axisTwo,
		                                 const float radius, const QRgb diffuseColor, const QRgb specularColor, 
										 const SurfelInterface::Flags flags = SurfelInterface::NO_FLAGS,
									     const float ambientCoefficient = 0.5f, const float diffuseCoefficient = 0.75f,
									     const float specularCoefficient = 0.25f, const float shininess = 0.75,
										 const MyDataTypes::TextureCoordinate textureCoordinate = defaultTextureCoordinate,
								         const bool emitSignal = true);
	/**
	 * Adds a new <code>SurfelInterface</code> with default properties to this <code>SurfelCollection</code> and
	 * returns a pointer to it.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @return a pointer to the newly inserted <code>SurfelInterface</code>
	 * @see #surfelCollectionChanged
	 */
	CORE_API SurfelInterface *addSurfel (const bool emitSignal = true);

	/**
	 * Adds a <code>SurfelInterface</code> to this <code>SurfelCollection</code> by value.
	 * 
	 * <b>Note</b>: a copy of the surfel is generated and added to this <code>SurfelCollection</code>, making
	 *              sure that the <code>SurfelInterface</code>s within this <code>SurfelCollection</code>
	 *              have the same type.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to an existing <code>SurfelInterface</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @return a pointer to the newly inserted <code>SurfelInterface</code>
	 * @see #surfelCollectionChanged
	 */
	CORE_API SurfelInterface *addSurfel (const SurfelInterface *surfel, const bool emitSignal = true);

	/**
	 * Add surfels to this <code>SurfelCollection</code> by value.
	 *
	 * <b>Note</b>: a copy of each surfel is generated and added to this <code>SurfelCollection</code>, making
	 *              sure that the <code>SurfelInterface</code>s within this <code>SurfelCollection</code>
	 *              have the same type.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a pointer to a <code>std::vector<SurfelInterface*></code> containing all the <code>SurfelInterface</code>s
	 *        to be added to this <code>SurfelCollection</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #surfelCollectionChanged
	 */
	CORE_API void addSurfels (std::vector<SurfelInterface*> *surfels, const bool emitSignal = true);

	/**
	 * Adds the <code>SurfelInterface</code>s in <code>surfels</code> to this <code>SurfelCollection</code>.
	 * The <code>SurfelInterface</code>s in <code>surfels</code> must be of the same type as the
	 * <code>SurfelInterface</code>s in this <code>SurfelCollection</code>.
	 *
	 * <b>Attention</b>: The <code>SurfelCollection</code> <code>surfels</code> is invalidated by this operation!
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a pointer to a <code>SurfelCollection</code> which contains the <code>SurfelInterface</code> to
	 *        be added to this <code>SurfelCollection</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #surfelCollectionChanged
	 */
	CORE_API void merge (SurfelCollection *surfels, const bool emitSignal = true);

	/**
	 * Removes the current <code>SurfelInterface</code> in this <code>SurfelCollection</code> according
	 * to the internal iterator.
	 */
	CORE_API void removeCurrentSurfel();
	
	/**
	 * Removes the <code>surfel</code> from this <code>SurfelCollection</code>.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfel
	 *        a pointer to a <code>SurfelInterface</code> to be removed from this <code>SurfelCollection</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @return <code>true</code> if the <code>surfel</code> could be removed; <code>false</code> else
	 * @see #surfelCollectionChanged
	 */
	CORE_API bool removeSurfel (SurfelInterface *surfel, const bool emitSignal = true);

	/**
	 * Removes the <code>surfels</code> from this <code>SurfelCollection</code>. 
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a pointer to a <code>QList<SurfelInterface></code> which contains the <code>SurfelInterface</code>s
	 *        to be removed from this <code>SurfelCollection</code>;
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #surfelCollectionChanged
	 */
	CORE_API void removeSurfels (const QList<SurfelInterface> *surfels, const bool emitSignal = true);


	/**
	 * Removes the <code>surfels</code> from this <code>SurfelCollection</code>. 
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param surfels
	 *        a pointer to a <code>std::vector<SurfelInterface*></code> which contains the <code>SurfelInterface</code>s
	 *        to be removed from this <code>SurfelCollection</code>;
     *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 * @see #surfelCollectionChanged
	 */
	CORE_API void removeSurfels (const std::vector<SurfelInterface *>* surfels, const bool emitSignal = true);
	
	/**
	 * Makes a copy of the <code>sourceCollection</code>. All surfels contained in this <code>SurfelCollection</code>
	 * are removed and <code>delete</code>d before copying.
	 *
	 * @param sourceCollection
	 *        a pointer to the source <code>SurfelCollection</code> which is to be copied
	 *        into this <code>SurfelCollection</code>
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 */
	CORE_API void copy (SurfelCollection *sourceCollection, const bool emitSignal = true);

	/**
	 * Clears all <code>SurfelInterface</code>s from this <code>SurfelCollection</code>.\
	 *
	 * <b>Note</b>: Clear sets only the size (number of surfels) of the collection to 0.
	 *				The capacity of the surfel collection is not changed.
	 *
	 * <b>Signals:</b>
	 * <ul>
	 *   <li><code>surfelCollectionChanged</code></li>
	 * </ul>
	 *
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>

	 * @see #surfelCollectionChanged
	 */
	CORE_API void clear (const bool emitSignal = true);

	/**
	 * inserts or erases surfels at the end such that the size (number of surfels) becomes <code>newNofSurfels</code>
	 *
	 * @param newNofSurfels
	 *		the new number of surfels of the collection
	 * @param emitSignal
	 *        set to <code>false</code> if the signal <code>surfelCollectionChanged</code> should
	 *        be supressed; useful if multiple operations have to be done before the image gets
	 *        re-rendered; use with <b>caution</b>
	 */
	CORE_API void resize(const int newNOfSurfels, const bool emitSignal = true);

	/**
	 * reserves memory for the collection
	 *
	 * If <code>newCapacity</code> is smaller than #capacity, this call has no effect. 
	 * Otherwise, it is a request for allocation of additional memory. 
	 * If the request is successful, then capacity() is equal to <code>newCapacity</code>; 
	 * otherwise, #capacity is unchanged. In either case, #getNofSurfels is unchanged
	 *
	 * @param newCapacity
	 *		the new capacity of the collection if <code>newCapacity</code> is larger than the actual capacity
	 *
	 * @see #getNofSurfels
	 */
	CORE_API void reserve(const int newCapacity);

	/**
	 * Returns the capacity of the collection
	 *
	 * @return the number of surfels for which memory is allocated
	 *
	 * @see reserve
	 * @see getNofSurfels
	 */
	CORE_API inline int capacity() const;

	/**
	 * set the increment of the capacity if the collection needs to allocate memory
	 *
	 * @params incrCapacityFactor
	 *			if the collection needs to allocat memory the new capacity of the collection will be 
	 *			<code>capacity + (capacity*incrCapacityFactor) + 1</code>.
	 *			<code>incrCapacityFactor</code> must be equal or larger than 0.0
	 *
	 * @see #SurfelCollection
	 * @see #reserve
	 * @see #capacity
	 */
	CORE_API void setIncrementCapacityFactor(const float incrCapacityFactor);

	/**
	 * Create a neighborhood search structure for all surfels in the surfel collection
	 * 
	 * <b>Note</b>: The search structure gets invalid if the positions change or surfels are removed/inserted.
	 *				The user needs to delete the neighborhood.
	 *
	 * @return the neighborhood search structure or NULL if the positions are not valid
	 */
	CORE_API NeighbourHood* buildNeighbourHood ();

signals:

	/**
	 * Emitted whenever the <code>SurfelCollection</code> has been changed.
	 *
	 * <b>Note</b>: Renderers should not connect to this <code>surfelCollectionChanged</code> signal directly,
	 *              they should connect to the signal <code>Scene::objectModified</code> instead, since
	 *              internally the <code>Object</code> already connects to this signal, which on its turn informs
	 *              the <code>Scene</code>.
	 * @see Scene#objectModified
	 */
	void surfelCollectionChanged();

	/**
	 * Emitted whenever the type of the surfels in this <code>SurfelCollection</code> has been changed.
	 *
	 * <b>Note</b>: Renderers should not connect to this <code>surfelTypeChanged</code> signal directly,
	 *              they should connect to the signal <code>Scene::objectModified</code> instead, since
	 *              internally the <code>Scene</code> already connects to this signal, which on its turn
	 *              converts all the <code>SurfelCollection</code>s of the other <code>Object</code>s
	 *              in the <code>Scene</code>.
	 * @see Scene#objectobjectModified
	 */
	void surfelTypeChanged (const SurfelInterface::PropertyDescriptor newPropertyDescriptor);





// Virtual Table (VT)
private:

	friend class SurfelInterface;

    Vector3D						(SurfelCollection::*_getPosition)(const int index);
	Vector3D						(SurfelCollection::*_getNormal)(const int index) ;
	Vector3D						(SurfelCollection::*_getAxisOne)(const int index);	
	Vector3D						(SurfelCollection::*_getAxisTwo)(const int index);	
	float							(SurfelCollection::*_getRadius)(const int index);	
	QRgb							(SurfelCollection::*_getDiffuseColor)(const int index) ;
	QRgb							(SurfelCollection::*_getSpecularColor)(const int index) ;
	bool							(SurfelCollection::*_isFlagOn)(const int index, const SurfelInterface::Flags flags) ;
	bool							(SurfelCollection::*_isSelected)(const int index) ;
	SurfelInterface::Flags							(SurfelCollection::*_getFlags)(const int index) ;
	float							(SurfelCollection::*_getAmbientCoefficient)(const int index) ;
	float							(SurfelCollection::*_getDiffuseCoefficient)(const int index) ;
	float							(SurfelCollection::*_getSpecularCoefficient)(const int index) ;
	float							(SurfelCollection::*_getShininess)(const int index) ;
	MyDataTypes::TextureCoordinate	(SurfelCollection::*_getTextureCoordinate)(const int index) ;
	unsigned int					(SurfelCollection::*_getNofAssociatedSurfels)(const int index) ;
	SurfelInterface*				(SurfelCollection::*_getAssociatedSurfel)(const int index, const int idx) ;
	Vector3D						(SurfelCollection::*_getDetailVector)(const int index) ;
	float							(SurfelCollection::*_getDetailCoefficient)(const int index) ;
	void							(SurfelCollection::*_setDetailVector)(const int index, const Vector3D detailVector);
	void							(SurfelCollection::*_setDetailCoefficient)(const int index, const float detailCoefficient);

	void							(SurfelCollection::*_setPosition)(const int index, const Vector3D newPosition);
	void							(SurfelCollection::*_setNormal)(const int index, const Vector3D newNormal);
	void							(SurfelCollection::*_setTangentAxes)(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							(SurfelCollection::*_setNormalTangentAxesSystem)(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							(SurfelCollection::*_setRadius)(const int index, float r);
	void							(SurfelCollection::*_setDiffuseColor)(const int index, const QRgb newDiffuseColor);
	void							(SurfelCollection::*_setSpecularColor)(const int index, const QRgb newSpecularColor);
	void							(SurfelCollection::*_setFlagOn)(const int index, const SurfelInterface::Flags flags, const bool on);
	void							(SurfelCollection::*_setFlags)(const int index, const SurfelInterface::Flags newFlags);
	void							(SurfelCollection::*_setAmbientCoefficient)(const int index, const float newAmbientCoefficient);
	void							(SurfelCollection::*_setDiffuseCoefficient)(const int index, const float newDiffuseCoefficient);
	void							(SurfelCollection::*_setSpecularCoefficient)(const int index, const float newSpecularCoefficient);
	void							(SurfelCollection::*_setShininess)(const int index, const float newShininess);
	void							(SurfelCollection::*_setTextureCoordinate)(const int index, const MyDataTypes::TextureCoordinate newTextureCoordinate);
	bool							(SurfelCollection::*_addAssociatedSurfel)(const int index, SurfelInterface *surfel);
	void							(SurfelCollection::*_clearAssociatedSurfels)(const int index);


private:

	Vector3D						getPositionNative(const int index);
	Vector3D						getNormalNative(const int index);
	Vector3D						getNormalDerivedFromTangentAxes(const int index);
	Vector3D						getAxisOneNative(const int index);	
	Vector3D						getAxisTwoNative(const int index);	
	Vector3D						getAxesOneDerivedFromNormal(const int index);
	Vector3D						getAxesTwoDerivedFromNormal(const int index);
	float							getRadiusNative(const int index);	
	float							getRadiusDerivedFromTangentAxes(const int index);
	QRgb							getDiffuseColorNative(const int index);
	QRgb							getSpecularColorNative(const int index);
	bool							isFlagOnNative(const int index, const SurfelInterface::Flags flags) ;
	bool							isSelectedNative(const int index) ;
	SurfelInterface::Flags							getFlagsNative(const int index) ;
	float							getAmbientCoefficientNative(const int index) ;
	float							getDiffuseCoefficientNative(const int index) ;
	float							getSpecularCoefficientNative(const int index) ;
	float							getShininessNative(const int index) ;
	MyDataTypes::TextureCoordinate	getTextureCoordinateNative(const int index) ;
	unsigned int					getNofAssociatedSurfelsComputed(const int index) ;
	SurfelInterface*				getAssociatedSurfelNative(const int index, const int idx);
	Vector3D						getDetailVectorNative(const int index);
	float							getDetailCoefficientNative(const int index);
	void							setDetailVector(const int index, const Vector3D detailVector);
	void							setDetailCoefficient(const int index, const float detailCoefficient);

	void							setPosition(const int index, const Vector3D newPosition);
	void							setNormal(const int index, const Vector3D newNormal);
	void							setNormalComputeAxesFromNormalAndRadius(const int index, const Vector3D newNormal);
	void							setNormalComputeAxesFromNormal(const int index, const Vector3D newNormal);
	void							setNormalTangentAxesSystem(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setNormalTangentAxesSystemComputeRadius(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setNormal(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxes(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxes(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxesComputeRadius(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxesComputeRadius(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxesComputeNormal(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxesComputeNormal_Radius(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setTangentAxesFromNormalAndRadius(const int index, const Vector3D newNormal);
	void							setTangentAxesFromNormal(const int index, const Vector3D newNormal);
	void							setRadius(const int index, float r);
	void							scaleAxes(const int index, float r);
	void							setRadiusScaleAxes(const int index, float r);
	void							setDiffuseColor(const int index, const QRgb newDiffuseColor);
	void							setSpecularColor(const int index, const QRgb newSpecularColor);
	void							setFlagOn(const int index, const SurfelInterface::Flags flags, const bool on);
	void							setFlags(const int index, const SurfelInterface::Flags newFlags);
	void							setAmbientCoefficient(const int index, const float newAmbientCoefficient);
	void							setDiffuseCoefficient(const int index, const float newDiffuseCoefficient);
	void							setSpecularCoefficient(const int index, const float newSpecularCoefficient);
	void							setShininess(const int index, const float newShininess);
	void							setTextureCoordinate(const int index, const MyDataTypes::TextureCoordinate newTextureCoordinate);
	void							copySurfelProperties(const int index, const SurfelInterface *copySurfel);
	void							combineColorProperties(const int index, const SurfelInterface *copySurfel1, const SurfelInterface *copySurfel2, const float factor);
	bool							addAssociatedSurfel(const int index, SurfelInterface *surfel);
	void							clearAssociatedSurfels(const int index);

	Vector3D						getPositionEmpty(const int index);
	Vector3D						getNormalEmpty(const int index);
	Vector3D						getAxisOneEmpty(const int index);	
	Vector3D						getAxisTwoEmpty(const int index);	
	void							setNormalTangentAxesSystemEmpty(const int index, const Vector3D newNormal, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	float							getRadiusEmpty(const int index);	
	QRgb							getDiffuseColorEmpty(const int index);
	QRgb							getSpecularColorEmpty(const int index);
	bool							isFlagOnEmpty(const int index, const SurfelInterface::Flags flags) ;
	bool							isSelectedEmpty(const int index) ;
	SurfelInterface::Flags							getFlagsEmpty(const int index) ;
	float							getAmbientCoefficientEmpty(const int index) ;
	float							getDiffuseCoefficientEmpty(const int index) ;
	float							getSpecularCoefficientEmpty(const int index) ;
	float							getShininessEmpty(const int index) ;
	MyDataTypes::TextureCoordinate	getTextureCoordinateEmpty(const int index) ;
	unsigned int					getNofAssociatedSurfelsEmpty(const int index) ;
	SurfelInterface*				getAssociatedSurfelEmpty(const int index, const int idx);
	Vector3D						getDetailVectorEmpty(const int index);
	float							getDetailCoefficientEmpty(const int index);
	void							setDetailVectorEmpty(const int index, const Vector3D detailVector);
	void							setDetailCoefficientEmpty(const int index, const float detailCoefficient);

	void							setPositionEmpty(const int index, const Vector3D newPosition);
	void							setNormalEmpty(const int index, const Vector3D newNormal);
	void							setTangentAxesEmpty(const int index, const Vector3D newAxisOne, const Vector3D newAxisTwo);
	void							setRadiusEmpty(const int index, float r);
	void							setDiffuseColorEmpty(const int index, const QRgb newDiffuseColor);
	void							setSpecularColorEmpty(const int index, const QRgb newSpecularColor);
	void							setFlagOnEmpty(const int index, const SurfelInterface::Flags flags, const bool on);
	void							setFlagsEmpty(const int index, const SurfelInterface::Flags newFlags);
	void							setAmbientCoefficientEmpty(const int index, const float newAmbientCoefficient);
	void							setDiffuseCoefficientEmpty(const int index, const float newDiffuseCoefficient);
	void							setSpecularCoefficientEmpty(const int index, const float newSpecularCoefficient);
	void							setShininessEmpty(const int index, const float newShininess);
	void							setTextureCoordinateEmpty(const int index, const MyDataTypes::TextureCoordinate newTextureCoordinate);
	bool							addAssociatedSurfelEmpty(const int index, SurfelInterface *surfel);
	void							clearAssociatedSurfelsEmpty(const int index);

protected:
	/**
	 * detaches the surfel pointer from the collection and sets the size of the collection to zero.
	 *
	 * If the surfel collection is deleted it does not delete the surfel references (but the surfel data).
	 * Therefore the surfel references need to be deleted by the user.
	 * detach is used internally when merging two collections
	 */
	void detach();

protected:
	typedef struct axes {
		Vector3D axisOne;
		Vector3D axisTwo;
	} Axes;

	typedef struct detail {
		Vector3D detailVector;
		float    detailCoefficient;
	} Detail;

	// Data arrays
	std::vector<SurfelInterface*>				*surfels;						
	std::vector<Vector3D>						*positions;						
	std::vector<Vector3D>						*normals;
	std::vector<Axes>							*axes;
	std::vector<float>							*radius;
	std::vector<QRgb>							*diffuseColors;
	std::vector<QRgb>							*specularColors;
	std::vector<float>							*ambientCoefficients;
	std::vector<float>							*diffuseCoefficients;
	std::vector<float>							*specularCoefficients;
	std::vector<float>							*shininess;
	std::vector<MyDataTypes::TextureCoordinate>	*textureCoordinates;
	std::vector<SurfelInterface::Flags>			*flags;
	std::vector<AssociatedSurfels>				*associatedSurfels;
	std::vector<Detail>							*details;


	SurfelInterface::PropertyDescriptor		nativePropertyDescriptor;		// the current type of surfels in this collection
	SurfelInterface::PropertyDescriptor		computedPropertyDescriptor;		// the computed type of surfels in this collection
	int										nOfSurfels;						// number of valid surfels in the collection
	int										nCapacity;						// allocated memory for the capacity number of surfels
	float									incrCapacityFactor;				// if new memory has to be allocated, the capacity will be capacity + incrCapacityFactor * capacity + 1
	int										currentSurfelIndex;				// current position of the iterator

private:

	typedef struct NAS {
		Vector3D	normal;
		float		radius;
		Vector3D	axisOne;
		Vector3D	axisTwo;
	} NormalAxesSystem;

	NormalAxesSystem						normalAxesSystem;				// store computation of axes from normal for reuse

	// creates and initiales a new surfel
	inline SurfelInterface* SurfelCollection::createNewSurfel(const int index);
	// removes and destroies a surfel
	inline void removeSurfel(const int index);
	// returns fac*color1 + (1.0-fac)*color2
	inline QRgb mixColor(const QRgb color1, const QRgb color2, const float fac) const;
};


inline void SurfelCollection::removeSurfel(const int index) {

	nOfSurfels--;
	delete surfels->at(index);

	if (associatedSurfels) {
		associatedSurfels->at(index).destroy();
	}

	if (index < nOfSurfels) {
		// copy the properties of the last surfel in the collection to index
		surfels->at(index) = surfels->at(nOfSurfels);	// the surfel reference is still valid,
		surfels->at(index)->index = index;				// but the index changes!
		if (positions)				positions->at(index) = positions->at(nOfSurfels);
		if (normals)				normals->at(index) = normals->at(nOfSurfels);
		if (axes)					axes->at(index) = axes->at(nOfSurfels);
		if (radius) 				radius->at(index) = radius->at(nOfSurfels);
		if (diffuseColors)			diffuseColors->at(index) = diffuseColors->at(nOfSurfels);
		if (specularColors) 		specularColors->at(index) = specularColors->at(nOfSurfels);
		if (ambientCoefficients)	ambientCoefficients->at(index) = ambientCoefficients->at(nOfSurfels);
		if (diffuseCoefficients) 	diffuseCoefficients->at(index) = diffuseCoefficients->at(nOfSurfels);
		if (specularCoefficients) 	specularCoefficients->at(index) = specularCoefficients->at(nOfSurfels);
		if (shininess)				shininess->at(index) = shininess->at(nOfSurfels);
		if (textureCoordinates) 	textureCoordinates->at(index) = textureCoordinates->at(nOfSurfels);
		if (flags) 					flags->at(index) = flags->at(nOfSurfels);
		if (associatedSurfels)	    associatedSurfels->at(index) = associatedSurfels->at(nOfSurfels).detach();
		if (details)				details->at(index) = details->at(nOfSurfels);

	}
}


inline SurfelInterface* SurfelCollection::createNewSurfel(const int index) {
	SurfelInterface *newSurfel;
	newSurfel = new SurfelInterface();
	newSurfel->init(index, this);
	surfels->at(index) = newSurfel;

	if (associatedSurfels) {
		associatedSurfels->at(index).init();
	}

	return newSurfel;
}

inline int SurfelCollection::capacity() const {
	return nCapacity;
}

inline uint SurfelCollection::getNofSurfels() const {
	return nOfSurfels;
}

inline QRgb SurfelCollection::mixColor(const QRgb color1, const QRgb color2, const float fac) const{
	
	QColor col1 = QColor(color1);
	QColor col2 = QColor(color2);
	return QColor(  col1.red()*fac + col2.red()*(1.0-fac),
					col1.green()*fac + col2.green()*(1.0-fac),
					col1.blue()*fac + col2.blue()*(1.0-fac)
				 ).rgb();
}


#endif  // __SURFELCOLLECTION_H_


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
