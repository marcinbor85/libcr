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

#include "cr/core.h"

static void routine1(struct cr_scheduler *scheduler, void *param)
{
        uint32_t time = 1000;
        const char *name = "slow";

        printf("[%05ld] - %s [%ld]\n", cr_port_get_time(), name, time);
        cr_routine_wait(scheduler->active_routine, time);
}

static void routine2(struct cr_scheduler *scheduler, void *param)
{
        uint32_t time = 300;
        const char *name = "fast";

        printf("[%05ld] - %s [%ld]\n", cr_port_get_time(), name, time);
        cr_routine_wait(scheduler->active_routine, time);
}

static void loop(struct cr_scheduler *scheduler, void *param)
{
        if (cr_port_get_time() > 3000)
                cr_stop(scheduler);
}

int main(void)
{
        struct cr_scheduler *scheduler;

        scheduler = cr_new("main", loop);

        cr_new_routine(scheduler, "routine_slow", routine1, NULL);
        cr_new_routine(scheduler, "routine_fast", routine2, NULL);

        puts("start");
        while (cr_service(scheduler) != false) {};
        puts("end");

        cr_delete(scheduler);

        return EXIT_SUCCESS;
}
