#include "stm32f10x.h"
#include "usart.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "delay.h"//��ʱ����

#include <stdio.h>
#include <math.h>



/*************************************************
����: fputc(int ch, FILE *f)
����: �ض���c�⺯��printf��USART1
����: ��
����: ��
**************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}


#define USR_I2C_USED I2C1


extern void test_bh1750_fun(void);
extern u8 max10300_Bus_Read(u8 Register_Address);
extern void max10300_init();

/*************************************************
����: int main(void)
����: main������
����: ��
����: ��
**************************************************/
int main(void)
{
	u8 temp_num=0;
	
	max10300_init();
while(1)
{
	delay_ms(1000);            
	max10300_init();
	temp_num = max10300_Bus_Read(0x16);
	printf("��ǰ�¶� = %d\r\n",temp_num);
}
	while(1)
	{	
		test_bh1750_fun();
	//	printf("\r\n Ultrasonic_Distance = %.2f mm,Ultrasonic_Number = %d \r\n" ,(float)Ultrasonic_Time*100/5.8,Ultrasonic_Number);

	}
}




#define max10300_WR_address 0xAE

void max10300_Bus_Write(u8 Register_Address, u8 Word_Data)
{
#if 0
	i2c_snd(max10300_W_address, TRUE, FALSE);
	i2c_snd(Register_Address, FALSE, FALSE);
	i2c_snd(Word_Data, FALSE, TRUE);
#else
	while(I2C_GetFlagStatus(USR_I2C_USED, I2C_FLAG_BUSY))
		; //���ÿ⺯�����I2C�����Ƿ���BUSY״̬
	I2C_GenerateSTART(USR_I2C_USED, ENABLE);

	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	 	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(USR_I2C_USED,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		; //ADDR=1�����EV6  

	I2C_SendData(USR_I2C_USED, Register_Address);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//��λ�Ĵ����ǿգ����ݼĴ����Ѿ��գ�����EV8���������ݵ�DR�ȿ�������¼�
	
		
	I2C_SendData(USR_I2C_USED, Word_Data);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//��λ�Ĵ����ǿգ����ݼĴ����Ѿ��գ�����EV8���������ݵ�DR�ȿ�������¼�
	 I2C_GenerateSTOP(USR_I2C_USED, ENABLE);	
#endif

}


u8 max10300_Bus_Read(u8 Register_Address)
{
#if 0
	uint8 data1;
	i2c_snd(max10300_W_address, TRUE, FALSE);
	i2c_snd(Register_Address, FALSE, FALSE);
	i2c_snd(max10300_R_address, TRUE, FALSE);
	i2c_rcv(&data1, TRUE);
#else
	u8 data1;
	while(I2C_GetFlagStatus(USR_I2C_USED, I2C_FLAG_BUSY))
		; //���ÿ⺯�����I2C�����Ƿ���BUSY״̬
	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	 	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(USR_I2C_USED,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		; //ADDR=1�����EV6  
		
	I2C_SendData(USR_I2C_USED, Register_Address);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//��λ�Ĵ����ǿգ����ݼĴ����Ѿ��գ�����EV8���������ݵ�DR�ȿ�������¼�

	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	 	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Receiver);

	while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(USR_I2C_USED, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    I2C_GenerateSTOP(USR_I2C_USED, ENABLE);			//����ֹͣλ
	
	
		while(!(I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)))
		; //��ȡ����
		data1 = I2C_ReceiveData(USR_I2C_USED);
	

	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);//��Ӧ��λʹ�ܻ�ȥ���ȴ��´�ͨ��
	
	return (data1);
#endif
}

static void i2c_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
	*/
	for (i = 0; i < 30; i++);
}



#define INTERRUPT_REG  						0X00
#define TEMPERATURE_REG  					0X16

