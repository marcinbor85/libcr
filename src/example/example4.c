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

#include <stdio.h>
#include <stdlib.h>

#include "cr/macros.h"

CR_DECLARE_ROUTINE(part2);
CR_DECLARE_ROUTINE(part3);

CR_DECLARE_ROUTINE(part1)
{
        printf("[%05ld] - %s - part1\n", cr_port_get_time(), scheduler->active_routine->name);
        cr_wait(1000, part2);
        printf("[%05ld] - %s - part2\n", cr_port_get_time(), scheduler->active_routine->name);
        cr_wait_for(cr_port_get_time() > 1500, part3);
        printf("[%05ld] - %s - part3\n", cr_port_get_time(), scheduler->active_routine->name);
        cr_end();
}

static void routine_repeat(struct cr_scheduler *scheduler, void *param)
{
        printf("[%05ld] - %s\n", cr_port_get_time(),  scheduler->active_routine->name);
}

static void loop(struct cr_scheduler *scheduler, void *param)
{
        if (cr_port_get_time() > 2000)
                cr_stop(scheduler);
}

int main(void)
{
        struct cr_scheduler *scheduler;

        scheduler = cr_new("main", loop);

        cr_new_routine(scheduler, "routine_long", part1, NULL);
        cr_repeat(scheduler, "routine_repeat", 300, 0, routine_repeat, NULL);

        puts("start");
        while (cr_service(scheduler) != false) {};
        puts("end");

        cr_delete(scheduler);

        return EXIT_SUCCESS;
}
