#include "../../lib/collections.h"
#include "../../unity/src/unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void)
{

    BIG_DICTIONARY = dictionary_init(free);
    PEERS = malloc(num_peers * sizeof(char*));
    for (size_t i = 0; i < num_peers; i++)
    {
        dictionary_set(big_dictionary, peers[i], channel_state_init());
        PEERS[i] = strdup(peers[i]);
        NUM_PEERS++;
    }
}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    // TODO: Fill in with calls to test functions
    return UNITY_END();
}
