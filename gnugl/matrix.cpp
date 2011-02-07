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

/*--------------------------- MATRIX MATH -------------------------------*/

#include <math.h>
#include "gnugl.h"

void WMATRIX::unit()
 {
    int i,j;
    for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        if (i==j) a[i][j] = 1.0;  
             else a[i][j] = 0.0;
 }

void WMATRIX::resize(double k)
 {
    int i,j;
    for(i=0;i<4;i++)
      for(j=0;j<4;j++)
        if (i==j) a[i][j] = k;  
             else a[i][j] = 0.0;

    a[3][3]=1.0; 
 }

void WMATRIX::resize(double kx,double ky,double kz)
 {
    int i,j;
    for(i=0;i<4;i++)
      for(j=0;j<4;j++) 
        a[i][j] = 0.0;  
  
    a[0][0] = kx; a[1][1] = ky; a[2][2] = kz; a[3][3] = 1.0;
 }

void WMATRIX::set_translation(WPOINT T)
 {
    a[0][3] = T.x;
    a[1][3] = T.y;
    a[2][3] = T.z;
 }

WPOINT WMATRIX::get_translation()
 {
   return WPOINT(a[0][3],a[1][3],a[2][3]);
 }

void WMATRIX::rotate(WPOINT R,double alfa)   // axe R goes via point [0,0,0]
  {
      double COS,SIN,x,y,z;
      WPOINT os=R;

      os.normalize();
      x = os.x;
      y = os.y;
      z = os.z;

      COS = cos(WPI*alfa/180.0);
      SIN = sin(WPI*alfa/180.0);

      a[0][0] = (1-COS)*x*x + COS;
      a[0][1] = (1-COS)*x*y - SIN*z;
      a[0][2] = (1-COS)*x*z + SIN*y;
      a[0][3] = 0;

      a[1][0] = (1-COS)*x*y + SIN*z;
      a[1][1] = (1-COS)*y*y + COS;
      a[1][2] = (1-COS)*y*z - SIN*x;
      a[1][3] = 0;

      a[2][0] = (1-COS)*x*z - SIN*y;
      a[2][1] = (1-COS)*y*z + SIN*x;
      a[2][2] = (1-COS)*z*z + COS;
      a[2][3] = 0;

      a[3][0] = 0;
      a[3][1] = 0;
      a[3][2] = 0;
      a[3][3] = 1;
  }

void WMATRIX::rotate(WPOINT axe, double angle, WPOINT zero)
 {
   WMATRIX mr,mt;

   mr.unit();  mr.set_translation(zero*(-1.0));      //   <--
   mt.rotate(axe,angle);                       
   mr=mr*mt;

   mt.unit();  mt.set_translation(zero);             //   --> 
   mr=mr*mt;
   *this=mr;
 }

void WMATRIX::orient(WPOINT a,WPOINT b)      // ab -> paralel with z-axis
 {
   WPOINT r,v=b-a,stred=(a+b)/2.0;
   double alfa,vs;

   r.x = -v.y;    r.y = v.x;    r.z = 0.0;
   vs = v.size();
   if (vs==0.0) alfa=0.0; else  alfa=acos(v.z/vs);   
   rotate(r,180.0*alfa/WPI);
   set_translation(stred);
 }

WMATRIX WMATRIX::operator*(WMATRIX B)
  {
    int i,j;
    WMATRIX D;

    for(i=0;i<4;i++)
      for(j=0;j<4;j++)

          D.a[i][j] = B.a[i][0]*a[0][j] + B.a[i][1]*a[1][j] +
                      B.a[i][2]*a[2][j] + B.a[i][3]*a[3][j];
    return D;
  } 

WPOINT WMATRIX::operator*(WPOINT P)
 {
    WPOINT B;
    B.x = P.x*a[0][0] + P.y*a[0][1] + P.z*a[0][2] + a[0][3];
    B.y = P.x*a[1][0] + P.y*a[1][1] + P.z*a[1][2] + a[1][3];
    B.z = P.x*a[2][0] + P.y*a[2][1] + P.z*a[2][2] + a[2][3];
    return B;
 }

/*------------------------------------------------------------------------*/
