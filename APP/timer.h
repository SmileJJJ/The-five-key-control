#ifndef _TIMER_H_
#define _TIMER_H_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
//define the number of the Timer
enum  {
           TIMERCOMMON=0,	  		
	   TIMERDELAY,
	   TIMERSTATUS_Machine, //		  
	   TIMERWAIT_Touch,//
	   TIMERCOLORTIME, //
	   TIMER_STUDY_ADD,
	   TIMER_STUDY_NUM,
	   TIMER_STUDY_ID,
           TIMER_emit,
           TIMER_IR,
           TIMERAMOUNT	,
	   
}; 
enum
{
    WAIT=0,
    OK,
    NOTSTART
};
/*define the on/off state*/
enum
{
   SYSOFF=0,
   SYSON

};
typedef struct
{
   u8 runningstate;/*ON:running ;OFF:stop*/
   //u8  timernumb;
   u32 interval;
   u8  overflag;  /*timer over flag*/   
} TIMERITEMDEF;

extern TIMERITEMDEF TimerTable[TIMERAMOUNT];
extern void StartTimer(u8 timernum, u32 interval);
extern void StopTimer(u8 timernum);
extern u8 ReadTimer(u8 timernum);
extern u32 ReadTimerValue(u8 timernum);
extern void DelayTimer(u16 interval);
 

#endif
