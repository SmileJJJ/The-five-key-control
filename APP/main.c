/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#define MAIN_GLO
#define STATUS_Standby 0          
#define STATUS_Open    1          
#define STATUS_Work    2  

#define  Motor_OPEN    1
#define  Motor_CLOSE   2
#define  Motor_DOWN    3
#define  Motor_UP      4

#define  TIME_KEY      1
#define  OFF_KEY       2
#define  UP_KEY        3
#define  DOWN_KEY      4
#define  ON_KEY        5
    
#define  motor_open_0  GPIO_WriteLow(GPIOD, GPIO_PIN_3);
#define  motor_open_1  GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
#define  motor_up_0    GPIO_WriteLow(GPIOD, GPIO_PIN_6);
#define  motor_up_1    GPIO_WriteHigh(GPIOD, GPIO_PIN_6);
#define  motor_down_0  GPIO_WriteLow(GPIOD, GPIO_PIN_4);
#define  motor_down_1  GPIO_WriteHigh(GPIOD, GPIO_PIN_4);;

#define LED1_ON    GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
#define LED1_OFF   GPIO_WriteLow(GPIOA, GPIO_PIN_2);
#define LED2_ON    GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
#define LED2_OFF   GPIO_WriteLow(GPIOA, GPIO_PIN_3);
#define LED3_ON    GPIO_WriteHigh(GPIOC, GPIO_PIN_6);
#define LED3_OFF   GPIO_WriteLow(GPIOC, GPIO_PIN_6);
#define LED4_ON    GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
#define LED4_OFF   GPIO_WriteLow(GPIOA, GPIO_PIN_1);
#define LED5_ON    GPIO_WriteHigh(GPIOC, GPIO_PIN_5);
#define LED5_OFF   GPIO_WriteLow(GPIOC, GPIO_PIN_5);

#define close_switch   GPIO_ReadInputPin(GPIOD, GPIO_PIN_5)
    
#include "main.h"

void main(void)
{   
  My_CLK_Init();   
  My_TIM2_Init();
  My_GPIO_Init();
  enableInterrupts(); 
  TIME_NUM=(*(u8 *)0x4000);
  while(1)      
   {    
        KEY_NUM=0;
        Get_KEY_NUM();
//        if((close_switch!=0)&&(status==STATUS_Standby))
//        {       
//          Motor_status=Motor_UP;
//        }
        switch(Motor_status)
        {
          case Motor_OPEN: MOTOR_OPEN(); break; 
          case Motor_CLOSE:MOTOR_CLOSE();break; 
          case Motor_DOWN: MOTOR_DOWN(); break; 
          case Motor_UP:   MOTOR_UP();   break;
          default:break;
        }
        switch(TIME_NUM)
        {
           case 0:LED1_ON;LED2_OFF;LED3_OFF;LED4_OFF;LED5_OFF;Time_Count_set=60; break; 
           case 1:LED1_OFF;LED2_ON;LED3_OFF;LED4_OFF;LED5_OFF;Time_Count_set=120;break;
           case 2:LED1_OFF;LED2_OFF;LED3_ON;LED4_OFF;LED5_OFF;Time_Count_set=180;break;
           case 3:LED1_OFF;LED2_OFF;LED3_OFF;LED4_ON;LED5_OFF;Time_Count_set=240;break;
           case 4:LED1_OFF;LED2_OFF;LED3_OFF;LED4_OFF;LED5_ON;Time_Count_set=300;break;
           default:break;
        }
	switch(status)  
	{ 
	   case STATUS_Standby:	
               timer_flg=0;
               Motor_status=0;
               motor_open_0;
               motor_up_0;   
               motor_down_0;
               switch(KEY_NUM)
               {
                 case TIME_KEY: TIME_NUM++;if(TIME_NUM>4){TIME_NUM=0;}TIME_NUM_SAVE(TIME_NUM);break;
                 case OFF_KEY:break;
                 case UP_KEY:break;
                 case DOWN_KEY:break;
                 case ON_KEY:Motor_status=Motor_OPEN;break;
                 default: break;
                }                                            
	      break;

	   case STATUS_Open: 
               switch(KEY_NUM)
               {
                 case TIME_KEY: break;
                 case OFF_KEY:status=STATUS_Standby;Motor_status=Motor_CLOSE;break;
                 case UP_KEY:break;
                 case DOWN_KEY:Motor_status=Motor_DOWN;StartTimer(TIMERSTATUS_Machine,100);Time_Count=0;break;
                 case ON_KEY:break;
                 default: break;
                }                                                 
	      break;
						   
	   case STATUS_Work: 
               switch(KEY_NUM)
               {
                 case TIME_KEY: break;
                 case OFF_KEY: Motor_status=Motor_UP;break;
                 case UP_KEY:  Motor_status=Motor_UP;break;
                 case DOWN_KEY:break;
                 case ON_KEY:break;
                 default: break;
                } 
               if(ReadTimer(TIMERSTATUS_Machine)==OK)
                 {
                   StartTimer(TIMERSTATUS_Machine,100);
                   Time_Count++; 
                 }
               if(Time_Count>=Time_Count_set)
                 {
                   Time_Count=0;
                   Motor_status=Motor_UP;
                 }
	      break;
	    default:
	      break;			
	}   
    }  
}

