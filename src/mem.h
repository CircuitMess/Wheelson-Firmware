#ifndef WHEELSON_FIRMWARE_MEM_H
#define WHEELSON_FIRMWARE_MEM_H

#include <Arduino.h>

void* w_malloc(size_t size);

void* w_calloc(size_t n,size_t size);

#endif //WHEELSON_FIRMWARE_MEM_H
