// Title:   FifoQueue.h
// Created: Thu Sep 25 14:30:03 2003
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

#ifndef __FIFOQUEUE_H_
#define __FIFOQUEUE_H_

/**
 * Fifo queue implemented as circual buffer
 *
 * @author Richard Keiser
 * @version 2.0
 */
template<class Obj>
class FifoQueue {

public:
	FifoQueue(unsigned int bufferSize) {
		m_bufferSize = bufferSize + 1;
		m_array = new Obj[m_bufferSize];	// always leave at least one cell free as a sentinel
		m_in = 0;
		m_out = 0;
	}

	~FifoQueue() {
		delete[] m_array;
	}

	inline void reset() {
		m_in = m_out = 0;
	}

	inline bool empty() {
		return (m_in == m_out);
	}

	inline bool full() {
		return (m_in+1) % m_bufferSize == m_out;
	}
	
	inline void enqueue(Obj obj) {
		ASSERT(!full());
		m_array[m_in] = obj;
		m_in = (m_in + 1) % m_bufferSize;
	}

	inline Obj front() {
		return m_array[m_out];
	}

	inline void dequeue() {
		ASSERT(!empty());
		m_out = (m_out + 1) % m_bufferSize;
	}

	inline int nbrOfElements() {
		if (m_out < m_in) {
			return m_in-m_out;
		}
		else {
			return m_bufferSize - (m_out-m_in);
		}
	}

private:
	Obj*			m_array;
	unsigned int	m_bufferSize;
	unsigned int	m_in;
	unsigned int	m_out;
};


#endif
// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
