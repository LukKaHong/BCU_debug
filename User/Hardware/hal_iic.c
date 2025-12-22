#include "hal_iic.h"




typedef enum
{
    IIC_LOW,                    // 低电平
    IIC_HIGH                    // 高电平
}IIC_OutputType;                // 输出类型

typedef enum
{
    IIC_OUTPUT,                 // 输出
    IIC_INPUT                   // 输入
}IIC_DirectionType;             // 引脚方向类型


/**********************************************************************************************************************
 *                                                 局部函数原型                                                       *
 *********************************************************************************************************************/
static void IIC_SetSdaDirection(IIC_IndexType iic_index,IIC_DirectionType direction);
static void IIC_SetSdaOut(IIC_IndexType iic_index,IIC_OutputType output);
static uint8_t IIC_ReadSdaIn(IIC_IndexType iic_index);
static void IIC_SetSclOut(IIC_IndexType iic_index,IIC_OutputType output);
static void IIC_DelayUs(uint32_t time);

/**********************************************************************************************************************
 *                                                 全局函数实现                                                       *
 *********************************************************************************************************************/
/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_SendStart
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :发送起始信号
 * *
 * *****************************************************************************/
void IIC_SendStart(IIC_IndexType iic_index)
{
    IIC_SetSclOut(iic_index,IIC_HIGH);//SCL-1
    IIC_SetSdaDirection(iic_index,IIC_OUTPUT);//SDA配置成输出模式
    IIC_SetSdaOut(iic_index,IIC_HIGH);//SDA-1
    IIC_SetSclOut(iic_index,IIC_HIGH);//SCL-1
    IIC_DelayUs(4);
    IIC_SetSdaOut(iic_index,IIC_LOW);//SDA-0
    IIC_DelayUs(4);
    IIC_SetSclOut(iic_index,IIC_LOW);//SCL-0
    IIC_DelayUs(4);
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_SendStop
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :发送停止信号
 * *
 * *****************************************************************************/
void IIC_SendStop(IIC_IndexType iic_index)
{
    IIC_SetSdaDirection(iic_index,IIC_OUTPUT);//SDA设置成输出
    IIC_SetSclOut(iic_index,IIC_LOW);//SCL-0
    IIC_SetSdaOut(iic_index,IIC_LOW);	//SDA-0
    IIC_DelayUs(4);
    IIC_SetSclOut(iic_index,IIC_HIGH);  //scl-1
    IIC_DelayUs(4);
    IIC_SetSdaOut(iic_index,IIC_HIGH);//sda-1
    IIC_DelayUs(4);
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_WaitAck
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :等待应答
 * *
 * *****************************************************************************/
uint8_t IIC_WaitAck(IIC_IndexType iic_index)
{
    IIC_ErrType state = IIC_ERR_OK;
    uint8_t err_time = 0;
    IIC_SetSdaDirection(iic_index,IIC_INPUT);//SDA设置成输入模式
    
    IIC_SetSdaOut(iic_index,IIC_HIGH);
    IIC_DelayUs(1);
    IIC_SetSclOut(iic_index,IIC_HIGH);//scl-1
    IIC_DelayUs(1);
    
    while(1 == IIC_ReadSdaIn(iic_index))
    {
        err_time++;
        if(err_time>250)
        {
            IIC_SendStop(iic_index);
            state= IIC_ERR_WRARRAY;
            return state;
        }
    }
    IIC_SetSclOut(iic_index,IIC_LOW);//scl-0
    //IIC_DelayUs(2);

    state = IIC_ERR_OK;
    return state;
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_Ack
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :产生ACK应答
 * *
 * *****************************************************************************/
void IIC_Ack(IIC_IndexType iic_index)
{
    IIC_SetSclOut(iic_index,IIC_LOW);
    IIC_SetSdaDirection(iic_index,IIC_OUTPUT);
    IIC_SetSdaOut(iic_index,IIC_LOW);
    IIC_DelayUs(2);
    IIC_SetSclOut(iic_index,IIC_HIGH);
    IIC_DelayUs(2);
    IIC_SetSclOut(iic_index,IIC_LOW);
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_NAck
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :不产生ACK应答
 * *
 * *****************************************************************************/
void IIC_NAck(IIC_IndexType iic_index)
{
    IIC_SetSclOut(iic_index,IIC_LOW);
    IIC_SetSdaDirection(iic_index,IIC_OUTPUT);
    IIC_SetSdaOut(iic_index,IIC_HIGH);
    IIC_DelayUs(2);
    IIC_SetSclOut(iic_index,IIC_HIGH);
    IIC_DelayUs(2);
    IIC_SetSclOut(iic_index,IIC_LOW);
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_WriteByte
 * *     >参    数  :外设编号，字节
 * *     >返 回 值  :void
 * *     >描    述  :写字节
 * *
 * *****************************************************************************/
void IIC_WriteByte(IIC_IndexType iic_index,uint8_t byte)
{
    uint8_t i =0;
    IIC_SetSdaDirection(iic_index,IIC_OUTPUT);//SDA设置成输出模式
    IIC_SetSclOut(iic_index,IIC_LOW);//SCL- 0
    for(i = 0; i < 8; i++)
    {
        IIC_SetSdaOut(iic_index,(IIC_OutputType)((byte & 0x80) >> 7));//SDA输出电平
        byte <<= 1;
        IIC_DelayUs(2);
        IIC_SetSclOut(iic_index,IIC_HIGH);//SCL - 1
        IIC_DelayUs(2);
        IIC_SetSclOut(iic_index,IIC_LOW);//SCL - 0
        IIC_DelayUs(2);
    }
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_ReadByte
 * *     >参    数  :外设编号
 * *     >返 回 值  :void
 * *     >描    述  :读字节
 * *
 * *****************************************************************************/
uint8_t IIC_ReadByte(IIC_IndexType iic_index)
{	
    uint8_t i =0;
    uint8_t rcv_data = 0;

    IIC_SetSdaDirection(iic_index,IIC_INPUT);
    for(i = 0; i < 8; i++)
    {
        IIC_SetSclOut(iic_index,IIC_LOW);
        IIC_DelayUs(2);
        IIC_SetSclOut(iic_index,IIC_HIGH);
        rcv_data <<= 1;
        if(1 == IIC_ReadSdaIn(iic_index))
        {
            rcv_data |= 0x01;
        }
        IIC_DelayUs(1);
    }	
    return  rcv_data;
}

/**********************************************************************************************************************
 *                                                 局部函数实现                                                       *
 *********************************************************************************************************************/
/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_SetSdaDirection
 * *     >参    数  :设备编号,(IIC_OUTPUT：输出	IIC_INPUT：输入）
 * *     >返 回 值  :void
 * *     >描    述  :设置SDA引脚方向
 * *
 * *****************************************************************************/
static void IIC_SetSdaDirection(IIC_IndexType iic_index,IIC_DirectionType direction)
{
    switch((uint8_t)iic_index)
    {
        case IIC_EE:
            {
                if(IIC_OUTPUT == direction)
                {
                    EE_SDA_PORT->MODER&=~((uint32_t)3<<(Get_GPIO_Pin_No(EEPROM_SDA_Pin)*2));
                    EE_SDA_PORT->MODER|=(1<<(Get_GPIO_Pin_No(EEPROM_SDA_Pin)*2));
                }
                else if(IIC_INPUT == direction)
                {
                    EE_SDA_PORT->MODER&=~((uint32_t)3<<(Get_GPIO_Pin_No(EEPROM_SDA_Pin)*2));
                    EE_SDA_PORT->MODER|=(0<<(Get_GPIO_Pin_No(EEPROM_SDA_Pin)*2));
                }
            }
            break;
        case IIC_RTC:
            {
                if(IIC_OUTPUT == direction)
                {
                    RTC_SDA_PORT->MODER&=~(3<<(Get_GPIO_Pin_No(RTC_I2C1_SDA_Pin)*2));
                    RTC_SDA_PORT->MODER|=(1<<(Get_GPIO_Pin_No(RTC_I2C1_SDA_Pin)*2));
                }
                else if(IIC_INPUT == direction)
                {
                    RTC_SDA_PORT->MODER&=~(3<<(Get_GPIO_Pin_No(RTC_I2C1_SDA_Pin)*2));
                    RTC_SDA_PORT->MODER|=(0<<(Get_GPIO_Pin_No(RTC_I2C1_SDA_Pin)*2));
                }
            }
            break;	
        default:
            break;
    }
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_SetSdaOut
 * *     >参    数  :外设编号,(IIC_LOW:低电平	IIC_HIGH：高电平)
 * *     >返 回 值  :void
 * *     >描    述  :设置SDA引脚输出
 * *
 * *****************************************************************************/
static void IIC_SetSdaOut(IIC_IndexType iic_index,IIC_OutputType output)
{
    switch((uint8_t)iic_index)
    {
        case IIC_EE:
        {
            if(IIC_HIGH == output)
            {
                HAL_GPIO_WritePin(EE_SDA_PORT,EE_SDA_PIN,GPIO_PIN_SET);
            }
            else if(IIC_LOW == output)
            {
				HAL_GPIO_WritePin(EE_SDA_PORT,EE_SDA_PIN,GPIO_PIN_RESET);
            }
        }
        break;
        
        case IIC_RTC:
        {
            if(IIC_HIGH == output)
            {
                HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_SET);
            }
            else if(IIC_LOW == output)
            {
				HAL_GPIO_WritePin(RTC_SDA_PORT,RTC_SDA_PIN,GPIO_PIN_RESET);
            }
        }
        break;	
        default:
        break;
    }
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_ReadSdaIn
 * *     >参    数  :外设编号
 * *     >返 回 值  :0：低电平	1：高电平
 * *     >描    述  :读取SDA引脚输入状态
 * *
 * *****************************************************************************/
static uint8_t IIC_ReadSdaIn(IIC_IndexType iic_index)
{
    switch((uint8_t)iic_index)
    {
        case IIC_EE:
            return HAL_GPIO_ReadPin(EE_SDA_PORT,EE_SDA_PIN);
        case IIC_RTC:
            return HAL_GPIO_ReadPin(RTC_SDA_PORT,RTC_SDA_PIN);
        default:
            break;
    }
    return 0;
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_ReadSdaIn
 * *     >参    数  :外设编号，(IIC_LOW:低电平	IIC_HIGH：高电平)
 * *     >返 回 值  :void
 * *     >描    述  :设置SCL引脚输出状态
 * *
 * *****************************************************************************/
static void IIC_SetSclOut(IIC_IndexType iic_index,IIC_OutputType output)
{
    switch((uint8_t)iic_index)
    {
        case IIC_EE:
        {
            if(IIC_HIGH == output)
            {
				HAL_GPIO_WritePin(EE_SCL_PORT,EE_SCL_PIN,GPIO_PIN_SET);
            }
            else if(IIC_LOW == output)
            {
				HAL_GPIO_WritePin(EE_SCL_PORT,EE_SCL_PIN,GPIO_PIN_RESET);
            }
        }
        break;
        
        case IIC_RTC:
        {
            if(IIC_HIGH == output)
            {
				HAL_GPIO_WritePin(RTC_SCL_PORT,RTC_SCL_PIN,GPIO_PIN_SET);
            }
            else if(IIC_LOW == output)
            {
				HAL_GPIO_WritePin(RTC_SCL_PORT,RTC_SCL_PIN,GPIO_PIN_RESET);
            }
        }
        break;	
        default:
            break;
    }
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2023年12月07日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :IIC_DelayUs
 * *     >参    数  :时间(单位：us)
 * *     >返 回 值  :void
 * *     >描    述  :阻塞式延时
 * *
 * *****************************************************************************/
static void IIC_DelayUs(uint32_t time)
{
	delay_us(time*2);
}








 


