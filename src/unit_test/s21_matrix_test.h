#ifndef SRC_UNIT_TEST_S21_STRING_TEST_H_
#define SRC_UNIT_TEST_S21_STRING_TEST_H_

#include <check.h>
#include <stdlib.h>

#include "../s21_matrix.h"

Suite *suite_matrix(void);

void test_sets(void);
void run_set(Suite *current_set);

#endif
