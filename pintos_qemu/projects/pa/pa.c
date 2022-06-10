#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "projects/pa/pa.h"

void print_alloc_pages(int pages) {
    printf("allocate %d pages.\n", pages);
}

void print_free_pages(int pages) {
    printf("free %d pages.\n", pages);
}

void run_patest(char **argv)
{   
    // user pool test
    palloc_get_status(PAL_USER);

    void * p1 = palloc_get_multiple(PAL_USER, 15);
    print_alloc_pages(15);

    palloc_get_status(PAL_USER);

    palloc_get_multiple(PAL_USER, 1);
    print_alloc_pages(1);

    palloc_get_status(PAL_USER);

    palloc_free_multiple(p1, 15);
    print_free_pages(15);

    palloc_get_status(PAL_USER);

    palloc_get_multiple(PAL_USER, 4);
    print_alloc_pages(4);

    palloc_get_status(PAL_USER);

    palloc_get_multiple(PAL_USER, 8);
    print_alloc_pages(8);

    palloc_get_status(PAL_USER);

    palloc_get_multiple(PAL_USER, 5);
    print_alloc_pages(5);

    palloc_get_status(PAL_USER);

    palloc_get_multiple(PAL_USER, 40);
    print_alloc_pages(40);

    palloc_get_status(PAL_USER);

    void *p2 = palloc_get_multiple(PAL_USER, 100);
    print_alloc_pages(100);

    palloc_get_status(PAL_USER);

    palloc_free_multiple(p2, 100);
    print_free_pages(100);

    // palloc_free_multiple(p2, 65);
    // if it is out of the free range, it will be ASSERT

    palloc_get_status(PAL_USER);

    // kernel pool test
    palloc_get_status(0);

    palloc_get_multiple(0, 55);
    print_alloc_pages(55);

    palloc_get_status(0);

    palloc_get_multiple(0, 21);
    print_alloc_pages(21);

    palloc_get_status(0);

    palloc_get_page(0);
    print_alloc_pages(1);

    palloc_get_status(0);

    palloc_get_multiple(0, 100);
    print_alloc_pages(100);

    palloc_get_status(0);

}