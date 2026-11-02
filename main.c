#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief Динамічно виділяє пам'ять під двовимірну матрицю розміром rows x cols.
 * @param rows Кількість рядків.
 * @param cols Кількість стовпців.
 * @return Вказівник на створену матрицю (int**).
 */
int** allocate_matrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (!matrix) return NULL;

    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

/**
 * @brief Звільняє динамічно виділену пам'ять під матрицю.
 * @param matrix Вказівник на матрицю.
 * @param rows Кількість рядків.
 */
void free_matrix(int** matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/**
 * @brief Виводить матрицю на екран.
 * @param matrix Вказівник на матрицю.
 * @param rows Кількість рядків.
 * @param cols Кількість стовпців.
 * @param name Назва матриці для відображення.
 */
void print_matrix(int** matrix, int rows, int cols, const char* name) {
    printf("\n--- Матриця %s (%dx%d) ---\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Заповнює матрицю обраним способом (ручний ввід, рандом, формула).
 * @param matrix Вказівник на матрицю.
 * @param rows Кількість рядків.
 * @param cols Кількість стовпців.
 */
void fill_matrix(int** matrix, int rows, int cols) {
    int mode;
    printf("\nОберіть спосіб заповнення елементів:\n");
    printf("1 - З клавіатури\n");
    printf("2 - Генератором випадкових чисел [-20..20]\n");
    printf("3 - За формулою (A[i][j] = i + j + 1)\n");
    printf("Ваш вибір: ");
    scanf("%d", &mode);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mode == 1) {
                printf("[%d][%d] = ", i, j);
                scanf("%d", &matrix[i][j]);
            } else if (mode == 2) {
                matrix[i][j] = (rand() % 41) - 20;
            } else {
                matrix[i][j] = i + j + 1;
            }
        }
    }
}

int main() {
// Встановлюємо кодування UTF-8 для консолі Windows (вхід і вихід)
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // Резервне налаштування локалі
    setlocale(LC_ALL, ".65001");

    srand((unsigned int)time(NULL));

    printf("Лабораторна робота №4. Частина 2.\n");
    return 0;
}
