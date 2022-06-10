#include "threads/palloc.h"
#include <bitmap.h>
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "threads/loader.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

#define BUDDY_NOT_FOUND SIZE_MAX

/* Page allocator.  Hands out memory in page-size (or
   page-multiple) chunks.  See malloc.h for an allocator that
   hands out smaller chunks.

   System memory is divided into two "pools" called the kernel
   and user pools.  The user pool is for user (virtual) memory
   pages, the kernel pool for everything else.  The idea here is
   that the kernel needs to have memory for its own operations
   even if user processes are swapping like mad.

   By default, half of system RAM is given to the kernel pool and
   half to the user pool.  That should be huge overkill for the
   kernel pool, but that's just fine for demonstration purposes. */

/* A memory pool. */
struct pool
  {
    struct lock lock;                   /* Mutual exclusion. */
    struct bitmap *used_map;            /* Bitmap of free pages. */
    uint8_t *base;                      /* Base of pool. */
  };

/* Two pools: one for kernel data, one for user pages. */
static struct pool kernel_pool, user_pool;
static size_t user_pages, kernel_pages;

static void init_pool (struct pool *, void *base, size_t page_cnt,
                       const char *name);
static bool page_from_pool (const struct pool *, void *page);
size_t bitmap_scan_buddy_and_flip (const struct bitmap *b, size_t page_cnt, bool value);
size_t next_pow2(size_t);

/* Initializes the page allocator.  At most USER_PAGE_LIMIT
   pages are put into the user pool. */
void
palloc_init (size_t user_page_limit)
{
  /* Free memory starts at 1 MB and runs to the end of RAM. */
  uint8_t *free_start = ptov (1024 * 1024);
  uint8_t *free_end = ptov (init_ram_pages * PGSIZE);
  size_t free_pages = (free_end - free_start) / PGSIZE;
  user_pages = free_pages / 2;

  
  if (user_pages > user_page_limit)
    user_pages = user_page_limit;
  kernel_pages = free_pages - user_pages;

  /* Give half of memory to kernel, half to user. */
  init_pool (&kernel_pool, free_start, kernel_pages, "kernel pool");
  init_pool (&user_pool, free_start + kernel_pages * PGSIZE,
             user_pages, "user pool");
}

/* Obtains and returns a group of PAGE_CNT contiguous free pages.
   If PAL_USER is set, the pages are obtained from the user pool,
   otherwise from the kernel pool.  If PAL_ZERO is set in FLAGS,
   then the pages are filled with zeros.  If too few pages are
   available, returns a null pointer, unless PAL_ASSERT is set in
   FLAGS, in which case the kernel panics. */
void *
palloc_get_multiple (enum palloc_flags flags, size_t page_cnt)
{
  struct pool *pool = flags & PAL_USER ? &user_pool : &kernel_pool;
  void *pages;
  size_t page_idx;
  size_t buddy_start;

  if (page_cnt == 0)
    return NULL;

  lock_acquire (&pool->lock);
  page_idx = bitmap_scan_buddy_and_flip (pool, pool->used_map, page_cnt, false);
  lock_release (&pool->lock);

  if (page_idx != BITMAP_ERROR)
    pages = pool->base + PGSIZE * page_idx;
  else
    pages = NULL;

  if (pages != NULL) 
    {
      if (flags & PAL_ZERO)
        memset (pages, 0, PGSIZE * page_cnt);
    }
  else 
    {
      if (flags & PAL_ASSERT)
        PANIC ("palloc_get: out of pages");
    }

  return pages;
}

/* Obtains a single free page and returns its kernel virtual
   address.
   If PAL_USER is set, the page is obtained from the user pool,
   otherwise from the kernel pool.  If PAL_ZERO is set in FLAGS,
   then the page is filled with zeros.  If no pages are
   available, returns a null pointer, unless PAL_ASSERT is set in
   FLAGS, in which case the kernel panics. */
void *
 palloc_get_page (enum palloc_flags flags) 
{
  return palloc_get_multiple (flags, 1);
}

