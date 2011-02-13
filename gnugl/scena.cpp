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

void Scena::set_observer(WPOINT p) { c.obs=p; }
WPOINT Scena::get_observer() { return c.obs; }
void Scena::set_look(double fi,double theta) { c.look_fi=fi; c.look_theta=theta; }
void Scena::set_zoom(double d) { c.zoom*=d; }
void Scena::set_model(int m) { c.model=m; }
void Scena::set_window(int w,int h) { c.Win_W=w; c.Win_H=h; }
void Scena::set_back_faces(int m) { c.back_face=m; }
void Scena::set_light_position(WPOINT p) { c.src=p; }
void Scena::set_light(double a,double s,double e,double d)
  {
    c.light_ambient=a; 
    c.light_specular=s;
    c.light_specular_exponent=e;
    c.light_diffuse=d;
  }

WPOINT2 Scena::proj(WPOINT a) { return a.proj(&c); }
WPOINT  Scena::unproj(WPOINT2 p,double z) { return un_proj(p.x,p.y,z,&c); }

WUCHAR shadec(WPOINT NormV, WPOINT Midle,WCONTROL *c)   
 {
   WPOINT Odraz,Obs,pmc;
   double cosinus, difuz, total;

   NormV.normalize();

   if (c->light_ambient + c->light_specular + c->light_diffuse == 0.0) 
    { 
      if (NormV.z>0.0) return((WUCHAR)( 128.0 *NormV.z)); 
                  else return((WUCHAR)(-255.99*NormV.z));
    }

   Odraz = c->src-Midle; Odraz.normalize();
   Obs   = c->obs-Midle;   Obs.normalize();

   difuz = Odraz*NormV;
   pmc   = NormV*difuz;
   pmc   = pmc*2.0;           // uhol dopadu == uhol odrazu 
   Odraz = pmc-Odraz;         // O = 2(O.N)N-O  (symetria podla N)

   cosinus = Odraz*Obs;

   if (cosinus< 0.0) cosinus=0.0;
   if (difuz  < 0.0) difuz=-difuz/4.0;   

   total=   
   c->light_ambient  +
   c->light_specular * pow(cosinus,c->light_specular_exponent) +
   c->light_diffuse  * difuz;
   
   if (total>255.0) total=255.0;

   return((WUCHAR)(total));
 }

int color_number(int clr, WUCHAR shade)  
 {                                              
   int r=(clr>>16)&0xff;
   int g=(clr>>8) &0xff;
   int b= clr     &0xff;

   r = ( r*shade ) / 255;
   g = ( g*shade ) / 255;
   b = ( b*shade ) / 255;

   return( (r<<16) + (g<<8) + b );
 }

/**************************************************************************/

void Scena::remove_object(int k)
  {
    if ((pt==0)||(k<0)||(k>=pt)) return; 
    object[k].freeo(); pt--;
    for(int i=k;i<pt;i++) object[i]=object[i+1];
  }

int Scena::add_object(int typ,WMATRIX pos,WTEXTUR *txt)
  {
    if (pt>=MAX_OBJECTS) return 0;
    if (object[pt].create(typ,pos,txt)) return 0;   // no success
    pt++; return 1;             // o.k.
  }

void Scena::rotate(WPOINT os,double alfa)
  {
     WMATRIX R;
     R.rotate(os,alfa);
     for(int i=0;i<pt;i++) object[i].transform(R);
  }

void Scena::rotate(WPOINT os,double alfa,WPOINT center)
  {
     WMATRIX R;
     R.rotate(os,alfa,center);
     for(int i=0;i<pt;i++) object[i].transform(R);
  }

void Scena::move(WPOINT T)       { for(int i=0;i<pt;i++) object[i].move(T); }
void Scena::transform(WMATRIX M) { for(int i=0;i<pt;i++) object[i].transform(M); }
void Scena::resize(double zv)    { for(int i=0;i<pt;i++) object[i].resize(zv); }
void Scena::project()            { for(int i=0;i<pt;i++) object[i].project(&c); }
void Scena::draw()               { for(int i=0;i<pt;i++) object[i].draw(&c,&zb); }

