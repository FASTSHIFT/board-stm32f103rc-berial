//=================================================================================
//���ڵ�can       4800bps
//
//sbit SCK=P2^3;
//sbit SI=P2^2;
//sbit SO=P2^1;
//sbit CS=P2^0;
//INT ��P3.3
//
//=================================================================================

#include "reg51.h"
#include "2515.h"
#include "SPI.h"
#include "Function.h"

#include "CAN0.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);


unsigned char gRXFlag=0,sRXFlag=0,yRXFlag=0;
unsigned char data Com_RecBuff[8];
unsigned char bdata flag_init;

//*************************
#define INBUF_LEN 8   //���ݳ���

unsigned char inbuf1[INBUF_LEN];
unsigned char checksum,count3=0;


unsigned char a[8];
unsigned char b[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};


/************************************************************************
*����ԭ��: void init_serialcomm(void)            *
*����˵��: ���ڳ�ʼ��                            *
*˵��: ��ֵ��Ƭ���Ķ�ʱ��1�ķ�ʽѡ������ �����ӳ���ֻ�����ڸ�λģʽ
************************************************************************/
void init_serialcomm(void)
{
  SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
  TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
  PCON |= 0x80;       //SMOD=1; 
  TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz  
  TL1   = 0xF4;       //
  IE   |= 0x90;       //Enable Serial Interrupt 
  TR1   = 1;          // timer 1 run
}

//��ʱ��0��ʼ��
void timer0initial()
{
  TMOD|=0x1;        //������ʽ16λ��ʱ������
  TH0=0xb8;TL0=0x00;  //50MS��ʱ��ֵ��T0��ʱ�ã�
  IE|=0x90;       //Enable Serial Interrupt 
  ET0=0;
  TR0=0;
}

//�򴮿ڷ���һ���ַ�
void send_char_com(unsigned char ch)
{
  SBUF=ch;
  while(!TI);
  TI=0;
}

//�򴮿ڷ���һ���ַ���,strlenΪ���ַ�������
void send_string_com(unsigned char *str,unsigned int strlen)
{
  unsigned int k=0;
  do
  {
    send_char_com(*(str+k));
    k++;
  }while(k<strlen);
}

//��ʱ��0�ж�,����8�����ڴ˷���
void time_intt0(void) interrupt 1 using 2
{
  ET0=0;
  TR0=0;
  TH0=0xb8;
  TL0=0x00;
  sRXFlag=1;
}

//���ڽ����жϺ���
void serial() interrupt 4 using 1
{
  unsigned char ch;

  if(RI)
  {
    RI=0;
    ET0=1;TR0=1;
    ch=SBUF;

    inbuf1[count3]=ch;
    count3=count3+1;
    if(count3==INBUF_LEN)
    {
      ET0=0;
      TR0=0;
      TH0=0xb8;
      TL0=0x00;  //�رն�ʱ�жϣ�ֹͣ��ʱ�ж�
      sRXFlag=1;
    }
  }
}


/****************************************************
**����ԭ�ͣ�  void ex1_int(void) interrupt 2 //using 1
**��    �ܣ�  �ⲿ�ж�1���պ���
**��ڲ���:   �� 
**���ڲ���:   RevceData[]����   
**˵    ��:   ��MCP2515�յ���ȷ�ı���ʱ,�����int1�ж�
*****************************************************/
void ex1_int(void) interrupt 2 using 1
{
  gRXFlag=1;
}


/****************************************************
**����ԭ�ͣ�  void Init_Cpu(void)
**��    �ܣ�  ��Ƭ����ʼ��,�����ⲿ�ж�1
**��ڲ���:   �� 
**���ڲ���:    
**˵    ��:   ��Ƭ����ʼ��,�����ⲿ�ж�1  
*****************************************************/
void Init_Cpu(void)
{
  PX1=1;
  EX1=1;
  IT1=0;
  EA=1;
}

void main()
{

  Init2515();
  init_serialcomm();     //��ʼ������
  timer0initial();       //��ʱ��0��ʼ��
  Init_Cpu();

  while(1)
  {
    if(sRXFlag)
    {
      sRXFlag=0;
      CAN_Send_anylength(inbuf1,count3);
      count3=0;
    }

    if(gRXFlag)
    {
      CAN_Receive_DLC(a);
      gRXFlag=0;
      send_string_com(a,DLC);
    }
  }
}
