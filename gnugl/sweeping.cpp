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

int revolution(WOBJECT *t,WPOINT os1,WPOINT os2,int n,WPOINT *p)  // rotate profile *p
 {
   int i,j;
   WPOINT os;
   WMATRIX m;
   int steps=24;

   if ((n<2)||(n>1024)) return 1;

   if (os1.dist(p[0])<=os2.dist(p[0]))  os=os1-os2; else os=os2-os1; 
   m.rotate(os,360.0/steps,os1);  

   t->typ      = 0;
   t->textur   = WTEXTUR(OFF,0xffffff,0);
   t->selected = 0;
   t->position.unit();

   if (t->alloc(n*steps,(n-1)*steps)) return 1;

   t->verts=n*steps;
   t->faces=(n-1)*steps;

   for(i=0;i<steps;i++)
    for(j=0;j<n;j++)
     {
       if (i) p[j]=m*p[j];
       t->vertex[i*n+j].bod = p[j];
     }
  
   for(i=0;i<steps;i++)
    for(j=0;j<(n-1);j++)
     {
       if (i!=(steps-1))
          t->face[i*(n-1)+j] = WFACE(i*n+j, i*n+j+1, i*n+j+n+1, i*n+j+n);
       else
          t->face[i*(n-1)+j] = WFACE(i*n+j, i*n+j+1, j+1, j);
     }
   return 0;
 }

int piping(WOBJECT *t,int n,WPOINT p[],double r)
 {
   int i,j;
   WPOINT A,B,S,T;
   WPOINT pp[256],os[256];
   WMATRIX m;
   double cosinus;
   int steps=24;

   if ((n<2)||(n>256)) return 1;

   for(i=1;i<n-1;i++)
    {
      A = p[i]-p[i-1]; A.normalize();
      B = p[i]-p[i+1]; B.normalize();
      os[i] = A-B;
      T = A+B;
      T.normalize();
      cosinus = A*B;
      S = A^B;
      if (S.size()==0.0) {}
      else 
       {
         if (S.z>0.0) T = T*( r/sqrt((1.0-cosinus)/2.0));
                 else T = T*(-r/sqrt((1.0-cosinus)/2.0));
       }
      pp[i] = p[i]+T; 
    } 
   os[0] = p[1]-p[0];
   os[n-1] = p[n-1]-p[n-2];
   pp[0]=pp[1]-p[1]; pp[0]=pp[0]+p[0];
   pp[n-1]=pp[n-2]-p[n-2]; pp[n-1]=pp[n-1]+p[n-1];
   
   t->typ      = 0;
   t->textur   = WTEXTUR(OFF,0xffffff,0);
   t->selected = 0;
   t->position.unit();

   if (t->alloc(n*steps,(n-1)*steps)) return 1;

   t->verts=n*steps;
   t->faces=(n-1)*steps;

   for(j=0;j<n;j++)
    {
      m.rotate(os[j],360.0/steps,p[j]);  

      for(i=0;i<steps;i++)
       {
         if (i) pp[j]=m*pp[j];
         t->vertex[i*n+j].bod = pp[j];
       }
    }
  
   for(i=0;i<steps;i++)
    for(j=0;j<(n-1);j++)
     {
       if (i!=(steps-1))
          t->face[i*(n-1)+j] = WFACE( i*n+j, i*n+j+n, i*n+j+n+1, i*n+j+1);
       else
          t->face[i*(n-1)+j] = WFACE( i*n+j, j, j+1, i*n+j+1);
     }
   return 0;
 }

/*------------------------------------------------------------------------*/
