// Title:   ColorConversion.h
// Created: Thu Sep 25 15:02:03 2003
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

#include <qcolor.h>

#include "UtilitiesDLL.h"

/**
 * This namespace provides simple color conversion functions.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
namespace ColorConversion {
  
  /**
   * Swaps the red channel with the blue channel, so
   * RGB becomes BGR and vice versa. The most significant
   * byte is the alpha channel.
   *
   * @param color
   *        the <code>QRgb</code> color which needs to be swapped
   * @return the <code>QRgb</code> color with swapped red and blue channels
   */
  inline QRgb RGBswapBGR (QRgb color) {
    
    char *ptr;
    QRgb retVal;
    
    ptr = (char *)&retVal;
    
    *ptr++ = (color & 0x00ff0000) >> 16;
    *ptr++ = (color & 0x0000ff00) >> 8;
    *ptr++ = (color & 0x000000ff);
    *ptr   = (color & 0xff000000);
    
    return retVal;
    
  }

};

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
