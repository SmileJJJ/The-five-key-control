
#ifndef SPI_H_
#define SPI_H_
#include "stm8s.h"



#define SS_PIN GPIO_PIN_3
#define SI_PIN GPIO_PIN_6
#define SO_PIN GPIO_PIN_7
#define SCK_PIN GPIO_PIN_5

#define SS_PORT GPIOA
#define SI_PORT GPIOC
#define SO_PORT GPIOC
#define SCK_PORT GPIOC

#define AFR_ADDRESS 0x4803
typedef enum{ 
AFR7_PD4_BEEP=(uint8_t)0x80,  
	AFR6_PB4_5_I2C_SDA_SCL=(uint8_t)0x40,  
	AFR5_PB3_2_1_0_TIM1_ETR_CH2_1_0=(uint8_t)0x20, 
	AFR4_PD7_TIM_CH4=(uint8_t)0x10,  
	AFR3_PD0_TIM1_BKIN=(uint8_t)0x08,
	AFR2_PD0_CLK_CCO=(uint8_t)0x04,  
	AFR1_PA3_TIM3_CH1_PD2_TIM2_CH3=(uint8_t)0x02,  
	AFR0_PD3_ADC_ETR=(uint8_t)0x01
	}
Remap_Function_TypeDef;

#ifdef SPI_GLO
#else 


#endif


void Spi_Init();
void SPI_Write_Byte(unsigned char add,unsigned char data);
u8 SPI_Send(u8 data);

u8 SPI_Read_Byte(unsigned char byte);
void AlternateFunctionRemapping(Remap_Function_TypeDef Function);

#endif 

