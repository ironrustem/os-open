#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>

typedef struct {
    char path[PATH_MAX];
    char name[FILENAME_MAX];
    int  size;
} file_t;

char util_name[FILENAME_MAX];
file_t *f_list;
int f_list_len = 0;

int names_eq(file_t file1, file_t file2) {
    return (strcmp(file1.name, file2.name) > 0);
}

int sizes_eq(file_t file1, file_t file2) {
    return ((file1.size - file2.size) < 0);
}

char* add_slash(char* path) {
    if (path[strlen(path) - 1] != '/')
        strcat(path, "/");
    return path;
}

int dirpass(const char *dir_name) {
    DIR *directory;

    if (!(directory = opendir(dir_name))) {
        fprintf(stderr, "%s: %s: %s\n", util_name, dir_name, strerror(errno));
        return -1;
    }

    struct dirent *dir_item;

    while((dir_item = readdir(directory)) != NULL) {
        char next_item[PATH_MAX];

        strcpy(next_item, dir_name);
        strcat(next_item, dir_item->d_name);

        if(strcmp(".", dir_item->d_name) == 0 ||
            strcmp("..", dir_item->d_name) == 0)
            continue;

        struct stat statbuf;

        lstat(next_item, &statbuf);

        if(S_ISDIR(statbuf.st_mode)) {
            dirpass(add_slash(next_item));
        } else if(S_ISREG(statbuf.st_mode)) {
            file_t file_tmp;

            strcpy(file_tmp.name, dir_item->d_name);
            strcpy(file_tmp.path, next_item);

            file_tmp.size = (int) statbuf.st_size;
            f_list = realloc(f_list, (++f_list_len)*sizeof(file_t));
            f_list[f_list_len - 1] = file_tmp;
        }
    }
    closedir(directory);
}

void sort(int sort_opt){
    if (sort_opt == 1) {
        int i, j;
        for (i = 0; i < f_list_len; i++) {
            for (j = 0; j < f_list_len - i - 1; j++) {
                if (names_eq(f_list[j], f_list[j+1])) {
                    file_t file_tmp = f_list[j];
                    f_list[j] = f_list[j+1];
                    f_list[j+1] = file_tmp;
                }
            }
        }
    } else {
        int i, j;
        for (i = 0; i < f_list_len; i++) {
            for (j = 0; j < f_list_len - i - 1; j++) {
                if (sizes_eq(f_list[j], f_list[j+1])) {
                    file_t file_tmp = f_list[j];
                    f_list[j] = f_list[j+1];
                    f_list[j+1] = file_tmp;
                }
            }
        }
    }
}

int copyFile(char *filein, char *fileout) {
    FILE *f_in;
    FILE *f_out;

    if (!(f_in = fopen(filein,"rb"))) return 1;
    if (fopen(fileout,"rb")) return 1;
    if (!(f_out = fopen(fileout,"wb"))) return 1;
    
    int bytes=0;
    char c = fgetc(f_in);
    while (!feof(f_in)) {
        fputc(c,f_out);
        bytes++;
        c = fgetc(f_in);
    }
    fclose(f_in);
    fclose(f_out);
    struct stat stat_f;
    stat(filein,&stat_f);
    chmod(fileout,stat_f.st_mode);
    printf("%d bytes ",bytes);
    return 0;
}

int main(int argc, char const *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "%s: Wrong number of arguments\n", util_name);
        return -1;
    }

    char dist_path[PATH_MAX];
    realpath(argv[3], dist_path);
    add_slash(dist_path);

    char sort_opt = atoi(argv[2]);
    if (sort_opt != 1 && sort_opt != 2) {
        fprintf(stderr,"%s: Wrong sort option (Must be: 1 or 2)\n", util_name);
        return -1;
    }

    char initdir[PATH_MAX];
    strcpy(initdir, argv[1]);
    dirpass(add_slash(initdir));

    sort(sort_opt);

    if (mkdir(dist_path, 0777) == -1 && errno != EEXIST) {
        fprintf(stderr, "%s: %s: %s\n", util_name, dist_path, strerror(errno));
        return -1;
    }

    int i, j;
    for(i = 0; i < f_list_len; i++) {
        j = i;
        while (j++ < f_list_len && !strcmp(f_list[i].name, f_list[j].name)) {
            char add_index[5];
            sprintf(add_index," (%d)", j-i);
            strcat(f_list[j].name, add_index);
        }

        char dist_file[PATH_MAX + 1];

        strcpy(dist_file, dist_path);
        strcat(dist_file, f_list[i].name);

        if (copyFile(f_list[i].path, dist_file) == -1) {
           fprintf(stderr, "%s: %s: %s\n", util_name, dist_file, strerror(errno));
        }else{
           printf( "%s: %s: %d kB\n", f_list[i].path, f_list[i].name, f_list[i].size);
        }
    }
    return 0;
}