void Scena::fit()
  {
     double xmin=1e99,xmax=-1e99,ymin=1e99,ymax=-1e99,zmin=1e99,zmax=-1e99;

     for(int i=0;i<pt;i++)
      {
        WOBJECT *t=&object[i];
        for(int j=0;j<t->verts;j++)
         {
           if (t->vertex[j].bod.x < xmin)  xmin = t->vertex[j].bod.x;
           if (t->vertex[j].bod.y < ymin)  ymin = t->vertex[j].bod.y;
           if (t->vertex[j].bod.z < zmin)  zmin = t->vertex[j].bod.z;
           if (t->vertex[j].bod.x > xmax)  xmax = t->vertex[j].bod.x;
           if (t->vertex[j].bod.y > ymax)  ymax = t->vertex[j].bod.y;
           if (t->vertex[j].bod.z > zmax)  zmax = t->vertex[j].bod.z;
         }
      }

     c.obs.x = (xmax+xmin)/2.0;
     c.obs.y = (ymax+ymin)/2.0;

     if ((xmax-xmin)>(ymax-ymin))
      {  c.zoom = 1.25*c.Win_W; c.obs.z = zmin - 1.5*(xmax-xmin); }
     else
      {  c.zoom = 1.25*c.Win_H; c.obs.z = zmin - 1.5*(ymax-ymin); }
  }

static int comp(const void *a,const void *b)
 {
   if ( *(double *)a < *(double *)b ) return (1);
   return(-1);
 }

typedef struct { double dist; WSHORT six; WSHORT tix; } sort_str;

void Scena::draw_sort()
 {
    int i,j, S=0, N=0;
    sort_str *v;

    for(j=0;j<pt;j++) S+=object[j].faces;
        
    v = new sort_str[S];
    if (v==NULL) return;   // Non enough memoaire pur d'Sortage
     
    for(j=0;j<pt;j++)
     for(i=0;i<object[j].faces;i++)
       if (object[j].face[i].visible(&object[j],&c))
         {
           v[N].dist = object[j].face[i].distance(&object[j],&c);
           v[N].six = i;
           v[N].tix = j;
           N++;
         }

    qsort(v,N,sizeof(sort_str),comp);

    for(i=0;i<N;i++)
      object[v[i].tix].face[v[i].six].draw(&object[v[i].tix],&c,&zb);
    delete v;
 }

void Scena::clear(char *fn) { if(!read_pcx(fn,0,0,c.Win_W,c.Win_H)) clear(0); }
void Scena::clear(int bg_color)
 {
   for(int y=0;y<c.Win_H;y++)
     for(int x=0;x<c.Win_W;x++)
       WPlot(x,y,bg_color);
 }

void Scena::go()
 {
   WMATRIX mt,mf;

   if (pt==0) return; 

   if ((c.model==ZBF)||(c.model==GRD)||(c.model==PNG))
    {
      if (zb.open(&c)) return;    // NO MEMORY FOR ZBUFFER 
      zb.clear(&c);
    }

   if ((c.look_fi!=0.0)||(c.look_theta!=0.0))
    {
      mt.rotate(WPOINT(1,0,0),c.look_theta,c.obs); 
      mf.rotate(WPOINT(0,1,0),c.look_fi,c.obs); 
      mt = mf*mt; transform(mt); c.src = mt * c.src;
    }

   project();  

   if ((c.model==DSF)||(c.model==DSC)) draw_sort(); else draw(); 
// sort also ZBF,GRD,PNG front to back, sort alloc 1st time only

   if ((c.look_fi!=0.0)||(c.look_theta!=0.0))
    {
      mt.rotate(WPOINT(1,0,0),-c.look_theta,c.obs); 
      mf.rotate(WPOINT(0,1,0),-c.look_fi,c.obs); 
      mt = mt*mf; transform(mt); c.src = mt * c.src;
    }
 }

/******************************** END **********************************/
