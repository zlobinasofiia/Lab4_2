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

/**
 * @brief Знаходить max і min елементи вище (або нижче) головної діагоналі.
 * @param A Квадратна матриця.
 * @param n_a Розмірність матриці A.
 * @param max_a Вказівник для збереження максимуму.
 * @param min_a Вказівник для збереження мінімуму.
 * @param variant Номер варіанту (парний - нижче діагоналі, непарний - вище).
 */
 void max_min(int** A, int n_a, int* max_a, int* min_a, int variant) {
    int first = 1;
    int is_even = (variant % 2 == 0);

    for (int i = 0; i < n_a; i++) {
        for (int j = 0; j < n_a; j++) {
            // Парний варіант: i > j (нижче); Непарний: i < j (вище)
            if ((is_even && i > j) || (!is_even && i < j)) {
                if (first) {
                    *max_a = A[i][j];
                    *min_a = A[i][j];
                    first = 0;
                } else {
                    if (A[i][j] > *max_a) *max_a = A[i][j];
                    if (A[i][j] < *min_a) *min_a = A[i][j];
                }
            }
        }
    }
 }

 /**
 * @brief Транспонує прямокутну матрицю B.
 * @param B Матриця розміром n_b x m_b.
 * @param n_b Кількість рядків B.
 * @param m_b Кількість стовпців B.
 * @return Нова транспонована матриця розміром m_b x n_b.
 */
 int** transpore_b(int** B, int n_b, int m_b) {
    int** B_T = allocate_matrix(m_b, n_b);
    if (!B_T) return NULL;

    for (int i = 0; i < n_b; i++) {
        for (int j = 0; j < m_b; j++) {
            B_T[j][i] = B[i][j];
        }
    }
    return B_T;
 }

 /**
 * @brief Множить матрицю A (na x na) на матрицю B (nb x mb).
 * @return Матриця С (na x mb) або NULL у разі неузгодженості.
 */
 int** mult_a_b(int** A, int n_a, int** B, int n_b, int m_b) {
    if (n_a != n_b) {
        printf("\n[Помилка] Матриці несумісні для множення! (к-сть стовпців A має дорівнювати к-сті рядків B)\n");
        return NULL;
    }

    int** C = allocate_matrix(n_a, m_b);
    if (!C) return NULL;

    for (int i = 0; i < n_a; i++) {
        for (int j = 0; j < m_b; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n_a; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
 }

 /**
 * @brief Допоміжна функція сортування бульбашкою для 1D масиву.
 */
 static void bubble_sort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
 }

 /**
 * @brief Сортує елементи матриці A (всю матрицю або один обраний рядок).
 * @param target_row Індекс рядка (-1 для всієї матриці).
 */
 void sort_matrix_a(int** a, int n_a, int target_row) {
    if (target_row >= 0 && target_row < n_a) {
        bubble_sort(A[target_row], n_a);
    } else if (target_row == -1) {
        int total = n_a * n_a;
        int* temp = (int*)malloc(total * sizeof(int));

        int idx = 0;
        for (int i = 0; i < n_a; i++) {
            for (int j = 0; j < n_a; j++) {
                temp[idx++] = A[i][j];
            }
        }

        bubble_sort(temp, total);

        idx = 0;
        for (int i = 0; i < n_a; i++) {
            for (int j = 0; j < n_a; j++) {
                A[i][j] = temp[idx++];
            }
        }
        free(temp);
    }
 }

 /**
 * @brief Обчислює та виводить суму елементів рядків A і стовпців B.
 */
 void print_sum(int** A, int n_a, int** B, int n_b, int m_b) {
   printf("\n=== Сума елементів рядків матриці A ===\n");
   for (int i = 0; i < n_a; i**) {
    int sum = 0;
    for (int j = 0; j < n_a; j++) sum += A[i][j];
    printf("Рядок %d: %d\n", i, sum);
   }

   printf("\n=== Сума елементів стовпців матриці B ===\n");
   for (int j = 0; j < m_b; j++) {
    int sum = 0;
    for (int i = 0; i < n_b; i++) sum += B[i][j];
    printf("Стовпець %d: %d\n", j, sum);
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
