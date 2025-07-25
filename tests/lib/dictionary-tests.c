#include "../../lib/collections.h"
#include "../../unity/src/unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}

void test_instantiate_and_populate_dictionary()
{
    const char* keys[] = {"1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
                          "10", "11", "12", "13", "14", "15", "16", "17", "18",
                          "19", "20", "21", "22", "23", "24", "25"};
    int values[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13,
                    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

    Dictionary* big_dictionary = dictionary_init(free);
    size_t num_keys = sizeof(keys) / sizeof(keys[0]);

    for (size_t i = 0; i < num_keys; i++)
    {
        int* value = malloc(sizeof(int));
        *value = values[i];
        dictionary_set(big_dictionary, keys[i], value);
    }
    
    // Verify that all values can be retrieved correctly
    for (size_t i = 0; i < num_keys; i++)
    {
        int* retrieved_value = (int*)dictionary_get(big_dictionary, keys[i]);
        TEST_ASSERT_EQUAL_INT(values[i], *retrieved_value);
    }
    
    // Free the dictionary when done
    dictionary_free(big_dictionary);
}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    test_instantiate_and_populate_dictionary();
    return UNITY_END();
}