/* Frees the PAGE_CNT pages starting at PAGES. */
void
palloc_free_multiple (void *pages, size_t page_cnt) 
{
  struct pool *pool;
  size_t page_idx;

  ASSERT (pg_ofs (pages) == 0);
  if (pages == NULL || page_cnt == 0)
    return;

  if (page_from_pool (&kernel_pool, pages))
    pool = &kernel_pool;
  else if (page_from_pool (&user_pool, pages))
    pool = &user_pool;
  else
    NOT_REACHED ();

  page_idx = pg_no (pages) - pg_no (pool->base);

#ifndef NDEBUG
  memset (pages, 0xcc, PGSIZE * page_cnt);
#endif

  ASSERT (bitmap_all (pool->used_map, page_idx, page_cnt));
  bitmap_set_multiple (pool->used_map, page_idx, next_pow2(page_cnt), false);
}

/* Frees the page at PAGE. */
void
palloc_free_page (void *page) 
{
  palloc_free_multiple (page, 1);
}

/* Initializes pool P as starting at START and ending at END,
   naming it NAME for debugging purposes. */
static void
init_pool (struct pool *p, void *base, size_t page_cnt, const char *name) 
{
  /* We'll put the pool's used_map at its base.
     Calculate the space needed for the bitmap
     and subtract it from the pool's size. */
  size_t bm_pages = DIV_ROUND_UP (bitmap_buf_size (page_cnt), PGSIZE);
  if (bm_pages > page_cnt)
    PANIC ("Not enough memory in %s for bitmap.", name);
  page_cnt -= bm_pages;

  printf ("%zu pages available in %s.\n", page_cnt, name);

  /* Initialize the pool. */
  lock_init (&p->lock);
  p->used_map = bitmap_create_in_buf (page_cnt, base, bm_pages * PGSIZE);
  p->base = base + bm_pages * PGSIZE;
}

/* Returns true if PAGE was allocated from POOL,
   false otherwise. */
static bool
page_from_pool (const struct pool *pool, void *page) 
{
  size_t page_no = pg_no (page);
  size_t start_page = pg_no (pool->base);
  size_t end_page = start_page + bitmap_size (pool->used_map);

  return page_no >= start_page && page_no < end_page;
}

bool page_is_empty_multiple(const struct bitmap *b, size_t idx, size_t cnt) {
  ASSERT(b != NULL);
  ASSERT(idx < bitmap_size(b));

  return !bitmap_contains(b, idx, cnt, true);
}

bool page_is_empty(const struct bitmap *b, size_t idx) {
  return page_is_empty_multiple(b, idx, 1);
}

/* Obtains a status of the page pool */
void
palloc_get_status (enum palloc_flags flags)
{
  struct pool *pool = flags & PAL_USER ? &user_pool : &kernel_pool;
  size_t size = bitmap_size(pool->used_map);
  size_t i;
  char *s = (pool == &user_pool) ? "user" : "kernel";

  printf("---------- The %s pool consists of %d pages. ----------\n", s, size);
  printf("%d%16d%16d%16d%16d\n",0,8,16,24,32);
  
  for (i = 0; i < size; i++) {
    printf("%d ", !page_is_empty(pool->used_map, i));

    // 32 page status per line
    if (i % 32 == 31)
      printf("\n");
  }

  printf("\n\n");
}

size_t next_pow2(size_t num) {
  size_t res = 1;
  while (res < num) res <<= 1;
  return res;
}

size_t buddy_find(const struct bitmap *b, size_t page_cnt, size_t start, size_t end) {

  size_t gap = end - start + 1;
  size_t upper;

  // If the gap is smaller than the page size, an error is returned.
  if (gap < 1)
    return BUDDY_NOT_FOUND;

  upper = next_pow2(gap);

  if ((upper / 2) < page_cnt && page_cnt <= upper) {
    
    if (page_is_empty_multiple(b, start, page_cnt))
      return start;
  }
  else {
    if (upper < 2)
      return BUDDY_NOT_FOUND;

    size_t first = buddy_find(b, page_cnt, start, start + upper / 2 - 1);
    if (first != BUDDY_NOT_FOUND)
      return first;
    size_t second = buddy_find(b, page_cnt, start + upper / 2, start + upper - 1);
    if (second != BUDDY_NOT_FOUND)
      return second;
  }
  return BUDDY_NOT_FOUND;
}

size_t bitmap_scan_buddy_and_flip (const struct bitmap *b, size_t page_cnt, bool value) {

  size_t msize = bitmap_size(b);
  size_t find;

  ASSERT (page_cnt <= msize);

  find = buddy_find(b, page_cnt, 0, msize-1);
  if (find == BUDDY_NOT_FOUND)
    return BITMAP_ERROR;
  bitmap_set_multiple(b, find, next_pow2(page_cnt), !value);

  return find;
}
