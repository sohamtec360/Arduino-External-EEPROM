/*
 * External_EEPROM.c
 *
 * Created: 9/6/2025 2:16:29 PM
 * Author : Soham
 */ 

#include <avr/io.h>

#define F_CPU	16000000

#include <util/delay.h>

#include "SPI.h"
#include "EEPROM.h"

int main(void)
{
	SPI_Init_Master(1000000, PORTB,PB0,1,SPI_Mode0);
	InitEEPROM();

	uint8_t DataArray[4] = {'A', 'H', 'O', 'M'};
		
	DDRB |= (1<<DDB4);
	PORTB |= (1<<PB4);					//Set this wp pin to high to disable write protection.
	DDRD = 0XFF;
	
	
	WriteArray(&DataArray, 1, 4);

	ReadData(&DataArray,1,4);
	ErasePage(0X01);
	ReadData(&DataArray,1,4);
    while (1) 
    {
    }
}

	// write to EEPROM
	
	// 	SPI_Transfer(&ReadStatusreg[0],2);
	// 	_delay_ms(1000);
	// 	PORTD = ReadStatusreg[1];

	// 	SPI_Transfer(&EEPROMCMD.WREN,1);
	// 	_delay_ms(1);
	// 	DeSelectSlave;
	// 	_delay_ms(19);
	// 	SPI_Transfer(&AddArray[0],3);
	// 	_delay_ms(100);
	// 	SPI_Transfer(&AddArray[3],4);
	// 	_delay_ms(500);
	// 	DeSelectSlave;
	// 	_delay_ms(1000);
	// 	SPI_Transfer(&ReadArray[0],7);
	// 	_delay_ms(100);
	// 	DeSelectSlave;


	// 	SPI_Transfer(&ReadStatusreg[0],2);
	// 	_delay_ms(10);
	// 	PORTD = ReadStatusreg[1];

	//
	// 	ErasePage();
	// 	_delay_ms(10);
	//
	// 	WriteArray(&AddArray[0], 0, 64);
	// 	_delay_ms(10);
	// 	ReadData(&ReadArray[0], 00, 64);