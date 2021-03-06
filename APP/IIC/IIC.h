/*********************************Copyright (c)*********************************
**                               
**
**--------------File Info-------------------------------------------------------
** File Name:               sim_i2c.h
** Last modified Date:      
** Last Version:            
** Description:             i2c配置文件
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:            
** Version:                 
** Descriptions:            
**
*******************************************************************************/

/******************************************************************************
更新说明:
    
******************************************************************************/

/******************************************************************************
*********************************  应 用 资 料 ********************************
******************************************************************************/


#ifndef _IIC_H_
#define _IIC_H_

#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
********************************* 文件引用部分 ********************************
******************************************************************************/
#include "SYS.h"
#include <stdbool.h>



//************************** ICC引脚 *********************************
//传感器I2C

/***** 操作宏定义 *****/
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_6)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_6)    
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_7)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_7)
#define SCL_read      (GPIOB->IDR  & GPIO_Pin_6)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_7)


//none
#define I2C_DELAY_VAL   2   //30对应100K,经测试最低到4还能写�,取值[7..255]
                             //4对应400K
                                                          

//I2C完整读写函数
extern void    I2C_IO_Init(void);               //配置I2C接口
extern bool    Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
extern bool    Fast_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
extern bool    Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

//I2C分立操作函数
extern bool I2C_Start(void);              //总线忙活或出错则返回false
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern bool I2C_WaitAck(void); 	          //返回为:=1有ACK,=0无ACK
extern void I2C_SendByte(u8 SendByte);    //数据从高位到低位
extern uint8_t I2C_RadeByte(void);  //数据从高位到低位



/*************************************************************
************************* 其 它 杂 项 ************************
*************************************************************/
//none
 
/******************************************************************************
***********************************   END  ************************************
******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif



