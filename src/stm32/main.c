/*
The MIT License (MIT)

Copyright (c) 2017 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "stm32f0xx_nucleo.h"

#include "cr/macros.h"

static struct cr_routine *routine_led;

CR_DECLARE_ROUTINE(led_part2);
CR_DECLARE_ROUTINE(led_part3);
CR_DECLARE_ROUTINE(led_part4);

CR_DECLARE_ROUTINE(led_part1)
{
        STM_EVAL_LEDOn(LED2);
        cr_wait(100, led_part2);
        STM_EVAL_LEDOff(LED2);
        cr_wait(200, led_part3);
        STM_EVAL_LEDOn(LED2);
        cr_wait(100, led_part4);
        STM_EVAL_LEDOff(LED2);
        cr_wait_and_back(1000, led_part1);
}

CR_DECLARE_ROUTINE(button)
{
        if (!STM_EVAL_PBGetState(BUTTON_USER)) {
                cr_routine_halt(routine_led);
        } else {
                if (routine_led->state == CR_ROUTINE_STATE_HALT)
                        cr_routine_resume(routine_led);
        }
}

int main(void)
{
        struct cr_scheduler *scheduler;

        STM_EVAL_LEDInit(LED2);
        STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

        scheduler = cr_new("main", NULL);

        routine_led = cr_new_routine(scheduler, "routine_led", led_part1, NULL);
        cr_new_routine(scheduler, "routine_button", button, NULL);

        while (1)
                cr_service(scheduler);
}
