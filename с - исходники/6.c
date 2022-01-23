#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main(int argc , char *argv[]) {
    DIR *directory;
    struct dirent *d;

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Current directory error/n");
        return 1;
    }

    char *current_dir = "./";
    if((directory = opendir(current_dir)) == NULL) {
        printf("Cannot open current directory");
        return 1;
    }

    printf("Current directory - %s\n", cwd);
    while((d = readdir(directory)) != NULL) {
        if (strcmp(d->d_name, ".") && strcmp(d->d_name, "..")) {
             printf("%s\n", d->d_name);
        }
    }

    if (closedir(directory) != 0) {
        printf("Cannot current directory close/n");
        return 1;
    }

    printf("\n");

    if((directory = opendir(argv[1])) == NULL) {
        printf("Cannot directory open/n");
        return 1;
    }

    printf("Directory - %s\n", argv[1]);
    while((d = readdir(directory)) != NULL) {
         if (strcmp(d->d_name, ".") && strcmp(d->d_name, "..")) {
             printf("%s\n", d->d_name);
        }
    }

    if (closedir(directory) != 0) {
        printf("Cannot directory close/n");
        return 1;
    }
}