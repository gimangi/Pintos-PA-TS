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

    // user pool test

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 15);

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 2);

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 5);

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 40);

    print_user_pool_page();

    // kernel pool test
    print_kernel_pool_page();

    palloc_get_multiple(0, 55);

    print_kernel_pool_page();

    palloc_get_multiple(0, 21);

    print_kernel_pool_page();

    palloc_get_page(0);

    print_kernel_pool_page();

    palloc_get_multiple(0, 100);

    print_kernel_pool_page();

}