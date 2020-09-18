#include "lcd.h"
//#include "stdlib.h"
//#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:QDtech2008@gmail.com 
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ8λ����ģʽ
//8λģʽ���ߣ�8λģʽ9341ģ��Ӹ�8λ���ߣ���:
//Һ����LCD_D0��Ӧ��Ƭ��PB8
//Һ����LCD_D1��Ӧ��Ƭ��PB9
//Һ����LCD_D2��Ӧ��Ƭ��PB10
//Һ����LCD_D3��Ӧ��Ƭ��PB11
//Һ����LCD_D4��Ӧ��Ƭ��PB12
//Һ����LCD_D5��Ӧ��Ƭ��PB13
//Һ����LCD_D6��Ӧ��Ƭ��PB14
//Һ����LCD_D7��Ӧ��Ƭ��PB15
//=======================================Һ���������߽���==========================================//
//LCD_RST	��PB7	//��λ�ź�
//LCD_CS	��PB6	//Ƭѡ�ź�
//LCD_RS	��PB5	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PB4	//д�ź�
//LCD_RD	��PB3	//���ź�
//=========================================������������=========================================//
//��ģ�鴥�������������ⲿXPT2046��������оƬ�����Ա����򲻽��д������ԡ�
//��ʹ�ô�������ģ�鱾��������������ɲ�����
**************************************************************************************************/	
	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = RED,BACK_COLOR = BLACK;  
u16 DeviceCode;	 


//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(u8 data)
{ 
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_RS_CLR;
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(data<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((data<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;

	LCD_CS_SET;
	
#else//ʹ��16λ������������ģʽ
	LCD_RS_CLR;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
			
#endif	
}

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u16 data)
{
	

#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_RS_SET;
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(data<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((data<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;

	LCD_CS_SET;
	
#else//ʹ��16λ������������ģʽ
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
			
#endif

}
//******************************************************************
//��������  LCD_DrawPoint_16Bit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    8λ���������д��һ��16λ����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint_16Bit(u16 color)
{
#if LCD_USE8BIT_MODEL==1
	
	LCD_RD_SET;
	LCD_RS_SET;//д��ַ
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(color);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|(color&0xff00));                                        //�������	
	LCD_WR_CLR;
	LCD_WR_SET;	
	LCD_PBh8dataOut(color<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((color<<8)&0xff00));	
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
#else
	LCD_WR_DATA(color); 
#endif


}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
	 
//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
#if LCD_USE8BIT_MODEL==1
	
	LCD_RD_SET;
	LCD_RS_SET;//д��ַ  
	LCD_CS_CLR;
	
	LCD_PBh8dataOut(POINT_COLOR);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|(POINT_COLOR&0xff00));                                        //�������	
	LCD_WR_CLR;
	LCD_WR_SET;	
	
	LCD_PBh8dataOut(POINT_COLOR<<8);
	//DATAOUT(((GPIOB->IDR)&0x00ff)|((POINT_COLOR<<8)&0xff00));
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
#else
	LCD_WR_DATA(POINT_COLOR); 
#endif


}

//******************************************************************
//��������  LCD_Clear
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
#if LCD_USE8BIT_MODEL==1
	LCD_RS_SET;//д���� 
	LCD_CS_CLR;
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_PBh8dataOut(Color);
		//DATAOUT(((GPIOB->IDR)&0x00ff)|(Color&0xff00));                                        //�������
		LCD_WR_CLR;
		LCD_WR_SET;
		
		LCD_PBh8dataOut(Color<<8);
		//DATAOUT(((GPIOB->IDR)&0x00ff)|((Color<<8)&0xff00));
		LCD_WR_CLR;
		LCD_WR_SET;
		
	}
	LCD_CS_SET;
#else //16λģʽ
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);		  
	}
#endif
	
} 

//******************************************************************
//��������  LCD_GPIOInit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Һ����IO��ʼ����Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_GPIOInit(void)
{
	Devices_Init(UserDevices,LCD);
	LCD_CS_SET;LCD_RS_SET;LCD_WR_SET;LCD_RD_SET;LCD_RST_SET;
}

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_SET;
	delay_ms(1);
	LCD_RST_CLR;
	delay_ms(30);
	LCD_RST_SET;
	delay_ms(100);
}

//******************************************************************
//��������  LCD_Init   ILI9341
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30);
	
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81);
	
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10);//LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x7A); //LCD_WR_DATA(0x78);
	
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02);
	
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00);
	
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x23);//LCD_WR_DATA(0x1B);   VRH[5:0] 
	
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x10);//LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30);//LCD_WR_DATA(0x3E); 	 //3F
	LCD_WR_DATA(0x30);//LCD_WR_DATA(0x28); 	 //3C
	
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); //LCD_WR_DATA(0x86);
	
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48);//
	
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A);//LCD_WR_DATA(0x18);
	
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A);//LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0xA2);//LCD_WR_DATA(0x82);
	LCD_WR_DATA(0x27);
	
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	delay_ms(120);
	LCD_WR_REG(0x29); //display on	
	
	//delay_ms(500);
	LCD_SetParam();//����LCD����	 
	//LCD_LED=1;//��������	 
	//LCD_Clear(WHITE);
}
  		  
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM	
} 

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;			 
	LCD_WriteReg(0x36,0x28);//    0x6C

#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;	
	LCD_WriteReg(0x36,0x88); // 0xC9    //���ﲻ���޸�
#endif
}	

