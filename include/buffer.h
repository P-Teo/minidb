#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "pageio.h"
#include <stdbool.h>

#define MAX_BUFFER_PAGES 16  // nr maxim de pagini în cache

typedef struct {
    page_id pid;             // ID pagina
    bool dirty;              // modificată
    int pin_count;           // nr de utilizatori
    unsigned long last_used; 
    byte data[PAGE_SIZE];    // conținutul paginii
    bool valid;       
} BufferFrame;

typedef struct {
    BufferFrame frames[MAX_BUFFER_PAGES];
    unsigned long tick;      // contor LRU
    int num_pages;           // nr cadre folosite
} BufferPool;

void buffer_init(int nframes);
void buffer_close();

void* buffer_get_page(page_id pid, bool for_write);
void buffer_unpin_page(page_id pid, bool dirty);
void buffer_flush_all();

#endif
