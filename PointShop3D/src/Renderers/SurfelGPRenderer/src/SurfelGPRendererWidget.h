//
// Title:   SurfelGPRendererWidget.h
// Created: Thu Sep 25 14:51:14 2003
// Authors: Oliver Knoll, Mark Pauly, Matthias Zwicker
//
// Copyright (c) 2001, 2002, 2003
// Computer Graphics Lab, ETH Zurich
// Mitsubishi Electric Research Laboratories (MERL), Cambridge MA
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
// IN NO EVENT SHALL MERL AND/OR ETH BE LIABLE TO ANY PARTY
// FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS
// SOFTWARE AND ITS DOCUMENTATION, EVEN IF MERL OR ETH HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
// 
// MERL AND ETH SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
// HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER MERL NOR ETH HAVE
// ANY OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
// ENHANCEMENTS, OR MODIFICATIONS.
// 
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// Contact info@pointshop3d.com if any conditions of this
// licensing are not clear to you.
//

#ifndef _SURFELGPRENDERERWIDGET_H_
#define _SURFELGPRENDERERWIDGET_H_

#include <qwidget.h>
#include <qevent.h>
#include <qtimer.h>

#include "SurfelGPFrameBuffer.h"

/**
 * This class contains the rendered image in a <code>QWidget</code>.
 *
 * @author Oliver Knoll
 * @version 1.2
 */
class SurfelGPRendererWidget : public QWidget {

	Q_OBJECT

public:

	SurfelGPRendererWidget (const FrameBufferInterface *frameBuffer, QWidget *parent = 0, const char *name = 0);
	virtual ~SurfelGPRendererWidget();

	void setFrameBuffer (const FrameBufferInterface *frameBuffer);
	const FrameBufferInterface *getFrameBuffer() const;

	QSize sizeHint() const;
	QSize minimumSizeHint() const;
	QSizePolicy sizePolicy() const;

signals:

	void widgetRepainted();

	/**
	 * Emitted whenever this <code>SurfelGPRendererWidget</code> has been resized.
	 *
	 * @param re
	 *        a pointer to the specific <code>QResizeEvent</code>
	 */
	void widgetResized (QResizeEvent *re);

protected:

	void mousePressEvent (QMouseEvent *me);
	void mouseMoveEvent (QMouseEvent *me);
	void mouseReleaseEvent (QMouseEvent *me);
	void keyPressEvent (QKeyEvent *ke);
	void wheelEvent (QWheelEvent *we);
	
	void paintEvent (QPaintEvent *pe);
	void resizeEvent (QResizeEvent *re);

private:

	const FrameBufferInterface *frameBuffer;
	QTimer                     *resizeTimer;
	bool                       isResizing;

private slots:

	void handleResizeTimerDone();

};

#endif  // _SURFELGPRENDERERWIDGET_H_

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
