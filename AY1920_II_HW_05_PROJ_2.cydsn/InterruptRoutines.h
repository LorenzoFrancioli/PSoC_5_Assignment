/* ========================================
 * Interrupt Routines header file
 *
*/
#ifndef _INTERRUPT_ROUTINES_H 
    
    #define _INTERRUPT_ROUTINES_H 
    
    #include "project.h" 
    #include "I2C_Interface.h"
    
    CY_ISR_PROTO(Custom_TIMER_ISR); 
    
    uint8_t AccData[6]; // Acceleration data, 2 for each of the 3 direction (X, Y, Z)
    
    volatile uint8 PacketReadyFlag; //Flag to communicate when new data are available and read, to be sent then with UART
 
#endif
/* [] END OF FILE */
