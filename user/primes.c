#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void set_up_pipe(int* fd);

int main() {
    
    int parent_to_child[2];
    pipe(parent_to_child);
    
    int pid = fork();
    if (pid == 0) {
        set_up_pipe(parent_to_child);
    } else {
        close(parent_to_child[0]);
        for (int i = 2; i <= 35; i ++) {
            write(parent_to_child[1], &i, sizeof(int));
        }
        int fin = 37;
        write(parent_to_child[1], &fin, sizeof(int));
        int wstatus;
        wait(&wstatus);
        exit(0);
    }
    exit(0);
}

void set_up_pipe(int* fd){
    char buffer[8];
    close(fd[1]);
    int parent_to_child[2];
    pipe(parent_to_child);
    read(fd[0], buffer, sizeof(int));
    int num = *((int *)buffer);
    if (num <= 35) {
        printf("prime %d\n", num);
        int pid = fork();
        if (pid == 0) {
            set_up_pipe(parent_to_child);
        } else {
            close(parent_to_child[0]);
            while (read(fd[0], buffer, sizeof(int))) {
                int get_num = *((int *) buffer);
                if (get_num % num != 0) {
                    write(parent_to_child[1], &get_num, sizeof (int));
                }

                if (get_num == 37) {
                    if (num != 31) {
                        int wstatus;
                        wait(&wstatus);
                        exit(0);
                    } else {
                        exit(0);
                    }
                }
            }
        }
    }
}