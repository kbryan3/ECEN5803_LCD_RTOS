# ECEN5803_LCD_RTOS
Project uses the mbed-rtos to run a few peripherals via multiple threads including an NHD_0216HZ LCD, DS131 temp sensor, LED and Potentiometer.  The Temperature is displayed on the LCD.


This project is meant to run on an STM32Nucleo dev board containing an Arm Cortex-M4 processor.  The project was built in keil, but requires external peripherals including a POT, LED, LCD, temeratute sensor and an 8 bit shift register(SN74HC595). 
