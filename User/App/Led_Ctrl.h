#ifndef __LED_CTRL_H__
#define __LED_CTRL_H__

#include "main.h"
#include "SysManage.h"

/* LED GPIO Definition - Adjust these based on actual hardware */
/* Assuming LED_DEBUG (PC1) is the Running LED */
#define LED_RUN_PIN         NULL
#define LED_RUN_PORT        NULL

/* Placeholders for other LEDs - Please update with actual pins */
/* Avoid using PC0 (WDI) */
#define LED_POWER_PIN       NULL
#define LED_POWER_PORT      NULL

#define LED_COMM_PIN        NULL
#define LED_COMM_PORT       NULL

#define LED_FAULT_PIN       NULL
#define LED_FAULT_PORT      NULL

/* LED Control Interface */
void Led_Init(void);
void Led_Ctrl(void);

#endif /* __LED_CTRL_H__ */
