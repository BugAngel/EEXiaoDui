/**
  ******************************************************************************
  * @file    OLED.c
  * @author  WangRong
  * @version V1.0
  * @date    2017.4.1
  * @note    spi����OLED
  ******************************************************************************
  */
#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

uint8_t OLED_GRAM[128][8];

//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
        OLED_WR_Byte (0x00, OLED_CMD);     //������ʾλ�á��е͵�ַ
        OLED_WR_Byte (0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
        for (n = 0; n < 128; n++)OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
    uint8_t i;
    if (cmd == 0)
    {
        OLED_DC_0;
    }
    else
    {
        OLED_DC_1;
    }

//  OLED_CS=0;
    for (i = 0; i < 8; i++)
    {
        OLED_SCLK_0;
        if (dat & 0x80)OLED_SDIN_1;
        else OLED_SDIN_0;
        OLED_SCLK_1;
        dat <<= 1;
    }
//  OLED_CS=1;
//  OLED_RS=1;
}


//����OLED��ʾ
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)OLED_GRAM[n][i] = 0X00;
    OLED_Refresh_Gram();//������ʾ
}
//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t pos, bx, temp = 0;
    if (x > 127 || y > 63)return; //������Χ��.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if (t)OLED_GRAM[x][pos] |= temp;
    else OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
    uint8_t x, y;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)OLED_DrawPoint(x, y, dot);
    }
    OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 12/16/24
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size,
                   uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size /
                    2);      //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ
    for (t = 0; t < csize; t++)
    {
        if (size == 12)temp = asc2_1206[chr][t];        //����1206����
        else if (size == 16)temp = asc2_1608[chr][t];   //����1608����
        else if (size == 24)temp = asc2_2412[chr][t];   //����2412����
        else return;                                //û�е��ֿ�
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
//m^n����
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)result *= m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ 0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
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
  * @brief  ��ʾ��λС��
  * @param
      @arg x,y :�������
      @arg als ��λ������
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
//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ
void OLED_ShowString(uint8_t x, uint8_t y, const uint8_t *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
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
//��ʼ��SSD1306
void OLED_Init(void)
{

    OLED_SDIN_1;
    OLED_SCLK_1;

//  OLED_CS=1;
    OLED_DC_1;

    OLED_RST_0;
    HAL_Delay(100);
    OLED_RST_1;

    OLED_WR_Byte(0xAE, OLED_CMD); //�ر���ʾ
    OLED_WR_Byte(0xD5, OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_WR_Byte(80, OLED_CMD);  //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_WR_Byte(0xA8, OLED_CMD); //��������·��
    OLED_WR_Byte(0X3F, OLED_CMD); //Ĭ��0X3F(1/64)
    OLED_WR_Byte(0xD3, OLED_CMD); //������ʾƫ��
    OLED_WR_Byte(0X00, OLED_CMD); //Ĭ��Ϊ0
    OLED_WR_Byte(0x40, OLED_CMD); //������ʾ��ʼ�� [5:0],����.
    OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�����
    OLED_WR_Byte(0x14, OLED_CMD); //bit2������/�ر�
    OLED_WR_Byte(0x20, OLED_CMD); //�����ڴ��ַģʽ
    OLED_WR_Byte(0x02,
                 OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_WR_Byte(0xA1, OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,
                 OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    OLED_WR_Byte(0xDA, OLED_CMD); //����COMӲ����������
    OLED_WR_Byte(0x12, OLED_CMD); //[5:4]����
    OLED_WR_Byte(0x81, OLED_CMD); //�Աȶ�����
    OLED_WR_Byte(0xEF, OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    OLED_WR_Byte(0xD9, OLED_CMD); //����Ԥ�������
    OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB, OLED_CMD); //����VCOMH ��ѹ����
    OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_WR_Byte(0xA4, OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_WR_Byte(0xA6,
                 OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_WR_Byte(0xAF, OLED_CMD); //������ʾ
    OLED_Clear();
}