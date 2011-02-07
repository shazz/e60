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

void WPlot(int x,int y, int c);  //  resolved by the app
 int WGetPixel(int x,int y);     //

//#define double float     

enum MODELS  { FFF, FFC, DSF, DSC, ZBF, GRD, PNG };
enum TEXTUR  { OFF, PLANAR, SPHER, CUBIC, BUBLE, RUBIC, CLOUDS, MARBLE, WOOD };
enum OBJCTS  { CUBE, SPHERE, TORUS, CYLINDER, TETRAHEDRON, CONUS, BICONUS, 
               SALAM, BAMBOO, DOUBLE, SPLAS, GOMEA, STAR5, STAR6, QUARK,
               FONTAN, RINGO, WURM, GRID1, GRID2, GRID3, GRID4, GRID5, GRID6,
               DROP, RPLANE, PILLOW };

#define WPI 3.14159265358979323

#define WUCHAR  unsigned char
#define WSHORT  unsigned short int  

class WPOINT2 { public: WSHORT x,y; };

class WPOINT
 {
   public:
    double x,y,z;

    WPOINT() {}
    WPOINT(double x0,double y0,double z0) { x=x0; y=y0; z=z0; }

     void zerov(); 
   double size(); 
   double dist(WPOINT a); 
     void normalize();

   WPOINT operator+ (WPOINT a); 
     void operator+=(WPOINT a); 
   WPOINT operator- (WPOINT a); 
     void operator-=(WPOINT a); 
   WPOINT operator* (double k); 
     void operator*=(double k); 
   WPOINT operator/ (double k);
     void operator/=(double k);

   double operator*(WPOINT a);           //   dot product
   WPOINT operator^(WPOINT a);           // cross product

  WPOINT2 proj(class WCONTROL*);
 };

class VERTEX {
   public:
        WPOINT  bod;                 //  vertex 3D(x,y,z)
        WPOINT2 bd;                  //  projected vertex 2D(x,y)
        char    fcount;              //  # of faces belongs to vertex
        WPOINT  normv;               //  'normal vector' of the vertex
        WSHORT  intensity; };

class WMATRIX
 {
  public:
    double a[4][4]; 
  
   void    unit();
   void    resize(double k);
   void    resize(double kx,double ky,double kz);
   void    rotate(WPOINT axe,double alfa);
   void    rotate(WPOINT axe,double alfa,WPOINT zero);
   void    set_translation(WPOINT T);
   WPOINT  get_translation();
   void    orient(WPOINT,WPOINT);
   WMATRIX operator*(WMATRIX B);
   WPOINT  operator*(WPOINT P);
 };

class WFACE
 {
   public:
       char verts;
     WUCHAR shade;
     WSHORT vertex[4];
  
        int visible(class WOBJECT*,WCONTROL*);
     double distance(WOBJECT*,WCONTROL*);
       void draw(WOBJECT*,WCONTROL*,class ZBUFFER *);
     WPOINT normal(WOBJECT*);
  
      WFACE() {}
      WFACE(int a,int b,int c)       { verts=3; shade=0; vertex[0]=a; vertex[1]=b; vertex[2]=c; vertex[3]=0; }
      WFACE(int a,int b,int c,int d) { verts=4; shade=0; vertex[0]=a; vertex[1]=b; vertex[2]=c; vertex[3]=d; }

   private:
     WUCHAR compute_shade(WOBJECT*,WCONTROL*);
       void WFillPoly(int N,WPOINT2 *b,int c);
       void WPoly    (int N,WPOINT2 *b,int c);
 }; 

   int color_number(int color,WUCHAR shade);
WUCHAR shadec(WPOINT,WPOINT,WCONTROL*);
WPOINT un_proj(int x,int y,double z,WCONTROL *);

class WTEXTUR 
 {
   public: 
     int typ;
     int color1,color2;
     WTEXTUR() {}
     WTEXTUR(int t,int c1,int c2) { typ=t; color1=c1; color2=c2; }
     int ComputeColor(WPOINT p);
 };

class WOBJECT
 { 
  public:
    int     typ;
    WMATRIX position;
    WTEXTUR textur;
    char    selected;

    int     verts;             // number of vertices
    int     faces;             // number of faces
    VERTEX  *vertex;
    WFACE   *face;   

  public:
    WOBJECT() { verts=0; faces=0; }
    void resize(double zv);
    void move(WPOINT M);
    void rotate(WPOINT axe, double alfa);     
    void rotate(WPOINT axe, double alfa, WPOINT zero);
    void transform(WMATRIX M);
     int create(int typ,WMATRIX locate,WTEXTUR *txtr);
    void draw(WCONTROL*,ZBUFFER*);
    void project(WCONTROL*);
    void freeo();
     int alloc(int v,int f);

  private:
    void init_PNG(WCONTROL*);
    int cube();
    int tetrahedron();
    int sphere();
    int torus();
    int cylinder();
    int conus();
    int biconus();
    int boojoom();
    int wurm();
    int ringo();
    int grid();
    int bezier();
 };

