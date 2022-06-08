#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/interrupt.h"
#include "devices/timer.h"
#include "projects/mfq/mfq.h"

#define RANGE 20000000

int end_count = 0;

/* One test_cycle executes 20 million loops.
*/
void test_loop(void *aux)
{
    tid_t id = thread_tid();
    unsigned int a = 1;

    while (1) {
        a = (a + 1) % RANGE;
        if (a == 0) 
            break;
    }
    end_count++;
}

void run_mfqtest(char **argv)
{   
    int cnt;
	char *token, *save_ptr;

    enum intr_level old_level;
    old_level = intr_disable ();

    /// TODO: make your own test
	cnt = 0;
	for (token = strtok_r (argv[1], ":", &save_ptr); token != NULL; 
		token = strtok_r (NULL, ":", &save_ptr)) {

        char *subtoken, *save_ptr2, *name;
        int priority;

        subtoken = strtok_r (token, ".", &save_ptr2);
        name = &subtoken[1];
        printf("thread name: %s\n", &subtoken[1]);
        subtoken = strtok_r (NULL, ".", &save_ptr2);
        priority = atoi(subtoken);
        printf("priority: %d\n", priority);

        // you can create threads here 
        thread_create(name, priority, test_loop, NULL);

		cnt++;
	}
    intr_set_level (old_level);
    
    while (1) {
        if (end_count == cnt)
            timer_msleep(100000);
    }
    // thread_exit();
    // while (1) {
    //     timer_msleep(1000);
    // }

}