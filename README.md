# libcr
Multiplatform asynchronous library for C.
Usefull in embedded systems, where we can't halt main loop with simple sleep function.
Helps to implement finite state machines and cooperative services.

## Features:
* easy to porting (also for embedded systems)
* dynamic memory allocations for routines and schedulers objects
* only one thread, hidden main loop operations
* unlimited fast software timers
* continous, periodic or future parametrized callbacks
* easy to adapt and extend
* simple and powerful

## Examples:

### Asynchronous wait:
```c
#include <stdio.h>
#include <stdlib.h>

#include "cr/macros.h"

CR_DECLARE_ROUTINE(part2);

CR_DECLARE_ROUTINE(part1)
{
        printf("[%05ld] - %s - part1\n", cr_port_get_time(), scheduler->active_routine->name);
        cr_wait(1000, part2);
        printf("[%05ld] - %s - part2\n", cr_port_get_time(), scheduler->active_routine->name);
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
```
#### Output:
```
start
[00000] - routine_repeat
[00000] - routine_long - part1
[00300] - routine_repeat
[00600] - routine_repeat
[00900] - routine_repeat
[01000] - routine_long - part2
[01200] - routine_repeat
[01500] - routine_repeat
[01800] - routine_repeat
end
```

### Basic co-routines:
```c
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
```
#### Output:
```
start
[00000] - fast [300]
[00000] - slow [1000]
[00300] - fast [300]
[00600] - fast [300]
[00900] - fast [300]
[01000] - slow [1000]
[01200] - fast [300]
[01500] - fast [300]
[01800] - fast [300]
[02000] - slow [1000]
[02100] - fast [300]
[02400] - fast [300]
[02700] - fast [300]
[03000] - fast [300]
[03000] - slow [1000]
end
```

### Future and periodic callbacks:
```c
#include <stdio.h>
#include <stdlib.h>

#include "cr/core.h"

static void once(struct cr_scheduler *scheduler, void *param)
{
        printf("[%05ld] - num = %ld\n", cr_port_get_time(), *(uint32_t*)param);
}

static void routine(struct cr_scheduler *scheduler, void *param)
{
        static uint32_t num = 0;

        printf("[%05ld] - %s\n", cr_port_get_time(), scheduler->active_routine->name);
        cr_after(scheduler, "routine_once", 200, once, &num);
        num++;
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

        cr_repeat(scheduler, "routine_repeat", 1000, 5, routine, NULL);

        puts("start");
        while (cr_service(scheduler) != false) {};
        puts("end");

        cr_delete(scheduler);

        return EXIT_SUCCESS;
}
```
#### Output:
```
start
[00000] - routine_repeat
[00200] - num = 1
[01000] - routine_repeat
[01200] - num = 2
[02000] - routine_repeat
[02200] - num = 3
[03000] - routine_repeat
[03200] - num = 4
[04000] - routine_repeat
[04200] - num = 5
end
```

### Co-routines management:
```c
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
```
#### Output:
```
start
[00000] - routine_repeat
[00500] - routine_repeat
[01000] - routine_repeat
[01500] - routine_repeat
[02000] - routine_repeat
[02500] - routine_repeat
[03000] - halt
[06000] - resume
[06000] - routine_repeat
[06500] - routine_repeat
[07000] - routine_repeat
[07500] - routine_repeat
[08000] - routine_repeat
[08500] - routine_repeat
[09000] - routine_repeat
[09500] - routine_repeat
[10000] - routine_repeat
end
```
