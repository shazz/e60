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
                                      
#define ZZ(x,y)  Z_buffer[ y * c->Win_W  + x ]

int ZBUFFER::open(WCONTROL *c)
  {
     if (opened) return 0;  
     Z_buffer = new double[c->Win_W * c->Win_H];
     if (Z_buffer==NULL) return 1;
     opened=1;
     return 0;
  }

void ZBUFFER::close()  { if (opened) { delete Z_buffer; opened=0; }  }

void ZBUFFER::clear(WCONTROL *c)
  {
    int x,y;
    for(y=0;y<c->Win_H;y++)
      for(x=0;x<c->Win_W;x++)
        ZZ(x,y)=1.0e99;
  }

WPOINT un_proj(int x,int y,double z,WCONTROL *c)
 {
   WPOINT B;

   double xb = (double)(x - c->Win_W/2 );
   double yb = (double)(y - c->Win_H/2 );

   B.x =  xb * (z - c->obs.z) / c->zoom + c->obs.x;
   B.y = -yb * (z - c->obs.z) / c->zoom + c->obs.y;
   B.z = z;

   return B;
 }

/********************************* Z-buffer *******************************/

void ZBUFFER::Line_X_inc_ZBF(int x1,double z1,int x2,double z2,int y,int clr,WCONTROL *c)
  {
     int x,dir;
     double k,z;

//   if (z1 < ZZ(x1,y))  { ZZ(x1,y)=z1; WPlot(x1,y,clr); }

     if (x1==x2) return;

     if (x1>x2) { dir= -1; k= (z2-z1)/(x1-x2); }
           else { dir=  1; k= (z2-z1)/(x2-x1); }

     x=x1; z=z1;

     while(x!=x2)
       {
          x+=dir; z=z+k;
          if (z < ZZ(x,y))  { ZZ(x,y)=z; WPlot(x,y,clr); }
       } 
   } 

static void Line_Y_inc(int x1,int y1,double z1,int x2,int y2,double z2,XZ_BUF *xz_buf)
  {
     int x,y,kx;
     double z,kz;
     
     xz_buf[y1].x=x1; xz_buf[y1].z=z1;  
     xz_buf[y2].x=x2; xz_buf[y2].z=z2;  

     if (y1==y2) return;

     if (y1>y2) { y=y1; y1=y2; y2=y;
                  x=x1; x1=x2; x2=x;
                  z=z1; z1=z2; z2=z; }
	  
     kx= ((x2-x1)<<16)/(y2-y1); 
     kz=(z2-z1)/(y2-y1); 

     y=y1; z=z1; x=x1<<16;

     while(y<y2)
      {
        y++; x+=kx; z=z+kz;
        xz_buf[y].x=(x+0x8000)>>16;      // x>>16 for 32-bit integer
        xz_buf[y].z=z;
      }
  }

void ZBUFFER::ZBF_Polygon(int N,WPOINT2 *b,double *Zet,int clr,WCONTROL *c)
  {
    int i,j,y;
    int ymin=b[0].y, ymax=b[0].y;

    for(i=0;i<N;i++)
     {
       if (b[i].y>ymax) ymax=b[i].y;
       if (b[i].y<ymin) ymin=b[i].y;
      
       j=(i+1)%N;
       if (b[i].y==b[j].y) continue;
       if (b[i].y>b[j].y) 
             Line_Y_inc(b[i].x, b[i].y, Zet[i], b[j].x, b[j].y, Zet[j], right);
       else  Line_Y_inc(b[i].x, b[i].y, Zet[i], b[j].x, b[j].y, Zet[j], left);
     }
    if (ymin==ymax) return;
    for(y=ymin; y<=ymax; y++)
      Line_X_inc_ZBF( left[y].x, left[y].z, right[y].x, right[y].z, y, clr,c);
  }

/********************************* Gouroud *********************************/

void ZBUFFER::Line_X_inc_GRD(int x1, double z1, int i1,
                             int x2, double z2, int i2, 
                             int y, WTEXTUR *txtr,WCONTROL *c)
  {
     int x,kx,i,ki;
     double kz,z;
     int farba;
     WPOINT midle;

     if (x1==x2) return;

     if (x1>x2) { kx= -1; kz= (z2-z1)/(x1-x2); ki=((i2-i1)<<16)/(x1-x2); }
           else { kx=  1; kz= (z2-z1)/(x2-x1); ki=((i2-i1)<<16)/(x2-x1); }

     x=x1; z=z1; i=i1<<16;

     while(x!=x2)
       {
          x+=kx; z=z+kz; i+=ki;
          if (z < ZZ(x,y)) 
            {
               ZZ(x,y)=z;
               if (txtr->typ)
                {
                  midle = un_proj(x,y,z,c);
                  farba = txtr->ComputeColor(midle);
                  farba = color_number(farba,(i+0x8000)>>16);
                }
               else farba=color_number(txtr->color1,(i+0x8000)>>16);
               WPlot(x,y,farba);
            }
       } 
   } 