#define INTERRUPT_REG_A_FULL  			(0X01<<7)
#define INTERRUPT_REG_TEMP_RDY  		(0X01<<6)
#define INTERRUPT_REG_HR_RDY  			(0X01<<5)
#define INTERRUPT_REG_SPO2_RDY  		(0X01<<4)
#define INTERRUPT_REG_PWR_RDY  			(0X01<<0)

void max10300_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	
	
	while(I2C_GetFlagStatus(USR_I2C_USED, I2C_FLAG_BUSY))
		; //���ÿ⺯�����I2C�����Ƿ���BUSY״̬
		

	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);   /*����1�ֽ�1Ӧ��ģʽ*/


	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(USR_I2C_USED,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		; //ADDR=1�����EV6 
		
	I2C_SendData(USR_I2C_USED, Register_Address);
	 while(! I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	 	;//��λ�Ĵ����ǿգ����ݼĴ����Ѿ��գ�����EV8���������ݵ�DR�ȿ�������¼�

	I2C_GenerateSTART(USR_I2C_USED, ENABLE);
	 while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_MODE_SELECT))
	 	; //���EV5
	
	I2C_Send7bitAddress(USR_I2C_USED, max10300_WR_address, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	

 	while (no)
    {	
	
		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7 
	    data1 = I2C_ReceiveData(USR_I2C_USED);

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
	    data2 = I2C_ReceiveData(USR_I2C_USED);

	    Word_Data[i][0] = (((u16)data1 << 8) | data2);  //



       while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data1 = I2C_ReceiveData(USR_I2C_USED);

	    if(no==1)
		{
     		I2C_AcknowledgeConfig(I2C1, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
    		I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
		
		}

		while(!I2C_CheckEvent(USR_I2C_USED, I2C_EVENT_MASTER_BYTE_RECEIVED)); // EV7
		data2 = I2C_ReceiveData(USR_I2C_USED);

		Word_Data[i][1] = (((u16)data1 << 8) | data2); 
		i++;

		
	    /* Decrement the read bytes counter */
	    no--;
    }

	
	I2C_AcknowledgeConfig(USR_I2C_USED, ENABLE);//��Ӧ��λʹ�ܻ�ȥ���ȴ��´�ͨ��
	I2C_GenerateSTOP(I2C1, ENABLE);			//����ֹͣλ
}

void max10300_init()
{
	max10300_Bus_Write(0x06, 0x0b);
	max10300_Bus_Write(0x01, 0xF0); // open all of interrupt
	max10300_Bus_Write(0x09, 0x33); // r_pa=3,ir_pa=3
//	max10300_Bus_Write(0x07, 0x47); // SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
	max10300_Bus_Write(0x07, 0x43); // SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS

	max10300_Bus_Write(0x02, 0x00);   // set FIFO write Pointer reg = 0x00 for clear it
	max10300_Bus_Write(0x03, 0x00);	// set Over Flow Counter  reg = 0x00 for clear it
	max10300_Bus_Write(0x04, 0x00);	// set FIFO Read Pointer  reg = 0x00 for clear it 
}


	

void test_bh1750_fun(void)
{
	u16 temp_num=0;
	u8  temperature=0;
	u16 fifo_word_buff[15][2];
	u16 i =0;
	u16 Heart_Rate=0;
	while(1)
	{

		temp_num = max10300_Bus_Read(INTERRUPT_REG);
		temperature = max10300_Bus_Read(TEMPERATURE_REG);
		
		//if( (INTERRUPT_REG_HR_RDY&temp_num) && (INTERRUPT_REG_SPO2_RDY&temp_num) )
		if( INTERRUPT_REG_A_FULL&temp_num )
		{
			max10300_FIFO_Read(0x05,fifo_word_buff,15);
			max10300_Bus_Write(0x02, 0x00);   //set FIFO write Pointer reg = 0x00 for clear it
			max10300_Bus_Write(0x03, 0x00);	//set Over Flow Counter  reg = 0x00 for clear it
			max10300_Bus_Write(0x04, 0x00);	//set FIFO Read Pointer  reg = 0x00 for clear it 
		}
						
							
	}
}

