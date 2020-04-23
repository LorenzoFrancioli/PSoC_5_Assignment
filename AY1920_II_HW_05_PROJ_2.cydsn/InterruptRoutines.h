/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef _INTERRUPT_ROUTINES_H 
    
    #define _INTERRUPT_ROUTINES_H 
    #include "project.h" 
    #include "I2C_Interface.h"
    CY_ISR_PROTO(Custom_TIMER_ISR); 
    
    uint8_t AccData[6];
    volatile uint8 PacketReadyFlag;
 
#endif
/* [] END OF FILE */
