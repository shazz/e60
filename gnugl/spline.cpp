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

#include <stdlib.h>
#include "gnugl.h"

static double rn() { return(((double)(rand()&0xffff))/0xffff); }

void WSPLINE::single_close()      // water drop
  {
     WPOINT  n(0,0,0);        
     WPOINT v1(4,0,2), v2(0,4,2), v3(-4,0,2), v4(0,-4,2);

     a[0][0]= n; a[0][1]= n; a[0][2]= n; a[0][3]= n;
     a[1][0]= n; a[1][1]=v1; a[1][2]=v2; a[1][3]= n;
     a[2][0]= n; a[2][1]=v4; a[2][2]=v3; a[2][3]= n;
     a[3][0]= n; a[3][1]= n; a[3][2]= n; a[3][3]= n;
  }

void WSPLINE::single_open()
  {
     int i,j;

     for(i=0;i<4;i++)
      for(j=0;j<4;j++)
       {
         a[i][j].x =  2.0 * (double)i/3 - 1.0;
         a[i][j].y =  2.0 * (double)j/3 - 1.0;
         a[i][j].z =  2.0 * rn() - 1.0;
       }
  }

void WSPLINE::copy(WSPLINE s)
  {
     int i,j;
     for(i=0;i<4;i++)
       for(j=0;j<4;j++)
         a[i][j]=s.a[i][j];
  }

void WSPLINE::single_cube()
  {
     WPOINT p[8],u[4];

     p[0]= WPOINT(1,0,1);      p[4]= WPOINT(1,0,-1);
     p[1]= WPOINT(0,1,1);      p[5]= WPOINT(0,1,-1);
     p[2]= WPOINT(-1,0,1);     p[6]= WPOINT(-1,0,-1);
     p[3]= WPOINT(0,-1,1);     p[7]= WPOINT(0,-1,-1);

     u[0] = (p[0]+p[4])/2.0;
     u[1] = (p[1]+p[5])/2.0;
     u[2] = (p[2]+p[6])/2.0;
     u[3] = (p[3]+p[7])/2.0;

     a[0][0]=u[2]; a[0][1]=u[2]; a[0][2]=u[1]; a[0][3]=u[1];
     a[1][0]=u[2]; a[1][1]=p[6]; a[1][2]=p[5]; a[1][3]=u[1];
     a[2][0]=u[3]; a[2][1]=p[7]; a[2][2]=p[4]; a[2][3]=u[0];
     a[3][0]=u[3]; a[3][1]=u[3]; a[3][2]=u[0]; a[3][3]=u[0];
  }

WPOINT WSPLINE::value(double u,double v)    // de Castilju algorithm
 {
   WPOINT P[4][4];
   int k,i,j;

   for(j=0;j<4;j++)
    for(i=0;i<4;i++) 
      P[i][j]=a[i][j];

   for(k=0;k<3;k++)         // reduce net 4x4 -> 3x3 -> 2x2 -> 1x1
    for(j=0;j<3-k;j++)
     for(i=0;i<3-k;i++)  
       P[i][j]  = ( P[i  ][j  ] * ((1-u)*(1-v)) ) +
                  ( P[i  ][j+1] * ((1-u)*   v ) ) +
                  ( P[i+1][j  ] * (   u *(1-v)) ) +
                  ( P[i+1][j+1] * (   u *   v ) );

   return P[0][0];
 }

/*-----------------------------------------------------------------------*/
