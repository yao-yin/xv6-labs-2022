#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int match(char* path, char* target) {
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
    p++;
    return strcmp(p, target) == 0;
}

int is_parent_or_self(char* path) {
    
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
    p++;

    return (strcmp(p, ".") == 0 || strcmp(p, "..") == 0);
}

void find(char* path, char* target) {
    int fd;
    char buffer[512];
    struct dirent de;
    struct stat st;
    char* p;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: can't open path: %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: can't stat path: %s\n", path);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        if (match(path, target)) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buffer)){
            printf("find: path too long\n");
            break;
        }
        
        strcpy(buffer, path);
        p = buffer + strlen(buffer);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof (de)) {
            if (de.inum == 0) continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            if (is_parent_or_self(buffer)) {
                continue;
            }
            
            find(buffer, target);
        }
        break;
    default:
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <target file>\n");
        exit(1);
    }

    char* path = argv[1];
    char* target = argv[2];
    find(path, target);
    exit(0);
}