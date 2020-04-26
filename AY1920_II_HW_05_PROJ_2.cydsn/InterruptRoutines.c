/* ========================================
 * The ISR caused by the timer is added to be sure to read data at a constant rate. 
 * The ISR is called with a frequency higher than the required frequency (100Hz) 
 * to avoid data loss, but the data are read only if a new data is available, 
 * thanks to the Status Register check. 
 *
 * ========================================
*/

#include "InterruptRoutines.h"

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18
/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

// New value check
#define LIS3DH_STATUS_REG_NEW_VALUE 0x08 //00001000 

/*
    ADRESS register OUT_X_L
*/
#define LIS3DH_OUT_X_L 0x28


#define REGISTER_COUNT 6

int error;
uint8_t status_register_;

CY_ISR(Custom_TIMER_ISR){
    
    Timer_ReadStatusRegister(); // Read Timer status register to bring interrupt line low
    
    //Read of the Status Register to check if a new data is available
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register_);
    
    if((error == NO_ERROR) && ((status_register_) & (LIS3DH_STATUS_REG_NEW_VALUE))) //if ZYXDA pin is high, a new data is avaiable
    {
        /* Multi-Read to read all the 6 (3 direction and for each one there is a 
        Low register and a High register) registers containing the acceleration data. */
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_X_L, REGISTER_COUNT,
                                            &AccData[0]);
        
        if (error == NO_ERROR)
        {
            PacketReadyFlag = 1; //Data are available and read
        }    
    }
}
/* [] END OF FILE */
