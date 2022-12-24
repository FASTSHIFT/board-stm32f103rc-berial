#ifndef __CAN_H__
#define __CAN_H__


#include "2515.h"
#include "SPI.h"
#include "reg51.h"
#include "Function.h"

#define CAN_10Kbps 0x31
#define CAN_25Kbps 0x13
#define CAN_50Kbps 0x09
#define CAN_100Kbps 0x04
#define CAN_125Kbps 0x03
#define CAN_250Kbps 0x01
#define CAN_500Kbps 0x00


unsigned char DLC,dummy;

void Init2515(void)
{
  SPIReset();
  delay1ms(1);

  //SPIByteWrite(CANCTRL,0x80);//CAN����������ģʽ

  SPIByteWrite(RXM0SIDH,0x00);
  SPIByteWrite(RXM0SIDL,0x00);

  SPIByteWrite(RXF0SIDH,0x00);
  SPIByteWrite(RXF0SIDL,0x00);
  //���ò�����Ϊ10Kbps
  //set CNF1,SJW=00,����Ϊ1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/8M=12.5us
  SPIByteWrite(CNF1,CAN_125Kbps);
  //set CNF2,SAM=0,�ڲ���������߽���һ�β�����PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
  SPIByteWrite(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
  //set CNF3,PHSEG2=(2+1)TQ=3TQ,ͬʱ��CANCTRL.CLKEN=1ʱ�趨CLKOUT����Ϊʱ�����ʹ��λ
  SPIByteWrite(CNF3,PHSEG2_3TQ);

  //set TXB0�����÷��ͻ�����0�ı�ʶ���ͷ��͵����ݣ��Լ����͵����ݳ���
  SPIByteWrite(TXB0SIDH,0xFF);//���÷��ͻ�����0�ı�׼��ʶ�������޸�***
  SPIByteWrite(TXB0SIDL,0xE0);//�õ���׼��ʶ��

  //SPIByteWrite(TXB0D0,0x1E);//�д��޸ļ�ȷ���Ƿ�ʹ��
  //SPIByteWrite(TXB0D1,0x10)��//�д��޸ļ�ȷ���Ƿ�ʹ��

  /*set TXB1
  SPIByteWrite(TXB1SIDH,0x50);    //Set TXB0 SIDH
  SPIByteWrite(TXB1SIDL,0x00);    //Set TXB0 SIDL
  SPIByteWrite(TXB1DLC,0x40 | DLC_8);    //Set DLC = 3 bytes and RTR bit*/

  //���ý��ջ�����0�ı�ʶ���ͳ�ʼ������
  SPIByteWrite(RXB0SIDH,0x00);//���ý��ջ�����0�ı�׼��ʶ�������޸�***
  SPIByteWrite(RXB0SIDL,0x60);//�õ���׼��ʶ��
  SPIByteWrite(RXB0CTRL,0x60);//�������ձ�׼��ʶ������Ч��Ϣ��FIILHIT0=0��ʾRXB0 ������FILHIT0
  SPIByteWrite(RXB0DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�

  SPIByteWrite(RXF0SIDH,0xFF);//��ʼ�������˲���0�����޸�***
  SPIByteWrite(RXF0SIDL,0xE0);
  SPIByteWrite(RXM0SIDH,0xFF);//��ʼ������������0�����޸�***
  SPIByteWrite(RXM0SIDL,0xE0);

  //���ý��ջ�����0�ж�
  SPIByteWrite(CANINTF,0x00);//����жϱ�־λ
  SPIByteWrite(CANINTE,0x01);//���ջ�����0���ж�ʹ��λ

  SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//��������ģʽ

  dummy=SPIByteRead(CANSTAT);
  if(OPMODE_NORMAL!=(dummy&&0xE0))
  {
    SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//�жϽ�����������ģʽ
  }

}

//CAN�������ⳤ���ֽ�
void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1)
{
  unsigned char tempdata,j;
  tempdata=SPIByteRead(CAN_RD_STATUS);
  SPIByteWrite(TXB0DLC,length1);
  for(j=0;j<length1;j++)
  {
    SPIByteWrite(TXB0D0+j,CAN_TX_Buf[j]);
  }

  if(tempdata&0x04)//�ж�TXREQ��־λ
  {
    delay1ms(1);
    SPIByteWrite(TXB0CTRL,0);//���TXREQ��־λ
    while(SPIByteRead(CAN_RD_STATUS)&0x04);//�ȴ�TXREQ����
  }
  CS=0;
  WriteSPI(CAN_RTS_TXB0);//���ͻ�����0������
  CS=1;
}

//CAN�������ⳤ�ȵ�����
void CAN_Receive_DLC(unsigned char *CAN_RX_Buf)
{
  unsigned char i;
  DLC=SPIByteRead(RXB0DLC);
  for(i=0;i<DLC;i++)
  {
    //�ѽ��ջ�����������ݣ��ŵ��ڲ�RAM������
    CAN_RX_Buf[i]=SPIByteRead(RXB0D0+i);
  }

  SPIByteWrite(CANINTF,0);
}

#endif
