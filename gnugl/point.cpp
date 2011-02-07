/***************************************************************************
 *             Copyright (c) 1994-2000 by Richard Gosiorovsky              *
 *                            gosiorov@yahoo.com                           *
 *                     http://www.geocities.com/gosiorov                   *
 * ----------------------------------------------------------------------- *
 *                         *** G n u G L ***                               *
 *   Description:                                                          *
 *     Universal 3D library, portable to all C++ platforms,                *
 *     hardware & operating system independent                             *
 *                                                                         *
 *   Last Modified: April 25th Y2K by the author                           *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   Library General Public License for more details. (copying.lib)        *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; if not, write to the Free            *
 *   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.    *
 *                                                                         *
 ***************************************************************************/

#include <math.h>
#include "gnugl.h"

   void WPOINT::zerov() { x=0.0; y=0.0; z=0.0; }
 double WPOINT::size() { return sqrt(x*x+y*y+z*z); }
 double WPOINT::dist(WPOINT a) { return (a-*this).size(); }
   void WPOINT::normalize() { double sz = this->size(); if (sz!=0.0) (*this)=(*this)/sz; }

 WPOINT WPOINT::operator+ (WPOINT a) { return WPOINT(x+a.x,y+a.y,z+a.z); }
   void WPOINT::operator+=(WPOINT a) { x+=a.x; y+=a.y; z+=a.z; }
 WPOINT WPOINT::operator- (WPOINT a) { return WPOINT(x-a.x,y-a.y,z-a.z); }
   void WPOINT::operator-=(WPOINT a) { x-=a.x; y-=a.y; z-=a.z; }
 WPOINT WPOINT::operator* (double k) { return WPOINT(k*x,k*y,k*z); }
   void WPOINT::operator*=(double k) { x*=k; y*=k; z*=k; }
 WPOINT WPOINT::operator/ (double k) { return WPOINT(x/k,y/k,z/k); }
   void WPOINT::operator/=(double k) { x/=k; y/=k; z/=k; }

 double WPOINT::operator* (WPOINT a) { return( x*a.x+y*a.y+z*a.z ); }
 WPOINT WPOINT::operator^ (WPOINT a) { return WPOINT(y*a.z-z*a.y,z*a.x-x*a.z,x*a.y-y*a.x); }

WPOINT2 WPOINT::proj(WCONTROL *c)
 {
   WPOINT2 b;
   if (c->obs.z>=z) { b.x=c->Win_W; b.y=c->Win_H; }  // don't display
   else
    { 
      b.x= (int) ( c->zoom * (c->obs.x-x)/(c->obs.z-z) + c->Win_W/2.0);
      b.y= (int) (-c->zoom * (c->obs.y-y)/(c->obs.z-z) + c->Win_H/2.0);
    }
   return(b);
 }
/*------------------------------------------------------------------------*/
