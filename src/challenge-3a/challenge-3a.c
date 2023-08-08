#include "../../lib/collections.h"
#include "../../lib/util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void single_node_event_loop();
void list_json_object_free(void* obj);
void broadcast_request_handler(json_object* client_request, List* message_list);
void read_request_handler(json_object* client_request, List* message_list);
void topology_handler(json_object* topology_msg);

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

    single_node_event_loop();
}

void single_node_event_loop()
{
    List* message_list = list_init(list_json_object_free);

    json_object* msg;
    while ((msg = msg_recv()) != NULL)
    {
        const char* type = msg_type(msg);

        if (strcmp(type, "broadcast") == 0)
        {
            broadcast_request_handler(msg, message_list);
        }
        else if (strcmp(type, "read") == 0)
        {
            read_request_handler(msg, message_list);
        }
        else if (strcmp(type, "topology") == 0)
        {
            topology_handler(msg);
        }
        else
        {
            fprintf(stderr,
                    "Error: follower_event_loop: unrecognized message type: "
                    "\"%s\"\n",
                    type);
            free(msg);
            list_free(message_list);
            exit(EXIT_FAILURE);
        }
    }
    list_free(message_list);
}

// Helper function for using `json_object`s in a `List`.
void list_json_object_free(void* obj) { json_object_put(obj); }

// Takes ownership of `client_request`.
void broadcast_request_handler(json_object* client_request, List* message_list)
{
    // Construct client response message
    json_object* broadcast_response = generic_reply(client_request);

    // Borrow the message from the client request; enqueue it
    json_object* message = json_object_object_get(
        json_object_object_get(client_request, "body"), "message");
    list_append(message_list, message);
    json_object_get(message);

    // Send read response (to client)
    msg_send(broadcast_response);

    // Clean up: free client request message
    json_object_put(client_request);
}

// Takes ownership of `client_request`.
void read_request_handler(json_object* client_request, List* message_list)
{
    // Construct client response message
    json_object* read_response = generic_reply(client_request);

    // Add borrowed received messages to JSON array object
    json_object* messages = json_object_new_array();
    for (size_t i = 0; i < list_length(message_list); i++)
    {
        json_object* message = list_get_at(message_list, i);
        json_object_array_add(messages, message);
        json_object_get(message);
    }
    // Add `messages` array to response body
    json_object_object_add(json_object_object_get(read_response, "body"),
                           "messages", messages);

    // Send read response (to client)
    msg_send(read_response);

    // Clean up: free client request message
    json_object_put(client_request);
}

// Takes ownership of `topology_msg`.
void topology_handler(json_object* topology_msg)
{
    // Construct topology response message
    json_object* topology_response = generic_reply(topology_msg);

    // Send topology response (to client)
    msg_send(topology_response);

    // Clean up: free topology message
    json_object_put(topology_msg);
}
