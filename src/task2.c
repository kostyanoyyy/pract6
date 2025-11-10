#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define ARRAY_SIZE 100
int array[ARRAY_SIZE];

// Циклическая блокировка (spinlock) для синхронизации
pthread_spinlock_t spinlock;
std::vector<int> results;

void* search(void* arg) {
    int target = *((int*)arg);

    // Захват блокировки
    pthread_spin_lock(&spinlock);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (array[i] == target) {
            results.push_back(i); // Добавляем индекс в результат
        }
    }
    // Освобождение блокировки
    pthread_spin_unlock(&spinlock);

    return NULL;
}

int main() {
    // Заполнение массива случайными числами
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 100;
    }

    int target = 50; // Искомое значение

    pthread_t threads[5];

    // Инициализация spinlock
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

    // Создаем несколько потоков для поиска
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, search, &target);
    }
    
    // Ожидаем завершения всех потоков
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Выводим все индексы найденных элементов
    if (!results.empty()) {
        printf("Индексы найденных элементов: ");
        for (int idx : results) {
            printf("%d ", idx);
        }
        printf("\n");
    } else {
        printf("Элементы не найдены\n");
    }

    pthread_spin_destroy(&spinlock);
    return 0;
}
