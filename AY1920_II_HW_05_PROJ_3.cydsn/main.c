/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
* \author Gabriele Belotti
* \date , 2020
*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18

/**
*   \brief Address of the WHO AM I register
*/
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

/**
*   \brief Hex value to set normal mode to the accelerator
*/
#define LIS3DH_NORMAL_MODE_CTRL_REG1 0x47

/**
*   \brief  Address of the Temperature Sensor Configuration register
*/
#define LIS3DH_TEMP_CFG_REG 0x1F

#define LIS3DH_TEMP_CFG_REG_ACTIVE 0xC0

/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x80

/**
*   \brief Address of the ADC output LSB register
*/
#define LIS3DH_OUT_ADC_3L 0x0C

/**
*   \brief Address of the ADC output MSB register
*/
#define LIS3DH_OUT_ADC_3H 0x0D

/*
    ADRESS register OUT_X_L
*/
#define LIS3DH_OUT_X_L 0x28

/**
*   \brief Hex value to set normal mode to the accelerator
*/
#define LIS3DH_HIGH_MODE_CTRL_REG1_ACC 0x57 //01010111 to set 100 Hz, high resolution mode and enable X-,Y-,Z-axis

#define LIS3DH_CTRL_REG4_HIGH_MODE_4G 0x18  //00011000 to set full scale range +-4g (with FS1 FS0), high resolution mode

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Peripheral_Start();
    UART_Debug_Start();
    Timer_Start();
    isr_TIMER_StartEx(Custom_TIMER_ISR);
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }
    
    /*      I2C Reading Status Register       */
    
    uint8_t status_register; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
    
    if (ctrl_reg1 != LIS3DH_HIGH_MODE_CTRL_REG1_ACC)
    {
        ctrl_reg1 = LIS3DH_HIGH_MODE_CTRL_REG1_ACC;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
        }
    }
    
    
    
    
     /******************************************/
     /* I2C Reading Control register 4 */
     /******************************************/

    
    uint8_t ctrl_reg4;
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    if (ctrl_reg4 != LIS3DH_CTRL_REG4_HIGH_MODE_4G)
    {
        ctrl_reg4 = LIS3DH_CTRL_REG4_HIGH_MODE_4G; // must be changed to the appropriate value
        
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG4,
                                             ctrl_reg4); 
         if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 successfully written as: 0x%02X\r\n", ctrl_reg4);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
        
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_CTRL_REG4,
                                            &ctrl_reg4);
        
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
        }
    }
    
    int16_t OutAccX;
    int16_t OutAccY;
    int16_t OutAccZ;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[14]; 
    float32 AccXms2f;
    float32 AccYms2f;
    float32 AccZms2f;
    int32 AccXms2;
    int32 AccYms2;
    int32 AccZms2;
    
    OutArray[0] = header;
    OutArray[13] = footer;

    PacketReadyFlag = 0;
    
    for(;;)
    {
  
        if (PacketReadyFlag == 1)
        {
            PacketReadyFlag = 0;
            OutAccX = ((int16)((AccData[0]) | ((AccData[1])<<8))>>4)*2;
            OutAccY = ((int16)((AccData[2]) | ((AccData[3])<<8))>>4)*2;
            OutAccZ = ((int16)((AccData[4]) | ((AccData[5])<<8))>>4)*2;
            AccXms2f = (OutAccX)*9.81;
            AccYms2f = (OutAccY)*9.81;
            AccZms2f = (OutAccZ)*9.81;
            AccXms2 = (int32) AccXms2f;
            AccYms2 = (int32) AccYms2f;
            AccZms2 = (int32) AccZms2f;
            OutArray[1] = (uint8_t)(AccXms2 & 0xFF); 
            OutArray[2] = (uint8_t)((AccXms2 >> 8) & 0xFF);
            OutArray[3] = (uint8_t)((AccXms2 >> 16) & 0xFF);
            OutArray[4] = (uint8_t)(AccXms2 >> 24);
            OutArray[5] = (uint8_t)(AccYms2 & 0xFF); 
            OutArray[6] = (uint8_t)((AccYms2 >> 8) & 0xFF);
            OutArray[7] = (uint8_t)((AccYms2 >> 16) & 0xFF);
            OutArray[8] = (uint8_t)(AccYms2 >> 24);
            OutArray[9] = (uint8_t)(AccZms2 & 0xFF); 
            OutArray[10] = (uint8_t)((AccZms2 >> 8) & 0xFF);
            OutArray[11] = (uint8_t)((AccZms2 >> 16) & 0xFF);
            OutArray[12] = (uint8_t)((AccZms2 >> 24) & 0xFF);
            UART_Debug_PutArray(OutArray, 14);
            
            
        }
    }
}

/* [] END OF FILE */