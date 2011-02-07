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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <unistd.h>

#include "gnugl.h"

enum FILE_TYPES { NIL, INI, D3D, TRG, PCX, BMP, JPG, CHE };

static int file_name_extension(char *name)
 {
    int i=0;
    char p[256]; 
    strcpy(p,name);
    //strupr(p);
    while (p[i]=='.') i++;
    while (p[i]) { i++; if (p[i]=='.') break; }
    if (p[i]==0) return NIL;
    i++; if (p[i]==0) return NIL;
    if (!strcmp(&p[i],"INI")) return INI;    
    if (!strcmp(&p[i],"3D"))  return D3D;    
    if (!strcmp(&p[i],"TRG")) return TRG;    
    if (!strcmp(&p[i],"PCX")) return PCX;    
    if (!strcmp(&p[i],"BMP")) return BMP;    
    if (!strcmp(&p[i],"JPG")) return JPG;    
    if (!strcmp(&p[i],"CHE")) return CHE;    
    return NIL;
 }   

int Scena::read(char *s)
 {
    switch(file_name_extension(s))
     {
       case D3D: return read_3d(s); break;
       case TRG: return read_trg(s); break;
       case CHE: return read_chm(s); break;
     }
    return 0;
 }

int Scena::save(char *s)
 {
    switch(file_name_extension(s))
     {
       case D3D: return save_3d(s); break;
       case TRG: return save_trg(s); break;
       case PCX: return save_pcx(s,0,0,c.Win_W,c.Win_H); break;
       case BMP: return save_bmp(s,0,0,c.Win_W,c.Win_H); break;
     }
    return 0;
 }

/***************************** chm *******************************/

int Scena::read_chm(char *fname)
 {
   WPOINT t,pa,pb;
   WMATRIX m1,m2;
   double R;
   char s[99],tok[99],cc='x';
   int l=0,a,b,n=pt-1,color;
   FILE *f=fopen(fname,"r");

   if (f==NULL) return 0;

   while (cc!=EOF) 
    {
      while((cc=fgetc(f))!=0xa)   // read line
       { if (cc==EOF) break; else { s[l]=cc; l++; } if (l>80) l=80; }
      s[l]=0; l=0;
      //**************************** ATOM **************************
      if ((s[0]>='A')&&(s[0]<='Z'))  
       {
#if defined (double)
         sscanf(s,"%s %f %f %f\n",tok,&t.x,&t.y,&t.z);
#else
         sscanf(s,"%s %lf %lf %lf\n",tok,&t.x,&t.y,&t.z);
#endif
         switch (s[0])
          {
            case 'C': color=0xffff40; R=0.3; break;
            case 'O': color=0x60A0ff; R=0.27; break;
            case 'H': color=0xffd0A0; R=0.2; break;
            case 'N': color=0xffc0ff; R=0.24; break;
            default : color=0xffffff; R=0.18;
          }
         WTEXTUR tx(OFF,color,0); 
         m1.unit();  m1.set_translation(t);
         m2.resize(R); 
         if (object[pt].create(SPHERE,m2*m1,&tx)) { fclose(f); return 0; }
         else pt++;
       }
      //**************************** JUNCTION **************************
      if (s[0]=='l')     
       {
         sscanf(s,"%s %d %d\n",tok,&a,&b);
         if (a==b) break;
         if (((n+a)>=pt)||((n+b)>=pt)) { fclose(f); return(0); } // !!!!!!!! bad file
         pa = object[n+a].position.get_translation();
         pb = object[n+b].position.get_translation();
         t = (pa+pb)/2.0;
         WTEXTUR tx(OFF,0xffffff,0);  
         m1.orient(pa,pb);  m1.set_translation(t);
         m2.resize(0.06,0.06,pa.dist(t)); 
         if (object[pt].create(CYLINDER,m2*m1,&tx)) { fclose(f); return 0; }
         else pt++;
       }

     if (pt>=MAX_OBJECTS) { fclose(f); return 0; }
   }
 fclose(f);
 return 1;
}

/******************************* 3d ****************************/

int Scena::save_3d(char *file_name)
  {
    int i,j,k;

    if (!pt) return(1);
    FILE *file=fopen(file_name,"w");
    if (file==NULL) return(0);

    fprintf(file,"BCAD-3D.DATA.FILE....%s.....%d.OBJECTS\n\n",file_name,pt);

    for(i=0;i<pt;i++)
     {
       WOBJECT *t= &object[i];
       fprintf(file,"%d %d %x %x\n",
               t->typ,t->textur.typ,t->textur.color1,t->textur.color2);

       for(j=0;j<4;j++) 
         { 
           for(k=0;k<4;k++) fprintf(file,"%f ",t->position.a[j][k]);
           fprintf(file,"\n");
         }
       fprintf(file,"\n");
     }

    fclose(file);
    return(1);
  }

int Scena::read_3d(char *file_name)
 {
   int i=pt,j,l,typ;
   WTEXTUR textur;
   char s[80];
   WMATRIX position;
   FILE *file=fopen(file_name,"r");
   if (file==NULL) return(0);
   fscanf(file,"%60s\n\n",s); s[12]=0;
   if (strcmp(s,"BCAD-3D.DATA")) { fclose(file); return(2); }
   while(!feof(file))
    {
      if (i>=MAX_OBJECTS) { pt=i; fclose(file); return(3); }
      fscanf(file,"%d %d %x %x\n",&typ,&textur.typ,&textur.color1,&textur.color2);
      if ((typ>50)||(typ<0)) { pt=i; fclose(file); return(4); }
      for(j=0;j<4;j++)
       {
#if defined (double)
         for(l=0;l<4;l++) fscanf(file,"%f ",&(position.a[j][l])); 
#else
         for(l=0;l<4;l++) fscanf(file,"%lf ",&(position.a[j][l])); 
#endif
         fscanf(file,"\n");
       }
      fscanf(file,"\n");
      if (object[i].create(typ,position,&textur)==0) i++;
    }
   pt=i;  
   fclose(file);
   return(1);
 }