void MOTOR_OPEN()
{
    motor_open_1;
    status=STATUS_Open;
}
void MOTOR_CLOSE()
{
    motor_open_0;  
}
void MOTOR_DOWN()
{
    motor_up_1;   
    motor_down_0;
    status=STATUS_Work;
}
void MOTOR_UP()
{   
    StopTimer(TIMERSTATUS_Machine);
    if(timer_flg==0)
    { 
      motor_up_0;   
      motor_down_0;
      DelayTimer(80);
      StartTimer(TIMERDELAY,50);
      timer_flg=1;
    }
    if((close_switch!=0))
    { 
      motor_up_0;   
      motor_down_1;
    }
    if((close_switch==0)&&(ReadTimer(TIMERDELAY)==OK))
    {
       status=STATUS_Standby;
    }
}
  
void Get_KEY_NUM()
{
   if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0)
   {while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0);KEY_NUM=TIME_KEY;}
   else if(GPIO_ReadInputPin(GPIOB, GPIO_PIN_4)==0)
   {while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0);KEY_NUM=OFF_KEY;}
   else if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_3)==0)
   {while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0);KEY_NUM=UP_KEY;} 
   else if(GPIO_ReadInputPin(GPIOC, GPIO_PIN_7)==0)
   {while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0);KEY_NUM=DOWN_KEY;} 
   else if(GPIO_ReadInputPin(GPIOD, GPIO_PIN_2)==0)
   {while(GPIO_ReadInputPin(GPIOB, GPIO_PIN_5)==0);KEY_NUM=ON_KEY;} 
   else ;
}

void TIME_NUM_SAVE(u16 Num)
{
         do
  {

       FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
       FLASH->DUKR = FLASH_RASS_KEY1;
  }   
        while((FLASH->IAPSR & 0x08) == 0); 
        *(PointerAttr uint8_t*) (uint16_t)0X4000 = Num;   
        while((FLASH->IAPSR & 0x04) == 0);
        FLASH->IAPSR &= (uint8_t)FLASH_MEMTYPE_DATA;  
}

/* 功能: 选功能重映射的设置//
* 形参: Function 需要重映射的功能? ?? ?
* 返回: 无
* 说明: 如果调试需要打印数据的时候
? ?? ?? ?在自定义宏的地方添加#define DEBUG_AFR即可，下载程序后只有第一次执行
? ?? ?? ?打印出来设置前的功能才是对的，执行完之前就被写进flash,如果不修改的话，
? ?? ?? ?每次都出来的数据都是一样，所以只执行一次就行了，建议用的时候，就在调
? ?? ?? ?试的时候进行执行一次就可以，以后都不需要出现了，除非再次更改。
? ?? ?? ?在调用AlternateFunctionRemapping函数之前需要初始化串口函数Uart_Init
? ?? ?? ?这些设置写进flash,运行一次即可,在调试的时候运行即可
******************************************************************************/
void AlternateFunctionRemapping(Remap_Function_TypeDef Function)
{
   uint8_t Function_temp;
   FLASH_Unlock(FLASH_MEMTYPE_DATA);/*要对选项字节写操作，必须要先解锁FLASH*/
   while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/
   //备选功能重映射的设置可以同时写多个不会被覆盖，因此在操作之前读取当前的状态，再根据当前的状态进行修改*/
   Function_temp=(uint8_t)(FLASH_ReadOptionByte(0x4803)>>8);
   while(!(FLASH->IAPSR & FLASH_IAPSR_DUL));/*等待解锁完毕*/
   FLASH_ProgramOptionByte(0x4803,Function|Function_temp);
   FLASH_Lock(FLASH_MEMTYPE_DATA);/*操作完要加锁*/
}


/*功能:单片机基本配置
  参数说明：   *** CLK时钟配置
               *** TIM2配置
               *** GPIO配置
******************************************************************************/
void My_CLK_Init(void)   // CLK时钟配置
{
       /* Clear High speed internal clock prescaler */
       CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
       /* Set High speed internal clock prescaler */
       CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV4;
}

void My_TIM2_Init(void)   //TIM2配置
{
       TIM2_DeInit();
       TIM2_TimeBaseInit(TIM2_PRESCALER_4,9999);//100us 一次中断
       TIM2->IER |= (uint8_t)TIM2_IT_UPDATE; 
     //  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);
       TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN; 
}

void My_GPIO_Init(void)   //GPIO配置
{ 
  GPIO_Init(KEY1_PORT, KEY1_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY2_PORT, KEY2_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY3_PORT, KEY3_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY4_PORT, KEY4_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(KEY5_PORT, KEY5_PIN, GPIO_MODE_IN_PU_NO_IT);
  
  GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(LED5_PORT, LED5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(LED3_PORT, LED3_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  
  GPIO_Init(MOTOR_OPEN_PORT, MOTOR_OPEN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOTOR_DOWN_PORT, MOTOR_DOWN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(MOTOR_UP_PORT,   MOTOR_UP_PIN,   GPIO_MODE_OUT_PP_LOW_SLOW);
  
  GPIO_Init(CLOSE_SWITCH_PORT, CLOSE_SWITCH_PIN, GPIO_MODE_IN_PU_NO_IT);
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
