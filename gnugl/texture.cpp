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
#include <math.h>
#include "gnugl.h"

/**************************************************************************/

static int s[17][17];

static void init_s()     // random matrix
 {
   static int init=0;
   int i,j;
   if (init) return;
   for(i=0;i<16;i++)
     for(j=0;j<16;j++) s[i][j]=rand()&0xff;

   for(i=0;i<16;i++) s[i][16]=s[i][0];
   for(j=0;j<16;j++) s[16][j]=s[0][j];   
   s[16][16]=s[0][0];
   init=1;
 }

static int sx(unsigned int x,unsigned int y)     // random matrix linearly interpolated
 {
   int xx=x%256,yy=y%256;                        // linear Perlin's function
   int xa=xx/16,xb=xa+1;                         // perodicaly extrapolated 
   int ya=yy/16,yb=ya+1;
   int r = (16*xb-xx)*(16*yb-yy)*s[xa][ya] +
           (16*xb-xx)*(yy-16*ya)*s[xa][yb] +
           (xx-16*xa)*(16*yb-yy)*s[xb][ya] +
           (xx-16*xa)*(yy-16*ya)*s[xb][yb];
   return r/256;
 }

static int mraky(unsigned int x,unsigned int y)    // recursively built random fractal
 {
   int xx=x%256,yy=y%256;
   int i,s=0,k=1;
   for(i=0;i<8;i++) { s+= sx(k*xx,k*yy)/k;  k*=2; }
   return s/2;          
 }

/*-------------------------------------------------------------------------*/

static int blend_color(int clr1,int clr2,WUCHAR balance)  
 {                                              
   int r2 = (clr2>>16) &0xff;
   int g2 = (clr2>>8)  &0xff;
   int b2 =  clr2      &0xff;

   int r1 = (clr1>>16) &0xff;
   int g1 = (clr1>>8)  &0xff;
   int b1 =  clr1      &0xff;

   r1 = ( r2*(255-balance) + r1*balance ) / 255;
   g1 = ( g2*(255-balance) + g1*balance ) / 255;
   b1 = ( b2*(255-balance) + b1*balance ) / 255;

   return( (r1<<16) + (g1<<8) + b1 );
 }

int WTEXTUR::ComputeColor(WPOINT B)
 {
    double k;    
    switch(typ)
     {
       case PLANAR:
         k = (B.x + B.y + B.z)/50.0;
         k-= floor(k);                          
         k = 128 + 127.99*cos(2.0*WPI*k);          
         break;
       case SPHER:
         k = B.size()/30.0;
         k-= (int)k;       
         if (k>0.5) k=255; else k=0;
         break;
       case CUBIC:
         k = (B.x*B.x + B.y*B.y + B.z*B.z)/10.0;
         k-= (int)k;       
         if (k>0.5) k=255; else k=0;
         break;
       case BUBLE:
         B/=40.0;
         B.x = -1.0 + 2.0*( B.x - floor(B.x));            //  -1.0 - 1.0
         B.y = -1.0 + 2.0*( B.y - floor(B.y));            //  -1.0 - 1.0
         k = B.x*B.x + B.y*B.y;                           //   0.0 - 3.0
         k = 128 + 127.99*cos(2.0*WPI*k/3.0);
         break;
       case RUBIC:
         B/=80.0;
         B.x = -1.0 + 2.0*( B.x - floor(B.x));          //  -1.0 - 1.0
         B.y = -1.0 + 2.0*( B.y - floor(B.y));          //  -1.0 - 1.0
         if ((B.x*B.y) > 0.0) k=255; else k=0;
         break;
       case CLOUDS:
         init_s();
         k = mraky((unsigned int)(B.x),(unsigned int)(B.y));
         break;
       case MARBLE:
         init_s();
         k = B.x/25.0 + (double)mraky((unsigned int)(B.x),(unsigned int)(B.y))/50.0;
         k = 1.0 + cos(k);
         k = 128 + 127.99*cos(WPI*k);
         break;
       case WOOD:
         init_s();
         k = B.x/2.0 + ((double)sx((unsigned int)(2.0*B.x),(unsigned int)(2.0*B.y)))/200.0;
         k = 128 + 127.99*cos(k);
         break;
       default:
         k = 255;
     }

    return blend_color(color1,color2,(WUCHAR)k);
 }

/*--------------------------------------------------------------------------------*/
