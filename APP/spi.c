#define SPI_GLO
#include "spi.h"
#define CLK_SPI 5
void Spi_Init(){	
	 // SPI_DeInit( );
	
	   GPIO_Init(SS_PORT, SS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	 GPIO_Init(SI_PORT, SI_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
	GPIO_Init(SO_PORT, SO_PIN, GPIO_MODE_IN_FL_NO_IT);
 	GPIO_Init(SCK_PORT, SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
 /*		 SPI_Init(SPI_FIRSTBIT_MSB, 
              SPI_BAUDRATEPRESCALER_128, 
              SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, 
              SPI_CLOCKPHASE_1EDGE, 
              SPI_DATADIRECTION_2LINES_FULLDUPLEX, 
              SPI_NSS_HARD, 1);
  SPI_Cmd(ENABLE);
  SPI_NSSInternalSoftwareCmd(ENABLE);*/

}
void delay_us(u8 nus)
{
	while(nus--);
}
u8 SPI_Send(u8 data)
{
		u8 i=0;
		u8 read=0;
		for(i=0;i<8;i++)
			{	delay_us(CLK_SPI);
				GPIO_WriteHigh(SCK_PORT,SCK_PIN);
					read<<=1;
				if((SO_PORT->IDR&SO_PIN)==SO_PIN)
					{
					read+=1;
					}

				if((data&0x80)==0x80)
					{
						GPIO_WriteHigh(SI_PORT,SI_PIN);
					}
				else
					{
						GPIO_WriteLow(SI_PORT,SI_PIN);
					}
               data<<=1;
				delay_us(CLK_SPI);
				GPIO_WriteLow(SCK_PORT,SCK_PIN);
			}
		return read;
}

void SPI_Write_Byte(unsigned char add,unsigned char data)
{ u8 stat=0;
 GPIO_WriteLow(SS_PORT,SS_PIN);
 stat=SPI_Send(add);
  stat=SPI_Send(data);
  GPIO_WriteHigh(SS_PORT,SS_PIN);

}
u8 SPI_Read_Byte(unsigned char byte)
{
 u8 data=0;
 u8 stat=0;
 GPIO_WriteLow(SS_PORT,SS_PIN);
 stat=SPI_Send(byte);
 data=SPI_Send(0x00);
 GPIO_WriteHigh(SS_PORT,SS_PIN);
 return data;
}

/* ����: ѡ������ӳ������� 
* �β�: Function ��Ҫ��ӳ��Ĺ���     
* ����: ��  * ˵��: ���������Ҫ��ӡ���ݵ�ʱ��       
���Զ����ĵط����
#define DEBUG_AFR
���ɣ����س����ֻ�е�һ��ִ��       
��ӡ��������ǰ�Ĺ��ܲ��ǶԵģ�ִ����֮ǰ�ͱ�д��flash,������޸ĵĻ���     
ÿ�ζ����������ݶ���һ��������ִֻ��һ�ξ����ˣ������õ�ʱ�򣬾��ڵ�         
�Ե�ʱ�����ִ��һ�ξͿ��ԣ��Ժ󶼲���Ҫ�����ˣ������ٴθ��ġ�     
�ڵ���AlternateFunctionRemapping����֮ǰ��Ҫ��ʼ�����ں���Uart_Init         
��Щ����д��flash,����һ�μ���,�ڵ��Ե�ʱ�����м���  
******************************************************************************/  
void AlternateFunctionRemapping(Remap_Function_TypeDef Function) { 
uint8_t Function_temp; 
FLASH_Unlock(FLASH_MEMTYPE_DATA);/*Ҫ��ѡ���ֽ�д����������Ҫ�Ƚ���FLASH*/
while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*�ȴ��������*/  
/*��ѡ������ӳ������ÿ���ͬʱд������ᱻ���ǣ�����ڲ���֮ǰ��ȡ��ǰ��״̬���ٸ��ݵ�ǰ��״̬�� */
Function_temp=(uint8_t)(FLASH_ReadOptionByte(AFR_ADDRESS)>>8);  
#ifdef  DEBUG_AFR  
printf("\r\nFunction_temp:%x\r\n",Function_temp);  
#endif  
while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*�ȴ��������*/ 
FLASH_ProgramOptionByte(AFR_ADDRESS,Function|Function_temp);   
#ifdef  DEBUG_AFR    
Function_temp=(uint8_t)(FLASH_ReadOptionByte(AFR_ADDRESS)>>8);
printf("\r\nFunction_temp:%x\r\n",Function_temp); 
#endif  
FLASH_Lock(FLASH_MEMTYPE_DATA);/*������Ҫ����*/    
}