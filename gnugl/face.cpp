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

WPOINT WFACE::normal(WOBJECT *t)
 {
   WPOINT A,B,C,Nor;

   A= t->vertex[vertex[0]].bod;
   B= t->vertex[vertex[1]].bod;
   C= t->vertex[vertex[2]].bod;
   Nor  = (B-A)^(C-A);  // cross product

   if ( ((Nor.x*Nor.x+Nor.y*Nor.y+Nor.z*Nor.z) < 1e-6) &&(verts==4) )  // degenerovany na Trg
    {
      B= t->vertex[vertex[2]].bod;
      C= t->vertex[vertex[3]].bod;
      Nor = (B-A)^(C-A);
    } 
   return(Nor);
 }

WUCHAR WFACE::compute_shade(WOBJECT *t,WCONTROL *c)  // 0-255
 {
   WPOINT NormV,Midle;
   Midle = ( t->vertex[vertex[0]].bod + t->vertex[vertex[2]].bod )/2.0;
   NormV = normal(t);
   return shadec(NormV,Midle,c);
 }

double WFACE::distance(WOBJECT *t,WCONTROL *c)
   {
     WPOINT b = (t->vertex[vertex[0]].bod + t->vertex[vertex[2]].bod) /2.0;
     b = b - c->obs;
     return(b.x*b.x+b.y*b.y+b.z*b.z);
   }

int WFACE::visible(WOBJECT *t,WCONTROL *c)
   {
       WPOINT Nor,Obs;
       if (c->back_face) return(1);
       Obs = t->vertex[vertex[0]].bod - c->obs;
       Nor = normal(t);
       if ( (Obs*Nor) < 0.0) return(1);
       return(0);
   }

void WFACE::draw(WOBJECT *t,WCONTROL *c,ZBUFFER *zb)
 {
   WPOINT2 a[8];
   double z_coord[8];
   WSHORT intensity[8];
   WPOINT normls[8];
   int i,clr;

   for(i=0;i<verts;i++)
    {
      a[i] = t->vertex[vertex[i]].bd;
      if ((a[i].x<=0)||(a[i].x>=c->Win_W)||(a[i].y<=0)||(a[i].y>=c->Win_H)) return;
// Clipping ?
//    TextXY(a[i].x,a[i].y,itoa(face->vertex[i],sss,10),WHITE);
    }

   switch(c->model)
    {
      case DSF:  WFillPoly(verts,a,0);     // Black
                 WPoly(verts,a,t->textur.color1); 
                 break;
      case DSC:
      case FFC:  shade = compute_shade(t,c);
                 clr = color_number(t->textur.color1,shade);
                 WFillPoly(verts,a,clr);
                 break;
      case FFF:  WPoly(verts,a,t->textur.color1); 
                 break; 
      case ZBF:  shade = compute_shade(t,c);
                 clr = color_number(t->textur.color1,shade);
                 for(i=0;i<verts;i++) z_coord[i]=t->vertex[vertex[i]].bod.z;
                 zb->ZBF_Polygon(verts,a,z_coord,clr,c);
                 break;
      case GRD:  for(i=0;i<verts;i++)
                  {
                    intensity[i]=t->vertex[vertex[i]].intensity;
                      z_coord[i]=t->vertex[vertex[i]].bod.z;
                  }
                 zb->GRD_Polygon(verts,a,z_coord,intensity,&t->textur,c);
                 break;
      case PNG:  for(i=0;i<verts;i++)
                  {
                      normls[i]=t->vertex[vertex[i]].normv;
                     z_coord[i]=t->vertex[vertex[i]].bod.z;
                  }
                 zb->PNG_Polygon(verts,a,z_coord,normls,&t->textur,c);
                 break; 
    }
 }

/*-----------------------------------------------------------------------*/
