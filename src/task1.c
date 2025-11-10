#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100
int array[ARRAY_SIZE];

// Циклическая блокировка (spinlock) для синхронизации
pthread_spinlock_t spinlock;

void* search(void* arg) {
    int target = *((int*)arg);
    int* result = (int*)malloc(sizeof(int));
    *result = -1; // Если не найдено

    // Захват блокировки
    pthread_spin_lock(&spinlock);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (array[i] == target) {
            *result = i; // Если нашли, сохраняем индекс
            break;
        }
    }
    // Освобождение блокировки
    pthread_spin_unlock(&spinlock);

    return result;
}

int main() {
    // Заполнение массива случайными числами
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }

    int target = 50; // Искомое значение

    pthread_t thread;
    int* result;

    // Инициализация spinlock
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

    // Создаем поток для поиска
    pthread_create(&thread, NULL, search, &target);
    
    // Ожидаем завершения потока
    pthread_join(thread, (void**)&result);

    if (*result != -1) {
        printf("Элемент найден на индексе %d\n", *result);
    } else {
        printf("Элемент не найден\n");
    }

    free(result);
    pthread_spin_destroy(&spinlock);
    return 0;
}
