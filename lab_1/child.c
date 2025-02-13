#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Ошибка: отсутствует имя выходного файла\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1]; // Имя выходного файла из аргументов
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t bytesRead = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (bytesRead <= 0) {
            break; // Ошибка или конец потока
        }

        buffer[bytesRead - 1] = '\0'; // Удаляем символ новой строки

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Обработка чисел
        float sum = 0.0;
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            sum += atof(token);
            token = strtok(NULL, " ");
        }

        // Записываем результат в файл
        FILE *file = fopen(filename, "a");
        if (file != NULL) {
            fprintf(file, "Сумма: %.2f\n", sum);
            fclose(file);
        } else {
            perror("Ошибка при открытии файла");
        }
    }

    exit(EXIT_SUCCESS);
}










/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe1[2]; // pipe для передачи данных от родителя к дочернему

    // Читаем имя файла от аргумента (это делается в родительском процессе)
    char filename[256];

    // Читаем данные от родителя
    char buffer[BUFFER_SIZE];
    while (1) {
        read(pipe1[0], buffer, BUFFER_SIZE);
        if (strcmp(buffer, "exit") == 0) {
            break; // Выход при получении команды exit
        }

        // Обработка чисел
        float sum = 0.0;
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            sum += atof(token);
            token = strtok(NULL, " ");
        }

        // Записываем результат в файл
        FILE *file = fopen(filename, "a");
        if (file != NULL) {
            fprintf(file, "Сумма: %.2f\n", sum);
            fclose(file);
        } else {
            perror("fopen");
        }
    }

    close(pipe1[0]);
    exit(EXIT_SUCCESS);
} */