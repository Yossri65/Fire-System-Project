/* 
 * File:   Application.h
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:08 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H


/* ------------------------------------Includes--------------------------------------*/
#include "ECU_Layer/ecu_intialize.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/Interrupt/mcal_internal_interrupt.h"
#include "MCAL_Layer/EEPROM/mcal_eeprom.h"
#include "MCAL_Layer/MCAL_ADC/mcal_adc.h"

#include "MCAL_Layer/Timer0/mcal_timer0.h"
#include "MCAL_Layer/Timer1/mcal_timer1.h"
#include "MCAL_Layer/Timer2/mcal_timer2.h"
#include "MCAL_Layer/Timer3/mcal_timer3.h"
#include "MCAL_Layer/CCP1/hal_ccp.h"
#include "MCAL_Layer/ECCP/hal_eccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/SPI/hal_spi.h"
#include "MCAL_Layer/I2C/hal_i2c.h"
/* ------------------------------------Macro Declarations--------------------------- */
#define true  1
#define false 0
/* ------------------------------------Macro Function Declarations------------------ */

/* ------------------------------------Data Type Declarations----------------------- */
extern Ch_LCD lcd1;
extern Key_Pad_t key_pad1;
extern Dc_Motor_t dc_motor1;
extern Led_t led_red;
extern Led_t led_yellow;


uint8 value_PRESSED = NOT_PRESSED;
uint8 check_pass_1 = 0 ,check_pass_2 = 0 ,check_pass_3 = 0;
uint8 eeprom_address = 0X0000;
uint8 counter_set_new = 1 ,counter_set_new_read = 0;
uint8 pass_status = false;
uint16 conv_result_temp ,conv_result_smoke;
uint8 adc_flag = 0 ;
float step_size_temp_smk = 0 ;
uint16 temperature = 0 ,smoke_level = 0;
uint8 smoke_level_rate;
uint8 temperature_txt[7], smoke_level_txt[7];

uint8 flag;


void ADC_Temp_Smk_ISR(void);
ADC_Conf_t adc_temp_smk =
{
  .ADC_IntterruptHandeler = ADC_Temp_Smk_ISR ,
  .ADC_Acquisition_Time =  ADC_12_TAD ,
  .ADC_Conversion_Clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16 ,
  .ADC_Channel = ADC_CHANNEL_AN0,
  .format_status = ADC_RIGHT_JUSTIFIED ,
  .voltage_ref = ADC_VOLTAGE_REF_DISABLE
};

Pin_Config_t Buzzer =
{
  .port = PortB_Index,
  .pin = pin2 ,
  .direction = Direction_Outpt,
  .logic = Logic_Low
};

USART_CONFIG_t uart =
{
  .baudrate = 9600,
  .buad_rate_gen = BAUDRATE_ASYN_8BIT_lOW_SPEED,
  
  .sync_async_mode = EUART_ASYNCHRONOUS_MODE ,
  .UART_TX_Enable.TX_Enable = EUSART_ASYNCHRONOUS_TX_ENABLE ,
  .UART_TX_Enable.TX_9Bit_Enable = EUART_ASYNCHRONOUS_TX_9BIT_DISABLE,
  .UART_TX_Enable.TX_Interrupt_Enable = EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE,
  
  .UART_RX_Enable.RX_Enable= EUSART_ASYNCHRONOUS_RX_ENABLE,
  .UART_RX_Enable.RX_9Bit_Enable = EUART_ASYNCHRONOUS_RX_9BIT_DISABLE,
  .UART_RX_Enable.RX_Interrupt_Enable = EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE,
  .EUSART_FRAMINGERRORDefaultHandler = NULL,
  .EUSART_OVERRUNERRORDefaultHandler =NULL,
  .EUSART_RXDefaultInterruptHandler =NULL,
  .EUSART_TxDefaultInterruptHandler =NULL
};

/* ------------------------------------Software Interface Declarations-------------- */



#endif	/* APPLICATION_H */

