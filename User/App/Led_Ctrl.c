#include "Led_Ctrl.h"

/* Internal counters for blinking logic */
static uint32_t Run_Led_Cnt = 0;
static uint32_t Comm_Led_Cnt = 0;
static uint32_t Fault_Led_Cnt = 0;

/* Blink periods in 100ms units (SysManage_Task cycle) */
#define BLINK_SLOW_PERIOD       10  // 10 * 100ms = 1000ms period (500ms on/off)
#define BLINK_FAST_PERIOD       2   // 2 * 100ms = 200ms period (100ms on/off)

/**
  * @brief  Initialize LED pins
  */
void Led_Init(void)
{
    /* Note: GPIO clocks should be enabled in main.c/gpio.c */
    
    /* Power LED: Always ON after system starts */
    HAL_GPIO_WritePin(LED_POWER_PORT, LED_POWER_PIN, GPIO_PIN_SET);
    
    /* Initialize other LEDs to OFF */
    HAL_GPIO_WritePin(LED_RUN_PORT, LED_RUN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_COMM_PORT, LED_COMM_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_FAULT_PORT, LED_FAULT_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  Main LED Control Function (Called periodically, e.g., 100ms)
  */
void Led_Ctrl(void)
{
    /* 1. Power LED: Always On (Handled in Init, but can reinforce here) */
    HAL_GPIO_WritePin(LED_POWER_PORT, LED_POWER_PIN, GPIO_PIN_SET);

    /* 2. Running LED: Slow Blink */
    Run_Led_Cnt++;
    if (Run_Led_Cnt >= (BLINK_SLOW_PERIOD / 2))
    {
        HAL_GPIO_TogglePin(LED_RUN_PORT, LED_RUN_PIN);
        Run_Led_Cnt = 0;
    }

    /* 3. Communication LED:
     * Logic: Blink if connected to EMS, OFF if not.
     * Fault == 1 -> Disconnected -> OFF
     * Fault == 0 -> Connected -> Blink
     */
    if (GetDeviceCommPointer()->ECU.Fault == 0) // Connected
    {
        Comm_Led_Cnt++;
        if (Comm_Led_Cnt >= (BLINK_SLOW_PERIOD / 2))
        {
            HAL_GPIO_TogglePin(LED_COMM_PORT, LED_COMM_PIN);
            Comm_Led_Cnt = 0;
        }
    }
    else // Disconnected
    {
        HAL_GPIO_WritePin(LED_COMM_PORT, LED_COMM_PIN, GPIO_PIN_RESET);
        Comm_Led_Cnt = 0;
    }

    /* 4. Fault LED:
     * Logic:
     * Lv0: Off
     * Lv1: Slow Blink
     * Lv2: Medium Blink
     * Lv3: Fast Blink
     * Lv4: Very Fast Blink
     */
    uint32_t blink_threshold = 0;

    switch (GetSysInfoPointer()->SysFaultLv)
    {
        case SysFaultLv_0:
            blink_threshold = 0; // Off
            break;
        case SysFaultLv_1:
            blink_threshold = 10; // 1000ms period
            break;
        case SysFaultLv_2:
            blink_threshold = 5;  // 500ms period
            break;
        case SysFaultLv_3:
            blink_threshold = 2;  // 200ms period
            break;
        case SysFaultLv_4:
            blink_threshold = 1;  // 100ms period (toggle every tick)
            break;
        default:
            blink_threshold = 10;
            break;
    }

    if (blink_threshold == 0)
    {
        HAL_GPIO_WritePin(LED_FAULT_PORT, LED_FAULT_PIN, GPIO_PIN_RESET);
        Fault_Led_Cnt = 0;
    }
    else
    {
        Fault_Led_Cnt++;
        if (Fault_Led_Cnt >= blink_threshold)
        {
            HAL_GPIO_TogglePin(LED_FAULT_PORT, LED_FAULT_PIN);
            Fault_Led_Cnt = 0;
        }
    }
}
