/*
 * CFile1.c
 *
 * Created: 9/7/2025 9:29:47 PM
 *  Author: Soham
 */ 

/*
Fro 24LC512 has 64Kbyte of memory, 128 byte of page and 16Kbyte of sector
*/

/*
Pending task

*/


#include "EEPROM.h"
#include "SPI.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "avr/io.h"

void InitEEPROM()
{
	EEPROMCMD.READ=3;		//0b00000011;
	EEPROMCMD.WRITE=2;		//0b00000010;
	EEPROMCMD.WREN=6;		//0b00000110;
	EEPROMCMD.WRDI=4;		//0b00000100;
	EEPROMCMD.RDSR=5;		//0b00000101;
	EEPROMCMD.WRSR=1;		//0b00000001;
	EEPROMCMD.PAGEE=66;		//0b01000010;
	EEPROMCMD.SECTORE=216;	//0b11011000;
	EEPROMCMD.CHIPE=199;	//0b11000111;
	EEPROMCMD.RDID=171;		//0b10101011;
	EEPROMCMD.DPD=185;		//0b10111001;
	
	STREG.WIP = 0;
	STREG.WEL = 1;
	STREG.BP0 = 2;
	STREG.BP1 = 3;
	STREG.WPEN = 7;
	

//	SPI_Init_Master(1000000, PORTB,PB0,1,SPI_Mode0);			//as per data sheet data is transfered in mode 0.

//Initially in power on state EEPROM is in standby mode
//so to activate it High to low level transition is required for that
// select slave and after delay deselect that slave for next operation.
	SELECT_SLAVE;
	DESELECT_SLAVE;
}

void WriteArray(void *DataArray, uint16_t Address, uint8_t Length)
{
	uint8_t TEMP;
	uint8_t Add[2];
	
	Add[0] = Address >> 8;
	Add[1] = Address & 0XFF;
	
	TEMP = EEPROMCMD.WREN;
	SPI_POLL_Transfer(&TEMP, 1);				//ENABLE LATCH
	TEMP = EEPROMCMD.WRITE;
	SPI_POLL_Continue_Transfer(&TEMP, 1);
	SPI_POLL_Continue_Transfer(&Add[0], 2);
	SPI_POLL_Transfer(DataArray,Length);				//DESELECT SS pin at the end of transmission
	
	IS_Busy();										//Wait till internal operation completed
}

void ReadData(void *DataArray,uint16_t Address, uint8_t Length)
{
	uint8_t TEMP;
	uint8_t Add[2];
	
	Add[0] = Address >> 8;
	Add[1] = Address & 0XFF;
	
	TEMP = EEPROMCMD.READ;
	SPI_POLL_Continue_Transfer(&TEMP, 1);
	SPI_POLL_Continue_Transfer(&Add[0], 2);
	SPI_POLL_Transfer(DataArray,Length);
}

uint8_t ReadStatusReg()
{
	uint8_t StatusReg,TEMP;								//status register read instruction
	uint16_t i;
	TEMP = EEPROMCMD.RDSR;
	SPI_POLL_Continue_Transfer(&TEMP,1);
	SPI_POLL_Transfer(&StatusReg,1);
	for (i=0; i<160; i++)
	{
	}
	
	return StatusReg;
}

void IS_Busy()
{
	uint8_t CurrState;
	CurrState = ReadStatusReg();
	PORTD = CurrState;
	while(CurrState & (1<<STREG.WIP))
	{
		CurrState = ReadStatusReg();
	}
}

void ErasePage(uint16_t PageEraseAdd)
{
	uint8_t TEMP;
	uint8_t Add[2];
		
	Add[0] = PageEraseAdd >> 8;
	Add[1] = PageEraseAdd & 0XFF;
	
	TEMP = EEPROMCMD.WREN;
	SPI_POLL_Transfer(&TEMP,1);
	TEMP = EEPROMCMD.PAGEE;
	SPI_POLL_Continue_Transfer(&TEMP,1);
	SPI_POLL_Transfer(&Add[0],2);
	IS_Busy();
}