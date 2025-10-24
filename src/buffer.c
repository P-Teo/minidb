#include "buffer.h"
#include <stdio.h>
#include <string.h>

static BufferPool pool;
static int buffer_capacity = MAX_BUFFER_PAGES;

void buffer_init(int nframes)
{
    if (nframes > MAX_BUFFER_PAGES) nframes = MAX_BUFFER_PAGES;
    buffer_capacity = nframes;
    memset(&pool, 0, sizeof(BufferPool));
    pool.num_pages = 0;
    pool.tick = 0;
    printf("[Buffer] Inițializat cu %d cadre.\n", nframes);
}

static int find_frame(page_id pid)
{
    for (int i = 0; i < buffer_capacity; i++)
        if (pool.frames[i].valid && pool.frames[i].pid == pid)
            return i;
    return -1;
}

static int find_free_frame()
{
    for (int i = 0; i < buffer_capacity; i++)
        if (!pool.frames[i].valid)
            return i;
    return -1;
}

static int evict_frame()
{
    unsigned long oldest_tick = (unsigned long)-1;
    int evict_index = -1;

    for (int i = 0; i < buffer_capacity; i++) {
        BufferFrame *f = &pool.frames[i];
        if (f->pin_count == 0 && f->valid) {
            if (f->last_used < oldest_tick) {
                oldest_tick = f->last_used;
                evict_index = i;
            }
        }
    }

    if (evict_index == -1) {
        fprintf(stderr, "[Buffer] Nu pot face eviction: toate paginile sunt pin-uite!\n");
        return -1;
    }

    BufferFrame *victim = &pool.frames[evict_index];
    if (victim->dirty) {
        write_page(victim->pid, victim->data);
    }
    victim->valid = false;
    return evict_index;
}

void* buffer_get_page(page_id pid, bool for_write)
{
    pool.tick++;

    int idx = find_frame(pid);
    if (idx >= 0) {
        BufferFrame *f = &pool.frames[idx];
        f->pin_count++;
        f->last_used = pool.tick;
        return f->data;
    }

    int free_idx = find_free_frame();
    if (free_idx < 0) free_idx = evict_frame();
    if (free_idx < 0) return NULL;

    BufferFrame *f = &pool.frames[free_idx];
    if (read_page(pid, f->data) < 0)
        return NULL;

    f->pid = pid;
    f->valid = true;
    f->dirty = false;
    f->pin_count = 1;
    f->last_used = pool.tick;

    return f->data;
}

void buffer_unpin_page(page_id pid, bool dirty)
{
    int idx = find_frame(pid);
    if (idx < 0) return;

    BufferFrame *f = &pool.frames[idx];
    if (f->pin_count > 0) f->pin_count--;
    if (dirty) f->dirty = true;
}

void buffer_flush_all()
{
    for (int i = 0; i < buffer_capacity; i++) {
        BufferFrame *f = &pool.frames[i];
        if (f->valid && f->dirty) {
            write_page(f->pid, f->data);
            f->dirty = false;
        }
    }
}

void buffer_close()
{
    buffer_flush_all();
    memset(&pool, 0, sizeof(BufferPool));
    printf("[Buffer] Închis și toate paginile scrise.\n");
}
