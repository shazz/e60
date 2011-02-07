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

//*********************************************************************
//               RAYTRACING --> Under construction...
//*********************************************************************
                                     
int Polygon_ray(WPOINT poly_point[],WPOINT normal_vector,WPOINT ray_start,WPOINT ray_direction,WPOINT *intersection)
  {
     double menovatel,citatel,t;

// 1. test in-out voci vrcholom polygonu

// citatel = vzdialenost BODU start od ROVINY polygonu
// menovatel = cos(normal,direct);
// t = vzdialenost BODU start od ROVINY polygonu v smere LUCA ray_direction

     menovatel = normal_vector * ray_direction;
     if (menovatel==0.0) return 0;    // ray parallel with polygon
     citatel = (normal_vector * poly_point[0]) - (normal_vector * ray_start);
     t = citatel / menovatel;
     *intersection = ray_start + (ray_direction*t);
     return 1;
  }

/**************************** - select object - **************************/

static void hull(int N,int b[][2],int *x1,int *x2,int *y1,int *y2)
 {
    int i;
    *x1=0;*x2=0;*y1=0;*y2=0;
    for(i=1;i<N;i++)
     {
       if (b[i][0]<b[*x1][0]) *x1=i;
       if (b[i][0]>b[*x2][0]) *x2=i;
       if (b[i][1]<b[*y1][1]) *y1=i;
       if (b[i][1]>b[*y2][1]) *y2=i;
     }
 }

static int select_face(WOBJECT *t,int x, int y)
 {
   WFACE *face;
   int a[8][2];
   int i,j,ix1,ix2,iy1,iy2;

   for(j=0;j<t->faces;j++)
    {
     face= &(t->face[j]);
     for(i=0;i<face->verts;i++)
        { a[i][0]=t->vertex[face->vertex[i]].bd.x;
          a[i][1]=t->vertex[face->vertex[i]].bd.y; }
     hull(face->verts, a, &ix1,&ix2,&iy1,&iy2);
     if ((x>=a[ix1][0])&&(x<=a[ix2][0])&&(y>=a[iy1][1])&&(y<=a[iy2][1])) return j;
    }
   return -1;
 }

void Scena::select_object(int x, int y)
 {
   int i,m,l;

   for(i=0;i<pt;i++)
    if ( select_face(&object[i],x,y)!= -1)
//    if (!(object[i].selected))
       {
         m=c.model;
         l=object[i].textur.color1;
         c.model=FFF;

         if (!(object[i].selected)) object[i].textur.color1=0xff5555;
         object[i].selected^=1;

         object[i].draw(&c,&zb);
         c.model=m;
         object[i].textur.color1=l;
         return;
       }
 }

/**************************************************************************/
