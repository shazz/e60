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
#include <stdlib.h>
#include "gnugl.h"

void WOBJECT::project(WCONTROL *c)
 {
   for(int i=0;i<verts;i++)  vertex[i].bd= vertex[i].bod.proj(c);
 }

void WOBJECT::transform(WMATRIX m)   
 {
    for(int i=0;i<verts;i++)  vertex[i].bod = m * vertex[i].bod;
    position = position * m;
 }

void WOBJECT::rotate(WPOINT os,double alfa)   
 {
    WMATRIX m; m.rotate(os,alfa);
    for(int i=0;i<verts;i++)  vertex[i].bod = m * vertex[i].bod;
    position = position * m;
 }

void WOBJECT::rotate(WPOINT os,double alfa,WPOINT center)   
 {
    WMATRIX m; m.rotate(os,alfa,center);
    for(int i=0;i<verts;i++)  vertex[i].bod = m * vertex[i].bod;
    position = position * m;
 }

void WOBJECT::move(WPOINT T)   
 {
     for(int i=0;i<verts;i++) vertex[i].bod += T;
     WPOINT L=position.get_translation();
     position.set_translation(L+T);
 }

void WOBJECT::resize(double zv)
 {
    WMATRIX rm; 
    rm.resize(zv);
    position = position * rm;
    for(int i=0;i<verts;i++) vertex[i].bod *= zv;
 }

void WOBJECT::draw(WCONTROL *c,ZBUFFER *zb)
 {
   if ((c->model==GRD)||(c->model==PNG)) init_PNG(c);
   for(int i=0;i<faces;i++)
     if (face[i].visible(this,c))
       face[i].draw(this,c,zb);
 }

int WOBJECT::alloc(int vrts,int fcs)
  {
    int k=0;
    vertex = new VERTEX[vrts];
    if (vertex)
     { face = new WFACE[fcs];
       if (face) {} else { delete vertex; k=1; } }
    else k=1;  
    return k;
  }

void WOBJECT::freeo()
  {
     delete face; faces=0; 
     delete vertex; verts=0;
  }

void WOBJECT::init_PNG(WCONTROL *c)
 {
   int i,j;

   for(i=0;i<verts;i++)
    { vertex[i].intensity=0;
      vertex[i].fcount=0;
      vertex[i].normv.zerov(); }

   for(i=0;i<faces;i++)
    if (face[i].visible(this,c))
     {
       WPOINT NormV = face[i].normal(this);      
       NormV.normalize();
       for(j=0;j<face[i].verts;j++)
        {
          vertex[face[i].vertex[j]].normv += NormV;
          vertex[face[i].vertex[j]].fcount++;
        }
     }

   if (c->model==GRD)
    for(i=0;i<verts;i++)  
     if (vertex[i].fcount)
      vertex[i].intensity = shadec(vertex[i].normv,vertex[i].bod,c); 

   if (c->model==PNG)
    for(i=0;i<verts;i++)  
     if (vertex[i].fcount)
      vertex[i].normv /= (double)vertex[i].fcount;
 }

 void WOBJECT::set_texture(WTEXTUR *t)   
 {
	textur = *t;
 }

int WOBJECT::create(int typi,WMATRIX positioni,WTEXTUR *texturi)
 {
    int ret=1;

      typ = typi;
   textur = *texturi;
 selected = 0;
    position.unit(); 

    switch(typ)
     {
        case SPHERE      : ret=sphere(); break;
        case CUBE        : ret=cube(); break;
        case TETRAHEDRON : ret=tetrahedron(); break;
        case TORUS       : ret=torus(); break;
        case CYLINDER    : ret=cylinder(); break;
        case CONUS       : ret=conus(); break;
        case BICONUS     : ret=biconus(); break;

        case SALAM       :
        case BAMBOO      :
        case DOUBLE      :
        case SPLAS       :
        case GOMEA       :
        case STAR5       : 
        case STAR6       :
        case QUARK       :
        case FONTAN      : ret=boojoom(); break;
        case RINGO       : ret=ringo(); break;
        case WURM        : ret=wurm(); break;

        case GRID1       :
        case GRID2       :
        case GRID3       :
        case GRID4       :
        case GRID5       :
        case GRID6       : ret=grid(); break;

        case DROP        :
        case RPLANE      :
        case PILLOW      : ret=bezier(); break;

        default          : ret=1; 
     }

    if (ret) { verts=0; faces=0; return 1; }     // cannot alloc
    transform(positioni);
    return 0;
 }
/*----------------------------------------------------------------------*/
