#ifndef _USART_H_
#define _USART_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
u8 isGetdata();
 void sendData(u8* str,  u8 length);
 void USARTIRQHandl(void);
#endif
