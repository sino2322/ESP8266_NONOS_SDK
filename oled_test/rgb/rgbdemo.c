#include "rgb.h"
#include "rgbdemo.h"
#include "osapi.h"
#include "debug.h"

os_timer_t timer0;
char zt = 1; 
extern color CurRGB;
color color1;

color* ICACHE_FLASH_ATTR
RGBDemo_ColorStructure(u8 red,u8 gre,u8 blu)
{
    color1.R=red;
    color1.G=gre;
    color1.B=blu;
    return &color1;
}

/******************************************************************************
 * FunctionName : timer0_callback
 * Description  : auto change aim color
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
timer0_callback(){  
    color c1;
    if(zt == 1)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(255,0,0),60);
		INFO("Red\r\n");
        zt = 2; 
		return; 
    }
    if(zt == 2)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(255,225,0),60);
		INFO("RED&GREEN\r\n");
        zt = 3; 
		return; 
    }
	if(zt == 3)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(0,255,0),60);
		INFO("GREEN\r\n");
        zt = 4;  
		return;
    }  
    if(zt == 4)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(0,255,255),60);
		INFO("GREEN&BLUE\r\n");
        zt = 5; 
		return; 
    }
	if(zt == 5)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(0,0,255),60);
		INFO("BLUE\r\n");
        zt = 6;  
		return;
    } 
	if(zt == 6)
    {  
		FluentColor(&CurRGB,RGBDemo_ColorStructure(255,0,255),60);
		INFO("BLUE&RED\r\n");
        zt = 1;  
		return;
    } 
}  

/******************************************************************************
 * FunctionName : RGB_Demo
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_Demo()
{
    os_timer_disarm(&timer0);
	os_timer_setfn(&timer0,(os_timer_func_t *)timer0_callback,NULL);
	os_timer_arm(&timer0,3000,1);
	INFO("\r\nTIMER SET READY\r\n");
}