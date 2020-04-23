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
#define LIS3DH_STATUS_REG_NEW_VALUE 0x08

/*
    ADRESS register OUT_X_L
*/
#define LIS3DH_OUT_X_L 0x28

int error;
uint8_t status_register_;

CY_ISR(Custom_TIMER_ISR){
    
    Timer_ReadStatusRegister(); // Read Timer status register to bring interrupt line low
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register_);
    
    if((error == NO_ERROR) && (status_register_ & LIS3DH_STATUS_REG_NEW_VALUE))
    {
        error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_OUT_X_L, 6,
                                            &AccData[0]);
        
        if (error == NO_ERROR)
        {
            PacketReadyFlag = 1;
        }    
    }
}
/* [] END OF FILE */
