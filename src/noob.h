#ifndef NOOB_H
#define NOOB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Build System

typedef struct BuildCommand {
  char *command;
  size_t length;
} BuildCommand;

BuildCommand *CreateBuildCommand(size_t commandLength) {
  BuildCommand *bc = (BuildCommand *)malloc(sizeof(BuildCommand));
  if (bc == NULL) {
    printf("buy more ram\n");
    exit(1);
  }

  bc->command = (char *)malloc(sizeof(char) * commandLength);

  if (bc == NULL) {
    printf("buy more ram\n");
    exit(1);
  }

  bc->length = commandLength;

  return bc;
}

void AddCommand(BuildCommand *bc, char *cmd) {
  size_t alen = strlen(cmd);
  size_t blen = strlen(bc->command);

  if (alen + blen > bc->length) {
    printf("Command to long.\n");
    exit(1);
  }
  strcat(bc->command, cmd);
}

int RunCommand(BuildCommand *bc) {
  int result = system(bc->command);

  if (result == 0) {
    printf("Build successful.\n");
    return 0;
  } else {
    printf("Build failed.\n");
    return 1;
  }
}

// RebuildYourself

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
