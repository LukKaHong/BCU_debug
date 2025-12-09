#include "DO.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Write_DO_ctrl_Pin(uint8_t port, GPIO_PinState state)
{
    switch (port)
    {
    case 1:
        return HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, state);
    case 2:
        return HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, state);
    case 3:
        return HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, state);
    case 4:
        return HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, state);
    case 5:
        return HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, state);
    case 6:
        return HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, state);
    default:
        break;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void DO_Ctrl(DO_Ctrl_e ctrl, uint8_t onoff)
{
    for (uint8_t i = 1; i <= PortConfig_DO_Num; i++)
    {
        PortConfig_DO_t *_do = GetPortConfig_DO(i);

        if(_do == NULL)
            continue;

        if (_do->ctrl == ctrl)
        {
            Write_DO_ctrl_Pin(i, (GPIO_PinState)(onoff == 1 ? _do->valid : (!_do->valid)));
            break;
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/






















