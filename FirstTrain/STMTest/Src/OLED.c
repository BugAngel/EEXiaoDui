/**
  ******************************************************************************
  * @file    OLED.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    spi驱动OLED
  ******************************************************************************
  */
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

uint8_t OLED_GRAM[128][8];



//更新显存到LCD
void OLED_Refresh_Gram(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //设置页地址（0~7）
        OLED_WR_Byte (0x00, OLED_CMD);     //设置显示位置—列低地址
        OLED_WR_Byte (0x10, OLED_CMD);     //设置显示位置—列高地址
        for (n = 0; n < 128; n++)OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}
#if OLED_MODE==0
void OLED_WriteChar(uint8_t dat)
{
	uint8_t i;

    for (i = 0; i < 8; i++)
    {
        OLED_SCLK=0;
        if (dat & 0x80)OLED_SDIN=1;
        else OLED_SDIN=0;
        OLED_SCLK=1;
        dat <<= 1;
    }
}
#else
void IIC_WriteChar(uint8_t byte)
{
	uint8_t i,temp;
	OLED_SCLK = 0;
	temp = byte;
	for(i = 0;i<8;i++)
	{
		OLED_SDIN = (temp&0x80)>>7;
		OLED_SCLK = 1;
		OLED_SCLK = 0;
		temp = temp<<1;	
	}
}
/*****************************************************
** name：IIC_Wait_Ack
** brief：IIC通讯中的应答信号，无需判断应答，但要走应答步骤
******************************************************/
void IIC_Wait_Ack()
{
	OLED_SCLK=1;
	OLED_SCLK=0;
}

/*****************************************************
** name：IIC_Start
** brief：IIC通讯中的开始信号
******************************************************/
void IIC_Start()
{

	OLED_SCLK=1;
	OLED_SDIN=1;
	OLED_SDIN=0;
	OLED_SCLK=0;
}

/*****************************************************
** name：IIC_Stop
** brief：IIC通讯中的结束信号
******************************************************/
void IIC_Stop()
{
	OLED_SCLK=1;
	OLED_SDIN=0;
	OLED_SDIN=1;
}

/*****************************************************
** name：IIC_WriteC
** brief：向OLED写入一个命令
******************************************************/
void IIC_WriteC(uint8_t cmd)
{
	IIC_Start();							//开始传输
	IIC_WriteChar(0x78);			//传输地址
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_WriteChar(0x00);			//发送控制字节
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_WriteChar(cmd);					//发送命令字节
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_Stop();							//结束传输
	
}

/*****************************************************
** name：IIC_WriteD
** brief：向OLED写入一个数据
** parameter：1.dat：需要写入的数据
******************************************************/
void IIC_WriteD(uint8_t dat)
{
	IIC_Start();							//开始传输
	IIC_WriteChar(0x78);			//传输地址
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_WriteChar(0x40);			//发送控制字节
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_WriteChar(dat);					//发送数据字节
	IIC_Wait_Ack();							//接收1306返回的确认信号
	IIC_Stop();							//结束传输
	
}

#endif
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
#if OLED_MODE == 0
    if (cmd == 0)
    {
        OLED_DC_0;
    }
    else
    {
        OLED_DC_1;
    }

//  OLED_CS=0;
	OLED_WriteChar(dat);
//  OLED_CS=1;
//  OLED_RS=1;
#else
			if(cmd == 0)
		{
			IIC_WriteC(dat);
		}
		else
		{
			IIC_WriteD(dat);
		}
#endif
}


//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)OLED_GRAM[n][i] = 0X00;
    OLED_Refresh_Gram();//更新显示
}
//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;
    if (x > 127 || y > 63)return; //超出范围了.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if (t)OLED_GRAM[x][pos] |= temp;
    else OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,清空;1,填充
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
    uint8_t x, y;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)OLED_DrawPoint(x, y, dot);
    }
    OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size,
                   uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size /
                    2);      //得到字体一个字符对应点阵集所占的字节数
    chr = chr - ' '; //得到偏移后的值
    for (t = 0; t < csize; t++)
    {
        if (size == 12)temp = asc2_1206[chr][t];        //调用1206字体
        else if (size == 16)temp = asc2_1608[chr][t];   //调用1608字体
        else if (size == 24)temp = asc2_2412[chr][t];   //调用2412字体
        else return;                                //没有的字库
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)OLED_DrawPoint(x, y, mode);
            else OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
//m^n函数
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)result *= m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式 0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size / 2)*t, y, ' ', size, 1);
                continue;
            }
            else enshow = 1;

        }
        OLED_ShowChar(x + (size / 2)*t, y, temp + '0', size, 1);
    }
}
/**
  * @brief  显示两位小数
  * @param
      @arg x,y :起点坐标
      @arg als 两位浮点数
  * @retval None
  */
void myOLED_Decimals(uint8_t x, uint8_t y, float als)
{
    uint16_t temp0 = 0, temp1 = 0, temp2 = 0 , temp3 = 0;
    uint16_t temp16 = 0;
    if (als > 0)
    {
        OLED_ShowChar(x, y, ' ', 12, 1);
    }
    else
    {
        OLED_ShowChar(x, y, '-', 12, 1);
        als = -als;
    }
    if (als > 100)
    {
        OLED_ShowNum(x + 6, y, 1, 1, 12);
        als = als - 100;
    }
    else
    {
        OLED_ShowChar(x + 6, y, ' ', 12, 1);
    }

    temp16 = (uint16_t)(als * 100);
    temp0 = (uint16_t)(temp16 / 1000);
    temp1 = (uint16_t)(temp16 % 1000 / 100);
    temp2 = (uint16_t)(temp16 % 100 / 10);
    temp3 = (uint16_t)(temp16 % 10 / 1);


    OLED_ShowNum(x + 12, y, temp0, 1, 12);
    OLED_ShowNum(x + 18, y, temp1, 1, 12);
    OLED_ShowChar(x + 24, y, '.', 12, 1);
    OLED_ShowNum(x + 30, y, temp2, 1, 12);
    OLED_ShowNum(x + 36, y, temp3, 1, 12);


}
//显示字符串
//x,y:起点坐标
//size:字体大小
//*p:字符串起始地址
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x > (128 - (size / 2)))
        {
            x = 0;
            y += size;
        }
        if (y > (64 - size))
        {
            y = x = 0;
            OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, size, 1);
        x += size / 2;
        p++;
    }

}

