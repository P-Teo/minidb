#pragma once
#ifndef PAGEIO_H
#define PAGEIO_H

#include "common.h"
#include <stdio.h>

int open_data_file (const char *path); //deschid un fiser binar de date
void close_data_file(); //inchid un fisier binar de date
int read_page(page_id pid, void *buffer); //citesc o pagina in buffer
int write_page(page_id pid, const void* buffer); //scriu o pagina in buffer
page_id allocate_page(); //creez o pagina noua

#endif