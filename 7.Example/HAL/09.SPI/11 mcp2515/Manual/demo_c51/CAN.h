#ifndef	__CAN_H__
#define	__CAN_H__
//	write your header here
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



//void SPIReset(void);
//void SPIByteWrite(unsigned char ,unsigned char );
//unsigned char SPIByteRead(unsigned char );
unsigned char dummy;
 
void Init2515(unsigned int IDF)
{
	SPIReset();
	delay_ms(1);
	
	//SPIByteWrite(CANCTRL,0x80);//CAN����������ģʽ
	
    SPIByteWrite(RXM0SIDH,0x00);
	SPIByteWrite(RXM0SIDL,0x00);
	SPIByteWrite(RXF0SIDH,0x00);
	SPIByteWrite(RXF0SIDL,0x00);
	
	SPIByteWrite(RXM1SIDH,0x00);
	SPIByteWrite(RXM1SIDL,0x00);
	SPIByteWrite(RXF2SIDH,0x00);
	SPIByteWrite(RXF2SIDL,0x00);
	
	//���ò�����Ϊ10Kbps
	//set CNF1,SJW=00,����Ϊ1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/8M=12.5us
	SPIByteWrite(CNF1,CAN_125Kbps);
	//set CNF2,SAM=0,�ڲ���������߽���һ�β�����PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
	SPIByteWrite(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
	//set CNF3,PHSEG2=(2+1)TQ=3TQ,ͬʱ��CANCTRL.CLKEN=1ʱ�趨CLKOUT����Ϊʱ�����ʹ��λ
	SPIByteWrite(CNF3,PHSEG2_3TQ);
	
	//set TXB0�����÷��ͻ�����0�ı�ʶ���ͷ��͵����ݣ��Լ����͵����ݳ���
	 SPIByteWrite(TXB0CTRL,0x03);//���÷��ͻ�����0���ƼĴ�������TXREQ,�������ȼ����
	//BitModiMcp2515(TXB0CTRL,0x08,0x00);
	
	SPIByteWrite(TXB0SIDH,IDF);//���÷��ͻ�����0�ı�׼��ʶ�������޸�***
	SPIByteWrite(TXB0SIDL,0xE0);//�õ���׼��ʶ��
	SPIByteWrite(TXB0DLC,DLC_8);//���÷������ݵĳ���Ϊ8���ֽ�
	//SPIByteWrite(TXB0D0,0x1E);//�д��޸ļ�ȷ���Ƿ�ʹ��
	//SPIByteWrite(TXB0D1,0x10)��//�д��޸ļ�ȷ���Ƿ�ʹ��
		
	/*set TXB1
	SPIByteWrite(TXB1SIDH,0x50);    //Set TXB0 SIDH
 	SPIByteWrite(TXB1SIDL,0x00);    //Set TXB0 SIDL
 	SPIByteWrite(TXB1DLC,0x40 | DLC_8);    //Set DLC = 3 bytes and RTR bit*/
 	
 	//���ý��ջ�����0�ı�ʶ���ͳ�ʼ������
 	//SPIByteWrite(RXB0SIDH,0x55);//���ý��ջ�����0�ı�׼��ʶ�������޸�***
 	//SPIByteWrite(RXB0SIDL,0xE0);//�õ���׼��ʶ��
 	SPIByteWrite(RXB0CTRL,0x20);//�������ձ�׼��ʶ������Ч��Ϣ��FIILHIT0=0��ʾRXB0 ������FILHIT0
 	SPIByteWrite(RXB0DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�
 	
    SPIByteWrite(RXF0SIDH,0xE0);//��ʼ�������˲���0�����޸�***
 	SPIByteWrite(RXF0SIDL,0xE0);
 	SPIByteWrite(RXM0SIDH,0xFF);//��ʼ������������0�����޸�***
 	SPIByteWrite(RXM0SIDL,0xE0);            
 	
 	//���ý��ջ�����1�ı�ʶ���ͳ�ʼ������
 	 //SPIByteWrite(RXB1SIDH,IDF); //���ý��ջ�����0�ı�׼��ʶ�������޸�***
     //SPIByteWrite(RXB1SIDL,0xE0);//�õ���׼��ʶ��
 	 SPIByteWrite(RXB1CTRL,0x20);//�������ձ�׼��ʶ������Ч��Ϣ��FIILHIT0=0��ʾRXB0 ������FILHIT0
 	 SPIByteWrite(RXB1DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�
 	
 	 SPIByteWrite(RXF2SIDH,0x00);//��ʼ�������˲���2�����޸�***
 	 SPIByteWrite(RXF2SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF3SIDH,0x01);//��ʼ�������˲���3�����޸�***
 	 SPIByteWrite(RXF3SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF4SIDH,0x02);//��ʼ�������˲���4�����޸�***
 	 SPIByteWrite(RXF4SIDL,0xE0);
 	 
 	 SPIByteWrite(RXF5SIDH,0x56);//��ʼ�������˲���5�����޸�***
 	 SPIByteWrite(RXF5SIDL,0xE0);
 	 
 	 SPIByteWrite(RXM1SIDH,0xFF);//��ʼ������������1�����޸�***
 	 SPIByteWrite(RXM1SIDL,0xE0);
 	
 	//���ý��ջ�����0�ж�
 	SPIByteWrite(CANINTF,0x00);//������һ�α�����жϱ�־λ��0
 	SPIByteWrite(CANINTE,0x02);//���ջ�����0���ж�ʹ��λ***�޸ĳ�01-02
 	//���õ�����ģʽ1 ʹ�ܣ����ĳ��Է���һ�Σ�0��ֹ��������Ҫ���Ļ��ط�
 	//BitModiMcp2515(CANCTRL,0x08,0x00);
 	SPIByteWrite(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//��������ģʽ
 	BitModiMcp2515(CANCTRL,0xE7,0x04);
  	dummy=SPIByteRead(CANSTAT);
		if (OPMODE_NORMAL != (dummy && 0xE0))
	   SPIByteWrite(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//�жϽ�����������ģʽ
	  //SPIByteWrite(CANCTRL,0x00);//ѡ����������ģʽ
	 
}



//CAN���ͳ���
void CAN_Send(unsigned char *CAN_TX_Buf)
{
	unsigned char tempdata;
	tempdata=SPIByteRead(CAN_RD_STATUS);
	if(tempdata&0x04)            //�ж�TXREQ��־λ
	{
		delay_ms(1);
      //SPIByteWrite(TXB0CTRL,0);//���TXREQ��־λ
        BitModiMcp2515(TXB0CTRL,0x08,0x00);
	while(SPIByteRead(CAN_RD_STATUS)&0x04);//�ȴ�TXREQ����
   	 }
	SPIByteWrite(TXB0D0,CAN_TX_Buf[0]);
	SPIByteWrite(TXB0D1,CAN_TX_Buf[1]);
	SPIByteWrite(TXB0D2,CAN_TX_Buf[2]);
	SPIByteWrite(TXB0D3,CAN_TX_Buf[3]);
	SPIByteWrite(TXB0D4,CAN_TX_Buf[4]);
	SPIByteWrite(TXB0D5,CAN_TX_Buf[5]);
	SPIByteWrite(TXB0D6,CAN_TX_Buf[6]);
	SPIByteWrite(TXB0D7,CAN_TX_Buf[7]);
/*	if(tempdata&0x04)            //�ж�TXREQ��־λ
	{
		delay_ms(2);
      //SPIByteWrite(TXB0CTRL,0);//���TXREQ��־λ
        BitModiMcp2515(TXB0CTRL,0x08,0x00);
		while(SPIByteRead(CAN_RD_STATUS)&0x04);//�ȴ�TXREQ����
	}*/
	CS=LOW;
	//WriteSPI(CAN_RTS_TXB0);//���ͻ�����0������
	BitModiMcp2515(TXB0CTRL,0x08,0xFF);
	CS=HIGH;
}

//CAN���ճ���
/*void CAN_Receive(unsigned char *CAN_RX_Buf)
{
	CAN_RX_Buf[0]=SPIByteRead(RXB0D0);
	CAN_RX_Buf[1]=SPIByteRead(RXB0D1);
	CAN_RX_Buf[2]=SPIByteRead(RXB0D2);
	CAN_RX_Buf[3]=SPIByteRead(RXB0D3);
	CAN_RX_Buf[4]=SPIByteRead(RXB0D4);
	CAN_RX_Buf[5]=SPIByteRead(RXB0D5);
	CAN_RX_Buf[6]=SPIByteRead(RXB0D6);
	CAN_RX_Buf[7]=SPIByteRead(RXB0D7);
	SPIByteWrite(CANINTF,0x00);//��ս����жϱ�־λ
	//�򿪽��ջ�������������Ϣ���þ����д
}*/
void CAN_Receive(unsigned char *CAN_RX_Buf)
{
	BitModiMcp2515(CANINTF,0x02,0x00);//��ս����жϱ�־λRX1IF
	
	CAN_RX_Buf[0]=SPIByteRead(RXB1D0);
	CAN_RX_Buf[1]=SPIByteRead(RXB1D1);
	CAN_RX_Buf[2]=SPIByteRead(RXB1D2);
	CAN_RX_Buf[3]=SPIByteRead(RXB1D3);
	CAN_RX_Buf[4]=SPIByteRead(RXB1D4);
	CAN_RX_Buf[5]=SPIByteRead(RXB1D5);
	CAN_RX_Buf[6]=SPIByteRead(RXB1D6);
	CAN_RX_Buf[7]=SPIByteRead(RXB1D7);
	//SPIByteWrite(CANINTF,0x00);     //��ս����жϱ�־λ
	//BitModiMcp2515(CANINTF,0x02,0x00);//��ս����жϱ�־λRX1IF
	                                  //�򿪽��ջ�������������Ϣ���þ����д
}
#endif
