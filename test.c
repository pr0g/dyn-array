#include <unity.h>

#include "dyn-array.h"

typedef struct elem_t {
  int val_;
} elem_t;

void setUp(void) {
}

void tearDown(void) {
}

void test_uninitialized_array_has_zero_size(void) {
  elem_t* elems = NULL;
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
}

void test_uninitialized_array_is_empty(void) {
  elem_t* elems = NULL;
  const bool empty = array_empty(elems);
  TEST_ASSERT_TRUE(empty);
}

void test_zero_reserved_array_has_zero_size(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 0);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
  array_free(elems);
}

void test_zero_reserved_array_is_empty(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 0);
  const bool empty = array_empty(elems);
  TEST_ASSERT_TRUE(empty);
  array_free(elems);
}

void test_array_reserve_one_element_from_uninitialized(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 1);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(1, capacity);
  array_free(elems);
}

void test_array_reserve_n_element_from_uninitialized(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 5);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(5, capacity);
  array_free(elems);
}

void test_array_push_grows_by_one_initially(void) {
  elem_t* elems = NULL;
  array_push(elems, (elem_t){.val_ = 1});
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(1, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(1, capacity);
  array_free(elems);
}

void test_array_push_grows_exponentially(void) {
  const int growth_rate[] = {1, 2, 4, 4, 8, 8, 8, 8, 16, 16};
  elem_t* elems = NULL;
  for (int i = 0; i < 10; ++i) {
    array_push(elems, (elem_t){.val_ = 1});
    const int size = array_size(elems);
    TEST_ASSERT_EQUAL_INT32(i + 1, size);
    const int capacity = array_capacity(elems);
    TEST_ASSERT_EQUAL_INT32(growth_rate[i], capacity);
  }
  array_free(elems);
}

void test_array_reserved_and_not_resized(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 10);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(10, capacity);
  array_free(elems);
}

void test_array_reserved_and_not_resized_after_initial_reserve(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 10);
  array_reserve(elems, 20);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(20, capacity);
  array_free(elems);
}

void test_array_does_not_grow_after_push_with_reservation(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 20);
  for (int i = 0; i < 20; ++i) {
    array_push(elems, (elem_t){.val_ = i});
  }
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(20, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(20, capacity);
  array_free(elems);
}

void test_array_growth_doubles_after_exceeding_reservation(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 20);
  for (int i = 0; i < 21; ++i) {
    array_push(elems, (elem_t){.val_ = i});
  }
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(21, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(40, capacity);
  array_free(elems);
}

void test_array_can_be_resized_from_uninitialized(void) {
  elem_t* elems = NULL;
  array_resize(elems, 10);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(10, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(10, capacity);
  array_free(elems);
}

void test_array_can_be_resized_to_increase_after_initial_resize(void) {
  elem_t* elems = NULL;
  array_resize(elems, 10);
  array_resize(elems, 20);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(20, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(20, capacity);
  array_free(elems);
}

void test_array_can_be_resized_to_decrease_after_initial_resize(void) {
  elem_t* elems = NULL;
  array_resize(elems, 50);
  array_resize(elems, 30);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(30, size);
  const int capacity = array_capacity(elems);
  TEST_ASSERT_EQUAL_INT32(50, capacity);
  array_free(elems);
}

void test_array_single_element_pop(void) {
  elem_t* elems = NULL;
  array_push(elems, (elem_t){.val_ = 5});
  array_pop(elems);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
}

void test_array_size_can_be_decreased_with_pop(void) {
  elem_t* elems = NULL;
  array_resize(elems, 50);
  array_pop(elems);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(49, size);
}

void test_array_pop_on_empty_has_no_effect(void) {
  elem_t* elems = NULL;
  array_reserve(elems, 0);
  array_pop(elems);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
}

void test_array_pop_on_uninitialized_has_no_effect(void) {
  elem_t* elems = NULL;
  array_pop(elems);
  const int size = array_size(elems);
  TEST_ASSERT_EQUAL_INT32(0, size);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_uninitialized_array_has_zero_size);
  RUN_TEST(test_uninitialized_array_is_empty);
  RUN_TEST(test_zero_reserved_array_has_zero_size);
  RUN_TEST(test_zero_reserved_array_is_empty);
  RUN_TEST(test_array_reserve_one_element_from_uninitialized);
  RUN_TEST(test_array_reserve_n_element_from_uninitialized);
  RUN_TEST(test_array_push_grows_by_one_initially);
  RUN_TEST(test_array_push_grows_exponentially);
  RUN_TEST(test_array_reserved_and_not_resized);
  RUN_TEST(test_array_reserved_and_not_resized_after_initial_reserve);
  RUN_TEST(test_array_does_not_grow_after_push_with_reservation);
  RUN_TEST(test_array_growth_doubles_after_exceeding_reservation);
  RUN_TEST(test_array_can_be_resized_from_uninitialized);
  RUN_TEST(test_array_can_be_resized_to_increase_after_initial_resize);
  RUN_TEST(test_array_can_be_resized_to_decrease_after_initial_resize);
  RUN_TEST(test_array_single_element_pop);
  RUN_TEST(test_array_size_can_be_decreased_with_pop);
  RUN_TEST(test_array_pop_on_empty_has_no_effect);
  RUN_TEST(test_array_pop_on_uninitialized_has_no_effect);
  return UNITY_END();
}
