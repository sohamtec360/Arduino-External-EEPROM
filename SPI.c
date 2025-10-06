/*
 * SPI.c
 *
 * Created: 8/15/2024 11:09:36 PM
 *  Author: Soham
 */ 
 
 /*
 Pending task
 
 --->		update ss pin as per user requrement, ex. if user want to continue write operation
			even at the end of array length.
 --->		ss pin should be selectable as per user requirement.(On hold)
 
 
 */

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "SPI.h"

void SPI_Init_Master(uint32_t Clock_Frq, uint8_t SSPinPort, uint8_t SSPinNumb, bool IS_LSB_MSB , uint8_t SPI_Mode)
{
	sei();
	DDRB |= (1<<DDB0) |  (1<<DDB1) | (1<<DDB2);				//slave select, SCK, MOSI
	DDRB &= (~(1<<DDB3));									//MISO

// 	SSPORT(&SSPinPort);			//slave port and pin selection
// 	SSPIN(SSPinNumb);
	
	if (Clock_Frq >= F_CPU/2){
		SPCR &= (~Clock_Mask);
		SPCR |= Double_Clock_Div_2;
		SPSR |= (1<<SPI2X);
	}else if (Clock_Frq >= F_CPU/4){
		SPCR &= (~Clock_Mask);
		SPCR |= Clock_Div_4;
	}else if (Clock_Frq >= F_CPU/8){
		SPCR &= (~Clock_Mask);
		SPCR |= Double_Clock_Div_8;
		SPSR |= (1<<SPI2X);
	}else if (Clock_Frq >= F_CPU/16){
		SPCR &= (~Clock_Mask);
		SPCR |= Clock_Div_16;
	}else if (Clock_Frq >= F_CPU/32){
		SPCR &= (~Clock_Mask);
		SPCR |= Double_Clock_Div_32;
		SPSR |= (1<<SPI2X);
	}else if (Clock_Frq >= F_CPU/64){
		SPCR &= (~Clock_Mask);
		SPCR |= Clock_Div_64;
	}else if (Clock_Frq >= F_CPU/128){
		SPCR &= (~Clock_Mask);
		SPCR |= Clock_Div_128;
	}
	
	if (SPI_Mode==0X00){
		SPCR &= (~Mode_Mask);
		SPCR |= (SPI_Mode0);
	}else if (SPI_Mode==0X04){
		SPCR &= (~Mode_Mask);
		SPCR |= (SPI_Mode1);
	}else if (SPI_Mode==0X08){
		SPCR &= (~Mode_Mask);
		SPCR |= (SPI_Mode2);
	}else if (SPI_Mode==0X0C){
		SPCR &= (~Mode_Mask);
		SPCR |= (SPI_Mode3);
	}

 	MSTR_SELECT;
// 	if(IS_INTERRUPT){
// 		INTERRUPT_ENABLE;
// 	}else{
// 		INTERRUPT_DISABLE;
// 	}
	DATA_ORDER_MSB_FIRST;
	SPI_ENABLE;					//Enable the SPI at the end of all SPCR configuration
								//because if enable SPI first then it will pick up initial state of pin as data or slave selection.
	return;
}

void SPI_Init_Slave()
{
	sei();
	DDRB |= (1<<DDB3);										//MISO
	DDRB &= (~(1<<DDB0))&(~(1<<DDB1))&(~(1<<DDB2));			//Slave select, SCK, MOSI
//	SPCR |= (1<<SPE) | (1<<SPIE);							//spi enable, interrupt enable, 16 cock divider
	INTERRUPT_ENABLE;
	SPI_ENABLE;
}

//*****//// This function is created by me As per arduino "Transfer" function //*****/////
//Hear write buffer of user is going to be used to store data received from slave
//so no need to worry about length of read data buffer

void SPI_INT_Transfer(uint8_t *Data, uint8_t Length)	//USE THIS FUNDTION IN INTERRUPT ENABLE
{
	SELECT_SLAVE;
	WR_Buff_Add = Data;
	BuffDepth = Length;
	
	SPDR = *WR_Buff_Add;							//For the slave it will put data in a shift register but it will only transfer data when it low logic at SS pin and CLK pulse at
	return;											// CLK pin.
}

void SPI_POLL_Transfer(uint8_t *Data, uint8_t Length)	//use for polling method
{
	SELECT_SLAVE;
	while(Length--)
	{
		SPDR = *Data;									//Write data into SPDR register to transfer
		while(!(SPSR & (1<<SPIF))){}
		*Data = SPDR;									//End of transmission reading data from SPDR
		Data ++;
	}
	DESELECT_SLAVE;
}

void SPI_POLL_Continue_Transfer(uint8_t *Data, uint8_t Length)
{
	SELECT_SLAVE;
	while(Length--)
	{
		SPDR = *Data;
		while(!(SPSR & (1<<SPIF))){}
		*Data = SPDR;
		Data ++;
	}
}

void SPI_POLL_Stop_Transfer()
{
	DESELECT_SLAVE;
}

ISR(SPI_STC_vect)
{
	if (--BuffDepth>0){
		*WR_Buff_Add=SPDR;			//first read SPDR register because interrupt is occurred due to  first data transmission.
		SPDR = *(++WR_Buff_Add);
	}else{
		DESELECT_SLAVE;
	}
	return;
}


//*****//// This function is created by me As per my idea//*****/////

// uint8_t* SPI_Transfer(uint8_t *Data, uint8_t Length)
// {
// 	Write_Buff_Add = Data;
// 	BuffDepth = Length;
// 	Read_Buff_Add = &Read_Buff[0];
//
// 	SPDR = *Write_Buff_Add;
// 	return &Read_Buff[0];
// }