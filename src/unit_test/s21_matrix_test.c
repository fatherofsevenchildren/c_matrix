#include "s21_matrix_test.h"
// Создаем функцию которая поочередна проганяет тестовый набор

void test_sets(void) { run_set(suite_matrix()); }

void run_set(Suite *suite_matrix) {
  // Представляет собой окружение для запуска тестов. Запускает наш набор
  // тестов.
  SRunner *all_test = srunner_create(suite_matrix);

  srunner_set_fork_status(all_test, CK_NOFORK);
  srunner_run_all(all_test, CK_NORMAL);

  srunner_free(all_test);
}

int main(void) {
  test_sets();
  return 0;
}
