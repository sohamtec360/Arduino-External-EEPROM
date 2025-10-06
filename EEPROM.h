/*
 * EEPROM.h
 *
 * Created: 9/7/2025 9:30:35 PM
 *  Author: Soham
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

typedef struct CMDLIST
{
	uint8_t READ;		//Read data from memory array beginning at selected address
	uint8_t WRITE;		//Write data to memory array beginning at selected address
	uint8_t WREN;		//Set the write enable latch (enable write operations)
	uint8_t WRDI;		//Reset the write enable latch (disable write operations)
	uint8_t RDSR;		//Read STATUS register
	uint8_t WRSR;		//Write STATUS register
	uint8_t PAGEE;		//Page Erase – erase one page in memory array
	uint8_t SECTORE;		//Sector Erase – erase one sector in memory array
	uint8_t CHIPE;		//Chip Erase – erase all sectors in memory array
	uint8_t RDID;		//Release from Deep power-down and read electronic signature
	uint8_t DPD;		//Deep Power-Down mode	
}Command;

Command EEPROMCMD;

typedef struct SREGLIST
{
	uint8_t WIP;
	uint8_t WEL;
	uint8_t BP0;
	uint8_t BP1;
	uint8_t WPEN;
}STATUSREG;

STATUSREG STREG;

uint8_t ReadDataArray[10];

/*Setup SPI*/

void InitEEPROM(void);

/*
	@param	void* data array.
	Length of array to write.
*/
void WriteArray(void *,uint16_t, uint8_t);
void ReadData(void *,uint16_t, uint8_t);
uint8_t ReadStatusReg();
void IS_Busy();
void ErasePage(uint16_t);

#endif /* EEPROM_H_ */