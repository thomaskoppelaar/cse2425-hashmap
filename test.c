/* Import test framework. */
#include <stdlib.h>
#include <stdio.h>
#include "gest.h"
#include "gest-run.h"

struct HashMap* create_hashmap(size_t size);

void* rcc(void* old_data, void* new_data) {
    return new_data;
}

void ddc(void* data) {
    //printf("we here now\n");
   free(data);
}

/* Import submission. */
#include "solution.c"
/* One may use a number of different assertions:
    - assert_int_equals(a,b) to compare two integers.
    - assert_dbl_equals(a,b) to compare two floating point numbers.
    - assert_ptr_equals(a,b) to compare two pointers.
    - assert_str_equals(a,b) to compare two strings.
    - assert_that(c) to check if a condition holds. (similar to assert(c) from assert.h)
    - assert_true(c) synonym for assert_that.
    - assert_false(c) holds when assert_true fails and the other way around.
    - assert_fail(m) to fail with a given message.
*/
/* Test cases. */



void test1() {
    size_t size = 4;
    HashMap* hm = create_hashmap(size);
    assert_true(get_data(hm, "aaa") == NULL);

    int data1 = 42;
    void* ptr1 = &data1;
    insert_data(hm, "aaa", ptr1, rcc);

    delete_hashmap(hm, NULL);
}




/* Register all test cases. */
void register_tests() {
    register_test(test1);
    
}

