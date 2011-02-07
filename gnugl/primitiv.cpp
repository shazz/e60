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

/*-------------------------------------------------------------------------*/
int WOBJECT::cube()
   {
      verts=8; 
      faces=6;

      if (alloc(verts,faces)) return 1;

      WPOINT point[8] = { WPOINT(-1.0,-1.0, 1.0),  WPOINT( 1.0,-1.0, 1.0),
                          WPOINT( 1.0, 1.0, 1.0),  WPOINT(-1.0, 1.0, 1.0),
                          WPOINT(-1.0,-1.0,-1.0),  WPOINT( 1.0,-1.0,-1.0),
                          WPOINT( 1.0, 1.0,-1.0),  WPOINT(-1.0, 1.0,-1.0) };

      face[0] = WFACE(0,1,2,3); face[1] = WFACE(1,5,6,2);
      face[2] = WFACE(5,4,7,6); face[3] = WFACE(4,0,3,7);
      face[4] = WFACE(4,5,1,0); face[5] = WFACE(3,2,6,7); 

      for(int i=0;i<verts;i++)  vertex[i].bod = point[i];
      return 0;
   }  
/*-------------------------------------------------------------------------*/
int WOBJECT::tetrahedron()
   {
      verts=4; faces=4;

      if (alloc(verts,faces)) return 1;

      vertex[0].bod = WPOINT( 0.0,    1.0, -0.354);
      vertex[1].bod = WPOINT( 0.866, -0.5, -0.354);
      vertex[2].bod = WPOINT(-0.866, -0.5, -0.354);
      vertex[3].bod = WPOINT( 0.0,    0.0,  1.061);

      face[0] = WFACE(1,0,3);  face[1] = WFACE(2,1,3);
      face[2] = WFACE(0,2,3);  face[3] = WFACE(0,1,2);

      return 0;
   } 
/*----------------------------------------------------------------------*/
int WOBJECT::sphere()
   {
       double theta,fi;
       int m,n,i=0,M=20,N=20;

       verts = M*N+N;
       faces = M*N;

       if (alloc(verts,faces)) return 1;

       for(m=0;m<=M;m++)
        {
          if (m==0) theta= -WPI/2.0 + 0.001;
               else theta= -WPI/2.0 + WPI*(double)m/(double)M;

          for(n=0;n<N;n++)
           {
              fi=2.0*WPI*(double)n/(double)N;

              vertex[i].bod.x= cos(theta)*cos(fi);
              vertex[i].bod.y= cos(theta)*sin(fi);
              vertex[i].bod.z= sin(theta);

              if (m<M)
               {
                 if(n==(N-1)) face[i] = WFACE(i,i-N+1,i+1,i+N);
                         else face[i] = WFACE(i,i+1,i+N+1,i+N);
               }
              i++;
           }
         }
     return 0;
  }
/*----------------------------------------------------------------------*/
int WOBJECT::torus()
  {
    double theta,fi,r=0.5,R=1.0;
    int n,m,i=0;
    int M=20,N=36;

    verts=M*N;
    faces=M*N;

    if (alloc(verts,faces)) return 1;

    for(m=0;m<M;m++)
     {
      theta= 2.0*WPI*(double)m/(double)M;

      for(n=0;n<N;n++)
       {
         fi= 2.0*WPI*(double)n/(double)N;

         vertex[i].bod.x = (R + r*cos(theta))*cos(fi);
         vertex[i].bod.y = (R + r*cos(theta))*sin(fi);
         vertex[i].bod.z =      r*sin(theta);

         if(n==(N-1)) face[i] = WFACE(i,i-N+1,i+1,i+N);
                 else face[i] = WFACE(i,i+1,i+N+1,i+N);
         i++;
       }
     }
    for(n=0;n<N;n++) face[n+i-N] = WFACE(n+i-N,n+i-N+1,n+1,n);
                     face[i-1]   = WFACE(i-1,i-N,0,N-1);
    return 0;
  }
