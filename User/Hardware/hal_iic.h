#ifndef _HAL_IIC_H
#define _HAL_IIC_H


#include "main.h"

typedef enum
{
	IIC_ERR_OK,                 // 正确
	IIC_ERR_WRARRAY             // 错误
}IIC_ErrType;


typedef enum
{
	IIC_EE,
	IIC_RTC,
	IIC_INDEX_MAX_NUM
}IIC_IndexType;




/*EE*/
#define		EE_SCL_PORT			EEPROM_SCL_GPIO_Port
#define		EE_SCL_PIN			EEPROM_SCL_Pin

#define		EE_SDA_PORT			EEPROM_SDA_GPIO_Port
#define		EE_SDA_PIN			EEPROM_SDA_Pin

/*RTC*/
#define		RTC_SCL_PORT		RTC_I2C1_SCL_GPIO_Port
#define		RTC_SCL_PIN			RTC_I2C1_SCL_Pin

#define		RTC_SDA_PORT		RTC_I2C1_SDA_GPIO_Port
#define		RTC_SDA_PIN			RTC_I2C1_SDA_Pin


/**********************************************************************************************************************
 *                                                 全局函数原型                                                       *
 *********************************************************************************************************************/
void IIC_SendStart(IIC_IndexType iic_index);
void IIC_SendStop(IIC_IndexType iic_index);
uint8_t IIC_WaitAck(IIC_IndexType iic_index);
void IIC_Ack(IIC_IndexType iic_index);
void IIC_NAck(IIC_IndexType iic_index);
void IIC_WriteByte(IIC_IndexType iic_index,uint8_t byte);
uint8_t IIC_ReadByte(IIC_IndexType iic_index);







#endif
