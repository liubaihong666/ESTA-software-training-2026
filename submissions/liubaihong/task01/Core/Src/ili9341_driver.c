/**
  ******************************************************************************
  * @file    LCD.c
  * @brief   ILI9341 SPI DRIVERS FOR TM4C
  * @author  Lihaozhe from ICEC-SAST of HEU
  * @version V2.0
  * @date    2015-7-20
  ******************************************************************************
  */
	
#include "ili9341_driver.h"
#include "font.h"



uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色	  

#ifndef SPI_WRITEBy_posTE
#define SPI_WRITEBy_posTE(byte)\
{\
if(byte & 0x80) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x40) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x20) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x10) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x08) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x04) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x02) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
if(byte & 0x01) ILI9341_SDI_H else ILI9341_SDI_L	\
ILI9341_SCK_L ILI9341_SCK_H \
}
#endif



static void delayms(unsigned int nms)///延时函数
{
  HAL_Delay(nms);
}

void ILI9341_wr_data8(char da) //发送数据-8位参数///一次只能传送8位数据
{
    ILI9341_CS_L;
    ILI9341_RS_H;
	SPI_WRITEBy_posTE(da);
    ILI9341_CS_H;
}  

 void ILI9341_wr_data(int da)
{
    ILI9341_CS_L;
    ILI9341_RS_H;
	SPI_WRITEBy_posTE(da>>8);
	SPI_WRITEBy_posTE(da & 0XFF);
    ILI9341_CS_H;
}	  

void ILI9341_wr_reg(char da)	 
{
    ILI9341_CS_L;
    ILI9341_RS_L;
	SPI_WRITEBy_posTE(da);
    ILI9341_CS_H;
}
 void ILI9341_wr_reg_data(int reg,int da)
{
    ILI9341_CS_L;
    ILI9341_wr_reg(reg);
    ILI9341_wr_data(da);
    ILI9341_CS_H;
} 

void ILI9341_address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)//2016.14.14 地址设置函数为非人为设定函数
{  
   ILI9341_wr_reg(0x2a);///设置列地址
   ILI9341_wr_data8(x1>>8);
   ILI9341_wr_data8(x1);
   ILI9341_wr_data8(x2>>8);
   ILI9341_wr_data8(x2);
  
   ILI9341_wr_reg(0x2b);///设置页地址
   ILI9341_wr_data8(y1>>8);
   ILI9341_wr_data8(y1);
   ILI9341_wr_data8(y2>>8);
   ILI9341_wr_data8(y2);

   ILI9341_wr_reg(0x2c);///储存器写操作
}

