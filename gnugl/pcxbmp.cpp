// Taken somewhere from internet and slightly modified

#include <stdio.h>
#include <stdlib.h>
#include "gnugl.h"

/************************************** PCX ********************************/

static int fputw(int w, FILE *f)
 {
   int b1, b2;

   b1 = (w & 0xFF00) >> 8;
   b2 = w & 0x00FF;

   if (fputc(b2,f)==b2)
      if (fputc(b1,f)==b1)
         return w;

   return EOF;
 }

static int fgetw(FILE *f)
 {
   int b1, b2;

   if ((b1 = fgetc(f)) != EOF)
      if ((b2 = fgetc(f)) != EOF)
         return ((b2 << 8) | b1);

   return EOF;
 }

int Scena::read_pcx(char *filename,int x0,int y0,int w,int h)   // 8 bpp only
 {
   FILE *f;
   int cc,x,y;
   int width, height;
   int bytes_per_line;
   int r,g,b,paleta[256];
   unsigned char ch, *pixmap;

   f = fopen(filename,"rb");
   if (!f) return 0;

   fgetc(f);                    // skip manufacturer ID 
   fgetc(f);                    // skip version flag 
   fgetc(f);                    // skip encoding flag 

   if (fgetc(f) != 8)           // we can only handle 8 bits per pixel 
     { fclose(f); return 0; }

   width  = -(fgetw(f));        // xmin 
   height = -(fgetw(f));        // ymin 
   width  += fgetw(f) + 1;      // xmax 
   height += fgetw(f) + 1;      // ymax 

   pixmap = (unsigned char*)malloc(width*height*sizeof(unsigned char));
   if (pixmap==NULL) { fclose(f); return 0; }

   fgetw(f);
   fgetw(f);        // skip DPI values 

   for (cc=0; cc<16; cc++)          // read the 16 color pallete 
    {
       r=fgetc(f);
       g=fgetc(f);
       b=fgetc(f);
//     SetColor((long)cc,r,g,b);
    }

   fgetc(f);

   if (fgetc(f) != 1)               // must be a 256 color file 
    { fclose(f); return NULL; }

   bytes_per_line = fgetw(f);

   for (cc=0; cc<60; cc++)  fgetc(f);        // skip some more junk 

   for (y=0; y<height; y++)
    {                            // read RLE encoded PCX data 
      x = 0;
      while (x < bytes_per_line)
       {
         ch = fgetc(f);
         if ((ch & 0xC0) == 0xC0) { cc = (ch & 0x3F); ch = fgetc(f); }
         else cc = 1;
         while (cc--) { pixmap[y*width+x]=ch; x++; }
       }
    }

   while (!feof(f))           // look for a 256 color pallete 
    if (fgetc(f)==0x0C)
       {
         for (cc=0; cc<256; cc++)
           {
             r=fgetc(f);
             g=fgetc(f);
             b=fgetc(f);
//           GrSetColor((long)cc,r,g,b);
             paleta[cc]=(r<<16)|(g<<8)|b;
           }
         break;
       }

   fclose(f);

   for(y=y0;y<y0+h;y++)
    for(x=x0;x<x0+w;x++)
     {
       cc = pixmap[ ((x-x0)%width) + width*((y-y0)%height) ];
       WPlot(x,y,paleta[cc]);
     }

   free(pixmap);
   return 1;
 }

typedef unsigned char wcolor[3];

static void QueryColor(int c,unsigned char *r,unsigned char *g,unsigned char *b)
 {
   *r=(c>>16)&0xff; 
   *g=(c>>8 )&0xff; 
   *b= c     &0xff; 
 }

static int BitsPerPixel() { return(24); }

int Scena::save_pcx(char *filename,int x0,int y0,int w,int h)  // 8,16,24 bpp
 {
   FILE *f;
   int x,y,cc,p;
   int runcount, planes;
   unsigned char runchar, ch, r, g, b;
   char s[35]=" Created by GnuGL (c) 2000     ";
   wcolor *scanline;

   f = fopen(filename, "wb");
   if (!f) return 0;

   planes = BitsPerPixel() / 8;
   if (planes!=1) planes=3;

   if (planes==3) 
    {
       scanline = (wcolor*)malloc(w*sizeof(wcolor));
       if (scanline==NULL) { fclose(f); return 0; }
    }

   fputc(10, f);                /* manufacturer */
   fputc(5, f);                 /* version */
   fputc(1, f);                 /* run length encoding  */
   fputc(8, f);                 /* 8 bits per pixel */
   fputw(0, f);                 /* xmin */
   fputw(0, f);                 /* ymin */
   fputw(w-1, f);               /* xmax */
   fputw(h-1, f);               /* ymax */
   fputw(320, f);               /* HDpi */
   fputw(200, f);               /* VDpi */

   for (cc=0; cc<16; cc++)
    {
      QueryColor(cc,&r,&g,&b);
      fputc(r, f);
      fputc(g, f);
      fputc(b, f);
    }

   fputc(0, f);                 /* reserved */
   fputc(planes, f);            /* one or three color plane */
   fputw(w, f);                 /* number of bytes per scanline */
   fputw(1, f);                 /* color pallete */
   fputw(w, f);                 /* hscreen size */
   fputw(h, f);                 /* vscreen size */

   for (cc=0; cc<30; cc++)  fputc(s[cc], f);
   for (cc=0; cc<24; cc++)  fputc(0, f);           /* filler */

   for (y=0; y<h; y++)            /* for each scanline... */
    {
      runcount = 0;
      runchar = 0;
      for (p=0;p<planes;p++)
       for (x=0; x<w; x++)      /* for each pixel... */
        {
         if (planes==1) ch = WGetPixel(x0+x,y0+y);
         else
          {
            if (p==0) QueryColor(WGetPixel(x0+x,y0+y),
                      &scanline[x][0],&scanline[x][1],&scanline[x][2]); 
            ch = scanline[x][p];
          }
 
         if (runcount==0) { runcount = 1; runchar = ch; }
         else
          {
            if ((ch != runchar) || (runcount >= 0x3f))
             {
               if ((runcount > 1) || ((runchar & 0xC0) == 0xC0))
                 fputc(0xC0 | runcount, f);
               fputc(runchar,f);
               runcount = 1;
               runchar = ch;
             }
            else  runcount++;
          }
       }
      if ((runcount > 1) || ((runchar & 0xC0) == 0xC0)) 
        fputc(0xC0 | runcount, f);
      fputc(runchar,f);
   }

   if (planes==1)
    {
      fputc(12,f);                 /* 256 color pallete flag */
      for (cc=0; cc<256; cc++)
       {
         QueryColor(cc,&r,&g,&b);
         fputc(r, f);
         fputc(g, f);
         fputc(b, f);
       }
    }

   if (planes==3) free(scanline);
   fclose(f);
   return 1; 
 }

