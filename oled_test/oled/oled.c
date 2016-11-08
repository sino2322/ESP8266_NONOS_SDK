	#include "oled.h"
#include "oledfont.h"
#include "oledbmp.h"
/*
OLED議�坿�
贋慧鯉塀泌和
喇噐駁強泡仟庁塀議�渣藤�y row瓜協吶撹8倖�駛愆麸［�8議page��耽肝泡仟page坪議侭嗤�駛�
page吉揖噐y恫炎
|page| x row
|[0] |	0 1 2 3 ... 127	
|[1] |	0 1 2 3 ... 127	
|[2] |	0 1 2 3 ... 127	
|[3] |	0 1 2 3 ... 127	
|[4] |	0 1 2 3 ... 127	
|[5] |	0 1 2 3 ... 127	
|[6] |	0 1 2 3 ... 127	
|[7] |	0 1 2 3 ... 127 	
*/

/******************************************************************************
 * FunctionName : IIC_Start
 * Description  : I2C start signal
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
IIC_Start()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/******************************************************************************
 * FunctionName : IIC_Stop
 * Description  : I2C stop signal
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
IIC_Stop()
{
    OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

/******************************************************************************
 * FunctionName : IIC_Wait_Ack
 * Description  : IIC_Wait_Ack
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}

/******************************************************************************
 * FunctionName : Write_IIC_Byte
 * Description  : write a byte over i2c pin
 * Parameters   : u8 IIC_Byte		what will be writen
 * Returns      : none
 *******************************************************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	u8 i;
	u8 m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
		m=da;
		m=m&0x80;
		if(m==0x80)
		{
            OLED_SDIN_Set();
        }
		else
		{
            OLED_SDIN_Clr();
        }
		da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}

/******************************************************************************
 * FunctionName : Write_IIC_Command
 * Description  : write ssd1306 comand through i2c
 * Parameters   : u8 IIC_Command			command will be writen
 * Returns      : none
 *******************************************************************************/
void Write_IIC_Command(u8 IIC_Command)
{
    IIC_Start();
    Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
    Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
    Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
    IIC_Stop();
}

/******************************************************************************
 * FunctionName : Write_IIC_Data
 * Description  : write ssd1306 data through i2c
 * Parameters   : u8 IIC_Data		data will be writen
 * Returns      : none
 *******************************************************************************/
void Write_IIC_Data(u8 IIC_Data)
{
    IIC_Start();
    Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
    Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
    Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
    IIC_Stop();
}

/******************************************************************************
 * FunctionName : OLED_WR_Byte
 * Description  : 
 * Parameters   : unsigned dat
				  unsigned cmd			cmd=0	write command to i2c
				  						cmd=1	write data to i2c
 * Returns      : none
 *******************************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
        Write_IIC_Data(dat);
    }
	else
	{
        Write_IIC_Command(dat);	
	}
}

/******************************************************************************
 * FunctionName : OLED_Set_Pos
 * Description  : set position
 * Parameters   : u8 x		x row
 				  u8 y		y row
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_Set_Pos(u8 x, u8 y) 
{ 	
    OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}

/******************************************************************************
 * FunctionName : OLED_Display_On
 * Description  : enable oled disp
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC凋綜
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/******************************************************************************
 * FunctionName : OLED_Display_Off
 * Description  : disable oled disp
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC凋綜
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

/******************************************************************************
 * FunctionName : OLED_Clear_Black
 * Description  : clean disp(black)
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/		   			   
void ICACHE_FLASH_ATTR
OLED_Clear_Black(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //譜崔匈仇峽��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //譜崔�塋称志叩�双詰仇峽
		OLED_WR_Byte (0x10,OLED_CMD);      //譜崔�塋称志叩�双互仇峽   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //厚仟�塋�
}

/******************************************************************************
 * FunctionName : OLED_Clear_White
 * Description  : clean disp(white)
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_Clear_White(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //譜崔匈仇峽��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //譜崔�塋称志叩�双詰仇峽
		OLED_WR_Byte (0x10,OLED_CMD);      //譜崔�塋称志叩�双互仇峽   
		for(n=0;n<128;n++)OLED_WR_Byte(0xff,OLED_DATA); 
	} //厚仟�塋�
}

/******************************************************************************
 * FunctionName : OLED_Clear_Black_In_Page
 * Description  : clean page(black)
 * Parameters   : u8 page
 * Returns      : none
 *******************************************************************************/		   			   
void ICACHE_FLASH_ATTR
OLED_Clear_Black_In_Page(u8 page)  
{
	u8 n;
	OLED_WR_Byte (0xb0+page,OLED_CMD);    //譜崔匈仇峽��0~7��
	OLED_WR_Byte (0x00,OLED_CMD);      //譜崔�塋称志叩�双詰仇峽
	OLED_WR_Byte (0x10,OLED_CMD);      //譜崔�塋称志叩�双互仇峽   
	for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);  //厚仟�塋�
}