void ILI9341_init(uint16_t Color)///初始化操作
{
	ILI9341_RST_L;///RST引脚写低电平
	delayms(100);
	ILI9341_RST_H;
	delayms(50);///复位后延时用于稳定状态

	ILI9341_LED_H;

	ILI9341_wr_reg(0xCB);  ///功耗控制A （5个参数）
  ILI9341_wr_data8(0x39); 
  ILI9341_wr_data8(0x2C); 
  ILI9341_wr_data8(0x00); ///前三个参数为默认参数
  ILI9341_wr_data8(0x34); ///内核电压控制 （1.6V）
  ILI9341_wr_data8(0x02); ///DDVDH控制 （5.6V）

  ILI9341_wr_reg(0xCF);  ///功耗控制B  （3个参数）
  ILI9341_wr_data8(0x00); ///默认
  ILI9341_wr_data8(0XC1); ///
  ILI9341_wr_data8(0X30); ///
 
  ILI9341_wr_reg(0xE8);  ///驱动时序控制A （3个参数）
  ILI9341_wr_data8(0x85); 
  ILI9341_wr_data8(0x00); 
  ILI9341_wr_data8(0x78); 
 
  ILI9341_wr_reg(0xEA);  ///驱动时序控制B （2个参数）
  ILI9341_wr_data8(0x00); 
  ILI9341_wr_data8(0x00); 
 
  ILI9341_wr_reg(0xED);  ///电源序列控制 （4个参数）
	ILI9341_wr_data8(0x64); 
  ILI9341_wr_data8(0x03); 
  ILI9341_wr_data8(0X12); 
  ILI9341_wr_data8(0X81); 

  ILI9341_wr_reg(0xF7);  ///汞比控制
	ILI9341_wr_data8(0x20); 
  
  ILI9341_wr_reg(0xC0);    //Power control ///功耗控制
	ILI9341_wr_data8(0x23);   //VRH[5:0] ///设置GVDD电平，设置为4.6V
 
	ILI9341_wr_reg(0xC1);    //Power control ///设置用于升压电路的因子
	ILI9341_wr_data8(0x10);   //SAP[2:0];BT[3:0] 
 
	ILI9341_wr_reg(0xC5);    //VCM control ///设置VCOM电压
	ILI9341_wr_data8(0x3e); //对比度调节
	ILI9341_wr_data8(0x28); ///3.7V
 
	ILI9341_wr_reg(0xC7);    //VCM control2 
	ILI9341_wr_data8(0x86);  //--
 
	ILI9341_wr_reg(0x36);    // Memory Access Control 
#ifdef USE_HORIZONTAL
    ILI9341_wr_data8(0xE8); //C8	   //48 68竖屏//28 E8 横屏
#else
		ILI9341_wr_data8(0x48); 
#endif

	ILI9341_wr_reg(0x3A);    ///像素格式设置
	ILI9341_wr_data8(0x55); 

	ILI9341_wr_reg(0xB1);    ///帧速率控制 (两个参数)
	ILI9341_wr_data8(0x00);  ///内部时钟分频
	ILI9341_wr_data8(0x18);  ///设置用于一行的时间 (31clocks)
 
	ILI9341_wr_reg(0xB6);    // Display Function Control ///显示功能控制 （4个参数）
	ILI9341_wr_data8(0x08);  ///
	ILI9341_wr_data8(0x82);
	ILI9341_wr_data8(0x27);  
 
	ILI9341_wr_reg(0xF2);    // 3Gamma Function Disable 
	ILI9341_wr_data8(0x00); 
 
	ILI9341_wr_reg(0x26);    //Gamma curve selected 
	ILI9341_wr_data8(0x01); 
 
	ILI9341_wr_reg(0xE0);    //Set Gamma 
	ILI9341_wr_data8(0x0F); 
	ILI9341_wr_data8(0x31); 
	ILI9341_wr_data8(0x2B); 
	ILI9341_wr_data8(0x0C); 
	ILI9341_wr_data8(0x0E); 
	ILI9341_wr_data8(0x08); 
	ILI9341_wr_data8(0x4E); 
	ILI9341_wr_data8(0xF1); 
	ILI9341_wr_data8(0x37); 
	ILI9341_wr_data8(0x07); 
	ILI9341_wr_data8(0x10); 
	ILI9341_wr_data8(0x03); 
	ILI9341_wr_data8(0x0E); 
	ILI9341_wr_data8(0x09); 
	ILI9341_wr_data8(0x00); 

	ILI9341_wr_reg(0XE1);    //Set Gamma 
	ILI9341_wr_data8(0x00); 
	ILI9341_wr_data8(0x0E); 
	ILI9341_wr_data8(0x14); 
	ILI9341_wr_data8(0x03); 
	ILI9341_wr_data8(0x11); 
	ILI9341_wr_data8(0x07); 
	ILI9341_wr_data8(0x31); 
	ILI9341_wr_data8(0xC1); 
	ILI9341_wr_data8(0x48); 
	ILI9341_wr_data8(0x08); 
	ILI9341_wr_data8(0x0F); 
	ILI9341_wr_data8(0x0C); 
	ILI9341_wr_data8(0x31); 
	ILI9341_wr_data8(0x36); 
	ILI9341_wr_data8(0x0F); 

	ILI9341_wr_reg(0x11);    //Exit Sleep ///退出睡眠模式，需要延时120ms
	delayms(200);  // 手册上面写的是120ms，在这里延时长一点
				
	ILI9341_wr_reg(0x29);    //Display on ///开显示
	ILI9341_wr_reg(0x2c);    ///储存器写
	
	BACK_COLOR=Color;
	ILI9341_clear(Color);
	BACK_COLOR=Color;
}


