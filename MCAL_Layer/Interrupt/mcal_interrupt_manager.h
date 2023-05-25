/* 
 * File:   mcal_interrupt_manager.h
 * Author: Adnan
 *
 * Created on May 7, 2023, 5:19 PM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/* Section: Includes */
#include "mcal_interrupt_config.h"

/* Section: Macro Declarations */


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */


/* Section: Software Interface Declaration*/
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

void RB4_ISR(uint8_t RB4_src);
void RB5_ISR(uint8_t RB5_src);
void RB6_ISR(uint8_t RB6_src);
void RB7_ISR(uint8_t RB7_src);

#endif	/* MCAL_INTERRUPT_MANAGER_H */

