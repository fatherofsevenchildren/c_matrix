
#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = 0;
  if (rows > 0 && columns > 0 &&
      (result->matrix = (double **)calloc(rows, sizeof(double *)))) {
    result->rows = rows;
    result->columns = columns;
    for (int i = 0; (error == 0) && (i != rows); i++)
      if (!(result->matrix[i] = (double *)calloc(columns, sizeof(double)))) {
        error = 2;
        for (int y = 0; y < i; y++) free(result->matrix[y]);
        free(result->matrix);
      }
  } else
    error = 1;
  return error;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i != A->rows; i++) free(A->matrix[i]);
    free(A->matrix);
    A->matrix = NULL;
  }
  A->rows = 0;
  A->columns = 0;
}

int check(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (!A || !B || !result || A->rows < 1 || A->columns < 1 || B->rows < 1 ||
      B->columns < 1)
    res = 1;
  return res;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !B || A->rows < 1 || A->columns < 1 || B->rows < 1 ||
      B->columns < 1 || A->rows != B->rows || A->columns != B->columns)
    return 0;
  int res = 1;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++)
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) res--;
  }
  if (res < 1) res = 0;
  return res;
}

// znak == 0 => сложение и znak == 1 => вычитание

int s21_sum_and_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result,
                           int znak) {
  if (check(A, B, result)) return 1;
  if (A->rows != B->rows || A->columns != B->columns ||
      s21_create_matrix(A->rows, A->columns, result) == 2)
    return 2;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (!znak)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      else
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return 0;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sum_and_sub_matrix(A, B, result, 0);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_sum_and_sub_matrix(A, B, result, 1);
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (!A || !result || A->rows < 1 || A->columns < 1) return 1;
  if (s21_create_matrix(A->rows, A->columns, result) == 2) return 2;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++)
      result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return 0;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (check(A, B, result)) return 1;
  if (A->columns != B->rows) return 2;
  int error = s21_create_matrix(A->rows, B->columns, result);
  for (int i = 0; !error && (i != A->rows); i++) {
    for (int j = 0; j != B->columns; j++) {
      result->matrix[i][j] = 0;
      for (int k = 0; k != A->columns; k++)
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
    }
  }
  return error;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (!A || !result || A->rows < 1 || A->columns < 1) return 1;
  if (s21_create_matrix(A->columns, A->rows, result) == 2) return 2;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) result->matrix[j][i] = A->matrix[i][j];
  }
  return 0;
}

// void print_matrix (matrix_t res) {
//     for(int i = 0; i < res.rows; i++){
//         for(int j = 0; j < res.columns; j++) {
//         printf(" %lf", res.matrix[i][j]);
//         if (j == res.columns - 1) printf("\n");
//         }
//     }
// }

matrix_t matrix_minus_odin(matrix_t A, int i, int j) {
  matrix_t result = {0, 0, 0};
  s21_create_matrix(A.rows - 1, A.columns - 1, &result);
  for (int a = 0; a < result.rows; a++) {
    int skip_a = 0;
    if (a >= i) skip_a++;
    for (int b = 0; b < result.columns; b++) {
      int skip_b = 0;
      if (b >= j) skip_b++;
      result.matrix[a][b] = A.matrix[a + skip_a][b + skip_b];
    }
  }
  return result;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (!A || A->columns < 1 || A->rows < 1 || !result) return 1;
  if (A->columns != A->rows ||
      s21_create_matrix(A->rows, A->columns, result) == 2)
    return 2;
  if (A->columns == 1)
    result->matrix[0][0] = A->matrix[0][0];
  else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        matrix_t var_matrix = {};
        var_matrix = matrix_minus_odin(*A, i, j);
        s21_determinant(&var_matrix, &result->matrix[i][j]);
        result->matrix[i][j] *= pow(-1, (i + j));
        s21_remove_matrix(&var_matrix);
      }
    }
  }
  return 0;
}

int s21_determinant(matrix_t *A, double *result) {
  if (!A || A->columns < 1 || A->rows < 1 || !result) return 1;
  if (A->columns != A->rows) return 2;
  double cur = 0;
  if (A->columns == 1)
    cur = A->matrix[0][0];
  else if (A->columns == 2) {
    cur = A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    for (int j = 0; j < A->columns; j++) {
      matrix_t var_matrix = matrix_minus_odin(*A, 0, j);
      double var_res = 0;
      s21_determinant(&var_matrix, &var_res);
      cur += A->matrix[0][j] * pow(-1.00, (double)j) * var_res;
      s21_remove_matrix(&var_matrix);
    }
  }
  *result = cur;
  return 0;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A->columns < 1 || A->rows < 1 || !result) return 1;
  if (A->columns != A->rows) return 2;
  int error = 0;
  if (A->columns == 1) {
    if (!A->matrix[0][0]) error = 2;
    s21_create_matrix(1, 1, result);
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
  } else {
    double var_det = 0;
    int res = s21_determinant(A, &var_det);
    if (!var_det || res == 2 || res == 1) return 2;
    matrix_t var_matrix1 = {}, var_matrix2 = {};
    if (!s21_calc_complements(A, &var_matrix1)) {
      s21_transpose(&var_matrix1, &var_matrix2);
      s21_remove_matrix(&var_matrix1);
      s21_mult_number(&var_matrix2, 1.00 / var_det, result);
      s21_remove_matrix(&var_matrix2);
    } else
      error = 2;
  }
  return error;
}
