#include "../../lib/collections.h"
#include "../../lib/tcp.h"
#include "../../lib/util.h"
#include "json_object.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void single_node_event_loop(const char** peers, const int num_peers);
void list_json_object_free(void* obj);
void broadcast_request_handler(json_object* client_request, List* message_list,
                               const char** peers, const int num_peers);
void forwarded_broadcast_handler(json_object* forwarded_msg,
                                 List* message_list);
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

    const char** peers = node_ids(init_msg);
    const int num_peers = node_ids_count(init_msg);
    tcp_init(peers, num_peers);

    single_node_event_loop(peers, num_peers);

    tcp_free();
    json_object_put(init_msg);
}

void single_node_event_loop(const char** peers, const int num_peers)
{
    List* message_list = list_init(list_json_object_free);

    json_object* msg;
    while ((msg = msg_recv_listener()) != NULL)
    {
        const char* type = msg_type(msg);

        if (strcmp(type, "broadcast") == 0)
        {
            broadcast_request_handler(msg, message_list, peers, num_peers);
        }
        if (strcmp(type, "forwarded") == 0)
        {
            forwarded_broadcast_handler(msg, message_list);
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
void broadcast_request_handler(json_object* client_request, List* message_list,
                               const char** peers, const int num_peers)
{
    // Construct client response message
    json_object* broadcast_response = generic_reply(client_request);

    // Take ownership of the message from the `client_request`; append to
    // `message_list`
    json_object* message = json_object_object_get(
        json_object_object_get(client_request, "body"), "message");
    list_append(message_list, message);
    json_object_get(message);

    // Send read response (to client)
    msg_send(broadcast_response);

    // Broadcast message to all peers
    for (int i = 0; i < num_peers; i++)
    {
        json_object* broadcast_msg = json_object_new_object();
        json_object* src = json_object_object_get(client_request, "dest");
        json_object_object_add(broadcast_msg, "src", src);
        json_object_get(src);
        json_object_object_add(broadcast_msg, "dest",
                               json_object_new_string(peers[i]));
        json_object* body = json_object_object_get(client_request, "body");
        json_object_object_add(broadcast_msg, "body", body);
        json_object_get(body);

        msg_send_pusher(broadcast_msg);
    }

    // Clean up: free `client_request` message
    json_object_put(client_request);
}

// Takes ownership of `forwarded_msg`.
void forwarded_broadcast_handler(json_object* forwarded_msg, List* message_list)
{
    // Take ownership of the message from the `forwarded_msg`; append to
    // `message_list`
    json_object* message = json_object_object_get(
        json_object_object_get(forwarded_msg, "body"), "message");
    list_append(message_list, message);
    json_object_get(message);

    // Clean up: free `forwarded_msg`
    json_object_put(forwarded_msg);
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

// WARN: Doesn't do anything right now other than send a `topology_ok`
// response. Takes ownership of `topology_msg`.
void topology_handler(json_object* topology_msg)
{
    // Construct topology response message
    json_object* topology_response = generic_reply(topology_msg);

    // Send topology response (to client)
    msg_send(topology_response);

    // Clean up: free topology message
    json_object_put(topology_msg);
}
