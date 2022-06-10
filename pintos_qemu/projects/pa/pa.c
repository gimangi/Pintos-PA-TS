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
    
        /* Test for kernel page */

        palloc_get_status(0);
        void * kerenl_palloc1 = palloc_get_multiple(0,13);
        palloc_get_status(0);

        void * kerenl_palloc2 = palloc_get_multiple(0,17);
        palloc_get_status(0);

        void * kerenl_palloc3 = palloc_get_multiple(0,3);
        palloc_get_status(0);

        void * kernel_palloc4 = palloc_get_multiple(0,16);
        palloc_get_status(0);

        palloc_free_multiple(kerenl_palloc2,17);
        palloc_get_status(0);

        palloc_free_multiple(kerenl_palloc3,3);
        palloc_get_status(0);

        void * kernel_palloc5 = palloc_get_multiple(0,5);
        palloc_get_status(0);

        void * kernel_palloc6 = palloc_get_multiple(0,8);
        palloc_get_status(0);

        void * kernel_palloc7 = palloc_get_multiple(0,22);
        palloc_get_status(0);

        void * kernel_palloc8 = palloc_get_multiple(0,30);
        palloc_get_status(0);

        palloc_free_multiple(kernel_palloc4,16);
        palloc_get_status(0);

        palloc_free_multiple(kernel_palloc5,5);
        palloc_get_status(0);

        palloc_free_multiple(kernel_palloc6,8);
        palloc_get_status(0);

        palloc_free_multiple(kerenl_palloc1,13);
        palloc_get_status(0);

        palloc_free_multiple(kernel_palloc8,30);
        palloc_get_status(0);

        palloc_free_multiple(kernel_palloc7,22);
        palloc_get_status(0);


        /* Test for user page */
        palloc_get_status(PAL_USER);
        void * user_palloc = palloc_get_multiple(PAL_USER,15);
        palloc_get_status(PAL_USER);

        void * user_palloc2 = palloc_get_multiple(PAL_USER,17);
        palloc_get_status(PAL_USER);

        void * user_palloc3 = palloc_get_multiple(PAL_USER,4);
        palloc_get_status(PAL_USER);

        void * user_palloc4 = palloc_get_multiple(PAL_USER,16);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc2,17);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc3,4);
        palloc_get_status(PAL_USER);

        void * user_palloc5 = palloc_get_multiple(PAL_USER,5);
        palloc_get_status(PAL_USER);

        void * user_palloc6 = palloc_get_multiple(PAL_USER,33);
        palloc_get_status(PAL_USER);

        void * user_palloc7 = palloc_get_multiple(PAL_USER,50);
        palloc_get_status(PAL_USER);

        void * user_palloc8 = palloc_get_multiple(PAL_USER,50);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc4,16);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc5,5);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc6,33);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc,15);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc8,50);
        palloc_get_status(PAL_USER);

        palloc_free_multiple(user_palloc7,50);
        palloc_get_status(PAL_USER);

    
    /*
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

*/
}