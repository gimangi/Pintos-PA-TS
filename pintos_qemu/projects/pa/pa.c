#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "projects/pa/pa.h"

void run_patest(char **argv)
{   

    while (1) {
        printf("----------------------- use pool pages -----------------------");
        palloc_get_status(PAL_USER);

        printf("----------------------- kernel pool pages --------------------");
        palloc_get_status(0);

        timer_msleep(1000);
    }
}