/*----------------------------------------------------------------------*/
int WOBJECT::cylinder()
   {
       double fi;
       int m,n,i=0,j=0,N=20;

       verts=2*N+2;
       faces=3*N;

       if (alloc(verts,faces)) return 1;

       for(m=-1;m<2;m=m+2)          
         for(n=0;n<N;n++)
          {
            fi=2.0*WPI*(double)n/(double)N;
            vertex[i].bod = WPOINT(cos(fi),sin(fi),(double)m);
            i++;
          }

      for(n=0;n<N;n++)       
       {
         if(n==(N-1)) face[j] = WFACE(n,n-N+1,n+1,n+N);
                else  face[j] = WFACE(n,n+1,n+N+1,n+N);
         j++;
       }

     for(m=0;m<2;m++)          
      {
        vertex[i].bod = WPOINT( 0.0, 0.0, 2*m-1);
        i++;

        for(n=0;n<N;n++)
         {
          if(n==(N-1)) face[j] = WFACE( m*N +m*(N-1), N-1 + m*N -m*(N-1), i-1);
                  else face[j] = WFACE( n+1 + m*N -m, n + m*N +m, i-1);
          j++;
         }
      }  
    return 0;
  }
/*----------------------------------------------------------------------*/
int WOBJECT::conus()
 {
       double fi;
       int n,i=0,j=0,N=20;

       verts=N+2;
       faces=2*N;

       if (alloc(verts,faces)) return 1;

       for(n=0;n<N;n++)                  // vrcholy podstavy 
        {
          fi=2.0*WPI*(double)n/(double)N;
          vertex[i].bod = WPOINT(cos(fi),sin(fi),0.0);
          i++;
        }

       vertex[i].bod = WPOINT(0.0,0.0, 0.0); i++;
       vertex[i].bod = WPOINT(0.0,0.0,-2.0); i++;

       for(n=0;n<N;n++) { face[j] = WFACE(N+1, (n+1)%N, n); j++; }
       for(n=0;n<N;n++) { face[j] = WFACE(  n, (n+1)%N, N); j++; }
       return 0;
  }
/*----------------------------------------------------------------------*/
int WOBJECT::biconus()
 {
       double fi;
       int n,i=0,j=0,N=20;

       verts=N+2;
       faces=2*N;

       if (alloc(verts,faces)) return 1;

       for(n=0;n<N;n++)  
        {
          fi=2.0*WPI*(double)n/(double)N;
          vertex[i].bod = WPOINT(cos(fi),sin(fi),0.0);
          i++;
        }

       vertex[i].bod = WPOINT(0.0,0.0,-1.0); i++;
       vertex[i].bod = WPOINT(0.0,0.0, 1.0); i++;

       for(n=0;n<N;n++) { face[j] = WFACE( n,(n+1)%N,N+1); j++; }
       for(n=0;n<N;n++) { face[j] = WFACE( (n+1)%N, n, N); j++; }
       return 0;
  }