/*---------------------------------------------------------------
函数功能：清屏
Color: 要清屏的填充色
---------------------------------------------------------------*/
void ILI9341_clear(uint16_t Color)///从0,0 开始不断地依次给每位写颜色
{
	uint8_t VH,VL;
	uint16_t i,j;
	VH=Color>>8;
	VL=Color;	
	
	ILI9341_address_set(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
    for(i=0;i<X_MAX_PIXEL;i++)
	 {
	   for (j=0;j<Y_MAX_PIXEL;j++)
	   {
        ILI9341_wr_data8(VH);
				ILI9341_wr_data8(VL);	

	     }
	  }
}
/*---------------------------------------------------------------
函数功能：在指定位置显示一个汉字(32*33大小)
dcolor: 内容颜色
gbcolor: 背静颜色
---------------------------------------------------------------*/
void ILI9341_draw_hanzi(unsigned int x,unsigned int y,unsigned char *displayStr,unsigned char index,uint16_t COLOR)	//x,y 30个单位
{ 
	unsigned char i,j;
	unsigned char *temp=displayStr;    
	
  ILI9341_address_set(x,y,x+31,y+31); //设置区域      
	temp+=index*128;	
	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				ILI9341_wr_data(COLOR);
			} 
			else
			{
				ILI9341_wr_data(BACK_COLOR);
			}   
		}
		temp++;
	 }
}

//变量说明：displayStr 欲显示的字符串
//          sizeOfStr  此字符串长度
//Rewrited by TLewis

void ILI9341_draw_hanzi_string(uint16_t x,uint16_t y,unsigned char *displayStr,uint16_t sizeOfStr,uint16_t COLOR)
{         
    int i=0;

		for(i=0;i<sizeOfStr;i++){
			if(x>X_MAX_PIXEL-16){
				x=0;y+=16;
			}
			if(y>Y_MAX_PIXEL){
				y=x=0;
			}
			
			ILI9341_draw_hanzi(x,y,displayStr,i,COLOR);
			x+=32;
		}
	
}
/*---------------------------------------------------------------
函数功能：画点
POINT_COLOR: 此点的颜色
---------------------------------------------------------------*/
void ILI9341_draw_point(uint16_t x,uint16_t y,uint16_t COLOR)
{
	ILI9341_address_set(x,y,x,y);//设置光标位置 
	ILI9341_wr_data(COLOR); 	    
} 	 
/*---------------------------------------------------------------
函数功能：画一个大点
POINT_COLOR: 此点的颜色
---------------------------------------------------------------*/
void ILI9341_draw_point_big(uint16_t x,uint16_t y,uint16_t COLOR)
{
	ILI9341_fill(x-1,y-1,x+1,y+1,COLOR);
} 
/*---------------------------------------------------------------
函数功能：在指定区域内填充指定颜色
区域大小:
	(xend-xsta)*(yend-ysta)
color: 使用的颜色
---------------------------------------------------------------*/
void ILI9341_fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	
	ILI9341_address_set(xsta,ysta,xend,yend);      //设置光标位置 
	
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
		ILI9341_wr_data(color);//设置光标位置
	}
}  
/*---------------------------------------------------------------
函数功能：画线
x1,y1: 起点坐标
x2,y2: 终点坐标  
---------------------------------------------------------------*/
void ILI9341_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t COLOR)//
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	
	for(t=0;t<=distance+1;t++)//画线输出 
	{
		ILI9341_draw_point(uRow,uCol,COLOR);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ;
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 
}    
/*---------------------------------------------------------------
函数功能：画矩形
x1,y1: 起始点 (x范围0到500)
x2.y2: 终止点 (y范围0到500)
---------------------------------------------------------------*/
void ILI9341_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t COLOR)
{
	ILI9341_draw_line(x1,y1,x2,y1,COLOR);
	ILI9341_draw_line(x1,y1,x1,y2,COLOR);
	ILI9341_draw_line(x1,y2,x2,y2,COLOR);
	ILI9341_draw_line(x2,y1,x2,y2,COLOR);
}
/*---------------------------------------------------------------
函数功能：在指定位置画一个指定大小的圆
(x0,y0): 中心点
r    : 半径
COLOR: 颜色
---------------------------------------------------------------*/
void ILI9341_draw_circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t COLOR)
{
	int a,b;
	int di;
	a=0;b=r;	  
	
	di=3-(r<<1);             //判断下个点位置的标志
	
	while(a<=b)
	{
		ILI9341_draw_point(x0-b,y0-a,COLOR);             //3           
		ILI9341_draw_point(x0+b,y0-a,COLOR);             //0           
		ILI9341_draw_point(x0-a,y0+b,COLOR);             //1       
		ILI9341_draw_point(x0-b,y0-a,COLOR);             //7           
		ILI9341_draw_point(x0-a,y0-b,COLOR);             //2             
		ILI9341_draw_point(x0+b,y0+a,COLOR);             //4               
		ILI9341_draw_point(x0+a,y0-b,COLOR);             //5
		ILI9341_draw_point(x0+a,y0+b,COLOR);             //6 
		ILI9341_draw_point(x0-b,y0+a,COLOR);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		ILI9341_draw_point(x0+a,y0+b,COLOR);
	}
} 
/*---------------------------------------------------------------
函数功能：在指定位置显示一个字符，行间隔16，列间隔9，最大14行，最大34列
x: 行
y: 列
num: 要显示的字符:" "--->"~"
mode: 叠加方式(1)还是非叠加方式(0)
---------------------------------------------------------------*/
void ILI9341_draw_char(uint16_t x,uint16_t y,char num,uint8_t mode,uint16_t COLOR)//x为行 隔8 y为列 隔16   字符范围32到127  数字范围48到58 字母65到90 97到122
{
  uint8_t temp;
  uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=COLOR;      
	
  if(x>X_MAX_PIXEL-16||y>Y_MAX_PIXEL-16)return;	    ///如果不足以写，则结束函数
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	ILI9341_address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	///非叠加方式与叠加方式的区别
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		      if(temp&0x01){POINT_COLOR=colortemp;}
					else POINT_COLOR=BACK_COLOR;
					ILI9341_wr_data(POINT_COLOR);	
					temp>>=1; 
					x++;
		    }
			x=x0;
			y++;
		}	
	}
	else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		  temp=asc2_1608[(uint16_t)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		  {   
		     if(temp&0x01)ILI9341_draw_point(x+t,y+pos,POINT_COLOR);//画一个点     
		     temp>>=1; 
		  }
		}
	}
