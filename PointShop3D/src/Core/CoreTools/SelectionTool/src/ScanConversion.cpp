// Title:   ScanConversion.cpp
// Created: Thu Sep 25 14:21:58 2003
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

#include <qrect.h>
#include <qpointarray.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>  // for qsort()
#include <memory.h>  // for memcpy()

#include "ScanConversion.h"

typedef struct {                /* a polygon edge */
    double x;   /* x coordinate of edge's intersection with current scanline */
    double dx;  /* change in x with respect to y */
    int i;      /* edge number: edge i goes from pt[i] to pt[i+1] */
} Edge;

static int n;                   /* number of vertices */
static const QPointArray *pointArray; /* vertices */

static int nact;                /* number of active edges */
static Edge *active;            /* active edge list:edges crossing scanline y */

/* remove edge i from active list */
static void cdelete (int i) {
    int j;

    for (j = 0; j < nact && active[j].i != i; j++);
    
	if (j>=nact) {
		return;        /* edge not in active list; happens at win->y0*/
	}
    nact--;
    memcpy (&active[j], &active[j+1], (nact-j)*sizeof active[0]);
}

/* append edge i to end of active list */
static void cinsert(int i, int y) {

    int j;
    double dx;
    QPoint p, q;

    j = i < n-1 ? i+1 : 0;
    if (pointArray->point(i).y() < pointArray->point(j).y()) {
		p = pointArray->point(i);
		q = pointArray->point(j);
	}
    else {
		p = pointArray->point(j);
		q = pointArray->point(i);
	}
	
    /* initialize x position at intersection of edge with scanline y */
    dx = (double) (q.x() - p.x()) / (double) (q.y() - p.y());
    active[nact].dx = dx;
    active[nact].x = dx * (y + 0.5 - p.y()) + p.x();
    active[nact].i = i;
    nact++;

}

/* comparison routines for qsort */
int compare_ind (const void *u, const void *v)  {
	return pointArray->point (*(int *)u).y() <= pointArray->point (*(int *)v).y() ? -1 : 1;
}

int compare_active (const void *u, const void *v) {
	return ((Edge *)u)->x <= ((Edge *)v)->x ? -1 : 1;
}

int ScanConversion::concavePolygon (const int nofPoints,
								    const QPointArray *polygonLine,
									const QRect clippingRectangle,
									const SpanFunction spanFunction,							
									SurfelInterface::Flag selectionFlag,
									bool select) {

	int numCoveredSurfels = 0;
    int k, y0, y1, y, i, j, xl, xr;
    int *ind;           /* list of vertex indices, sorted by pt[ind[j]].y */

    n = nofPoints;
    if (n <= 0) {
      return 0;
    }
    pointArray = polygonLine;

    ind = new int[n];
    active = new Edge[n];

    /* create y-sorted array of indices ind[k] into vertex list */
    for (k=0; k<n; k++)
        ind[k] = k;
    qsort (ind, n, sizeof ind[0], compare_ind);  /* sort ind by pt[ind[k]].y */

    nact = 0;                           /* start with empty active list */
	k = 0;

	/* ymin of polygon */
    if (clippingRectangle.top() > (int) ceil (pointArray->point (ind[0]).y() - 0.5)) {
		y0 = clippingRectangle.top();
	}
	else {
		y0 = (int) ceil (pointArray->point (ind[0]).y() - 0.5);
	}

	/* ymax of polygon */
	if (clippingRectangle.bottom() < (int) floor (pointArray->point (ind[n-1]).y() - 0.5)) {
		y1 = clippingRectangle.bottom();
	}
	else {
		y1 = (int) floor (pointArray->point (ind[n-1]).y() -0.5);
	}


    for (y = y0; y <= y1; y++) {            /* step through scanlines */
        /* scanline y is at y+.5 in continuous coordinates */

        /* check vertices between previous scanline and current one, if any */
         while (k < n && pointArray->point (ind[k]).y() <= y + 0.5) {

            /* to simplify, if pt.y=y+.5, pretend it's above */
            /* invariant: y-.5 < pt[i].y <= y+.5 */
            i = ind[k]; 
            /*
             * insert or delete edges before and after vertex i (i-1 to i,
             * and i to i+1) from active list if they cross scanline y
             */
            j = i > 0 ? i-1 : n-1;        /* vertex previous to i */
            if (pointArray->point(j).y() <= y - 0.5) {        /* old edge, remove from active list */
                cdelete(j);
			}
            else if (pointArray->point(j).y() > y + 0.5) {   /* new edge, add to active list */
                cinsert(j, y);
                	}
            j = i < n-1 ? i+1 : 0;        /* vertex next after i */
            if (pointArray->point (j).y() <= y - 0.5) {        /* old edge, remove from active list */
                cdelete(i);
               	}
            else if (pointArray->point (j).y() > y + 0.5) {   /* new edge, add to active list */
                cinsert(i, y);
         	}

			k++;

        }

        /* sort active edge list by active[j].x */
        qsort (active, nact, sizeof active[0], compare_active);

        /* draw horizontal segments for scanline y */

		

		/* draw horizontal segments */
        for (j = 0; j < nact; j += 2) {			
			
            /* span between j & j+1 is inside, span between j+1 & j+2 is outside */
            xl = (int) ceil (active[j].x - 0.5);          /* left end of span */
            if (xl < clippingRectangle.left()) {
				xl = clippingRectangle.left();
			}
            xr = (int) floor (active[j+1].x - 0.5);       /* right end of span */
            if (xr > clippingRectangle.right()) {
				xr = clippingRectangle.right();
			}
            if (xl <= xr) {
                numCoveredSurfels += (*spanFunction) (y, xl, xr, selectionFlag, select);         /* draw pixels in span */
			}
            active[j].x += active[j].dx;        /* increment edge coords */
            active[j+1].x += active[j+1].dx;

        }

    }

	delete[] ind;
	delete[] active;

	return numCoveredSurfels;

}

