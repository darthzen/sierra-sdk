#include "sqf.h"

int
bitarray_init(bitarray_s* ctx, uint8_t* buf, unsigned sz)
{
    ctx->buf = buf;
    ctx->sz = sz;
}

int
bitarray_set(bitarray_s* ctx, unsigned idx)
{
    unsigned offset;
    uint8_t mask;
    offset = idx/8;
    mask = (1<<idx%8);
    ctx->buf[offset] |= mask;
}

int
bitarray_max(bitarray_s* ctx)
{
    unsigned offset;
    uint8_t mask;
    unsigned i;

    for(i=ctx->sz-1;i>0;i--)
    {
        offset = i/8;
        mask = (1<<i%8);

        if (ctx->buf[offset]&mask)
            break;
    }
    return i;
}

int
bitarray_get(bitarray_s* ctx, unsigned idx)
{
    unsigned offset;
    uint8_t mask;
    offset = idx/8;
    mask = (1<<idx%8);
    return ((ctx->buf[offset] & mask) != 0);
}
