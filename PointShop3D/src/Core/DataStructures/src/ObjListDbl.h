// Title:   ObjListDbl.h
// Created: Thu Sep 25 14:29:59 2003
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

#ifndef __OBJLISTDBL_H_
#define __OBJLISTDBL_H_

/**
 * a double linked list of objects
 *
 * @author Richard Keiser
 * @version 2.0
 */
template<class Obj>
class ObjListDbl {

public:

	/**
	 * creates the object list
	 */
	ObjListDbl ();

	/**
	 * destrois the object list
	 */
	virtual ~ObjListDbl (); 

	/**
	 * returns the number of objects in the list
	 *
	 * @return the number of objects
	 */
	unsigned int count () const;

	/**
	 * returns true if no object is in the list
	 *
	 * @return true if the list is empty
	 */
	bool isEmpty () const;

	/**
	 * if auto delete is enabled, the inserted objects are deleted when the list is destroyed or they are removed from the list.
	 *
	 * Auto delete is false by default.
	 *
	 * <b>Note</b>: if auto delete is enabled, <code>Obj</code> needs to be a pointer
	 *
	 * @param enable
	 *		  true if auto delete should be enabled.
	 */
	void setAutoDeleteEnabled (const bool enable);

	/**
	 * insert <code>obj</code> at the end of the list
	 *
	 * @param obj
	 *		  object to insert
	 */
	void append (Obj obj);
  
	/**
	 * insert <code>obj</code> at the beginning of the list
	 *
	 * @param obj
	 *		  object to insert
	 */
	void prepend (Obj obj);
	
	/**
	 * insert <code>obj</code> after the current object
	 *
	 * @param obj
	 *		  object to insert
	 */
	void insert (Obj obj);
	
	/**
	 * remove current object
	 *
	 * if auto delete is enabled, the object is deleted
	 *
	 * @return true if the object could be deleted
	 *
	 * @see #setAutoDeleteEnabled
	 */
	bool remove ();

	/**
	 * removes all objects
	 *
	 * if auto delete is enabled, the objects are deleted
	 *
	 * @see #setAutoDeleteEnabled
	 */	 
	virtual void clear ();

	/**
	 * returns the current object
	 */
	Obj current () const;

	/**
	 * returns the first object in the list
	 */
	Obj getFirst () const;

	/**
	 * returns the last object in the list
	 */
	Obj getLast () const;

	/**
	 * returns the next object in the list
	 */
	Obj getNext () const;

	/**
	 * returns the previous object in the list
	 */
	Obj getPrev () const;

	/**
	 * set the iterator to the first object and returns it
	 */
	Obj first ();

	/**
	 * set the iterator to the last object and returns it
	 */
	Obj last ();

	/**
	 * set the iterator to the next object and returns it
	 */
	Obj next ();

	/**
	 * set the iterator to the previous object and returns it
	 */
	Obj prev ();

private:

	typedef struct element {
		Obj				obj;
		struct element* next;
		struct element* prev;
	} Element;


	Element *firstObj;
	Element *lastObj;
	Element *currentObj;
	int nObjs;
	bool autoDelete;
};

template<class Obj>
ObjListDbl<Obj>::ObjListDbl () {

	firstObj = 0;
	lastObj = 0;
	currentObj = 0;
	nObjs = 0;
	autoDelete = false;
}

template<class Obj>
ObjListDbl<Obj>::~ObjListDbl () {

	clear();
}

template<class Obj>
unsigned int ObjListDbl<Obj>::count () const {

	return nObjs;
}

template<class Obj>
bool ObjListDbl<Obj>::isEmpty () const {

	return firstObj ? true : false;
}

template<class Obj>
void ObjListDbl<Obj>::append (Obj obj) {

	nObjs++;
	
	Element* el = new Element;
	el->obj = obj;

	if (lastObj == 0) {
		firstObj = el;
		firstObj->prev = NULL;
		firstObj->next = NULL;
	} 
	else {
		lastObj->next = el;
		el->prev = lastObj;
		el->next = 0;
	}
	lastObj = el;

	currentObj = el;
}

template<class Obj>
void ObjListDbl<Obj>::prepend (Obj obj) {

	nObjs++;

	Element *el = new Element;
	el->obj = obj;

	if (firstObj == 0) {
		el->next = 0;
		el->prev = 0;

		lastObj = el;
	} 
	else {
		firstObj->prev = el;
		el->next = firstObj;
		el->prev = 0;
	}
	firstObj = el;

	currentObj = el;
}

template<class Obj>
void ObjListDbl<Obj>::insert (Obj obj) {
	
	if (currentObj == 0) {
		append(obj);
		return;
	}
	
	nObjs++;

	Element *el = new Element;
	el->obj = obj;
	el->next = currentObj->next;
	el->prev = currentObj;

	el->next->prev = el;
	currentObj->next = el;
	currentObj = el;
}

template<class Obj>
bool ObjListDbl<Obj>::remove () {

	if (currentObj) {
		Element *el = currentObj;

		nObjs--;

		if(currentObj == firstObj) {
			firstObj = currentObj->next;
			firstObj->prev = 0;
			currentObj = currentObj->next;

			if(currentObj == lastObj) {
				lastObj = 0;
			}
		} 
		else {
			currentObj->prev->next = currentObj->next;

			if (currentObj == lastObj) {
				lastObj = currentObj->prev;
				lastObj->next = 0;
				currentObj = lastObj;
			}
			else {
				currentObj->next->prev = currentObj->prev;
				currentObj = currentObj->next;
			}
		}

		if (autoDelete == true) {
			delete el->obj;
		}

		delete el;

		return true;
	}

	return false;
}

template<class Obj>
void ObjListDbl<Obj>::clear () {

	Element *obj,
			*tmp;

	obj = firstObj;
	while(obj != 0) {
		tmp = obj;
		obj = obj->next;

		if (autoDelete) {
			delete tmp->obj;
		}
		delete tmp;
	}

	firstObj = 0;
	lastObj = 0;
	currentObj = 0;
	nObjs = 0;
}

template<class Obj>
Obj ObjListDbl<Obj>::current () const {

	return currentObj->obj;
}

template<class Obj>
Obj ObjListDbl<Obj>::getFirst () const {

	return firstObj->obj;
}

template<class Obj>
Obj ObjListDbl<Obj>::getNext () const {

	if (currentObj && currentObj->next) {
		return currentObj->next->obj;
	}
	else {
		return NULL;
	}
}

template<class Obj>
Obj ObjListDbl<Obj>::getPrev () const {

	if (currentObj && currentObj->prev) {
		return currentObj->prev->obj;
	}
	else {
		return NULL;
	}
}

template<class Obj>
Obj ObjListDbl<Obj>::getLast () const {

	return lastObj->obj;
}

template<class Obj>
Obj ObjListDbl<Obj>::first () {

	currentObj = firstObj;
	return firstObj->obj;
}

template<class Obj>
Obj ObjListDbl<Obj>::last () {

	currentObj = lastObj;
	return lastObj->obj;
}

template<class Obj>
Obj ObjListDbl<Obj>::next () {

	if(currentObj->next) {
		currentObj = currentObj->next;
		return currentObj->obj;
	} 
	else {
		return 0;
	}
}

template<class Obj>
Obj ObjListDbl<Obj>::prev () {

	if(currentObj->prev) {
		currentObj = currentObj->prev;
		return currentObj->obj;
	}
	else {
		return 0;
	}
}

template<class Obj>
void ObjListDbl<Obj>::setAutoDeleteEnabled(bool enable) {

	autoDelete = enable;
}






#endif //__OBJLISTDBL_H_
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
