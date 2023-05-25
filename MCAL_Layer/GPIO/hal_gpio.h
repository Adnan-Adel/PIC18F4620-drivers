/* 
 * File:   hal_gpio.h
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 2:07 PM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/* Section: Includes */
#include "../mcal_std_types.h"
#include "pic18f4620.h"
#include "../device_config.h"
#include "hal_gpio_cfg.h"

/* Section: Macro Declarations */
#define PINS_MAX_NUMBER          8
#define PORTS_MAX_NUMBER         5

/* Section: Macro Functions Declarations */
#define HWREG8(_x)      (*((volatile uint8_t *)(_x)))

#define SET_BIT(REG,BIT_POS)        (REG |= (1 << BIT_POS))
#define CLEAR_BIT(REG,BIT_POS)      (REG &= ~(1 << BIT_POS))
#define TOGGLE_BIT(REG,BIT_POS)     (REG ^= (1 << BIT_POS))
#define GET_BIT(REG,BIT_POS)        ((REG >> BIT_POS) & 0x01)

/* Section: Data Type Declarations */
typedef enum
{
    GPIO_LOW= 0,
    GPIO_HIGH
}logic_t;

typedef enum
{
    GPIO_DIRECTION_OUTPUT= 0,
    GPIO_DIRECTION_INPUT
}direction_t;

typedef enum
{
    GPIO_PIN0= 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7
}pin_index_t;

typedef enum
{
    PORTA_INDEX= 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX
}port_index_t;

typedef struct
{
  uint8_t port : 3;         /* @ref port_index_t */
  uint8_t pin : 3;          /* @ref pin_index_t */
  uint8_t direction : 1;    /* @ref direction_t */
  uint8_t logic : 1;        /* @ref logic_t */
}pin_config_t;

/* Section: Software Interface Declaration*/
std_ReturnType gpio_pin_direction_initialize(const pin_config_t *pin_config);
std_ReturnType gpio_pin_get_direction_status(const pin_config_t *pin_config, direction_t *direction_status);
std_ReturnType gpio_pin_write_logic(const pin_config_t *pin_config, logic_t logic);
std_ReturnType gpio_pin_read_logic(const pin_config_t *pin_config, logic_t *logic_value);
std_ReturnType gpio_pin_toggle_logic(const pin_config_t *pin_config);
std_ReturnType gpio_pin_initialize(const pin_config_t *pin_config);

std_ReturnType gpio_port_direction_initialize(port_index_t port, uint8_t direction);
std_ReturnType gpio_port_get_direction_status(port_index_t port, uint8_t *direction_status);
std_ReturnType gpio_port_write_logic(port_index_t port, uint8_t logic);
std_ReturnType gpio_port_read_logic(port_index_t port, uint8_t *logic_value);
std_ReturnType gpio_port_toggle_logic(port_index_t port);




#endif	/* HAL_GPIO_H */

