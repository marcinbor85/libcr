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

#include "core.h"

static void loop_service(struct cr_scheduler *scheduler, void *param)
{
        if (scheduler->callback != NULL)
                scheduler->callback(scheduler, NULL);
}

struct cr_scheduler* cr_new(const char *name, cr_callback_t callback)
{
        struct cr_scheduler *scheduler;
        struct cr_routine *routine;

        if (cr_port_is_initialized() == false)
                cr_port_init();

        scheduler = cr_port_malloc(sizeof(struct cr_scheduler));
        scheduler->name = name;
        scheduler->callback = callback;

        routine = cr_port_malloc(sizeof(struct cr_routine));
        scheduler->loop_routine = routine;

        routine->scheduler = scheduler;
        routine->name = "__loop";
        routine->param = NULL;
        routine->callback = loop_service;
        routine->state = CR_ROUTINE_STATE_RUN;
        routine->prev_routine = scheduler->loop_routine;
        routine->next_routine = scheduler->loop_routine;

        scheduler->active_routine = scheduler->loop_routine;
        scheduler->run = true;

        return scheduler;
}

static void future_callback(struct cr_scheduler *scheduler, void *param)
{
        struct cr_future_callback *future = param;

        future->callback(scheduler, future->param);
        cr_routine_wait(scheduler->active_routine, scheduler->active_routine->wait_time);

        if (future->repeats == 0)
                return;

        if (--future->repeats == 0) {
                cr_port_free(future);
                cr_routine_delete(scheduler->active_routine);
        }
}

struct cr_routine* cr_after(struct cr_scheduler *scheduler, const char *name, cr_port_time_t time, cr_callback_t callback, void *param)
{
        struct cr_routine *routine;

        routine = cr_repeat(scheduler, name, time, 1, callback, param);
        cr_routine_wait(routine, time);

        return routine;
}

struct cr_routine* cr_repeat(struct cr_scheduler *scheduler, const char *name, cr_port_time_t period, cr_port_repeats_t repeats, cr_callback_t callback, void *param)
{
        struct cr_future_callback *future;
        struct cr_routine *routine;

        future = cr_port_malloc(sizeof(struct cr_future_callback));

        future->callback = callback;
        future->param = param;
        future->repeats = repeats;

        routine = cr_new_routine(scheduler, name, future_callback, future);
        routine->wait_time = period;

        return routine;
}

struct cr_routine* cr_new_routine(struct cr_scheduler *scheduler, const char *name, cr_callback_t callback, void *param)
{
        struct cr_routine *routine;

        routine = cr_port_malloc(sizeof(struct cr_routine));

        routine->name = name;
        routine->scheduler = scheduler;
        routine->callback = callback;
        routine->state = CR_ROUTINE_STATE_RUN;
        routine->param = param;

        routine->next_routine = scheduler->loop_routine->next_routine;
        routine->prev_routine = scheduler->loop_routine;

        scheduler->loop_routine->next_routine->prev_routine = routine;
        scheduler->loop_routine->next_routine = routine;

        return routine;
}

void cr_routine_delete(struct cr_routine *routine)
{
        struct cr_routine *prev = routine->prev_routine;
        struct cr_routine *next = routine->next_routine;

        cr_port_free(routine);

        prev->next_routine = next;
        next->prev_routine = prev;
}

void cr_routine_halt(struct cr_routine *routine)
{
        routine->state = CR_ROUTINE_STATE_HALT;
}

void cr_routine_resume(struct cr_routine *routine)
{
        routine->state = CR_ROUTINE_STATE_RUN;
}

void cr_routine_wait(struct cr_routine *routine, cr_port_time_t time)
{
        routine->wait_time = time;
        routine->last_time = cr_port_get_time();
        routine->state = CR_ROUTINE_STATE_WAIT;
}

void cr_routine_set_callback(struct cr_routine *routine, cr_callback_t callback, void *param)
{
        routine->callback = callback;
        routine->param = param;
}

void cr_stop(struct cr_scheduler *scheduler)
{
        scheduler->run = false;
}

void cr_delete(struct cr_scheduler *scheduler)
{
        struct cr_routine *routine = scheduler->loop_routine->next_routine;
        struct cr_routine *next;

        while (routine != scheduler->loop_routine) {
                next = routine->next_routine;
                cr_port_free(routine);
                routine = next;
        }

        cr_port_free(routine);
        cr_port_free(scheduler);
}

bool cr_service(struct cr_scheduler *scheduler)
{
        struct cr_routine *routine = scheduler->active_routine;

        switch (routine->state) {
        case CR_ROUTINE_STATE_RUN:
                routine->callback(scheduler, routine->param);
                break;
        case CR_ROUTINE_STATE_HALT:
                break;
        case CR_ROUTINE_STATE_WAIT:
                if (cr_port_get_time() - routine->last_time >= routine->wait_time)
                        routine->state = CR_ROUTINE_STATE_RUN;
                break;
        }

        scheduler->active_routine = routine->next_routine;

        return scheduler->run;
}
