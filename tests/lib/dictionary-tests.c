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

void test_dictionary_length()
{
    Dictionary* dict = dictionary_init(free);
    
    // Test empty dictionary length
    TEST_ASSERT_EQUAL_UINT(0, dictionary_length(dict));
    
    // Add one item and test length
    int* value = malloc(sizeof(int));
    *value = 42;
    dictionary_set(dict, "test", value);
    TEST_ASSERT_EQUAL_UINT(1, dictionary_length(dict));
    
    // Add multiple items and test length
    for (int i = 0; i < 10; i++) {
        char key[10];
        sprintf(key, "key%d", i);
        
        int* val = malloc(sizeof(int));
        *val = i * 10;
        dictionary_set(dict, key, val);
    }
    TEST_ASSERT_EQUAL_UINT(11, dictionary_length(dict));
    
    dictionary_free(dict);
}

void test_dictionary_contains()
{
    Dictionary* dict = dictionary_init(free);
    
    // Test empty dictionary
    TEST_ASSERT_FALSE(dictionary_contains(dict, "non-existent"));
    
    // Add items and test contains
    int* value1 = malloc(sizeof(int));
    *value1 = 123;
    dictionary_set(dict, "exists", value1);
    
    int* value2 = malloc(sizeof(int));
    *value2 = 456;
    dictionary_set(dict, "also-exists", value2);
    
    TEST_ASSERT_TRUE(dictionary_contains(dict, "exists"));
    TEST_ASSERT_TRUE(dictionary_contains(dict, "also-exists"));
    TEST_ASSERT_FALSE(dictionary_contains(dict, "non-existent"));
    
    dictionary_free(dict);
}

void test_dictionary_update_value()
{
    Dictionary* dict = dictionary_init(free);
    
    // Add initial value
    int* value1 = malloc(sizeof(int));
    *value1 = 100;
    dictionary_set(dict, "key", value1);
    
    // Verify initial value
    int* retrieved = dictionary_get(dict, "key");
    TEST_ASSERT_EQUAL_INT(100, *retrieved);
    
    // Update with new value
    int* value2 = malloc(sizeof(int));
    *value2 = 200;
    dictionary_set(dict, "key", value2);
    
    // Verify updated value and length hasn't changed
    retrieved = dictionary_get(dict, "key");
    TEST_ASSERT_EQUAL_INT(200, *retrieved);
    TEST_ASSERT_EQUAL_UINT(1, dictionary_length(dict));
    
    dictionary_free(dict);
}

void test_dictionary_collision_handling()
{
    // This test verifies the dictionary can handle hash collisions
    // We'll add many keys to force rebuilding and potential collisions
    Dictionary* dict = dictionary_init(free);
    
    // Add 100 items with generated keys
    for (int i = 0; i < 100; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        
        int* val = malloc(sizeof(int));
        *val = i;
        dictionary_set(dict, key, val);
    }
    
    // Verify all items can be retrieved
    for (int i = 0; i < 100; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        
        int* val = dictionary_get(dict, key);
        TEST_ASSERT_EQUAL_INT(i, *val);
    }
    
    // Check that dictionary length is correct
    TEST_ASSERT_EQUAL_UINT(100, dictionary_length(dict));
    
    dictionary_free(dict);
}

void test_dictionary_rebuild()
{
    // Test specifically for the dictionary rebuild functionality
    Dictionary* dict = dictionary_init(free);
    
    // Add enough items to trigger at least one rebuild
    // The rebuild threshold is when length >= max_length / 2
    // Initial max_length is INITIAL_DICTIONARY_MAX_LENGTH (16)
    // So we need to add at least 8 items to trigger a rebuild
    for (int i = 0; i < 30; i++) {
        char key[20];
        sprintf(key, "rebuild-key%d", i);
        
        int* val = malloc(sizeof(int));
        *val = i * 100;
        dictionary_set(dict, key, val);
    }
    
    // Verify all items are still accessible after rebuild
    for (int i = 0; i < 30; i++) {
        char key[20];
        sprintf(key, "rebuild-key%d", i);
        
        TEST_ASSERT_TRUE(dictionary_contains(dict, key));
        int* val = dictionary_get(dict, key);
        TEST_ASSERT_EQUAL_INT(i * 100, *val);
    }
    
    TEST_ASSERT_EQUAL_UINT(30, dictionary_length(dict));
    
    dictionary_free(dict);
}

void test_dictionary_error_handling()
{
    Dictionary* dict = dictionary_init(free);
    
    // Add a test key
    int* value = malloc(sizeof(int));
    *value = 999;
    dictionary_set(dict, "test-key", value);
    
    // Test that dictionary_get finds the existing key
    int* retrieved = dictionary_get(dict, "test-key");
    TEST_ASSERT_EQUAL_INT(999, *retrieved);
    
    // We can't directly test the error cases because they call exit()
    // which would terminate our test process.
    // In a real-world scenario, these would be handled differently.
    
    dictionary_free(dict);
}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_instantiate_and_populate_dictionary);
    RUN_TEST(test_dictionary_length);
    RUN_TEST(test_dictionary_contains);
    RUN_TEST(test_dictionary_update_value);
    RUN_TEST(test_dictionary_collision_handling);
    RUN_TEST(test_dictionary_rebuild);
    RUN_TEST(test_dictionary_error_handling);
    return UNITY_END();
}
