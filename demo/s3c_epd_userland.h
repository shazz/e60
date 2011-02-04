/*
 * drivers/video/s3c/s3cfb.h
 *
 * $Id: s3cfb.h,v 1.1 2008/11/17 11:12:08 jsgood Exp $
 *
 * Copyright (C) 2008 Jinsung Yang <jsgood.yang@samsung.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 *
 *	S3C Frame Buffer Driver
 *	based on skeletonfb.c, sa1100fb.h, s3c2410fb.c
 */

#ifndef _S3C_EPD_USERLAND_H_
#define _S3C_EPD_USERLAND_H_

/*---------- Structure Definitions ----------*/

extern "C" {

/**
 * Structure with fixed information of framebuffer
 */

				
				
#define GC	3
#define GU	2
#define DU	1

//GC-FullUpdate(eFULL), GU-PartialUpdate(ePART), DU-PartialUpdate(ePART_SMOOTH), DU-PartialUpdate Async(ePART_FASTEST), GC-PartialUpdate(ePART_CLEAR) in order
enum upd_type {eFULL , ePART, ePART_SMOOTH, ePART_FASTEST, ePART_CLEAR} upd_type_t;

struct image_update_data {
	uint16_t x;//	screen offset
	uint16_t y;//	screen offset
	uint16_t data_x;	//	data offset
	uint16_t data_y;	//	data offset 
	uint16_t dfmt;	// packed pixel 2
	uint16_t width;
	uint16_t height;
	uint8_t  *img; 
	uint8_t  erase_flag;	//black-0 or white-1
	uint8_t buf_index;		//0~14 Previously this was Img_buf
	uint8_t async_mtd;	//Async approach
	uint8_t overlap_check; //Overlap Check (But This causes performance slow-down)
	enum upd_type type;
};


struct epd_control_data {
	uint16_t x;//	screen offset
	uint16_t y;//	screen offset
	uint16_t data_x;	//	data offset
	uint16_t data_y;	//	data offset 
	uint16_t dfmt;	// packed pixel 2
	uint16_t width;
	uint16_t height;
	uint8_t  erase_flag;	//black-0 or white-1
	uint8_t buf_index;		//0~14
	uint8_t async_mtd;	//Async approach	
	uint8_t overlap_check; //Overlap Check (But This causes performance slow-down)
	uint8_t update_only; //Skip Loading Part and do only update
	
	enum upd_type type;	
};

struct epd_queue_data {
	uint16_t x;//	screen offset
	uint16_t y;//	screen offset
	uint16_t data_x;	//	data offset
	uint16_t data_y;	//	data offset 
	uint16_t dfmt;	// packed pixel 2
	uint16_t width;
	uint16_t height;
	uint8_t  erase_flag;	//black-0 or white-1
	uint8_t buf_index;		//0~14
	uint8_t async_mtd;	//Async approach	
	uint8_t overlap_check; //Overlap Check (But This causes performance slow-down)
	
	enum upd_type type;	
};


struct fb_area {
	int x; 
	int y; 
	int w; 
	int h; 
}; 
	
	
#define PDEBUG(fmt, args...)    printk("%s:%s:%d> " fmt, __FILE__,__func__, __LINE__, ##args)
//typedef enum { OFF, IDLE, STANDBY, SLEEP, CPU_STANDBY } epd_state_t;

#define FBIO_EPD_FB_FLUSH _IOW('@', 5, struct fb_area )
#define FBIO_EPD_FB_FLUSH_ENABLE _IOW('@', 6, unsigned long)
#define FBIO_EPD_FB_BACKLIGHT _IOW('@', 7, unsigned long)
#define FBIO_EPD_FB_LCDIF _IOW('@', 8, unsigned long)
//#define FBIO_EPD_FB_WFM_UPDATE _IOWR('@', 8, unsigned long)
#define FBIO_EPD_FB_BACKLIGHT_MODE _IOW('@', 9, unsigned long)
#define FBIO_EPD_FB_DRAW _IOW('@', 10,  struct fb_area )
#define FBIO_EPD_FB_SET_DEPTH _IOW('@', 12, unsigned long )
#define FBIO_EPD_FB_LD_IMG_WITHOUTPACKED _IOW('@', 13, unsigned long)
#define FBIO_EPD_FB_ASYNC_UPDATE _IOW('@', 14, unsigned long)
#define FBIO_EPD_FB_AUTOWAVE_ENABLE  _IOW('@', 78, unsigned long)
#define FBIO_EPD_FB_AUTOWAVE_DISABLE  _IOW('@',87, unsigned long)
//c Expired #define FBIO_EPD_FB_SYNC_UPDATE _IOW('@', 41, unsigned long)
#define FBIO_EPD_FB_LD_IMG_AREA_DATA _IOW('@', 15, unsigned long)
#define FBIO_EPD_FB_UPD_PART_AREA _IOW('@', 16, unsigned long )
#define FBIO_EPD_FB_CLEAR_SCREEN _IOW('@', 11, unsigned long )
#define FBIO_EPD_FB_BUZZER _IOW('@', 17, unsigned long )
#define FBIO_EPD_FB_PM _IOW('@', 18, unsigned long)
#define FBIO_EPD_FB_UPD_FULL_AREA _IOW('@', 19, unsigned long )
#define FBIO_EPD_FB_DEBUG _IOW('@', 20, unsigned long )
#define FBIO_EPD_FB_SLEEP _IOW('@', 30, unsigned long )

#define FBIO_EPD_OVERLAP_CHECK_ENABLE _IOW('@', 76, unsigned long)
#define FBIO_EPD_OVERLAP_CHECK_DISABLE _IOW('@', 67, unsigned long)

//Kather. for queued-update
#define FBIO_EPD_LD_UPD_ENQUEUE _IOW('@',89,unsigned long)
//Choi. for Load/Update One-Go 
#define FBIO_EPD_LD_UPD_ONE_GO _IOW('@',90,unsigned long)

//Choi. SPI Flash Interface
#define FBIO_EPD_SPI_WRITE_INSTRUCTION _IOW('@', 50, unsigned long )
#define FBIO_EPD_SPI_WRITE_WAVEFORM _IOW('@', 51, unsigned long ) //for 8Gray Waveform
#define FBIO_EPD_SPI_WRITE_WAVEFORM_WE _IOW('@', 52, unsigned long ) //for 16Gray Waveform

#define FBIO_EPD_SPI_READ_INSTRUCTION _IOW('@', 57, unsigned long )
#define FBIO_EPD_SPI_READ_WAVEFORM _IOW('@', 58, unsigned long ) //for 8Gray Waveform
#define FBIO_EPD_SPI_READ_WAVEFORM_WE _IOW('@', 59, unsigned long ) //for 16Gray Waveform


//Choi. For Enhancement
//#define FBIO_EPD_FULL_UPDATE _IOW('@', 32, unsigned long )


//jw.choi 2009.10.16 #endif

}

#endif


