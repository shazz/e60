#ifndef TS_H
#define TS_H

typedef struct _pixel_node_t
{
   int x;
   int y;
   int pr;
   int eraser;
} pixel_node_t; 

typedef enum _TOUCH_TYPE
{
   eUNKNOWN = -1, //-1
   eHANWANG_5 = 0, //0
   eHANWANG_6, //1
   eHANWANG_9, //2
   eWACOM_5, //3
   eWACOM_6, //4
   eWACOM_6_NEW, //5
} TOUCH_TYPE;

#define CMD_HANWANG_DETECT_TSP 0x5F

#define TSP_SET_PEN_CORRECTION 0x01 //('@', 1, int nDirection)
#define TSP_GET_PEN_CORRECTION 0x02 //('@', 2, void)

#define TSP_SET_TOUCHSCREEN 0x03 //('@', 3, TOUCH_TYPE tsp_type)
#define TSP_DETECT_TOUCHSCREEN 0x04 //('@', 4, void)

#define TSP_BEGIN_PARSER 0x05 //('@', 5, void)
#define TSP_END_PARSER 0x06 //('@', 6, void)

#endif
