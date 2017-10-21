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

#ifndef CR_CORE_H_
#define CR_CORE_H_

#include "port.h"

struct cr_scheduler;

typedef void (*cr_callback_t)(struct cr_scheduler *scheduler, void *param);

struct cr_future_callback {
        cr_callback_t callback;
        cr_port_repeats_t repeats;
        void *param;
};

struct cr_scheduler {
        struct cr_routine *active_routine;
        struct cr_routine *loop_routine;
        const char *name;
        bool run;
        cr_callback_t callback;
};

typedef enum {
        CR_ROUTINE_STATE_RUN,
        CR_ROUTINE_STATE_WAIT,
        CR_ROUTINE_STATE_HALT
} cr_routine_state_e;

struct cr_routine {
        cr_routine_state_e state;
        struct cr_scheduler *scheduler;
        cr_port_time_t wait_time;
        cr_port_time_t last_time;
        const char *name;
        cr_callback_t callback;
        void *param;
        struct cr_routine *prev_routine;
        struct cr_routine *next_routine;
};

struct cr_scheduler* cr_new(const char *name, cr_callback_t callback);
struct cr_routine* cr_new_routine(struct cr_scheduler *scheduler, const char *name, cr_callback_t callback, void *param);
struct cr_routine* cr_after(struct cr_scheduler *scheduler, const char *name, cr_port_time_t time, cr_callback_t callback, void *param);
struct cr_routine* cr_repeat(struct cr_scheduler *scheduler, const char *name, cr_port_time_t period, cr_port_repeats_t repeats, cr_callback_t callback, void *param);

void cr_delete(struct cr_scheduler *scheduler);
void cr_stop(struct cr_scheduler *scheduler);
bool cr_service(struct cr_scheduler *scheduler);

void cr_routine_delete(struct cr_routine *routine);
void cr_routine_halt(struct cr_routine *routine);
void cr_routine_resume(struct cr_routine *routine);
void cr_routine_wait(struct cr_routine *routine, cr_port_time_t time);
void cr_routine_set_callback(struct cr_routine *routine, cr_callback_t callback, void *param);

#endif /* CR_CORE_H_ */
