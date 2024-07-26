#ifndef NOOB_H
#define NOOB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct BuildCommand {
  char *command;
} BuildCommand;

int nb_GetLastModified(const char *filepath) {
  struct stat file_stat;

  if (stat(filepath, &file_stat) != 0) {
    perror("Error getting file stats");
    return -1;
  }

  return file_stat.st_mtime;
}

int nb_Recompile() {
  char command[256];

  if ((system("gcc -fsyntax-only noob.c noob.h")) != 0) {
    printf("Recompilation failed due to errors.\n");
    return 1;
  }

  if ((system("rm -rf noob")) != 0) {
    printf("Removing failed.\n");
    return 0;
  }

  snprintf(command, sizeof(command), "gcc noob.c noob.h -o noob");

  int result = system(command);

  if (result == 0) {
    printf("Compilation successful.\n");
    return 0;
  } else {
    printf("Compilation failed.\n");
    return 1;
  }
  return 0;
}

void RebuildYourself() {
  int source = nb_GetLastModified("noob.c");
  int exec = nb_GetLastModified("noob");

  if (source > exec) {
    printf("Rebuilding\n");
    if (nb_Recompile() == 0) {
      system("./noob");
      exit(0);
    }
    exit(1);
  }
}

#endif
