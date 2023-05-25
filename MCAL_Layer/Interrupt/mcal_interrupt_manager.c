/* 
 * File:   mcal_interrupt_manager.c
 * Author: Adnan
 *
 * Created on May 7, 2023, 5:19 PM
 */
#include "mcal_interrupt_manager.h"

static volatile uint8_t RB4_flag=1, RB5_flag=1, RB6_flag=1, RB7_flag=1;

#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
void __interrupt() InterruptManagerHigh(void)
{
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {
        INT0_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR();
    }
    else{ /* Nothing */ }
}

void __interrupt(low_priority) InterruptManagerLow(void)
{
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF))
    {
        INT1_ISR();
    }
    else{ /* Nothing */ }
}
#else
void __interrupt() InterruptManagerHigh(void)
{
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF))
    {
        INT0_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF))
    {
        INT1_ISR();
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF))
    {
        INT2_ISR();
    }
    else{ /* Nothing */ }
    /* -------------------------------- PORTB External on change Interrupt -------------------------------- */
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_HIGH == PORTBbits.RB4) && (RB4_flag == 1))
    {
        RB4_flag= 0;
        RB4_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_LOW == PORTBbits.RB4) && (RB4_flag == 0))
    {
        RB4_flag= 1;
        RB4_ISR(1);
    }
    else{ /* Nothing */ }
    
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_HIGH == PORTBbits.RB5) && (RB5_flag == 1))
    {
        RB5_flag= 0;
        RB5_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_LOW == PORTBbits.RB5) && (RB5_flag == 0))
    {
        RB5_flag= 1;
        RB5_ISR(1);
    }
    else{ /* Nothing */ }
    
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_HIGH == PORTBbits.RB6) && (RB6_flag == 1))
    {
        RB6_flag= 0;
        RB6_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_LOW == PORTBbits.RB6) && (RB6_flag == 0))
    {
        RB6_flag= 1;
        RB6_ISR(1);
    }
    else{ /* Nothing */ }
    
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_HIGH == PORTBbits.RB7) && (RB7_flag == 1))
    {
        RB7_flag= 0;
        RB7_ISR(0);
    }
    else{ /* Nothing */ }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) && (GPIO_LOW == PORTBbits.RB7) && (RB7_flag == 0))
    {
        RB7_flag= 1;
        RB7_ISR(1);
    }
    else{ /* Nothing */ }
}

#endif