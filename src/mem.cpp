#include "mem.h"
#include <esp32-hal-psram.h>

void* w_malloc(size_t size){
	if(psramFound()){
		return ps_malloc(size);
	}else{
		return malloc(size);
	}
}

void* w_calloc(size_t n,size_t size){
	if(psramFound()){
		return  ps_calloc(n,size);
	}else{
		return calloc(n,size);
	}
}