/***************************** trg ****************************/

int Scena::read_trg(char *fname)
 {
   WOBJECT *t=&object[pt];
   double x,y,z;
   char s[99],tok[99],cc='x';
   int vv=0,ff=0,k=0,l=0,a,b,c,d,start=0;
   struct { int v; int f; } oo[MAX_OBJECTS+2];
   FILE *f=fopen(fname,"r");

   if (f==NULL) return 0;

   while(cc!=EOF)
    {
      while((cc=fgetc(f))!=0xa)
       {
         if (cc==EOF) break; else { s[l]=cc; l++; }
         if (l>=80) l=80;
       }
      s[l]=0; l=0;

           if (s[0]=='v') { if (start) vv++; else { fclose(f); return 0; }}
      else if (s[0]=='f') { if (start) ff++; else { fclose(f); return 0; }}
      else if (s[0]=='c')
             { 
               start=1;
               oo[k].v=vv; vv=0;
               oo[k].f=ff; ff=0;
               k++; 
               if (k>MAX_OBJECTS) break;
             }
//    if ((vv>=65535)||(ff>=65535)) { fclose(f); return 0; }
    }

   oo[k].v=vv; vv=0;
   oo[k].f=ff; ff=0;
   k=1; cc='x';

   rewind(f); 

   while (cc!=EOF) 
    {
      while((cc=fgetc(f))!=0xa) { if (cc==EOF) break; else { s[l]=cc; l++; } }
      s[l]=0; l=0;

      if (s[0]=='v')
       { 
#if defined (double)
         sscanf(s,"%s %f %f %f\n",tok,&x,&y,&z);
#else
         sscanf(s,"%s %lf %lf %lf\n",tok,&x,&y,&z);
#endif
         t->vertex[vv].bod.x = x;
         t->vertex[vv].bod.y = y;
         t->vertex[vv].bod.z = z;
         vv++;
         if (vv>=65535) vv=65535;
        }
       else if (s[0]=='f') 
        {
          if (s[1]=='4')
           {
              t->face[ff].verts=4;
              sscanf(s,"%s %d %d %d %d\n",tok,&a,&b,&c,&d);
              t->face[ff].vertex[3]=d-1;
           }
          else
           {
              t->face[ff].verts=3;
              sscanf(s,"%s %d %d %d\n",tok,&a,&b,&c);
           }
         t->face[ff].shade=0;  
         t->face[ff].vertex[0]=a-1;
         t->face[ff].vertex[1]=b-1;
         t->face[ff].vertex[2]=c-1;
         ff++;
         if (ff>=65535) ff=65535;
        }
       else if (s[0]=='c')
        {
          if (pt>=MAX_OBJECTS) break;

          t= &object[pt];
          t->typ=0;
          t->position.unit();
          sscanf(s,"%s %d %x %x\n",tok,&t->textur.typ,&t->textur.color1,&t->textur.color2);
          t->selected=0;
          if (t->alloc(oo[k].v,oo[k].f)) { fclose(f); return 0; }
          t->verts=oo[k].v; vv=0;
          t->faces=oo[k].f; ff=0;
          cc='x';
          pt++; k++;
        }   
    }
 fclose(f);
 return 1; 
}

int Scena::save_trg(char *fname)
 {
   int i,j,k,vrch=0,sten=0;
   FILE *f=fopen(fname,"w");
   if (f==NULL) return 0;
   fprintf(f,"# %s\n",fname);

   for(k=0;k<pt;k++)
    {
      WOBJECT *t= &object[k];
      vrch+=t->verts;
      sten+=t->faces;

      fprintf(f,"# -------------------\n");
      fprintf(f,"c %d %6.6x %6.6x\n",t->textur.typ,t->textur.color1,t->textur.color2);         //RrGgBb
      fprintf(f,"#\n");

      for(i=0;i<t->verts;i++)
         fprintf(f,"v %.4f %.4f %.4f\n",t->vertex[i].bod.x,
                                  t->vertex[i].bod.y,
                                  t->vertex[i].bod.z);

      fprintf(f,"# ------\n");

      for(j=0;j<t->faces;j++)
        if (t->face[j].verts==4) 
          fprintf(f,"f4 %d %d %d %d\n",t->face[j].vertex[0]+1,
                                       t->face[j].vertex[1]+1,     
                                       t->face[j].vertex[2]+1,
                                       t->face[j].vertex[3]+1);  
        else
          fprintf(f,"f3 %d %d %d\n",t->face[j].vertex[0]+1,
                                    t->face[j].vertex[1]+1,   
                                    t->face[j].vertex[2]+1);  
    }
  
  fprintf(f,"#\n");
  fprintf(f,"# total: %d objects %d vertices %d faces\n",pt,vrch,sten);
  fprintf(f,"# end of %s\n",fname);
  fclose(f);
  return 1; 
}

/********************************* end *******************************/
