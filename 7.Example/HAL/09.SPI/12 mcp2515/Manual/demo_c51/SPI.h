#ifndef __SPI_h__
#define __SPI_h__

#include <REG51.h>
#include <string.h>
#include <intrins.h>
#include <stdio.h>
#include "Function.h"

//�ܽ�����
sbit SCK=P2^3;
sbit SI=P2^2;
sbit SO=P2^1;
sbit CS=P2^0;


//����˵��������һ���ֽ�
unsigned char ReadSPI(void)
{
  unsigned char n=8;//��MISO���϶�ȡһ�������ֽ�
  unsigned char tdata;
  SCK=0;
  while(n--)
  {
    _nop_();
    _nop_();
    //delay1ms(1);//���޸�
    SCK=1;
    //delay1ms(1);//ͬ��
    tdata=tdata<<1;//����һλ
    if(SO==1)
    {
      tdata=tdata|0x01;//���յ���λΪ1,�����ݵ����һλ��1
    }
    else
    {
      tdata=tdata&0xfe;//�������ݵ����һλ��0
    }
    
    SCK=0;
  }
  return tdata;
}

//���ܺ���������һ���ֽ�
void WriteSPI(unsigned char ch)
{
  unsigned char n=8;//��SI�Ϸ���һλ�����ֽڣ�����λ
  SCK=0;
  while(n--)
  {
    _nop_();
    SCK=0;
    if((ch&0x80)==0x80)//��Ҫ���͵��������λΪ1������1
    {
      SI=1;
    }
    else
    {
      SI=0;
    }
    _nop_();
    SCK=1;
    _nop_();
    //delay1ms(1);
    ch=ch<<1;//��������1λ
  }
}

//��2515ָ����ַaddrдһ���ֽ�����value
void SPIByteWrite(unsigned char addr,unsigned char value)
{
  CS=0;
  WriteSPI(CAN_WRITE);
  WriteSPI(addr);
  WriteSPI(value);
  CS=1;
}

// ��2515ָ����ַaddr��ȡһ���ֽ�����value
unsigned char SPIByteRead(unsigned char addr)
{
  unsigned char tempdata;

  CS=0;
  WriteSPI(CAN_READ);
  WriteSPI(addr);
  tempdata=ReadSPI();
  CS=1;
  return tempdata;
}

//SPI��λ
void SPIReset(void)
{
  CS=0;
  WriteSPI(CAN_RESET);
  CS=1;
}

#endif
