#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>

static void traverse(const char *path);
static char *filetype(mode_t mode);

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "%s: no argument\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        traverse(argv[i]);
    }
    return 0;
}

static void traverse(const char *path)
{
    DIR *d = opendir(path);
    struct dirent *ent;
    struct stat st;

    if (!d) {
        perror(path);
        exit(1);
    }

    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        size_t target_file_size = strlen(path) + strlen(ent->d_name) + 2;
        char *target_file = malloc(target_file_size);
        if (!target_file) {
            perror("malloc");
            exit(1);
        }

        snprintf(target_file, target_file_size, "%s/%s", path, ent->d_name);

        if (lstat(target_file, &st) < 0) {
            perror(target_file);
            free(target_file);
            exit(1);
        }

        if (S_ISLNK(st.st_mode)) {
        } else if (S_ISDIR(st.st_mode)) {
            traverse(target_file);
        } else {
            struct stat st;
            if (lstat(target_file, &st) < 0){
                perror(target_file);
                exit(1);
            }
            struct passwd *user;
            user = getpwuid(st.st_uid);
            time_t time = st.st_mtime;
            printf("%s\t%s\t%s\n", user->pw_name, ctime(&time), target_file);
        }

        free(target_file);
    }

    closedir(d);
}

static char*
filetype(mode_t mode)
{
  if (S_ISREG(mode)) return "file";
}