/*----------------------------------------------------------------------*/
int WOBJECT::boojoom()            // sphere derived objects
 {
   double theta,fi;
   int m,n,i=0;
   int M=20,N=20;

   if ((typ==STAR5)||(typ==STAR6)) N=60;
   if (typ==QUARK) M=40;
   if (typ==FONTAN) M=30;

   verts=M*N+N;
   faces=M*N;

   if (alloc(verts,faces)) return 1;

   for(m=0;m<=M;m++)
    {
     if (m==0) theta= -WPI/2.0+0.001;
          else theta= -WPI/2.0+WPI*(double)m/(double)M;

     for(n=0;n<N;n++)
      {
         fi=2*WPI*(double)n/(double)N;

         switch(typ)
          {
            case SALAM:
             vertex[i].bod.x= (2.0-cos(theta))* cos(theta)*cos(fi);
             vertex[i].bod.y= (2.0-cos(theta))* cos(theta)*sin(fi);
             vertex[i].bod.z= sin(theta);
             break;
            case BAMBOO:
             vertex[i].bod.x= 1.2 * (1.2-cos(theta)*cos(theta))* cos(theta)*cos(fi);
             vertex[i].bod.y= 1.2 * (1.2-cos(theta)*cos(theta))* cos(theta)*sin(fi);
             vertex[i].bod.z= sin(theta);
             break;
            case DOUBLE:
             vertex[i].bod.x= 0.7 * (1.2-cos(2.0*theta)*cos(2.0*theta))* cos(theta)*cos(fi);
             vertex[i].bod.y= 0.7 * (1.2-cos(2.0*theta)*cos(2.0*theta))* cos(theta)*sin(fi);
             vertex[i].bod.z= sin(theta);
             break;
            case SPLAS:
             vertex[i].bod.x= cos(theta)*cos(fi);
             vertex[i].bod.y= cos(theta)*sin(fi);
             vertex[i].bod.z= 0.6 * (0.2+cos(theta)*cos(theta)) *sin(theta);
             break;
            case GOMEA:
             vertex[i].bod.x= (1.2-cos(2.0*theta)*cos(2.0*theta))* cos(theta)*cos(fi);
             vertex[i].bod.y= (1.2-cos(2.0*theta)*cos(2.0*theta))* cos(theta)*sin(fi);
             vertex[i].bod.z= 0.8 * (1.0-cos(2.0*theta)) *sin(theta);
             break;
            case STAR5:
             vertex[i].bod.x= cos(theta)*cos(fi) * (3.0+cos(5.0*fi))/4.0;
             vertex[i].bod.y= cos(theta)*sin(fi) * (3.0+cos(5.0*fi))/4.0;
             vertex[i].bod.z= 0.4 * sin(theta);
             break;
            case STAR6:
             vertex[i].bod.x= cos(theta)*cos(fi) * (3.0+cos(6.0*fi))/4.0;
             vertex[i].bod.y= cos(theta)*sin(fi) * (3.0+cos(6.0*fi))/4.0;
             vertex[i].bod.z= 0.4 * sin(theta);
             break;
            case QUARK:
             vertex[i].bod.x= 0.8 * (1.2-cos(4.0*theta)*cos(4.0*theta))* cos(theta)*cos(fi)/1.2;
             vertex[i].bod.y= 0.8 * (1.2-cos(4.0*theta)*cos(4.0*theta))* cos(theta)*sin(fi)/1.2;
             vertex[i].bod.z= 1.4 * sin(theta);
             break;
            case FONTAN:
             vertex[i].bod.x= cos(theta)*cos(fi);
             vertex[i].bod.y= cos(theta)*sin(fi);
             vertex[i].bod.z= 0.6 * (0.2+cos(4.0*theta)*cos(4.0*theta)) *sin(theta)/1.2;
             break;
            default:
             vertex[i].bod.x= cos(theta)*cos(fi);
             vertex[i].bod.y= cos(theta)*sin(fi);
             vertex[i].bod.z= sin(theta);
             break;
          }

       if (m<M)
        {
          if(n==(N-1)) face[i] = WFACE(i,i-N+1,i+1,i+N);
                 else  face[i] = WFACE(i,i+1,i+N+1,i+N);
        }
       i++;
     }
   } 
   return 0;
 }
/*----------------------------------------------------------------------*/
int WOBJECT::ringo()
 {
   double theta,fi,rr,r=0.4,R=1.0;
   int n,m,i=0;
   int M=20,N=40;

   verts=M*N;
   faces=M*N;

   if (alloc(verts,faces)) return 1;

   for(m=0;m<M;m++)
    {
      theta= 2.0*WPI*(double)m/(double)M;

      for(n=0;n<N;n++)
       {
          fi= 2.0*WPI*(double)n/(double)N;
          rr = r + r * cos(6.0*fi) / 4.0;

          vertex[i].bod.x= (R+ rr *cos(theta))*cos(fi);
          vertex[i].bod.y= (R+ rr *cos(theta))*sin(fi);
          vertex[i].bod.z=     rr *sin(theta);

          if(n==(N-1)) face[i] = WFACE(i,i-N+1,i+1,i+N);
                  else face[i] = WFACE(i,i+1,i+N+1,i+N);
          i++;
       }
    }

   for(n=0;n<N;n++) face[n+i-N] = WFACE(n+i-N,n+i-N+1,n+1,n);
                    face[i-1]   = WFACE(i-1,i-N,0,N-1); 
   return 0;
 }
