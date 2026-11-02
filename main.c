#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* --- Прототипи функцій --- */
int** allocate_matrix(int rows, int cols);
void free_matrix(int** matrix, int rows);
void print_matrix(int** matrix, int rows, int cols, const char* name);
void fill_matrix(int** matrix, int rows, int cols);

void max_min(int** A, int n_a, int* max_a, int* min_, int variant);
int** traspose_b(int** B, int n_b, int m_b);
void sort_matrix_a(int** A, int n_a, int target_row);
void print_sums(int** A, int n_a, int** B, int n_b, int m_b);

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
 void sort_matrix_a(int** A, int n_a, int target_row) {
    if (target_row >= 0 && target_row < n_a) {
        bubble_sort(A[target_row], n_a);
    } else if (target_row == -1) {
        int total = n_a * n_a;
        int* temp = (int*)malloc(total * sizeof(int));
        int idx = 0;
        for (int i = 0; i < n_a; i++) {
            for (int j = 0; j < n_a; j++) temp[idx++] = A[i][j];
        }
        bubble_sort(temp, total);
        idx = 0;
        for (int i = 0; i < n_a; i++) {
            for (int j = 0; j < n_a; j++) A[i][j] = temp[idx++];
        }
        free(temp);
    }
}

 /**
 * @brief Обчислює та виводить суму елементів рядків A і стовпців B.
 */
 void print_sums(int** A, int n_a, int** B, int n_b, int m_b) {
    printf("\n=== Суми рядків матриці A ===\n");
    for (int i = 0; i < n_a; i++) {
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

 /* --- Головна програма --- */
int main() {
// Встановлюємо кодування UTF-8 для консолі Windows (вхід і вихід)
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // Резервне налаштування локалі
    setlocale(LC_ALL, ".65001");
    srand((unsigned int)time(NULL));

    int variant, n_a, n_b, m_b;

    printf("=== Лабораторна робота №4 (Частина 2) ===\n");
    printf("Введіть номер вашого варіанту: ");
    scanf("%d", &variant);

    n_a = variant; // Розмірність квадратної матриці A = na x na
    printf("Введіть кількість рядків n_b для матриці B (для множення рекомендується %d): ", n_a);
    scanf("%d", &n_b);
    printf("Введіть кількість стовпців m_b для матриці B: ");
    scanf("%d", &m_b);

    int** A = allocate_matrix(n_a, n_a);
    int** B = allocate_matrix(n_b, m_b);

    if (!A || !B) {
        printf("Помилка виділення пам'яті!\n");
        return 1;
    }

    printf("\n--- Формування матриці A ---\n");
    fill_matrix(A, n_a, n_a);

    printf("\n--- Формування матриці B ---\n");
    fill_matrix(B, n_b, m_b);

    int choice;
    do {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Вивести поточні матриці A і B\n");
        printf("2. Пошук Max/Min елементів у A (відносно головної діагоналі)\n");
        printf("3. Транспонувати матрицю B\n");
        printf("4. Множення матриць A x B\n");
        printf("5. Сортування матриці A\n");
        printf("6. Вивести суми рядків A та стовпців B\n");
        printf("0. Вихід з програми\n");
        printf("Ваш вибір: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            print_matrix(A, n_a, n_a, "A");
            print_matrix(B, n_b, m_b, "B");
            break;
        case 2: {
            int max_val = 0, min_val = 0;
            max_min(A, n_a, &max_val, &min_val, variant);
            printf("\nДля варіанту %d (%s головної діагоналі):\n", variant, (variant % 2 == 0) ? "нижче" : "вище");
            printf("Max елемент: %d\nMin елемент: %d\n", max_val, min_val);
            break;
        }
        case 3: {
            int** B_T = transpore_b(B, n_b, m_b);
            if (B_T) {
                print_matrix(B_T, m_b, n_b, "B^T (Транспонована)");
                free_matrix(B_T, m_b);
            }
            break;
        }
        case 4: {
            int** C = mult_a_b(A, n_a, B, n_b, m_b);
            if (C) {
                print_matrix(C, n_a, m_b, "C = A x B");
                free_matrix(C, n_a);
            }
            break;
        }
        case 5: {
            int sort_opt;
            printf("\n1 - Сортувати всю матрицю A\n2 - Сортувати конкретний рядок\nВибір: ");
            scanf("%d", &sort_opt);
            if (sort_opt == 1) {
                sort_matrix_a(A, n_a, -1);
                print_matrix(A, n_a, n_a, "A (Повністю відсортована)");
            } else {
                int row;
                printf("Введіть номер рядка [0..%d]: ", n_a - 1);
                scanf("%d", &row);
                sort_matrix_a(A, n_a, row);
                print_matrix(A, n_a, n_a, "A (З відсортованим рядком)");
            }
            break;
        }
        case 6:
            print_sums(A, n_a, B, n_b, m_b);
            break;
        case 0:
            printf("\nЗавершення роботи...\n");
                break;
            default:
                printf("\nНекоректний вибір. Спробуйте ще раз.\n");
        }
    } while (choice != 0);

    // Звільнення основної пам'яті
    free_matrix(A, n_a);
    free_matrix(B, n_b);

    return 0;
}
