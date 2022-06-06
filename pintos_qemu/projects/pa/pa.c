#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "projects/pa/pa.h"

void print_user_pool_page() {
    printf("----------------------- user pool pages -----------------------\n");
    palloc_get_status(PAL_USER);
    printf("\n");
}

void print_kernel_pool_page() {
    printf("----------------------- kernel pool pages --------------------\n");
    palloc_get_status(0);
    printf("\n");
}

void run_patest(char **argv)
{   

    while (1) {
        print_kernel_pool_page();

        print_user_pool_page();
        palloc_get_multiple(0, 15);
        palloc_get_page(0);
        print_user_pool_page();
        print_kernel_pool_page();

        timer_msleep(1000);
    }
}