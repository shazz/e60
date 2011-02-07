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

#include "gnugl.h"

/**********************
 *    Draw a Line
 **********************/

static void WHLine(int x1,int x2,int y,int c)          // Horizontal Line
 {
   if (x1<x2) for(int x=x1; x<=x2; x++) WPlot(x,y,c);
         else for(int x=x2; x<=x1; x++) WPlot(x,y,c);
 }

static int wabs(int x) { if (x<0) return(-x); else return(x); }

static void WLine(int x1,int y1,int x2,int y2,int c)     // for 32-bit integers
 {
     int x,y,k,dir;
     WPlot(x1,y1,c);      

     if (wabs(x2-x1)>wabs(y2-y1))
       {
//       if (x1==x2) return;
         if (x1>x2) { dir= -1; k= ((y2-y1)<<16)/(x1-x2); } 
               else { dir=  1; k= ((y2-y1)<<16)/(x2-x1); } 
         x=x1; y=y1<<16;
         while(x!=x2) { x+=dir; y+=k; WPlot(x,(y+0x8000)>>16,c); }
       }
     else
       {    
         if (y1==y2) return;
         if (y1>y2) { dir= -1; k= ((x2-x1)<<16)/(y1-y2); }
               else { dir=  1; k= ((x2-x1)<<16)/(y2-y1); }
         y=y1; x=x1<<16;
         while(y!=y2) { y+=dir; x+=k; WPlot((x+0x8000)>>16,y,c); }
      }
 }

/************************
 *     Fill Polygon
 ************************/

static void Line_Y_buf(int x1,int y1,int x2,int y2,int *xbuf)
  {
     int x,y,kx;
     xbuf[y1]=x1; xbuf[y2]=x2;
     if (y1==y2) return;
     if (y1>y2) { y=y1; y1=y2; y2=y;  x=x1; x1=x2; x2=x; }
     kx=((x2-x1)<<16)/(y2-y1); y=y1; x=x1<<16;
     while(y<y2) { y++; x+=kx; xbuf[y]=(x+0x8000)>>16; }
  }

static int  left[YMaxRES];
static int right[YMaxRES];

void WFACE::WFillPoly(int N,WPOINT2 *b,int c)
 {
   int i,j,y;
   int ymin=b[0].y, ymax=b[0].y;

   for(i=0;i<N;i++)
     {
       if (b[i].y>ymax) ymax=b[i].y;
       if (b[i].y<ymin) ymin=b[i].y;
       
       j=(i+1)%N;
       if (b[i].y==b[j].y) continue;
       if (b[i].y>b[j].y) Line_Y_buf(b[i].x, b[i].y, b[j].x, b[j].y, right);
                     else Line_Y_buf(b[i].x, b[i].y, b[j].x, b[j].y, left);
     }
   if (ymin==ymax) return;   // WHLine( xmin, xmax, ymin, c);
   for(y=ymin; y<=ymax; y++)  WHLine( left[y], right[y], y, c);
 }

/************************
 *    Draw a Polygon
 ************************/

void WFACE::WPoly(int N,WPOINT2 *bod,int c)
 {
   for(int i=0;i<N-1;i++)
     WLine(bod[i  ].x, bod[i  ].y, bod[i+1].x, bod[i+1].y, c);
     WLine(bod[N-1].x, bod[N-1].y, bod[0  ].x, bod[0  ].y, c);
 }

/*--------------------------------------------------------------------------*/
