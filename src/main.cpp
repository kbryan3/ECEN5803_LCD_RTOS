/*----------------------------------------------------------------------------
LAB EXERCISE 12 - Real-Time Operating System
 ----------------------------------------
	Integrate functions developed in previous modules and run then concurrently
	in the mbed RTOS. The following four threads have to be implemented:
	
	1. Display the temperature on the LCD and to the PC
	2. Adjust the brightness of the RGB LED using a potentiometer
	3. Display an incrementing counter on the LCD and to the PC
	4. Blink an LED
  
  NOTE:  For this lab, the LCD, temp sensor and POT are virtual devices.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "rtos.h"
#include "DS1631.h"
#include "NHD_0216HZ.h"
#include "pindef.h"


//I2C address of temperature sensor DS1631
const int temp_addr = 0x90;

//Peripheral Instantiations
DS1631 tempSens(I2C_SDA, I2C_SCL,temp_addr);
NHD_0216HZ LCD(SPI_CS, SPI_MOSI, SPI_SCLK); 
Serial pc(UART_TX, UART_RX);

PwmOut LED_Ext(Dout0); //output to LED
AnalogIn pot1(Ain0); //Input from pot
DigitalOut LED_Board(LED); //Output to on board LED
	

//Mutex
Mutex LCD_mutex;

//Define a variable to store temperature measurement
float temp;
float pot1_in;



uint32_t count = 0;

//Display temperature on the LCD
void temp_thread(void const *args){
	
	while(1)
  {
    temp = tempSens.read();
    pc.printf("\n\r Temperature is %3.3f", temp);
    LCD_mutex.lock();
		LCD.set_cursor(0,0);
    LCD.printf("Temp: %3.3f", temp);
    LCD_mutex.unlock();
    Thread::wait(1000);
  }
	
}

//Adjust the brightness of the RGB LED
void adjust_brightness(void const *args){
	
	while(1)
	{
		pot1_in = pot1.read();
		LED_Ext.write(pot1_in);
		Thread::wait(100);
	}
	
	
}

//Blink an LED
void led1_thread(void const *args){
	
	LED_Board = 1;
	while(1)
	{
		LED_Board = !LED_Board;
		wait_ms(500);
	}
	
}

//Display a counter on the LCD
void count_thread(void const *args){
	
	while(1)
	{
		count++;
		pc.printf("\n\r Temperature is %3.3f", temp);
		LCD_mutex.lock();
		LCD.set_cursor(0,1);
    LCD.printf("Count: %d", count);
		LCD_mutex.unlock();
		Thread::wait(1500);
	}
	
}

/* RTOS API functions to initialize threads */

osThreadDef(temp_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(count_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(adjust_brightness, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(led1_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
	/*
	Initialise and clear the LCD display
	Start all threads
	*/
	LCD.init_lcd();
  LCD.clr_lcd();
  pc.baud(9600);
	 
  //start threads 	
	osThreadCreate(osThread(temp_thread), NULL);
	osThreadCreate(osThread(count_thread), NULL);
	osThreadCreate(osThread(adjust_brightness), NULL);
	osThreadCreate(osThread(led1_thread), NULL);
		
	Thread::wait(osWaitForever);
}
	

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
