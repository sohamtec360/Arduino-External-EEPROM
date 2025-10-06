/*
 * SPI.h
 *
 * Created: 8/15/2024 11:10:01 PM
 *  Author: Soham
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t *WR_Buff_Add;	//Read Write buffer Provided by user
uint8_t BuffDepth;
uint8_t *SS_Port, SS_Pin;
bool ContinueData;

#define F_CPU			16000000

//Slave port and pin selection
// #define SSPORT(SSPORTNUM)			(SS_Port = (uint8_t*)SSPORTNUM)
// #define SSPIN(SSPINNUM)				(SS_Pin = SSPINNUM)
// 
// #define DESELECT_SLAVE				(*SS_Port |= (1<<SS_Pin))			//De select slave
// #define SELECT_SLAVE				(*SS_Port &= (~(1<<SS_Pin)))		//Select slave

#define DESELECT_SLAVE				PORTB |= (1<<PB0)
#define SELECT_SLAVE				PORTB &= (~(1<<PB0))

//Control Operation
#define INTERRUPT_ENABLE			(SPCR |= (1<<SPIE))			// Interrupt Enable
#define INTERRUPT_DISABLE			(SPCR &= ~(1<<SPIE))		// Interrupt Disable
	
#define SPI_ENABLE					(SPCR |= (1<<SPE))			// Enable SPI
#define SPI_DISABLE					(SPCR &= ~(1<<SPE))			// Disable SPI

#define DATA_ORDER_LSB_FIRST		(SPCR |= (1<<DORD))			// LSB First
#define DATA_ORDER_MSB_FIRST		(SPCR &= ~(1<<DORD))		// MSB First

#define MSTR_SELECT					(SPCR |= (1<<MSTR))			//Configure as master
#define SLAVE_SELECT				(SPCR &= ~(1<<MSTR))		//Configure as slave

#define ClockPolarity				(1<<CPOL)
#define ClockPhase					(1<<CPHA)

#define SPI_Mode0					0X00
#define SPI_Mode1					0X04
#define SPI_Mode2					0X08
#define SPI_Mode3					0X0C

#define Double_Clock_Div_2			0X00
#define Clock_Div_4					0X00
#define Double_Clock_Div_8			0X01
#define Clock_Div_16				0X01
#define Double_Clock_Div_32			0X02
#define Clock_Div_64				0X02
#define Clock_Div_128				0X03
#define Double_Clock_Div_64			0X03

#define Mode_Mask					0X0C
#define Clock_Mask					0X03

void SPI_Init_Master(uint32_t, uint8_t, uint8_t, bool, uint8_t);
void SPI_Init_Slave(void);

void SPI_INT_Transfer(uint8_t*, uint8_t);
void SPI_POLL_Transfer(uint8_t*, uint8_t);
void SPI_POLL_Continue_Transfer(uint8_t*, uint8_t);
void SPI_POLL_Stop_Transfer();
#endif /* SPI_H_ */