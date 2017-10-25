/*
 * spiADC.cpp
 *
 *  Created on: May 14, 2016
 *      Author: Abraham
 */

#include "spiADC.hpp"
#include "LPC17xx.h"

int spiADC::read_ADC(char data)
{
    int outData;
    ADC_CS();

    spiTransfer(0x01);
    uint8_t a = spiTransfer(data);
    uint8_t b = spiTransfer(0x00);

    outData = (a << 8) | b;
    outData = outData & 0x000003ff;
    ADC_DS();

    return outData;
}

void spiADC::ADC_CS(void)
{
    LPC_GPIO0 -> FIOCLR = (1 << 0);
}

void spiADC::ADC_DS(void)
{
    LPC_GPIO0 -> FIOSET = (1 << 0);
}

void spiADC::control_init(void) {
    LPC_PINCON->PINSEL2 &= ~(3 << 18);  //SW0
    LPC_GPIO1->FIODIR &= ~(1 << 9);

    LPC_PINCON->PINSEL2 &= ~(3 << 20);  //SW1
    LPC_GPIO1->FIODIR &= ~(1 << 10);

    LPC_PINCON->PINSEL2 &= ~(3 << 28);  //SW2
    LPC_GPIO1->FIODIR &= ~(1 << 14);

    LPC_PINCON->PINSEL2 &= ~(3 << 30);  //SW3
    LPC_GPIO1->FIODIR &= ~(1 << 15);
}

char spiADC::spiTransfer(char in)
{
    LPC_SSP0 -> DR = in; //moves to Data Register
    while (LPC_SSP0->SR  & (1 << 4)); //while Status register is full
    return LPC_SSP0->DR;
}

void spiADC::SSP_Init(void)
{
    LPC_SC->PCONP |= (1 << 21);  //Starts up SSP0
    LPC_SC->PCLKSEL1 &= ~(3 << 10);  //Resets clock
    LPC_SC->PCLKSEL1 |=  (1 << 10);  //Enables clock

    //initializes ADC CS
    LPC_PINCON->PINSEL0 &= ~(3 << 0); //output
    // LPC_PINCON->PINSEL0 |= (1 << 0); //output
    LPC_GPIO0->FIODIR |= (1 << 0);    //Direction of P0.0 (I think)

    //initializes SCLK
    LPC_PINCON->PINSEL0 &= ~(3 << 30);
    LPC_PINCON->PINSEL0 |= (2 << 30);
    //initializes MOSI
    LPC_PINCON->PINSEL1 &= ~(3 << 4);
    LPC_PINCON->PINSEL1 |= (2 << 4);
    //initializes MISO
    LPC_PINCON->PINSEL1 &= ~(3 << 2);
    LPC_PINCON->PINSEL1 |= (2 << 2);

    LPC_SSP0->CR0 = 7; //8-bit width
    LPC_SSP0->CR1 = 2; //Master/Slave mode

    LPC_SSP0->CPSR = 8; //it works
}