POINT_COLOR=colortemp;	    	   	 	  
}   

//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
/*---------------------------------------------------------------
函数功能：显示2个数字
x,y : 起点坐标	 
num: 数值(0~4294967295)
len : 数字的位数
color: 颜色
---------------------------------------------------------------*/
void ILI9341_draw_num(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t COLOR)//x左右 隔10 y上下 隔15
{         	
	uint8_t t,temp;
	uint8_t endraw=0;
	num=(uint32_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(endraw==0&&t<(len-1))
		{
			if(temp==0)
			{
				ILI9341_draw_char(x+8*t,y,' ',1,COLOR);
				continue;
			}else endraw=1; 
		 	 
		}
	 	ILI9341_draw_char(x+8*t,y,temp+48,1,COLOR); 
	}
} 
/*---------------------------------------------------------------
函数功能：显示实数
x,y: 坐标
f_num: 输入实数
hp_len: 整数部分位数
ap_len: 小数部分位数
COLOR: 颜色
---------------------------------------------------------------*/

void ILI9341_draw_real(uint16_t x,uint16_t y,double f_num,uint_16 bp_len,uint_16 ap_len,uint16_t COLOR)///这是自己写的实数形显示
{
	uint_32 i_num,num;//i用于转化后求小数点前数字个数，num用于扩大倍术后的数
	int t,temp,len,dis;
	
	if(f_num < 0){
		ILI9341_draw_char(x,y,'-',0,COLOR);
		x+=8;
	}
	
	i_num=(uint_32)f_num;
	
	if(bp_len>0)//判断小数点前有几个数
	{
		for(;bp_len>0;--bp_len)
		{
			if((i_num/mypow(10,bp_len-1))!=0)
				break;
		}
	}
	
	len=bp_len+ap_len;//得出有效数字的长度
	num=(uint_32)(f_num*mypow(10,ap_len));///放大为整型
	
	for(t=0,dis=0;t<=len;t++,dis++)		
	{
		if(t==bp_len)
		{
			if(t==0)
			{
				ILI9341_draw_char(x+8*dis,y,48,0,COLOR);///如果数值<0,则写0
				//t++;
				dis++;
			}
			ILI9341_draw_char(x+8*dis,y,46,0,COLOR);///小数点
			dis++;
		}
		temp=(num/mypow(10,len-t-1))%10;
		ILI9341_draw_char(x+8*dis,y,temp+48,0,COLOR);
	}
}