/*----------------------------------------------------------------------*/
int WOBJECT::wurm()       
  {
     double theta,fi,rr,R=1.0,r=0.5,nzv=3.0;      // pocet zavitov
     int M=20,N=90,m,n,i=0;

     verts=M*N+M;
     faces=M*N;

     if (alloc(verts,faces)) return 1;

     for(n=0;n<=N;n++)
      {
       fi= nzv*WPI*((2.0*((double)n/(double)N)) - 1.0);

       for(m=0;m<M;m++)
        {
         theta= 2.0*WPI*(double)m/(double)M;
         rr = r * sqrt(1.001-((fi/(nzv*WPI))*fi/(nzv*WPI)));

         vertex[i].bod.x= (R*cos(fi/(nzv*3.0)) + rr*cos(theta)) *cos(fi) /(R+r);
         vertex[i].bod.y= (R*cos(fi/(nzv*3.0)) + rr*cos(theta)) *sin(fi) /(R+r);
         vertex[i].bod.z= ( r*fi/2.0 + rr*sin(theta) ) /(R+r);

         if (n<N)
          {
            if(m==(M-1))  face[i] = WFACE(i+M,i+1,i-M+1,i);
                    else  face[i] = WFACE(i+M,i+M+1,i+1,i);
          }
         i++;
       }
     } 
   return 0;
  }
/*----------------------------------------------------------------------*/
static double func(double x,double y,int sw)      // <-1.0;1.0> 
 {
   double z,r;
   switch(sw)
    {
      case GRID1: r = sqrt(x*x+y*y); if (r==0.0) z=0.5; else z=0.5*sin(6.0*WPI*r)/(6.0*WPI*r); break;
      case GRID2: r = sqrt(x*x+y*y); if (r>0.6) z=0.0; else z=1.2*(0.6-r); break;
      case GRID3: z = 0.1 * sin(2.0*WPI*(x+y)) * (x-y) * (x-y); break;
      case GRID4: r = x*x+y*y; z = exp(-4.0*r); break;
      case GRID5: z = sin(WPI*x)*sin(WPI*x)*(1.0-y*y); break;
      case GRID6: z = 0.0; break;
      default:    z = 0.0;
    }
   return z;
 }

int WOBJECT::grid()
 {
    int i,j,N=24;
    double x,y;

    verts= (N+1)*(N+1);
    faces= N*N;

    if (alloc(verts,faces)) return 1;

    for(i=0;i<=N;i++)
     for(j=0;j<=N;j++)
      {
        x = 2.0 * (double)i/N - 1.0;
        y = 2.0 * (double)j/N - 1.0;
        vertex[i+j*(N+1)].bod = WPOINT(x,y,-func(x,y,typ));
      }

   for(i=0;i<N;i++)
    for(j=0;j<N;j++)
     face[i+j*N] = WFACE(i+j*(N+1),i+(j+1)*(N+1),i+(j+1)*(N+1)+1,i+j*(N+1)+1);

   return 0;
 }
/*----------------------------------------------------------------------*/
int WOBJECT::bezier()
 {
    WSPLINE s;
    int N=20,i,j;
    double u,v;

    verts = (N+1)*(N+1);
    faces = N*N;

    if (alloc(verts,faces)) return 1;

    switch(typ) 
     {
       case DROP   : s.single_close(); break;
       case RPLANE : s.single_open();  break;
       case PILLOW : s.single_cube();  break;
       default     : s.single_open();
     }

   for(i=0;i<=N;i++)
    for(j=0;j<=N;j++)
     {
        u=(double)i/(double)N;     
        v=(double)j/(double)N;
        vertex[i+j*(N+1)].bod = s.value(u,v);
     }

   for(i=0;i<N;i++)
    for(j=0;j<N;j++)
     {
       face[i+j*N].verts = 4;
       face[i+j*N].vertex[0] = i+j*(N+1);
       face[i+j*N].vertex[3] = i+j*(N+1)+1;
       face[i+j*N].vertex[2] = i+(j+1)*(N+1)+1;
       face[i+j*N].vertex[1] = i+(j+1)*(N+1);
     }

   return 0;
 }
/*----------------------------------------------------------------------*/
