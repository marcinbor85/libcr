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

static struct cr_routine *routine;

static void callback(struct cr_scheduler *scheduler, void *param)
{
        printf("[%05ld] - %s\n", cr_port_get_time(), scheduler->active_routine->name);
}

static void control(struct cr_scheduler *scheduler, void *param)
{
        printf("[%05ld] - %s\n", cr_port_get_time(), scheduler->active_routine->name);

        if (param == NULL) {
                cr_routine_halt(routine);
        } else {
                cr_routine_resume(routine);
        }
}

static void loop(struct cr_scheduler *scheduler, void *param)
{
        if (cr_port_get_time() > 10000)
                cr_stop(scheduler);
}

int main(void)
{
        struct cr_scheduler *scheduler;

        scheduler = cr_new("main", loop);

        routine = cr_repeat(scheduler, "routine_repeat", 500, 0, callback, NULL);

        cr_after(scheduler, "halt", 3000, control, 0);
        cr_after(scheduler, "resume", 6000, control, 1);

        puts("start");
        while (cr_service(scheduler) != false) {};
        puts("end");

        cr_delete(scheduler);

        return EXIT_SUCCESS;
}
