/* 
 * File:   chr_lcd.h
 * Author: Adnan
 *
 * Created on April 25, 2023, 11:45 PM
 */

#ifndef CHR_LCD_H
#define	CHR_LCD_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */
#define _LCD_CLEAR                      0X01
#define _LCD_RETURN_HOME                0X02
#define _LCD_ENTRY_MODE                 0X06
#define _LCD_CURSOR_OFF_DISPLAY_ON      0X0C
#define _LCD_CURSOR_OFF_DISPLAY_OFF     0X08
#define _LCD_CURSOR_ON_BLINK_ON         0X0F
#define _LCD_CURSOR_ON_BLINK_OFF        0X0E
#define _LCD_DISPLAY_SHIFT_RIGHT        0X1C
#define _LCD_DISPLAY_SHIFT_LEFT         0X18
#define _LCD_8BIT_MODE_2_LINE           0X38
#define _LCD_4BIT_MODE_2_LINE           0X28
#define _LCD_CGRAM_START                0X40
#define _LCD_DDRAM_START                0X80

#define ROW1    1
#define ROW2    2
#define ROW3    3
#define ROW4    4


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef struct
{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[4];
}chr_lcd_4bit_t;

typedef struct
{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[8];
}chr_lcd_8bit_t;

/* Section: Software Interface Declaration*/
std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd_obj);
std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd_obj, uint8_t command);
std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd_obj, uint8_t data);
std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t data);
std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd_obj, uint8_t *str);
std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t *str);
std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd_obj, uint8_t row, uint8_t coloumn, 
                                                                const uint8_t _chr[], uint8_t mem_pos);

std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd_obj);
std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd_obj, uint8_t command);
std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd_obj, uint8_t data);
std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t data);
std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd_obj, uint8_t *str);
std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, uint8_t *str);
std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd_obj, uint8_t row, uint8_t coloumn, 
                                                                const uint8_t _chr[], uint8_t mem_pos);

std_ReturnType convert_byte_to_string(uint8_t value, uint8_t *str);
std_ReturnType convert_short_to_string(uint16_t value, uint8_t *str);
std_ReturnType convert_int_to_string(uint32_t value, uint8_t *str);

#endif	/* CHR_LCD_H */

