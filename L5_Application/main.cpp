/*
 * main.cpp
 *
 *  Created on: May 15, 2016
 *      Author: Kristen Kan
 */

#include "tasks.hpp"
#include "queue.h"
#include "event_groups.h"
#include "examples/examples.hpp"
#include <FreeRTOS.h>
#include "spiADC.hpp"
#include "midi_task.hpp"
#include "adc_task.hpp"
#include "command_handler.hpp"

CMD_HANDLER_FUNC(myCmdHandler)
{
    /* cmdParams is a str passed to you after user's command.
     * Commands are for hercules are "action rConsumer,
     * action sConsumer, action rProducer, action sProducer
     *
     * output is a CharDev class where the command came from, so
     * we can use this to output a reply message.
     * See "handlers.cpp" for more examples
     */

    /* return false will display command's help to the user */
    return true; /* return true if command was successful */
}

int main(void)
{
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));

    scheduler_add_task(new ADCTask(PRIORITY_MEDIUM));
    scheduler_add_task(new MIDITask(PRIORITY_HIGH));

    scheduler_start(); ///< This shouldn't return
    return -1;
}