/******************************************************************************
 * FunctionName : OLED_Clear_White_In_Page
 * Description  : clean page(white)
 * Parameters   : u8 page
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_Clear_White_In_Page(u8 page)
{  
	u8 n;
	OLED_WR_Byte (0xb0+page,OLED_CMD);    //譜崔匈仇峽��0~7��
	OLED_WR_Byte (0x00,OLED_CMD);      //譜崔�塋称志叩�双詰仇峽
	OLED_WR_Byte (0x10,OLED_CMD);      //譜崔�塋称志叩�双互仇峽   
	for(n=0;n<128;n++)OLED_WR_Byte(0xff,OLED_DATA);  //厚仟�塋�
}

/******************************************************************************
 * FunctionName : OLED_ShowChar
 * Description  : 壓峺協了崔�塋紹燦�忖憲,淫凄何蛍忖憲
 * Parameters   : x	0~127
				  y	0~7 page
				  mode	0,郡易�塋�;1,屎械�塋�
				  size	僉夲忖悶 16/12 
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	u8 c=0,i=0;	
	c=chr-' ';//誼欺陶卞朔議峙			
	if(x>Max_Column-1)
	{
		x=0;
		if(Char_Size==16)
		{
			y+=2;//size=16算2 page
		}
		else
		{
			y+=1;//size=8算1 page
		}
	}
	if(Char_Size ==16)
	{
        OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else 
    {	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	}
}

/******************************************************************************
 * FunctionName : oled_pow
 * Description  : m^n
 * Parameters   : m 
 				  n
 * Returns      : result
 *******************************************************************************/
u32 ICACHE_FLASH_ATTR
oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/******************************************************************************
 * FunctionName : OLED_ShowNum
 * Description  : �塋�2倖方忖
 * Parameters   : x   :軟泣x恫炎
 				  y   :page	 
				  len :方忖議了方
				  size:忖悶寄弌
				  mode:庁塀	0,野割庁塀;1,京紗庁塀
				  num:方峙(0~4294967295);	 
 * Returns      : none
 *******************************************************************************/		  	  
void ICACHE_FLASH_ATTR
OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

/******************************************************************************
 * FunctionName : OLED_ShowString
 * Description  : �塋紹燦�忖憲催堪,潮範峪嗤曾倖忖悶寄弌8x16,6x8
 * Parameters   : u8 x		x row
 				  u8 y		page
				  u8 *chr	chr point to 忖憲堪
				  u8 Char_Size 忖悶寄弌
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	u8 j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		if(Char_Size==16)//default 8x16
		{
			x+=8;
			if(x>120)//階竃算佩
			{
				x=0;
				y+=2;      
			}
		}
		else	//default 6x8
		{
			x+=6;
			if(x>122)//階竃算佩
			{
				x=0;
				y+=1;      
			}
		}
		j++;
	}
}

/******************************************************************************
 * FunctionName : OLED_ShowCHinese
 * Description  : �塋昇祭�
 * Parameters   : s峺�鯔�哈
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_ShowChinese(u8 x,u8 y,u8 *s)
{      			    
	u8 t,k=0;
	for(k=0;k<hz16_num;k++)//壓沫哈垂戦朴沫
	{
		if((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))//登僅頁倦嗤宸倖忖
		{
			OLED_Set_Pos(x,y);	
    		for(t=0;t<16;t++)
			{
				OLED_WR_Byte(hz16[k].Msk[t],OLED_DATA);
			}	
			OLED_Set_Pos(x,y+1);	
    		for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(hz16[k].Msk[t+16],OLED_DATA);
    		}
		}
	}
}

/******************************************************************************
 * FunctionName : OLED_ShowChineseString
 * Description  : �塋昇祭�
 * Parameters   : s峺�鰆侘鍔峽�堪
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_ShowChineseString(u8 x,u8 y,u8 *s)
{    
	u8 i=0,j=0;  	
	while(s[j]!='\0')
	{
		OLED_ShowChinese(x,y,s+2*i);
		i++;
		j+=2;//匯倖嶄猟2倖char
		x+=16;
		if(x>112)//階竃何蛍徭強算佩�┫亡χ師覿�GB162��泌惚頁8x8個撹120軸辛��
		{
			x=0;
			y+=2;//匆頁寞斤GB162��2page匯佩
		}
	}
}


/******************************************************************************
 * FunctionName : OLED_DrawBMP
 * Description  : �塋章塋�BMP夕頭128〜64軟兵泣恫炎(x,y),x議袈律0゛127��y葎匈議袈律0゛7
 * Parameters   : u8 x0			start x
 				  u8 y0			start y
				  u8 x1			
				  u8 y1			
				  const u8 BMP[]		
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,const u8 BMP[])
{ 	
	u32 j=0;
	u8 x,y;
  
	if(y1%8==0)
		y=y1/8;      
	else
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    	for(x=x0;x<x1;x++)
		{      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

/******************************************************************************
 * FunctionName : OLED_Init
 * Description  : 兜兵晒SSD1306
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/					    
void ICACHE_FLASH_ATTR
OLED_Init(void)
{ 	
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  

/******************************************************************************
 * FunctionName : oled_gpio_init
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
//for applications
void ICACHE_FLASH_ATTR
oled_gpio_init(void)
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);//SCL
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO14);//SDA
	OLED_Init();			//兜兵晒OLED 
	OLED_Clear_Black(); 
}

/******************************************************************************
 * FunctionName : oled_demo_string
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
oled_demo_string(void)
{
	OLED_Clear_Black(); 
	OLED_ShowChineseString(0,0,"これが恷恷恷瘁のこれが恷恷恷瘁の");
	OLED_ShowString(0,4,"0.96' OLED TEST",16);
    OLED_ShowString(0,6,"0.96' OLED TEST0.96' OLED TEST0.96' OLED TEST0.96' OLED TEST",8);
}

/******************************************************************************
 * FunctionName : oled_demo_bmp1
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
oled_demo_bmp1(void)
{
	OLED_Clear_Black(); 
	OLED_DrawBMP(0,0,128,8,BMP3);
}

/******************************************************************************
 * FunctionName : oled_demo_bmp2
 * Description  : 
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void ICACHE_FLASH_ATTR
oled_demo_bmp2(void)
{
	OLED_Clear_Black(); 
	OLED_DrawBMP(0,0,128,8,BMP4);
}
