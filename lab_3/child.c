#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#define SHM_SIZE 1024

void write_to_file(const char *filename, const char *data) {
    int file = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
    if (file != -1) {
        write(file, data, strlen(data));
        close(file);
    } else {
        perror("Ошибка при открытии файла");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Использование: <имя_файла>");
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    key_t key = ftok("/tmp", 'R');
    int shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("Ошибка при подключении к сегменту общей памяти");
        exit(EXIT_FAILURE);
    }

    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)(-1)) {
        perror("Ошибка при подключении к общей памяти");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (strlen(shm_ptr) > 0) {
            if (strcmp(shm_ptr, "exit") == 0) {
                break;
            }
            
            float sum = 0.0;
            char *token = strtok(shm_ptr, " ");
            while (token != NULL) {
                sum += atof(token);
                token = strtok(NULL, " ");
            }
            
            char output[50];
            snprintf(output, sizeof(output), "Сумма: %.2f\n", sum);
            write_to_file(filename, output);
            memset(shm_ptr, 0, SHM_SIZE);
        }
        sleep(1);
    }
    
    shmdt(shm_ptr);
    exit(EXIT_SUCCESS);
}



/* #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 1024

int main() {
    float *shared_sum;
    key_t key = IPC_PRIVATE;
    int shm_id = shmget(key, sizeof(float), IPC_CREAT | 0666);

    // Присоединяем общую память
    shared_sum = (float*) shmat(shm_id, NULL, 0);
    if (shared_sum == (float*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    float sum = 0.0;

    // Читаем команды от родительского процесса
    while (read(STDIN_FILENO, buffer, BUFFER_SIZE) > 0) {
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            sum += atof(token);
            token = strtok(NULL, " ");
        }
    }

    // Сохраняем сумму в общей памяти
    *shared_sum = sum;

    // Закрываем память и завершаем дочерний процесс
    shmdt(shared_sum);
    return 0;
} */