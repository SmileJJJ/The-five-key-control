#ifndef MAIN_H_
#define MAIN_H_
#include "stm8s.h"
#include "timer.h"

#define KEY1_PIN  GPIO_PIN_5
#define KEY1_PORT GPIOB
#define KEY2_PIN  GPIO_PIN_4
#define KEY2_PORT GPIOB
#define KEY3_PIN  GPIO_PIN_3
#define KEY3_PORT GPIOC
#define KEY4_PIN  GPIO_PIN_7
#define KEY4_PORT GPIOC
#define KEY5_PIN  GPIO_PIN_2
#define KEY5_PORT GPIOD

#define LED1_PIN  GPIO_PIN_2
#define LED1_PORT GPIOA
#define LED2_PIN  GPIO_PIN_3
#define LED2_PORT GPIOA
#define LED3_PIN  GPIO_PIN_6
#define LED3_PORT GPIOC
#define LED4_PIN  GPIO_PIN_1
#define LED4_PORT GPIOA
#define LED5_PIN  GPIO_PIN_5
#define LED5_PORT GPIOC

#define MOTOR_OPEN_PIN  GPIO_PIN_3
#define MOTOR_OPEN_PORT GPIOD
#define MOTOR_DOWN_PIN  GPIO_PIN_4
#define MOTOR_DOWN_PORT GPIOD
#define MOTOR_UP_PIN    GPIO_PIN_6
#define MOTOR_UP_PORT   GPIOD

#define CLOSE_SWITCH_PIN  GPIO_PIN_5
#define CLOSE_SWITCH_PORT GPIOD

typedef enum{
 AFR7_PD4_BEEP=(u8)0x80,
 AFR6_PB4_5_I2C_SDA_SCL=(u8)0x40,
 AFR5_PB3_2_1_0_TIM1_ETR_CH2_1_0=(u8)0x20,
 AFR4_PD7_TIM_CH4=(u8)0x10,
 AFR3_PD0_TIM1_BKIN=(u8)0x08,
 AFR2_PD0_CLK_CCO=(u8)0x04,
 AFR1_PA3_TIM3_CH1_PD2_TIM2_CH3=(u8)0x02,
 AFR0_PD3_ADC_ETR=(u8)0x01
}Remap_Function_TypeDef;

#ifdef MAIN_GLO
u8 KEY_NUM=0;
u8 TIME_NUM=0;
u8 status=0;
u8 Motor_status=0;
u8 timer_flg=0;
u16 Time_Count=0;
u16 Time_Count_set=0;

#else 
extern u8 KEY_NUM;
extern u8 TIME_NUM;
extern u8 status;
extern u8 Motor_status;
extern u8 timer_flg;
extern u16 Time_Count;
extern u16 Time_Count_set;
#endif
void MOTOR_OPEN();
void MOTOR_CLOSE();
void MOTOR_DOWN();
void MOTOR_UP();
void Get_KEY_NUM();
void TIME_NUM_SAVE(u16 Num);
void AlternateFunctionRemapping(Remap_Function_TypeDef Function);

void My_CLK_Init();
void My_TIM1_Init();
void My_TIM2_Init();
void My_GPIO_Init();
void My_EXTI_Init();
#endif  