/********************************** BMP ********************************/

typedef struct
 {
    short bf_type;
      int bf_size;
    short bf_reserved1;
    short bf_reserved2;
      int bf_offbits;
  } 
BMPfileheader;

typedef struct 
  {
     int bn_size;
     int bn_width;
     int bn_height;
   short bn_planes;
   short bn_bitcount;
     int bn_compression;
     int bn_sizeimage;
     int bn_xpelspermeter;
     int bn_ypelspermeter;
     int bn_clrused;
     int bn_clrimportant;
  }
BMPinfoheader;

#define  BMPFILEHEADERSIZE    14
#define  BMPINFOHEADERSIZE    40

int Scena::save_bmp(char *filename, int x1, int y1, int width, int height)
{
  FILE *handle;
  int dx, dy;
  unsigned char palette[256*4];
  unsigned char r,g,b;
  unsigned char *line;
  int pixcol, colors, i;
  BMPfileheader fileheader;
  BMPinfoheader infoheader;

  handle = fopen(filename,"wb");
  if (!handle) return 0;

  colors = 1<<BitsPerPixel();

  if ( width % 4 ) width += 4 - (width % 4);

  fileheader.bf_type = 19778; // "BM"

  if (colors==256) fileheader.bf_size = BMPINFOHEADERSIZE + BMPFILEHEADERSIZE + 256*4 + width*height;
              else fileheader.bf_size = BMPINFOHEADERSIZE + BMPFILEHEADERSIZE + (width*height*3);

  fileheader.bf_reserved1 = 0;
  fileheader.bf_reserved2 = 0;

  if (colors==256) fileheader.bf_offbits = BMPINFOHEADERSIZE + BMPFILEHEADERSIZE + 256*4;
              else fileheader.bf_offbits = BMPINFOHEADERSIZE + BMPFILEHEADERSIZE;

  infoheader.bn_size   = BMPINFOHEADERSIZE;
  infoheader.bn_width  = width;
  infoheader.bn_height = height;
  infoheader.bn_planes = 1;
  infoheader.bn_bitcount = ( colors == 256 ) ? 8 : 24;
  infoheader.bn_compression = 0L;
  infoheader.bn_sizeimage = width * height * (infoheader.bn_bitcount/8);
  infoheader.bn_xpelspermeter = 0L;
  infoheader.bn_ypelspermeter = 0L;
  infoheader.bn_clrused = 0L;
  infoheader.bn_clrimportant = 0L;

  if (colors==256)
    for ( i = 0; i < colors; i++ )
     {
       QueryColor(i, &r, &g, &b);
       palette[(i*4)]   = b;
       palette[(i*4)+1] = g;
       palette[(i*4)+2] = r;
       palette[(i*4)+3] = 0;
     }

  line = (unsigned char *)malloc(width*(infoheader.bn_bitcount / 8));
  if ( !line ) { fclose(handle); return 0; }

  fwrite( &fileheader.bf_type, 2, 1,handle);
  fwrite( &fileheader.bf_size, 4, 1,handle);
  fwrite( &fileheader.bf_reserved1, 2, 1,handle);
  fwrite( &fileheader.bf_reserved2, 2, 1,handle);
  fwrite( &fileheader.bf_offbits, 4, 1,handle);  

  fwrite( &infoheader.bn_size, 4, 1,handle);
  fwrite( &infoheader.bn_width, 4, 1,handle);
  fwrite( &infoheader.bn_height, 4, 1,handle);
  fwrite( &infoheader.bn_planes, 2, 1,handle);
  fwrite( &infoheader.bn_bitcount, 2, 1,handle);
  fwrite( &infoheader.bn_compression, 4, 1,handle);
  fwrite( &infoheader.bn_sizeimage, 4, 1,handle);
  fwrite( &infoheader.bn_xpelspermeter, 4, 1,handle);
  fwrite( &infoheader.bn_ypelspermeter, 4, 1,handle);
  fwrite( &infoheader.bn_clrused, 4, 1,handle);
  fwrite( &infoheader.bn_clrimportant, 4, 1,handle);  

  if (colors==256) fwrite(palette,256*4,1,handle);

  dy = height;
  do
   {
      dx = 0;
      do
        {
          pixcol = WGetPixel(x1+dx,y1+dy);
          if ( colors == 256 ) line[dx] = pixcol;
          else QueryColor(pixcol,&line[3*dx+2],&line[3*dx+1],&line[3*dx]);
        }
      while(++dx < width);
      fwrite(line,width*(infoheader.bn_bitcount / 8),1,handle);
   }
  while(--dy > 0);

  free((void *)line);
  fclose(handle);
  return 1;
}

/********************************** END ********************************/

