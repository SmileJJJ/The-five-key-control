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

/* 功能: 选功能重映射的设置 
* 形参: Function 需要重映射的功能     
* 返回: 无  * 说明: 如果调试需要打印数据的时候       
在自定义宏的地方添加
#define DEBUG_AFR
即可，下载程序后只有第一次执行       
打印出来设置前的功能才是对的，执行完之前就被写进flash,如果不修改的话，     
每次都出来的数据都是一样，所以只执行一次就行了，建议用的时候，就在调         
试的时候进行执行一次就可以，以后都不需要出现了，除非再次更改。     
在调用AlternateFunctionRemapping函数之前需要初始化串口函数Uart_Init         
这些设置写进flash,运行一次即可,在调试的时候运行即可  
******************************************************************************/  
void AlternateFunctionRemapping(Remap_Function_TypeDef Function) { 
uint8_t Function_temp; 
FLASH_Unlock(FLASH_MEMTYPE_DATA);/*要对选项字节写操作，必须要先解锁FLASH*/
while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/  
/*备选功能重映射的设置可以同时写多个不会被覆盖，因此在操作之前读取当前的状态，再根据当前的状态进 */
Function_temp=(uint8_t)(FLASH_ReadOptionByte(AFR_ADDRESS)>>8);  
#ifdef  DEBUG_AFR  
printf("\r\nFunction_temp:%x\r\n",Function_temp);  
#endif  
while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/ 
FLASH_ProgramOptionByte(AFR_ADDRESS,Function|Function_temp);   
#ifdef  DEBUG_AFR    
Function_temp=(uint8_t)(FLASH_ReadOptionByte(AFR_ADDRESS)>>8);
printf("\r\nFunction_temp:%x\r\n",Function_temp); 
#endif  
FLASH_Lock(FLASH_MEMTYPE_DATA);/*操作完要加锁*/    
}