#pragma once

#include "collections.h"
#include <json-c/json.h>

// Event Loops
void follower_event_loop();
void leader_event_loop();

// Handlers and Dispatchers
void conch_response_handler(json_object* conch_response);
void conch_request_handler(json_object* conch_request, Queue* conch_queue);
void conch_release_handler(json_object* conch_release, Conch* conch);
void conch_response_dispatcher(Conch* conch, Queue* request_queue);
void client_request_handler(json_object* client_request);