//显示2个数字
//x,y:起点坐标
//num:数值(0~99);	 
void ILI9341_draw2_Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t COLOR)
{         	
	uint8_t t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	ILI9341_draw_char(x+8*t,y,temp+'0',0,COLOR); 
	}
} 
/*---------------------------------------------------------------
函数功能：显示字符串 
x,y: 起点坐标  
*p: 字符串起始地址
COLOR: 颜色
---------------------------------------------------------------*/
void ILI9341_draw_string(uint16_t x,uint16_t y,const char *p,uint16_t COLOR)
{         
    while(*p!='\0')
    {       
        if(x>X_MAX_PIXEL-16){x=0;y+=16;}
        if(y>Y_MAX_PIXEL-16){y=x=0;}
        ILI9341_draw_char(x,y,*p,0, COLOR);
        x+=8;
        p++;
    }  
}

//控制光标移动 address为自然数
void ili9341_cursor_pos(char address)///???为什么只有一个地址
{
	static uint8_t x,y;
	POINT_COLOR=WHITE; 
	ILI9341_draw_point_big(x,y,POINT_COLOR);
	x=address%2;
	x=x*160;
	x+=4;
	y=address/2;
	y+=1;
	y=y*16;
	y+=4;
	POINT_COLOR=RED; 
	ILI9341_draw_point_big(x,y,POINT_COLOR);
	POINT_COLOR=WHITE; 
}
float CG(float i,int n)///实型数据的pow函数
{
   int t;
	float sum;
	for(t=0,sum=1;t<=n;t++)
	sum*=i;
	return sum;
}
int JC(int n)///阶乘
{
	int t,sum;
	for(t=1,sum=1;t<n;t++)
	sum*=t;
	return t;
}
void SJHS(float *sin,float *cos,float i)///???
{
   int n=0,p=1;
	for(;n<=23;n++)
	{
	*sin+= p*CG(i,2*n-1)/JC(2*n-1);
	*cos += (-p)*CG(i,2*n-2)/JC(2*n-2);
	}
}
///ILI9341_wr_data8();
void ili9341_display_some(uint16_t SR,uint16_t ER)///显示局部区域
{
	ILI9341_wr_reg(0x30);
	ILI9341_wr_data(SR);
	ILI9341_wr_data(ER);
	ILI9341_wr_reg(0x12);
}

void ili9341_display_all()///显示全部区域
{
	ILI9341_wr_reg(0x13);
}

void ili9341_VSCRDEF(uint16_t TFA,uint16_t VSA,uint16_t BFA,uint16_t VSP)///垂直滚动
{
	ILI9341_wr_reg(0x13);///垂直滚动定义
	ILI9341_wr_data(TFA);
	ILI9341_wr_data(VSA);
	ILI9341_wr_data(BFA);

	ILI9341_wr_reg(0x37);///垂直滚动开始地址
	ILI9341_wr_data(VSP);
}

float ili9341_Sc_Pr(uint16_t x,uint16_t y,uint8_t dis,uint16_t COLOR)//可支持正负式浮点数数据
{
	int output;
	uint8_t point=0,symbol=2,count=0,bp_len=0,ap_len=0;
	uint16_t input = 0, bp_f=0, ap_f=0;
	
	for(;;x+=dis,count++)
	{	
		if(input==13)
		{
			output=bp_f+ap_f*1.0/mypow(10,ap_len);//得到带正负的数据
			break;
		}
		if((input<'0'||input>'9')&&count!=0)	continue;
		ILI9341_draw_char(x,y,input,1,COLOR);
		
		if(count==0&&input=='+')//当输入正号时
			symbol=1;
		if(count==0&&input=='-')//当输入负号时
			symbol=0;
		
		if(input=='.')
			point=1;
			
		if('0'<=input && input<='9')
		{
			if(point==0)//小数点前数据
			{
				if(bp_len>0)
				{
					bp_f=bp_f*10+input-0x30;
				}
				if(count==0)//无输入正负号，默认为+
				{
					symbol=1;
					bp_f+=input-0x30;
					bp_len++;
				}
				if(count==1&&bp_len==0)//即有输入正负号的情况下
				{
					bp_f=input-0x30;
					bp_len++;
				}
			}
			else//小数点后的数据
			{
				ap_f=ap_f*mypow(10,ap_len)+input-0x30;
				ap_len++;
			}
		}
}

	
	if(symbol==1);
	else
		output=0-output;
	
	//ili9341_draw_float(100,100,output,5,5);
	ILI9341_draw_num(100,100,output,5,COLOR);
	return(output);
}

