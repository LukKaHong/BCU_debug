/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32H7)
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明
*		V1.0    2018-07-29  Eric2013   正式发布
*
*	Copyright (C), 2018-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V7    


/* 检查是否定义了开发板*/
#if !defined (STM32_V7)
	#error "Please define the board model : STM32_V7"
#endif

/* 定义 BSP 版本号 */
#define __STM32H7_BSP_VERSION		"1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 调试输出口 */
#define DEBUG_PORT_UART 1  //串口调试
#define DEBUG_PORT_ETH  2  //网口调试

#define DEBUG_PRINTF_PORT DEBUG_PORT_UART

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h 中定义此行，表示本函数移到 stam32f4xx_it.c。 避免重复定义 */

#define ERROR_HANDLER()		Error_Handler();

/* 默认是关闭状态 */
#define  Enable_EventRecorder  1

#if Enable_EventRecorder == 1
//	#include "EventRecorder.h"
#endif

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* 定义优先级分组 */
#define NVIC_PREEMPT_PRIORITY	4

typedef unsigned  char 	SOCKET;

#define EXTI9_5_ISR_MOVE_OUT		/* bsp.h 中定义此行，表示本函数移出 stam32h7xx_it.c。 避免重复定义 */



/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
//#include "bsp_msg.h"
//#include "bsp_user_lib.h"
//#include "bsp_timer.h"
//#include "bsp_led.h"
//#include "bsp_dwt.h"


//#include "bsp_cpu_rtc.h"

#include "bsp_spi_bus.h"


#include "bsp_wizchip_conf.h"
#include "bsp_w5500.h"
#include "bsp_socket.h"
#include "bsp_ethernet.h"


/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

void bsp_GetCpuID(uint32_t *_id);
//void Error_Handler(char *file, uint32_t line);
extern void SystemClock_Config(void);
extern void Debug_Printf(const char * szFormat, ...);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
