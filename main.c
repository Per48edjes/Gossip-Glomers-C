#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <json-c/json.h>

json_object* init_reply(json_object* init_msg);
json_object* echo_reply(json_object* echo_msg);

// TODO: Remove; only included for debugging (from json_object_private.h)
struct json_object
{
    enum json_type o_type;
    uint32_t _ref_count;
    json_object_to_json_string_fn* _to_json_string;
    struct printbuf* _pb;
    json_object_delete_fn* _user_delete;
    void* _userdata;
    // Actually longer, always malloc'd as some more-specific type.
    // The rest of a struct json_object_${o_type} follows
};

uint64_t REPLY_MSG_ID = 0;

int main()
{
    char* input_line = NULL;
    size_t input_len = 0;
    ssize_t input_bytes_read;

    while ((REPLY_MSG_ID == 0) &&
           (input_bytes_read = getline(&input_line, &input_len, stdin)) != -1)
    {
        json_object* init_msg = json_tokener_parse(input_line);
        printf("init_msg _ref_count at initialization: %u\n",
               init_msg->_ref_count);
        json_object* init_msg_reply = init_reply(init_msg);
        printf("%s\n", json_object_to_json_string(init_msg_reply));
        fflush(stdout);

        // ISSUE: Why does this cause a segfault?
        /*
        printf(
            "init_msg_reply:reply_dest _ref_count after print to STDOUT: "
            "%u\n",
            json_object_object_get(init_msg_reply, "reply_dest")->_ref_count);
        */

        printf("init_msg:src _ref_count BEFORE init_msg_reply "
               "_put calls: %u\n",
               json_object_object_get(init_msg_reply, "src")->_ref_count);

        printf("init_msg_reply _ref_count BEFORE put: %u\n",
               init_msg_reply->_ref_count);
        json_object_put(init_msg_reply);

        printf("init_msg:src _ref_count BEFORE init_msg "
               "_put calls: %u\n",
               json_object_object_get(init_msg, "src")->_ref_count);

        printf("init_msg _ref_count BEFORE put: %u\n", init_msg->_ref_count);
        json_object_put(init_msg);
    }

    while ((input_bytes_read = getline(&input_line, &input_len, stdin)) != -1)
    {
        json_object* echo_msg = json_tokener_parse(input_line);
        json_object* echo_msg_reply = echo_reply(echo_msg);
        printf("%s\n", json_object_to_json_string(echo_msg_reply));
        fflush(stdout);
        json_object_put(echo_msg_reply);
        json_object_put(echo_msg);
    }

    free(input_line);
}

json_object* init_reply(json_object* init_msg)
{
    // Access `dest`, `src`, `in_reply_to` fields
    json_object* reply_dest = json_object_object_get(init_msg, "src");
    json_object* reply_src = json_object_object_get(init_msg, "dest");
    json_object* reply_in_reply_to = json_object_object_get(
        json_object_object_get(init_msg, "body"), "msg_id");

    // Create `msg_id` field
    json_object* reply_msg_id = json_object_new_int64(REPLY_MSG_ID++);

    // Create `reply` and add dest`, `src` fields
    json_object* reply = json_object_new_object();
    json_object_object_add(reply, "src", reply_src);
    json_object_object_add(reply, "dest", reply_dest);

    // Create `body` field
    json_object* reply_body = json_object_new_object();
    json_object_object_add(reply_body, "type",
                           json_object_new_string("init_ok"));
    json_object_object_add(reply_body, "msg_id", reply_msg_id);
    json_object_object_add(reply_body, "in_reply_to", reply_in_reply_to);

    // Add `body` to `reply` object
    json_object_object_add(reply, "body", reply_body);

    printf("reply_dest _ref_count BEFORE _get calls: %u\n",
           reply_dest->_ref_count);
    // Retain objects before returning them
    json_object_get(reply_src);
    json_object_get(reply_dest);
    json_object_get(reply_in_reply_to);

    printf("reply_dest _ref_count AFTER _get calls: %u\n",
           reply_dest->_ref_count);

    return reply;
}

json_object* echo_reply(json_object* echo_msg)
{
    // Access `dest`, `src`, `in_reply_to`, `echo` fields
    json_object* reply_dest = json_object_object_get(echo_msg, "src");
    json_object* reply_src = json_object_object_get(echo_msg, "dest");
    json_object* reply_in_reply_to = json_object_object_get(
        json_object_object_get(echo_msg, "body"), "msg_id");
    json_object* reply_echo = json_object_object_get(
        json_object_object_get(echo_msg, "body"), "echo");

    // Create `msg_id` field
    json_object* reply_msg_id = json_object_new_int64(REPLY_MSG_ID++);

    // Create `reply` and add dest`, `src` fields
    json_object* reply = json_object_new_object();
    json_object_object_add(reply, "src", reply_src);
    json_object_object_add(reply, "dest", reply_dest);

    // Create `body` field
    json_object* reply_body = json_object_new_object();
    json_object_object_add(reply_body, "type",
                           json_object_new_string("echo_ok"));
    json_object_object_add(reply_body, "msg_id", reply_msg_id);
    json_object_object_add(reply_body, "in_reply_to", reply_in_reply_to);

    // "Echo" the `echo` field
    json_object_object_add(reply_body, "echo", reply_echo);

    // Add `body` to `reply` object
    json_object_object_add(reply, "body", reply_body);

    // Retain objects before returning them
    json_object_get(reply_src);
    json_object_get(reply_dest);
    json_object_get(reply_in_reply_to);
    json_object_get(reply_echo);

    return reply;
}