/*void ili9341_draw_32num(uint_16 x,uint_16 y,uint32_t num,uint_8 len);//显示32位数据
{
	

}*/





char* get_num2str(double input,char *output,int set)
{
//	double com_num=1,incopy=input;
	int count=16,mod=0,NG_mark=1;
	char *f=output;
	if(input<0)
	{
		NG_mark=-1;
		*f='-';
		f++;
		*f='\0';
	}
	for(;count>(set>=0?set:set-1);)
	{
		switch(mod)
		{
			case 0:
			{
				if(count==0)
				{
					*f=(int)(NG_mark*input/pow(10,count))-(int)(NG_mark*input/pow(10,count+1))*10+48;
					f++;
					mod=2;
					count--;
					break;
				}
				if(pow(10,count)<=NG_mark*input)
				{
					mod=1;
					break;
				}
				count--;
				break;
			}
			case 1:
			{
				*f=(int)(NG_mark*input/pow(10,count))-(int)(NG_mark*input/pow(10,count+1))*10+48;
				if(*f>'9')
				{
					*(f-1)+=(*f-48)/10;
					if(*(f-1)>'9')
					{
						*(f-2)+=(*(f-1)-48)/10;
						*(f-1)=(*(f-1)-48)%10+48;
					}
					*f=(*f-48)%10+48;
				}
				else if(*f<'0')
				{
					*f=0;
				}
				if(count==0)
				{
					mod=2;
				}
				f++;
				count--;
				break;
			}
			case 2:
			{
				*f='.';
				mod=3;
				f++;
				break;
			}
			case 3:
			{
				*f=(int)(NG_mark*input/pow(10,count))-(int)(NG_mark*input/pow(10,count+1))*10+48;
				if(*f>'9')
				{
					*(f-1)+=(*f-48)/10;
					if(*(f-1)>'9')
					{
						*(f-2)+=(*(f-1)-48)/10;
						*(f-1)=(*(f-1)-48)%10+48;
					}
					*f=(*f-48)%10+48;
				}
				else if(*f<'0')
				{
					*f=0;
				}
				f++;
				count--;
				break;
			}
		}
	}
	*f='\0';
	return output;
}
/*******************Wunyje's part************************/
void ILI9341_clear_w1(uint16_t Color)
{
	uint8_t VH,VL;
	uint16_t i,j;
	VH=Color>>8;
	VL=Color;	
	BACK_COLOR=Color;
	
	ILI9341_address_set(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
    for(i=0;i<X_MAX_PIXEL;i++)
	 {
	  for (j=0;j<Y_MAX_PIXEL;j++)
	   	{
        ILI9341_wr_data8(VH);
				ILI9341_wr_data8(VL);	

	    }
	  }
}
void ILI9341_int_display(unsigned int x,unsigned int y,int a,uint16_t color)
{
	char buff[50];
	sprintf (buff,"%d",(unsigned int)a);
    			ILI9341_draw_string(x,y,buff,color);
}
void ILI9341_float_3_display(unsigned int x,unsigned int y,float a,uint16_t color)
{
	char buff[30]="";
	sprintf (buff,"%d.%03d",(unsigned int)a,(unsigned int)((a-(unsigned int)a)*1000));
    			ILI9341_draw_string(x,y,buff,color);
}
void ILI9341_float_5_display(unsigned int x,unsigned int y,float a,uint16_t color)
{
	char buff[50];
	sprintf (buff,"%d.%05d",(unsigned int)a,(unsigned int)((a-(unsigned int)a)*100000));
    			ILI9341_draw_string(x,y,buff,color);
}
void ILI9341_float_7_display(unsigned int x,unsigned int y,float a,uint16_t color)
{
	char buff[50];
	sprintf (buff,"%d.%07d",(unsigned int)a,(unsigned int)((a-(unsigned int)a)*10000000));
    			ILI9341_draw_string(x,y,buff,color);
}

void ILI9341_float_4_display(unsigned int x,unsigned int y,float a,uint16_t color)
{
	char buff[50];
	sprintf (buff,"%d.%04d",(unsigned int)a,(unsigned int)((a-(unsigned int)a)*10000));
    			ILI9341_draw_string(x,y,buff,color);
}
/*********************************************************/



