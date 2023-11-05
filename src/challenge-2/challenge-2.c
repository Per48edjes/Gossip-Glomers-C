#include "../../lib/util.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIGNED_INT64_STRLEN 20

json_object* uniq_id_reply(json_object* uniq_id_msg, const char* node_id,
                           int64_t* logical_clock);

int main(void)
{
    json_object* init_msg = msg_recv();
    if (init_msg == NULL)
    {
        fprintf(stderr, "Error: expected init message, got EOF\n");
        exit(EXIT_FAILURE);
    }

    // Node ID + logical timestamp gives unique ids
    const char* name = node_id(init_msg);
    char node_id[strlen(name) + 1];
    strcpy(node_id, name);
    int64_t logical_clock = 0;

    msg_send(generic_reply(init_msg));
    json_object_put(init_msg);

    json_object* uniq_id_msg;
    while ((uniq_id_msg = msg_recv()) != NULL)
    {
        msg_send(uniq_id_reply(uniq_id_msg, node_id, &logical_clock));
        json_object_put(uniq_id_msg);
    }
}

// Borrows `uniq_id_msg`.
// Returns an owned object.
json_object* uniq_id_reply(json_object* uniq_id_msg, const char* node_id,
                           int64_t* logical_clock)
{
    json_object* reply = generic_reply(uniq_id_msg);

    json_object* reply_uniq_id;

    // Construct the unique id (node ID + logical timestamp)
    char logical_timestamp[MAX_SIGNED_INT64_STRLEN + 1];
    sprintf(logical_timestamp, "%ld", (*logical_clock)++);
    char uniq_id[strlen(node_id) + strlen(logical_timestamp) + 1];
    strcat(stpcpy(uniq_id, node_id), logical_timestamp);

    reply_uniq_id = json_object_new_string(uniq_id);
    json_object_object_add(json_object_object_get(reply, "body"), "id",
                           reply_uniq_id);

    return reply;
}
