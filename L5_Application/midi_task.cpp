/*
 * midi_task.cpp
 *
 *  Created on: May 15, 2016
 *      Author: Kristen Kan
 */

#include "midi_task.hpp"

 void midi_ssp_init(void) {
    LPC_SC->PCONP |= (1 << 10);         //Start up SSP1
    LPC_SC->PCLKSEL0 &= ~(3 << 20);     //Reset clock
    LPC_SC->PCLKSEL0 |= (3 << 20);      //Enable clock

    LPC_PINCON->PINSEL0 &= ~(3 << 14);  //Reset/init SCLK
    LPC_PINCON->PINSEL0 |= (2 << 14);

    LPC_PINCON->PINSEL0 &= ~(3 << 18);  //Reset/init MOSI
    LPC_PINCON->PINSEL0 |= (2 << 18);

    LPC_PINCON->PINSEL0 &= ~(3 << 16);  //Reset/init MISO
    LPC_PINCON->PINSEL0 |= (2 << 16);

    LPC_SSP1->CR0 = 0;
    LPC_SSP1->CR0 = 7;                  //8-bit transfer
    LPC_SSP1->CR1 = 2;                  //Master/slave mode
    LPC_SSP1->CPSR = 8;                 //CPU speed / 8
 }

 void midi_ssp_cs(void) {
    LPC_GPIO0->FIOCLR = (1 << 6);       //SS
 }

 void midi_ssp_ds(void) {
    LPC_GPIO0->FIOSET = (1 << 6);       //SS
 }

 char midi_ssp_trx(char in) {
    LPC_SSP1->DR = in;                  //Write to data register
    while(LPC_SSP1->SR & (1 << 4));     //Wait until SSP controller (status register) is idle

    return LPC_SSP1->DR;
 }

 void midi_init(void) {
    LPC_PINCON->PINSEL0 &= ~(3 << 2);   //Set P0.1 as GPIO output for XDCS
    LPC_GPIO0->FIODIR |= (1 << 1);

    LPC_PINCON->PINSEL3 &= ~(3 << 14);  //Set P1.23 as GPIO output for RST
    LPC_GPIO1->FIODIR |= (1 << 23);
    LPC_GPIO1->FIOCLR = (1 << 23);
    LPC_GPIO1->FIOSET = (1 << 23);

    LPC_PINCON->PINSEL3 &= ~(3 << 8);
    LPC_GPIO1->FIODIR &= ~(1 << 20);    //Set P1.20 as GPIO input for DREQ
 }

 void midi_xdcs(void) {
    LPC_GPIO0->FIOCLR = (1 << 1);
 }

 void midi_xdds(void) {
    LPC_GPIO0->FIOSET = (1 << 1);
 }

char change_volume(char curr_volume) {
    char new_volume;

    if(LPC_GPIO1->FIOPIN & (1 << 9)) {
        new_volume = curr_volume + DEC;
        if(new_volume < 0)
            new_volume = 0x00;
        midi_send_msg(SET_PARAMETERS, SET_VOLUME, new_volume);
    }
    else if(LPC_GPIO1->FIOPIN & (1 << 10)) {
        new_volume = curr_volume + INC;
        if(new_volume > 127)
            new_volume = 0x7F;
        midi_send_msg(SET_PARAMETERS, SET_VOLUME, new_volume);
    }
    else new_volume = curr_volume;

    return new_volume;
}

char change_instr(char curr_instr) {
    char new_instr;

    if(LPC_GPIO1->FIOPIN & (1 << 14)) {
        new_instr = curr_instr + DEC;
        if(new_instr < 0)
            new_instr = 0x7F;
        midi_send_msg(SET_INSTR, new_instr, 0);
    }
    else if(LPC_GPIO1->FIOPIN & (1 << 15)) {
        new_instr = curr_instr + INC;
        if(new_instr > 0x7F)
            new_instr = 0x00;
        midi_send_msg(SET_INSTR, new_instr, 0);
    }
    else new_instr = curr_instr;

    return new_instr;
}

 void midi_send_msg(char cmd, char data1, char data2) {
     midi_xdcs();

     if(cmd == 0xC0) {
         midi_ssp_trx(BYTE_PADDING);
         midi_ssp_trx(cmd);
         midi_ssp_trx(BYTE_PADDING);
         midi_ssp_trx(data1);
     }
     else {
         midi_ssp_trx(BYTE_PADDING);
         midi_ssp_trx(cmd);
         midi_ssp_trx(BYTE_PADDING);
         midi_ssp_trx(data1);
         midi_ssp_trx(BYTE_PADDING);
         midi_ssp_trx(data2);
     }

     midi_xdds();

     return;
 }
