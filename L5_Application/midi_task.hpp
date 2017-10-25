/*
 * midi_task.hpp
 *
 *  Created on: May 15, 2016
 *      Author: Kristen Kan
 */

#ifndef L5_APPLICATION_MIDI_TASK_HPP_
#define L5_APPLICATION_MIDI_TASK_HPP_

#include "stdio.h"

#include "LPC17xx.h"
#include "scheduler_task.hpp"

#define QUEUE_SIZE      5
#define SET_PARAMETERS  0xB0
#define NOTE_ON         0x90
#define NOTE_OFF        0x80
#define SET_VOLUME      0x07
#define INIT_VOLUME     0x50
#define SET_INSTR       0xC0
#define INIT_INSTR      0x01
#define MAX_VOLUME      0x7F
#define MAX_INSTR       0x7F
#define MAX_VELOCITY    0x7F
#define BYTE_PADDING    0x00
#define INC             1
#define DEC             -1

enum note_t{
    quiet,
    note0,
    note1,
    note2,
    note3,
    note4,
    note5,
    note6,
    note7
};

enum shared_queue_id{
    MIDI_QUEUE
};

// SPI Functions
void ssp_init(void);
void ssp_cs(void);
void ssp_ds(void);
char ssp_trx(char in);

// MIDI Functions
void midi_init(void);
void midi_xdcs(void);
void midi_xdds(void);
void midi_send_msg(char, char, char);

char change_volume(char);
char change_instr(char);

// MIDI Task
class MIDITask : public scheduler_task
{
    private:

    public:
        MIDITask(uint8_t priority) : scheduler_task("MIDI", 512*4, priority)
        {
            QueueHandle_t queue = xQueueCreate(QUEUE_SIZE, sizeof(note_t));
            addSharedObject(MIDI_QUEUE, queue);
        }

        bool init(void)
        {
            midi_ssp_init();
            midi_init();

            return true;
        }

        bool run(void *p)
        {
            note_t note;
            QueueHandle_t qid = getSharedObject(MIDI_QUEUE);

            if(xQueueReceive(qid, &note, portMAX_DELAY)) {
                switch(note) {
                    case note0: midi_send_msg(NOTE_ON, 0x3C, MAX_VELOCITY); break;
                    case note1: midi_send_msg(NOTE_ON, 0x3B, MAX_VELOCITY); break;
                    case note2: midi_send_msg(NOTE_ON, 0x3A, MAX_VELOCITY); break;
                    case note3: midi_send_msg(NOTE_ON, 0x39, MAX_VELOCITY); break;
                    case note4: midi_send_msg(NOTE_ON, 0x38, MAX_VELOCITY); break;
                    case note5: midi_send_msg(NOTE_ON, 0x37, MAX_VELOCITY); break;
                    case note6: midi_send_msg(NOTE_ON, 0x36, MAX_VELOCITY); break;
                    case note7: midi_send_msg(NOTE_ON, 0x35, MAX_VELOCITY); break;
                    case quiet: break;
                }
            }

            return true;
        }
};

#endif /* L5_APPLICATION_MIDI_TASK_HPP_ */
