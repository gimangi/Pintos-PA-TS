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

void print_alloc_pages(int pages) {
    printf("allocate %d pages.\n", pages);
}

void print_free_pages() {
    printf("free pages.\n");
}

void run_patest(char **argv)
{   

    // user pool test

    print_user_pool_page();

    void * p1 = palloc_get_multiple(PAL_USER, 15);
    print_alloc_pages(15);

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 2);
    print_alloc_pages(2);

    print_user_pool_page();

    palloc_free_multiple(p1, 15);
    print_free_pages();    

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 5);
    print_alloc_pages(5);

    print_user_pool_page();

    palloc_get_multiple(PAL_USER, 40);
    print_alloc_pages(40);

    print_user_pool_page();

    // kernel pool test
    print_kernel_pool_page();

    palloc_get_multiple(0, 55);
    print_alloc_pages(55);

    print_kernel_pool_page();

    palloc_get_multiple(0, 21);
    print_alloc_pages(21);

    print_kernel_pool_page();

    palloc_get_page(0);
    print_alloc_pages(1);

    print_kernel_pool_page();

    palloc_get_multiple(0, 100);
    print_alloc_pages(100);

    print_kernel_pool_page();

}