#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[]) {
    
    int parent_to_child[2], child_to_parent[2];
    char buffer[2];
    memset(buffer, 0, sizeof buffer);

    int parent_to_child_pipe_status = pipe(parent_to_child);
    if (parent_to_child_pipe_status == -1) {
        fprintf(2, "Failed to create pipe from parent to child");
        exit(1);
    }

    int child_to_parent_pipe_status = pipe(child_to_parent);
    if (child_to_parent_pipe_status == -1) {
        fprintf(2, "Failed to create pipe from child to parent");
        exit(1);
    }
    
    int pid = fork();
    
    if (pid > 0) {
        // parent process
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        
        write(parent_to_child[1], "i", 1);
        read(child_to_parent[0], buffer, 1);
        printf("%d: received p%sng\n", getpid(), buffer);
        exit(0);
    } else if (pid == 0) {
        // child process
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        read(parent_to_child[0], buffer, 1);
        printf("%d: received p%sng\n", getpid(), buffer);
        write(child_to_parent[1], "o", 1);
        exit(0);
    } else {
        fprintf(2, "System error, fork failed");
        exit(1);
    }
    
}