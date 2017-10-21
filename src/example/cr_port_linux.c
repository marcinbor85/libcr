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

#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "cr/port.h"

static cr_port_time_t current_tick;

static bool initialized_flag = false;

static void tick_handler(int signum)
{
        current_tick++;
}

bool cr_port_is_initialized(void)
{
        return initialized_flag;
}

void cr_port_init(void)
{
        struct itimerval value;
        struct sigaction handler;

        current_tick = 0;

        value.it_interval.tv_sec = 0;
        value.it_interval.tv_usec = 1000;
        value.it_value.tv_sec = 0;
        value.it_value.tv_usec = 1000;
        setitimer(ITIMER_REAL, &value, NULL);

        handler.sa_handler = (void*)&tick_handler;
        handler.sa_flags = SA_NOMASK;
        sigaction(SIGALRM, &handler, NULL);

        initialized_flag = true;
}

void* cr_port_malloc(size_t size)
{
        return malloc(size);
}

void cr_port_free(void *ptr)
{
        free(ptr);
}

cr_port_time_t cr_port_get_time(void)
{
        return current_tick;
}
