#include "mgl_callback.h"
#include <string.h>
#include <stdlib.h>

MglCallback mgl_callback(void (*function)(void *data,void *context),void *data)
{
    MglCallback cb;
    cb.function = function;
    cb.data = data;
    return cb;
}

void mgl_callback_copy(MglCallback *dst,MglCallback src)
{
    if (!dst)return;
    dst->function = src.function;
    dst->data = src.data;
}

void mgl_callback_set(MglCallback *cb,void (*function)(void *data,void *context),void *data)
{
    if (!cb)return;
    cb->function = function;
    cb->data = data;
}

MglCallback *mgl_callback_new()
{
    MglCallback *cb;
    cb = (MglCallback *)malloc(sizeof(MglCallback));
    memset(cb,0,sizeof(MglCallback));
    return cb;
}

void mgl_callback_free(MglCallback **cb)
{
    if (!cb)return;
    if (!*cb)return;
    free(*cb);
    *cb = NULL;
}

void mgl_callback_delete(MglCallback *cb)
{
    if (!cb)return;
    free(cb);
}


/*eol@eof*/
