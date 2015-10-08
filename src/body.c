#include "body.h"


void body_reset(Body *body)
{
    if (!body)return;
    body->_needsBackoff = 0;
    body->_done = 0;    
}

void body_process(Body *body)
{
    if (!body)return;
    if (body->_needsBackoff)
    {
        vec3d_add(body->position,body->position,body->_stepOffVector);
    }
}

/*eol@eof*/
