// Title:   StringTools.h
// Created: Thu Sep 25 15:01:31 2003
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

#ifndef __STRINGTOOLS_H_
#define __STRINGTOOLS_H_

#include <qstring.h>

#include "UtilitiesDLL.h"

/**
 * This namespace provides utility functions for strings.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace StringTools {

	/**
	 * Appends a <code>sequenceNumber</code> to the <code>string</code>,
	 * for example <em>0001</em> (<code>sequenceNumber</code> = 1, <code>nofDigits</code> = 4).
	 *
	 * @param string
	 *        the <code>QString</code> to which the <code>sequenceNumber</code> has to be appended
	 * @param sequenceNumber
	 *        the sequence number to be appended; must be representable with <code>nofDigits</code> digits
	 * @param nofDigits
	 *        the number of digits to be used; the sequence number is filled up with leading zeros
	 * @return a <code>QString</code> containing the <code>string</code> with the <code>sequenceNumber</code>
	 *         appended
	 */
	UTILITIES_API QString appendSequenceNumber (const QString string, const uint sequenceNumber,
		                                        const uint nofDigits);

	/**
	 * Returns the sequence number of the <code>string</code>, that is the last number in the <code>string</code>
	 * which consists of at least <code>nofDigits</code>. If no such sequence number could be found -1 is returned.
	 * Examples:
	 *
	 * <table>
	 *   <tr>
	 *     <td><code><b>string</b></code></td>
	 *     <td><code><b>nofDigits</b></code></td>
	 *     <td><code><b>position</b></code></td>
	 *     <td><code><b>result</b></code></td>
	 *   </tr>
	 *   <tr>
	 *     <td>string_001</td>
	 *     <td>3</td>
	 *     <td>0</td>
	 *     <td>1</td>
	 *   </tr>
	 *   <tr>
	 *     <td>string_001</td>
	 *     <td>3</td>
	 *     <td>8</td>
	 *     <td>-1</td>
	 *   </tr>
	 *   <tr>
	 *     <td>string_001</td>
	 *     <td>4</td>
	 *     <td>0</td>
	 *     <td>-1</td>
	 *   </tr>
	 *   <tr>
	 *     <td>string123456</td>
	 *     <td>3</td>
	 *     <td>0</td>
	 *     <td>456</td>
	 *   </tr>
	 *   <tr>
	 *     <td>string_001</td>
	 *     <td>0</td>
	 *     <td>0</td>
	 *     <td>-1</td>
	 *   </tr>
	 * </table>
	 *
	 * @param string
	 *        a <code>QString</code> to be parsed
	 * @param nofDigits
	 *        the number of digits the sequence number to be parsed must consist of
	 * @param position
	 *        the position from where on the sequence number should be parsed; positions start at 0
	 * @return the sequence number        
	 */
	UTILITIES_API int getSequenceNumber (const QString string, const uint nofDigits, const uint position);

};

#endif  // __STRINGTOOLS_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
