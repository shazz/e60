/*
 * E60 donu demo using GnuGL (C) from Richard Gosiorovsky 
 * code : shazz
 * 
 * thanks to Yoda222 for the db example
 * 
 * 
 */
 
 //All you need to build 3D application 
 //    is to resolved WPlot(..), WGetPixel(..) functions … 
 //    (BMP/PCX files, Allegro, Grx23)

 
#include <stdio.h>
#include "../gnugl.h"
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#include <linux/input.h>


#include "s3c_epd_userland.h"

#define SCREEN_BPP		4
#define SCREEN_WIDTH	600
#define SCREEN_HEIGHT	800

#if SCREEN_BPP == 4
static char fb[SCREEN_WIDTH/2][SCREEN_HEIGHT];
#else
static int fb[SCREEN_WIDTH][SCREEN_HEIGHT];
#endif

void WPlot(int x,int y,int c) 
{ 
#if SCREEN_BPP == 32
	fb[x][y] = c; 
#else
	int r = (c & 0x00FF0000) >> 16;
	int g = (c & 0x0000FF00) >> 8;
	int b = (c & 0x000000FF);
	
	char grayscaleVal = (char) ((r + g + b) / 3);
	
	//printf("at[%d,%d] c %010x r: %x g: %x b:%x => g:%x\n", x, y, c, r, g ,b, grayscaleVal);
	int x2 = x / 2;
	if ( x % 2 != 0 )
	{
		grayscaleVal = grayscaleVal >> 4;
		fb[x2][y] = (fb[x2][y] & 0xF0) | grayscaleVal;
	}
	else
	{
		grayscaleVal = grayscaleVal & 0xF0;
		fb[x2][y] = (fb[x2][y] & 0x0F) | grayscaleVal;
	}
	//printf("fill table in x=%d with val %x => %x\n", x2, grayscaleVal, fb[x2][y]);
#endif
	
}

int WGetPixel(int x,int y) 
{ 
	printf("AAAAAAAAAAAAAARG\n");
	return fb[x][y]; 
}

void handler (int sig)
{
  printf ("\nexiting...(%d)\n", sig);
  exit (0);
}
 
void perror_exit (char *error)
{
  perror (error);
  handler (9);
}
 

int main(void)
{
	// 3D
	int obj_nb;
	
    // framebuffer
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;

	// events
	struct input_event ev[64];
	int fd, rd, size = sizeof (struct input_event);
	char name[256] = "Unknown";

    struct epd_control_data data;

    data.x=0;
    data.y=0;
    data.data_x=0;
    data.data_y=0;
    data.dfmt=2;
    data.width=600;
    data.height=800;
    data.erase_flag=1;
    data.buf_index=0;
    data.async_mtd=0;
    data.overlap_check=0;
    data.update_only=0;
    data.type=ePART;

	//Open Device
	if ((fd = open ("/dev/input/event0", O_RDONLY | O_NONBLOCK)) == -1)
		printf ("This is not a vaild device.\n");
		
	//Print Device Name
	ioctl (fd, EVIOCGNAME (sizeof (name)), name);
	printf ("Reading From : %s\n", name);
	
	// preparing fb
    /* Open the file for reading and writing */
    fbfd = open ("/dev/fb0", O_RDWR);
    if (!fbfd)
    {
        printf ("Error: cannot open framebuffer device.\n");
        return 1;
    }
    printf ("The framebuffer device was opened successfully.\n");

    /* Get fixed screen information */
    if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf ("Error reading fixed information.\n");
        return 2;
    }

    /* Get variable screen information */
    if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf ("Error reading variable information.\n");
        return 3;
    }

    printf("size vert:%i\thori:%i\tbpp:%i\n", vinfo.yres, vinfo.xres, vinfo.bits_per_pixel);
    printf("offset vert:%i\thori:%i\n", vinfo.yoffset, vinfo.xoffset);
    printf("length of a line in bytes:%i\n", finfo.line_length);

    /* Figure out the size of the screen in bytes */
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    /* Map the device to memory */
    fbp = (char *) mmap (0, screensize, PROT_READ | PROT_WRITE,
		       MAP_SHARED, fbfd, 0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory.\n");
        return 4;
    }
    printf ("The framebuffer device was mapped to memory successfully.\n");

    // les modif que j'ai faites ne sont pas generiques
    // suppose que les offset = 0	

	printf("Setting scene\n");
	Scena *sc1 = new Scena;
	
	// set matrix
	WMATRIX m; m.unit(); m.resize(180.0); 
	
	// set no texture
	WTEXTUR t(OFF,0x000000,0x000000);	
	//WTEXTUR t(RUBIC,0xa090f0,0x30f078);

	// set windows	
	sc1->set_window(SCREEN_WIDTH,SCREEN_HEIGHT);

	// set render type
	sc1->set_model(FFF);
	sc1->set_back_faces(0);

	printf("Add objects\n");
	obj_nb = sc1->add_object(TORUS,m,&t);
	//sc1->fit();
	sc1->set_zoom(0.60);
	
	bool nostop = 0;
	
	while(nostop == 0)
	{
		// clear screen
		sc1->clear(0xFFFFFF);
		
		// Compute position
		sc1->rotate(WPOINT(100,100,0),10);
		sc1->go();		
				
		// displaying on fb
		//memcpy ( (void *) fbp, (const void *) fb, screensize );
		int x,y;			
		int counter = 0;
		for (y = 0; y < 800; y++)
		{
			for (x = 0; x < 300; x++)
			{
				fbp[counter++] = fb[x][y];
			}
		}
		
		ioctl(fbfd, 0x4004405A, &data);
		
		// check key
		if (!(rd = read (fd, ev, size * 64)) < size)
		{
			int keycode = ev[0].code;
			
			if(keycode != 0)
			{
				if(keycode == 19) // press Prev button to exit
				{
					nostop = 1;
				}	
				else if(keycode == 11 && ev[0].value == 1) // Next button should change the rendering
				{
					//sc1->remove_object(0);
					
					//t.typ = RUBIC;
					//t.color1 = 0xa090f0;
					//t.color2 = 0x30f078;
					WTEXTUR tex(RUBIC,0xa090f0,0x30f078);
					sc1->set_model(ZBF);					
					obj_nb = sc1-> add_object(CUBE,m,&tex);	
				}	
				else
				{
					printf ("Key code [%d]\n", (keycode));
					if(ev[0].value == 0) 		printf ("Key is released\n");
					else if(ev[0].value == 1) 	printf ("Key is pressed\n");
					else if(ev[0].value == 2) 	printf ("Key is still pressed\n");							
				}
			}
		}
		else
		{
			perror_exit ((char *)"read()");   
		}
			   

			
	}
	
	printf("Close link ot fb\n");
    munmap (fbp, screensize);
    close (fbfd);	

	printf("Done.\n");

   return 0;
 }
 
 
