#include "../../lib/util.h"
#include "json-c/json_object.h"
#include <json-c/json.h>
#include <stdio.h>

void queue_json_object_free(void* obj);

int main(void)
{
    json_object* init_msg = msg_recv();
    if (init_msg == NULL)
    {
        fprintf(stderr, "Error: expected init message, got EOF\n");
        exit(EXIT_FAILURE);
    }

    msg_send(generic_reply(init_msg));
    json_object_put(init_msg);
}