class WCONTROL
 {
   public:
       int Win_W,Win_H;
       int model;
       int back_face;

    double light_ambient;
    double light_specular;
    double light_specular_exponent;
    double light_diffuse;

    double zoom;
    double look_fi;
    double look_theta; 
    WPOINT obs;          // an observer position
    WPOINT src;          // light source position 

    WCONTROL()
     {
       Win_W = 640; Win_H = 480;
       model = FFF;
       back_face = 0;

       light_ambient  =  40.0;            //  50.0
       light_specular =  80.0;            //  72.0
       light_specular_exponent = 8.0;     //   4.0
       light_diffuse  = 180.0;            // 134.0

       zoom = 640.0;       
       look_fi = 0.0;
       look_theta = 0.0;
       obs= WPOINT(0.0, 0.0, -600.0);
       src= WPOINT(0.0, 0.0, -10000.0);
     };
 };

#define YMaxRES 1200
typedef struct {int x; double z; int i; WPOINT n;} XZ_BUF;
// interpolate z(ZBUF), inensity(GOUROUD), norm.vector(PHONG) depends on x 

class ZBUFFER
 {
   private:
     int opened;
     XZ_BUF left[YMaxRES], right[YMaxRES];
     double *Z_buffer; 

   private:
     void close();
     void Line_X_inc_ZBF(int x1, double z1,            int x2, double z2,            int y, int clr , WCONTROL *c);
     void Line_X_inc_GRD(int x1, double z1,    int i1, int x2, double z2,    int i2, int y, WTEXTUR*, WCONTROL *c);
     void Line_X_inc_PNG(int x1, double z1, WPOINT n1, int x2, double z2, WPOINT n2, int y, WTEXTUR*, WCONTROL *c);

   public:
      ZBUFFER() { opened=0; }
     ~ZBUFFER() { close(); }
      int open( WCONTROL *c);
     void clear(WCONTROL *c);
     void ZBF_Polygon(int N,WPOINT2 *b,double *Zet,             int clr ,WCONTROL *c);
     void GRD_Polygon(int N,WPOINT2 *b,double *Zet,WSHORT *Ints,WTEXTUR*,WCONTROL *c);
     void PNG_Polygon(int N,WPOINT2 *b,double *Zet,WPOINT *nrml,WTEXTUR*,WCONTROL *c);
 };

class WSPLINE
 {
   private:
     WPOINT a[4][4];    // control points net

   public:
     WPOINT value(double u,double v);
       void single_close();
       void single_open();
       void single_cube();
       void copy(WSPLINE s2);
 };

/*************************************************************************/

#define MAX_OBJECTS 400         //  Max. number of objects

class Scena
 {
   private:
     int pt;                             // current number of objects
     WOBJECT object[MAX_OBJECTS];
     WCONTROL c;
     ZBUFFER zb;

   public:  
     Scena() { pt=0; };
      int read(char *fn);
      int save(char *fn);

     void move(WPOINT M);
     void rotate(WPOINT axe, double alfa);    
     void rotate(WPOINT axe, double alfa, WPOINT zero);
     void transform(WMATRIX M);
     void resize(double zv);
     void fit();
     void go();
     void clear(int bg_color);
     void clear(char *pcx_file_name);    // 8 bpp pcx_file only

     void set_window(int W,int H);
     void set_model(int);
     void set_observer(WPOINT);
     void set_light_position(WPOINT);
     void set_light(double,double,double,double);
     void set_look(double fi,double theta); 
     void set_zoom(double d); 
     void set_back_faces(int);

      int add_object(int type,WMATRIX m,WTEXTUR *t);
     void remove_object(int index);

    WPOINT unproj(WPOINT2,double z);
    WPOINT2 proj(WPOINT p);

  private:
      int read_trg(char *fn);
      int save_trg(char *fn);
      int read_chm(char *fn);
      int read_3d( char *fn);
      int save_3d( char *fn);
      int read_pcx(char *fname,int x,int y,int w,int h); 
      int save_pcx(char *fname,int x,int y,int w,int h); 
      int save_bmp(char *fname,int x,int y,int w,int h); 

     void draw();
     void draw_sort();
     void project();
     void select_object(int x,int y);
 };

/************************** END OF GnuGL.H *******************************/