static void Line_Y_inc_GRD(int x1, int y1, double z1, int i1,
                           int x2, int y2, double z2, int i2, XZ_BUF *xzi_buf)
  {
     int x,y,kx,ki,i;
     double z,kz;
     
     xzi_buf[y1].x=x1; xzi_buf[y1].z=z1; xzi_buf[y1].i=i1; 
     xzi_buf[y2].x=x2; xzi_buf[y2].z=z2; xzi_buf[y2].i=i2; 

     if (y1==y2) return;

     if (y1>y2) { y=y1; y1=y2; y2=y;
                  x=x1; x1=x2; x2=x;                 
                  z=z1; z1=z2; z2=z;
                  i=i1; i1=i2; i2=i; }
	  
     kx= ((x2-x1)<<16)/(y2-y1); 
     ki= ((i2-i1)<<16)/(y2-y1);
     kz= (z2-z1)/(y2-y1); 

     y=y1; z=z1; i=i1<<16; x=x1<<16;

     while(y<y2)
      {
        y++; x+=kx; i+=ki; z=z+kz;
        xzi_buf[y].x=(x+0x8000)>>16;     // x>>16 for 32-bit integer 
        xzi_buf[y].i=(i+0x8000)>>16;
        xzi_buf[y].z=z;
      }
  }

void ZBUFFER::GRD_Polygon(int N,WPOINT2 *b,double *Zet,WSHORT *Ints,WTEXTUR *txtr,WCONTROL *c)
  {
    int i,j,y;
    int ymin=b[0].y, ymax=b[0].y;

    for(i=0;i<N;i++)
     {
       if (b[i].y>ymax) ymax=b[i].y;
       if (b[i].y<ymin) ymin=b[i].y;
      
       j=(i+1)%N;
       if (b[i].y==b[j].y) continue;
       if (b[i].y>b[j].y) 
             Line_Y_inc_GRD(b[i].x, b[i].y, Zet[i], (int)Ints[i], 
                            b[j].x, b[j].y, Zet[j], (int)Ints[j], right);
       else  Line_Y_inc_GRD(b[i].x, b[i].y, Zet[i], (int)Ints[i],
                            b[j].x, b[j].y, Zet[j], (int)Ints[j], left);
     }
    if (ymin==ymax) return;

    for(y=ymin; y<=ymax; y++)
      Line_X_inc_GRD(  left[y].x,  left[y].z,  left[y].i, 
                      right[y].x, right[y].z, right[y].i, y, txtr, c);
  }

/********************************* Phong *********************************/

void ZBUFFER::Line_X_inc_PNG(int x1, double z1, WPOINT n1,
                             int x2, double z2, WPOINT n2,
                             int y, WTEXTUR *txtr,WCONTROL *c)
  {
     int x,kx;
     WPOINT n,kn;
     double z,kz;
     int farba;
     WUCHAR shade;
     WPOINT midle;

     if (x1==x2) return;

     if (x1>x2) { kx= -1; kz= (z2-z1)/(x1-x2); kn= (n2-n1)/(x1-x2); }
           else { kx=  1; kz= (z2-z1)/(x2-x1); kn= (n2-n1)/(x2-x1); }

     x=x1; z=z1; n=n1;

     while(x!=x2)
       {
          x+=kx; z=z+kz; n+=kn; 

          if (z < ZZ(x,y)) 
            {
               ZZ(x,y)=z;

               midle = un_proj(x,y,z,c);
               shade = shadec(n,midle,c);
        
               if (txtr->typ)
                    farba=color_number(txtr->ComputeColor(midle),shade);
               else farba=color_number(txtr->color1,shade);

               WPlot(x,y,farba);
            }
       } 
   } 

static void Line_Y_inc_PNG(int x1, int y1, double z1, WPOINT n1,
                           int x2, int y2, double z2, WPOINT n2, XZ_BUF *xzn_buf)
  {
     int x,y,kx;
     WPOINT n,kn;
     double z,kz;
     
     xzn_buf[y1].x=x1; xzn_buf[y1].z=z1; xzn_buf[y1].n=n1; 
     xzn_buf[y2].x=x2; xzn_buf[y2].z=z2; xzn_buf[y2].n=n2; 

     if (y1==y2) return;

     if (y1>y2) { y=y1; y1=y2; y2=y;
                  x=x1; x1=x2; x2=x;                 
                  z=z1; z1=z2; z2=z;
                  n=n1; n1=n2; n2=n; }
	  
     kx = ((x2-x1)<<16)/(y2-y1); 
     kz = (z2-z1)/(y2-y1); 
     kn = (n2-n1)/(y2-y1);

     y=y1; z=z1; n=n1; x=x1<<16;

     while(y<y2)
      {
        y++; x+=kx; n+=kn; z=z+kz;

        xzn_buf[y].x=(x+0x8000)>>16;    // x>>16 for 32-bit integer only
        xzn_buf[y].n=n;
        xzn_buf[y].z=z;
      }
  }

void ZBUFFER::PNG_Polygon(int N,WPOINT2 *b,double *Zet,WPOINT *nrml, WTEXTUR *txtr,WCONTROL *c)
  {
    int i,j,y;
    int ymin=b[0].y, ymax=b[0].y;

    for(i=0;i<N;i++)
     {
       if (b[i].y>ymax) ymax=b[i].y;
       if (b[i].y<ymin) ymin=b[i].y;
      
       j=(i+1)%N;
       if (b[i].y==b[j].y) continue;
       if (b[i].y>b[j].y) 
             Line_Y_inc_PNG(b[i].x, b[i].y, Zet[i], nrml[i], 
                            b[j].x, b[j].y, Zet[j], nrml[j], right);
       else  Line_Y_inc_PNG(b[i].x, b[i].y, Zet[i], nrml[i],
                            b[j].x, b[j].y, Zet[j], nrml[j], left);
     }
    if (ymin==ymax) return;

    for(y=ymin; y<=ymax; y++)
      Line_X_inc_PNG(  left[y].x,  left[y].z,  left[y].n, 
                      right[y].x, right[y].z, right[y].n, y, txtr, c);
  }

/********************************* END **************************************/