//初始化SSD1306
void OLED_Init(void)
{
#if OLED_MODE==0
	
    OLED_SDIN_1;
    OLED_SCLK_1;

//  OLED_CS=1;
    OLED_DC_1;

    OLED_RST_0;
    HAL_Delay(100);
    OLED_RST_1;

    OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示
    OLED_WR_Byte(0xD5, OLED_CMD); //设置时钟分频因子,震荡频率
    OLED_WR_Byte(80, OLED_CMD);  //[3:0],分频因子;[7:4],震荡频率
    OLED_WR_Byte(0xA8, OLED_CMD); //设置驱动路数
    OLED_WR_Byte(0X3F, OLED_CMD); //默认0X3F(1/64)
    OLED_WR_Byte(0xD3, OLED_CMD); //设置显示偏移
    OLED_WR_Byte(0X00, OLED_CMD); //默认为0
    OLED_WR_Byte(0x40, OLED_CMD); //设置显示开始行 [5:0],行数.
    OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵设置
    OLED_WR_Byte(0x14, OLED_CMD); //bit2，开启/关闭
    OLED_WR_Byte(0x20, OLED_CMD); //设置内存地址模式
    OLED_WR_Byte(0x02,
                 OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WR_Byte(0xA1, OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,
                 OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WR_Byte(0xDA, OLED_CMD); //设置COM硬件引脚配置
    OLED_WR_Byte(0x12, OLED_CMD); //[5:4]配置
    OLED_WR_Byte(0x81, OLED_CMD); //对比度设置
    OLED_WR_Byte(0xEF, OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WR_Byte(0xD9, OLED_CMD); //设置预充电周期
    OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB, OLED_CMD); //设置VCOMH 电压倍率
    OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WR_Byte(0xA4, OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WR_Byte(0xA6,
                 OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WR_Byte(0xAF, OLED_CMD); //开启显示
    OLED_Clear();
#else 

	/*关闭OLED显示*/
	OLED_WR_Byte(0xAE,OLED_CMD);//关显示命令
	
	/*设置正当频率、时钟分频因子*/
    OLED_WR_Byte(0xD5,OLED_CMD);//设置时钟分频因子、振荡频率
    OLED_WR_Byte(0x80,OLED_CMD);
		
	/*设置驱动路数*/
	OLED_WR_Byte(0xA8,OLED_CMD);			  //驱动路数设置命令
	OLED_WR_Byte(0x1F,OLED_CMD);							  //驱动路数值

	/*设置行显示偏移*/
	OLED_WR_Byte(0xD3,OLED_CMD);				  //显示偏移设置命令
	OLED_WR_Byte(0x00,OLED_CMD);							  //取值范围：0x00-->0x3F

	/*设置显示起始行*/
	OLED_WR_Byte(0x40,OLED_CMD);							  //取值范围：0x40---0x7F
	
	/*设置电荷泵*/
	OLED_WR_Byte(0x8D,OLED_CMD); 				  //电荷泵设置命令
	OLED_WR_Byte(0x14,OLED_CMD);							  //开启电荷泵

	/*设置内存地址模式*/
	OLED_WR_Byte(0x20,OLED_CMD);			  //内存地址模式设置命令
	OLED_WR_Byte(0x10,OLED_CMD);							  //设置地址模式值：0x00,列地址模式；0x01,行地址模式；0x10，页地址模式；
															  
	/*设置列地址映射*/										  
	OLED_WR_Byte(0xA1,OLED_CMD); 			  //列地址0映射到SEG127

	/*设置行地址映射*/
	OLED_WR_Byte(0xC8,OLED_CMD); 		  //行地址0映射到COM63 //扫描方向从COM[N-1]-->COM0
	
	/*设置COM硬件引脚配置*/									  
	OLED_WR_Byte(0xDA,OLED_CMD); 			  //设置COM硬件引脚配置命令
	OLED_WR_Byte(0x00,OLED_CMD); 

	/*对比度设置*/
	OLED_WR_Byte(0x81,OLED_CMD); 			   //对比度设置命令
	OLED_WR_Byte(0x8F,OLED_CMD); 							   //对比度值，取值范围1-255
	
	/*设置预充电周期*/
	OLED_WR_Byte(0xD9,OLED_CMD); 
	OLED_WR_Byte(0xF1,OLED_CMD); 
	
	/*设置VCOMH电源倍率*/
	OLED_WR_Byte(0xDB,OLED_CMD); 
	OLED_WR_Byte(0x30,OLED_CMD);

	/*关闭全局显示*/
    OLED_WR_Byte(0xA4,OLED_CMD); 

	/*设置OLED点亮方式  A6正常，A7背光*/
	OLED_WR_Byte(0xA6,OLED_CMD); 	 				//0：熄灭像素；1：点亮像素

	/*打开OLED显示*/
	OLED_WR_Byte(0xAF,OLED_CMD); 
	
	/*清屏操作*/
	OLED_Clear();
#endif
}
