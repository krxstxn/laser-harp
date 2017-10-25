/*
 * adc_task.hpp
 *
 *  Created on: May 15, 2016
 *      Author: Kristen Kan
 */

#ifndef L5_APPLICATION_ADC_TASK_HPP_
#define L5_APPLICATION_ADC_TASK_HPP_

#include "stdio.h"

#include "midi_task.hpp"
#include "LPC17xx.h"
#include "scheduler_task.hpp"

char curr_volume = INIT_VOLUME;
char curr_instr = INIT_INSTR;
bool NOTE1 = false, NOTE2 = false, NOTE3 = false, NOTE4 = false,
     NOTE5 = false, NOTE6 = false, NOTE7 = false, OCTAVE = false;

class ADCTask : public scheduler_task
{
    public:
        ADCTask(uint8_t priority) : scheduler_task("ADCTask", 512*4, priority)
        {
            QueueHandle_t my_queue = xQueueCreate(1, sizeof(note_t));
            addSharedObject(MIDI_QUEUE, my_queue);
        }

        bool init(void)
        {
            spiADC::SSP_Init(); //initializes SSP
            spiADC::control_init();

            //Set initial instrument bank/instrument
            midi_send_msg(SET_PARAMETERS, 0x00, 0x00);
            midi_send_msg(SET_INSTR, INIT_INSTR, 0);

            return true;
        }

        bool run(void *p)
        {
            curr_volume = change_volume(curr_volume);
            curr_instr = change_instr(curr_instr);

            int ch0, ch1, ch2, ch3, ch4, ch5, ch6, ch7;
            note_t key = quiet;

            ch0 = spiADC::read_ADC(0x80);
            ch1 = spiADC::read_ADC(0x90);
            ch2 = spiADC::read_ADC(0xA0);
            ch3 = spiADC::read_ADC(0xB0);
            ch4 = spiADC::read_ADC(0xC0);
            ch5 = spiADC::read_ADC(0xD0);
            ch6 = spiADC::read_ADC(0xE0);
            ch7 = spiADC::read_ADC(0xF0);

            printf("\nCh0 is %d\n", ch0);
            if (ch0 > 650)
            {
                //xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
            }
                printf("\nCh1 is %x\n", ch1);
            if (ch1 < 835)
            {
                if (NOTE1 == false)
                {

                    key = note1;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE1 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x3C, MAX_VELOCITY);
                NOTE1 = false;
            }

            printf("\nCh2 is %x\n", ch2);
            if (ch2 < 835)
            {
                if (NOTE2 == false)
                {
                    key = note2;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE2 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x3E, MAX_VELOCITY);
                NOTE2 = false;

            }
            printf("\nCh3 is %x\n", ch3);
            if (ch3 < 835)
            {
                if (NOTE3 == false)
                {
                    key = note3;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE3 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x40, MAX_VELOCITY);
                NOTE3 = false;
            }
            printf("\nCh4 is %x\n", ch4);
            if (ch4 < 835)
            {
                if (NOTE4 == false)
                {
                    key = note4;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE4 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x42, MAX_VELOCITY);
                NOTE4 = false;
            }
            printf("\nCh5 is %x\n", ch5);
            if (ch5 < 835)
            {
                if (NOTE5 == false)
                {
                    key = note5;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE5 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x44, MAX_VELOCITY);
                NOTE5 = false;
            }
            printf("\nCh6 is %x\n", ch6);
            if (ch6 < 835)
            {
                if (NOTE6 == false)
                {
                    key = note6;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE6 = true;
                }
            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x46, MAX_VELOCITY);
                NOTE6 = false;
            }
            printf("\nCh7 is %x\n", ch7);
            if (ch7 < 835)
            {
                if (NOTE7 == false)
                {
                    key = note7;
                    xQueueSend(getSharedObject(MIDI_QUEUE), &key, portMAX_DELAY);
                    NOTE7 = true;
                }

            }
            else
            {
                midi_send_msg(NOTE_OFF, 0x48, MAX_VELOCITY);
                NOTE7 = false;
            }

//            vTaskDelay(1000);

            return true;

        }
};

#endif /* L5_APPLICATION_ADC_TASK_HPP_ */