#ifndef _BA_H_
#define _BA_H_
#include <stdint.h>
#include "sqf.h"

int
bitarray_init(bitarray_s* ctx, uint8_t* buf, unsigned sz);

int
bitarray_set(bitarray_s* ctx, unsigned idx);

int
bitarray_get(bitarray_s* ctx, unsigned idx);
#endif
