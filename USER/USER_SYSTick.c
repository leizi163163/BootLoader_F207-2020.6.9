#include "stm32f2xx.h"

uint16_t SysTickCount;


/*******************************************************************************
*
*函数原型：USER_SysTick_Init();
*入口参数：无
*出口参数：无
*函数功能：系统滴答定时器初始化
*	1ms = 15000		5ms = 15000 * 5 = 75000
*******************************************************************************/
void USER_SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   			//开启SysTick中断
	SysTick->LOAD = 75000; 									//5ms进入一次中断   
}


/*******************************************************************************
*
*函数原型：USER_SysTick_Start();
*入口参数：无
*出口参数：无
*函数功能：开启系统滴答定时器，定时时间即为重装载里的值45000/9=5000us
*
*******************************************************************************/
void USER_SysTick_Start(void)
{
	SysTick->VAL = 0x00;	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//开启SysTick 
}


/*******************************************************************************
*
*函数原型：USER_SysTick_Stop();
*入口参数：无
*出口参数：无
*函数功能：关闭系统滴答定时器
*
*******************************************************************************/
void USER_SysTick_Stop(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   	//关闭SysTick
//	SysTick->VAL = 0x00;
}


/*******************************************************************************
*
*函数原型：USER_Delay1ms(uint16_t ms_count);
*入口参数：ms_count，ms数值
*出口参数：无
*函数功能：延时ms_count个1ms，在120MHz频率下。
*
*******************************************************************************/
void USER_Delay1ms(uint16_t ms_count)
{
	uint16_t i,j;
	
	for(i = 0; i < ms_count; i++)
	{
		for(j = 0; j < 15000; j++)
		{
			__nop();
			__nop();
		}
	}
}

