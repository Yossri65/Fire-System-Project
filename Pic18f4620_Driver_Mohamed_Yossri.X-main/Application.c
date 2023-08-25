/* 
 * File:   Application.c
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:09 PM
 */ 
#include <builtins.h>

#include "Application.h"
Std_ReturnType ret = E_NOT_OK;
void intialization (void);
uint8 wrong = 0;


int main(void) {
    intialization();
    
    
    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 7 , "Welcome");
    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 3 , "To Fire System");
    __delay_ms(1000);
    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "1:Enter New Pass");
    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "2:Enter Exist Pass");
    while(1)
    {
        /*adc start conv to get temperature and smoke level*/
        if(0 == adc_flag){
             ret = ADC_Start_Conversion_Interrupt_Mode(&adc_temp_smk ,ADC_CHANNEL_AN0);
        }else if(1 == adc_flag){
            ret = ADC_Start_Conversion_Interrupt_Mode(&adc_temp_smk ,ADC_CHANNEL_AN1);
        }
        else{/*Nothing*/}
       /*convert digital output to analog for processing*/
        temperature = 4.88f * conv_result_temp;
        smoke_level = 4.88f * conv_result_smoke;
        
        temperature /= 10;
        smoke_level /= 10;
        
        smoke_level_rate = (smoke_level*100)/ 500;
        ret = convert_short_to_string(temperature, temperature_txt);
        ret = convert_short_to_string(smoke_level_rate, smoke_level_txt);
        
        
        uint8 first_digit = NOT_PRESSED , Second_digit = NOT_PRESSED ,third_digit = NOT_PRESSED;
        ret = Key_Pad_Get_Value(&key_pad1 ,&value_PRESSED);
        
        
        if(NOT_PRESSED != value_PRESSED)
        {
            /*put limit to set new pass this limit is three time only*/
            ret = EEPROM_Data_ReadByte(0X0010 , &counter_set_new_read);
            if(value_PRESSED == '-'){
                    value_PRESSED = NOT_PRESSED;
                    ret = lcd_send_char_data(&lcd1 ,' ');
                    ret = lcd_send_command(&lcd1 , LCD_MOVE_CURSOR_LEFT_SHIFT);
                }
            if(value_PRESSED == '+'){
                __delay_ms(300);
                    value_PRESSED = NOT_PRESSED;
                    pass_status = false;
                    ret = Dc_Motor_Stop(&dc_motor1);
                    ret = Led_Turn_Off(&led_red);
                    ret = GPIO_Pin_Write_Logic(&Buzzer , Logic_Low);
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    break;
                }
            else if('1' == value_PRESSED && counter_set_new_read <3){
                __delay_ms(500);
                /*store counter_set_new in eeprom in 0x0010 to know how many time u set a new password */
                ret = EEPROM_Data_WriteByte(0X0010 ,counter_set_new);
                counter_set_new++;
                
                /*take pass from user and store it in eeprom from 0x0000 to 0x0003*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "1:Enter New Pass:");
                __delay_ms(500);    
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&first_digit);
                }while(first_digit == NOT_PRESSED );
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 1 , first_digit);
                ret = EEPROM_Data_WriteByte(eeprom_address ,first_digit);

                eeprom_address++;
                __delay_ms(300);
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&Second_digit);
                }while(Second_digit == NOT_PRESSED );
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 2 , Second_digit);
                ret = EEPROM_Data_WriteByte(eeprom_address ,Second_digit);

                eeprom_address++;
                __delay_ms(300);
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&third_digit);
                }while(third_digit == NOT_PRESSED );
                
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 3 , third_digit);
                ret = EEPROM_Data_WriteByte(eeprom_address ,third_digit);
                
                __delay_ms(500);
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "successfully added");
                value_PRESSED = NOT_PRESSED;
            }
            else if('2' == value_PRESSED)
            {
                __delay_ms(300);
                /*set exist password*/
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "2:Enter Exist Pass");
                __delay_ms(400);
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&first_digit);
                }while(first_digit == NOT_PRESSED );
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 1 , first_digit);
                __delay_ms(400);
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&Second_digit);
                }while(Second_digit == NOT_PRESSED );
                
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 2 , Second_digit);
                __delay_ms(400);
                do
                {
                    ret = Key_Pad_Get_Value(&key_pad1 ,&third_digit);
                }while(third_digit == NOT_PRESSED );
                ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 3 , third_digit);
                __delay_ms(400);
                
                /*take pass from eeprom to check if the pass is wrong or true*/
                eeprom_address = 0;
                ret = EEPROM_Data_ReadByte(eeprom_address , &check_pass_1);
                ret = EEPROM_Data_ReadByte(eeprom_address+1 , &check_pass_2);
                ret = EEPROM_Data_ReadByte(eeprom_address+2 , &check_pass_3);
                if(check_pass_1 != first_digit){
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "Wrong password!!");
                    pass_status = false;
                }else if(check_pass_2 != Second_digit){
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "Wrong password!!");
                    pass_status = false;
                }else if(check_pass_3 != third_digit){
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "Wrong password!!");
                    pass_status = false;
                }else{
                    ret = lcd_send_ATpos_string_data(&lcd1 , 3 , 1 , "success");
                    __delay_ms(500);
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    pass_status = true;
                }
                value_PRESSED = NOT_PRESSED; 
            }
            else {/*Nothing*/}
        }
        
        if(pass_status == true)
        {   
            if(smoke_level_rate <50)
            {
                if(temperature < 50){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "System is Fine");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Temperature : ");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 15 , temperature_txt);
                    ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 17 , 'c');
                    ret = Led_Turn_Off(&led_yellow);
                    __delay_ms(500);
                }else if(temperature >= 50){
                    ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                    ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Heat!!");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 1 , "Temperature : ");
                    ret = lcd_send_ATpos_string_data(&lcd1 , 2 , 15 , temperature_txt);
                    ret = lcd_send_ATpos_char_data(&lcd1 , 2 , 17 , 'c');
                    ret = Led_Turn_On(&led_yellow);
                    __delay_ms(500);
                }else{/*Nothing*/}
                
                ret = Dc_Motor_Stop(&dc_motor1);
                ret = Led_Turn_Off(&led_red);
                ret = GPIO_Pin_Write_Logic(&Buzzer , Logic_Low);
            }else if(temperature >=50 &&smoke_level_rate >50){
                ret = lcd_send_command(&lcd1 , LCD_CLEAR);
                ret = lcd_send_ATpos_string_data(&lcd1 , 1 , 1 , "Fire!!!!");
                ret = lcd_send_ATpos_string_data(&lcd1 , 4 , 1 , "press '+' to restart the program");
                ret = Dc_Motor_Turn_Right(&dc_motor1);
                ret = Led_Turn_On(&led_red);
                ret = GPIO_Pin_Write_Logic(&Buzzer , Logic_High);
                ret = Led_Turn_Off(&led_yellow);
                ret = Key_Pad_Get_Value(&key_pad1 ,&value_PRESSED);
                if(flag == 0){
                    ret = EUSART_ASYNC_WriteStringBlocking("message : fire found!!!!!\r" ,26);
                    flag=1;
                }
                
                __delay_ms(500);
            }else{/*Nothing*/}
            
        }
    }
    return (EXIT_SUCCESS);
}



void intialization (void)
{
    ecu_intialize();
    ret = ADC_Intialization(&adc_temp_smk);
    ret = ADC_Intialization(&adc_temp_smk);
    ret = GPIO_Pin_Direction_Intialize(&Buzzer);
    ret = USART_ASYNC_Intialization(&uart);
}

void ADC_Temp_Smk_ISR(void)
{
    if(0 == adc_flag){
        ret = ADC_Get_Conversion_Result(&adc_temp_smk ,&conv_result_temp);
        adc_flag = 1;
    }else if(1 == adc_flag){
        ret = ADC_Get_Conversion_Result(&adc_temp_smk ,&conv_result_smoke);
        adc_flag = 0;
    }else{/*Nothing*/}
}