int ScanConversion::ellipse (const int a, const int b,
						     const int centerX, const int centerY,
                             const QRect clippingRectangle,
                             const SpanFunction spanFunction,
							 SurfelInterface::Flag selectionFlag,
							 bool select) {
	int numCoveredSurfels = 0;
	int   x, y;
	float d1, d2;
	int   left, right;

	x = 0;
	y = b;

	d1 = b * b - a * a * b + a * a / 4;
	
	while (a * a * (y - 0.5f) > b * b * (x + 1)) {

		if (d1 < 0) {
			d1 += b * b * (2 * x + 3);
		}
		else {
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);

			// rasterize the previous line
			
			// clipping
			left  = (centerX - x >= clippingRectangle.left())  ? centerX - x : clippingRectangle.left();
			right = (centerX + x <  clippingRectangle.right()) ? centerX + x : clippingRectangle.right();
			if (centerY + y < clippingRectangle.bottom()) { 
				numCoveredSurfels += (*spanFunction) (centerY + y, left, right, selectionFlag, select);
			}
		
			if (centerY - y >= clippingRectangle.top()) {
				numCoveredSurfels += (*spanFunction) (centerY - y, left, right, selectionFlag, select);
			}
		
			y--;
				
		}
		x++;

	}

	// rasterize the previous line
			
	// clipping
	left  = (centerX - x >= clippingRectangle.left())  ? centerX - x : clippingRectangle.left();
	right = (centerX + x <  clippingRectangle.right()) ? centerX + x : clippingRectangle.right();
	if (centerY + y < clippingRectangle.bottom()) { 
		numCoveredSurfels += (*spanFunction) (centerY + y, left, right, selectionFlag, select);
	}

	if (centerY - y >= clippingRectangle.top()) {
		numCoveredSurfels += (*spanFunction) (centerY - y, left, right, selectionFlag, select);
	}

	d2 = b * b * (x + 0.5f) * (x + 0.5f) + a * a * (y - 1) * (y -1) - a * a * b * b;
	while (y > 1) {

		if (d2 < 0) {
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x++;
		}
		else {
			d2 += a * a * (-2 * y + 3);
		}
		y--;

		// clipping
		left  = (centerX - x >= clippingRectangle.left())  ? centerX - x : clippingRectangle.left();
		right = (centerX + x <  clippingRectangle.right()) ? centerX + x : clippingRectangle.right();
		if (centerY + y < clippingRectangle.bottom()) { 
			numCoveredSurfels += (*spanFunction) (centerY + y, left, right, selectionFlag, select);
		}
		if (centerY - y >= clippingRectangle.top()) {
			numCoveredSurfels += (*spanFunction) (centerY - y, left, right, selectionFlag, select);
		}

	}

	// handle the last row (y = 0) separately
	if (d2 < 0) {
		d2 += b * b * (2 * x + 2) + a * a * 3;
		x++;
	}
	else {
		d2 += a * a * 3;
	}

	// clipping
	left  = (centerX - x >= clippingRectangle.left())  ? centerX - x : clippingRectangle.left();
	right = (centerX + x <  clippingRectangle.right()) ? centerX + x : clippingRectangle.right();
	if (centerY < clippingRectangle.bottom()) { 
		numCoveredSurfels +=(*spanFunction) (centerY, left, right, selectionFlag, select);
	}

	return numCoveredSurfels;

}


// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
