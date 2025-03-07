#ifndef NOOB_H
#define NOOB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

// Check Flags

int	noob_has_flag(int argc, const char **argv, const char *flag) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0)
      return 1;
  }
  return 0;
}

// Build System

typedef struct noob_buildcommand_s {
  char *command;
  size_t length;
} noob_build_command;

noob_build_command *noob_create_build_command(size_t commandLength) {
  noob_build_command *bc = (noob_build_command *)malloc(sizeof(noob_build_command));
  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->command = (char *)malloc(sizeof(char) * commandLength);

  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->length = commandLength;

  return bc;
}

void noob_add_command(noob_build_command *bc, const char *cmd) {
  size_t alen = strlen(cmd);
  size_t blen = strlen(bc->command);

  if (alen + blen + 1 > bc->length) {
		size_t new_size = (alen + blen) * 2;
		char* t = realloc(bc->command, new_size);
		if(t) {
			bc->command = t;
		} else {
			printf("[err] buy more ram\n");
			exit(1);
		}
  }
  strcat(bc->command, cmd);
  strcat(bc->command, " ");
}

int noob_run_command(noob_build_command *bc) {
  int result = system(bc->command);

  if (result == 0) {
    printf("[info] command executed.\n");
    return 0;
  } else {
    printf("[err] command failed.\n");
    return 1;
  }
}

void * noob_run_command_v(void *bc) {
  int result = system(((noob_build_command*)bc)->command);

  if (result == 0) {
    printf("[info] async command %lu executed.\n", pthread_self());
  } else {
    printf("[err] command failed.\n");
  }
	return NULL;
}

void* noob_run_command_async(noob_build_command *bc) {
	pthread_t *t = (pthread_t *)malloc(sizeof(pthread_t));

	if(t == NULL) {
		printf("[err] buy more ram\n");
		exit(1);
	}

	int res = pthread_create(t, NULL, noob_run_command_v, bc);

	if (res != 0) {
		printf("[err] thread error encountered with code %d\n", res);
		exit(1);
	}

	return t;
}

void noob_join_async(void* t) {
	pthread_join(*(pthread_t *)t, NULL);
	free(t);
	printf("[info] async command joined\n");
}

void noob_free_command(noob_build_command *bc) {
  if (bc != NULL) {
    free(bc->command);
    free(bc);
  }
}

int noob_build_n_run(const char *cmd) {
  int result = system(cmd);

  if (result == 0) {
    return 0;
  } else {
    printf("[err] command failed.\n");
    return 1;
  }
}

// RebuildYourself

int noob_get_last_modified(const char *filepath) {
  struct stat file_stat;

  if (stat(filepath, &file_stat) != 0) {
    perror("[err] error getting file stats");
    return -1;
  }

  return file_stat.st_mtime;
}

int noob_is_outdated(const char *file_a, const char *file_b) {
	if (noob_get_last_modified(file_a) > noob_get_last_modified(file_b))
		return 1;
	return 0;
}

int noob_recomp() {
  char command[256];

  if ((system("cc -fsyntax-only noob.c noob.h")) != 0) {
    printf("[info] recompilation failed due to errors.\n");
    return 1;
  }

  if ((system("rm -rf noob")) != 0) {
    printf("[info] removing failed.\n");
    return 0;
  }

  snprintf(command, sizeof(command), "cc noob.c noob.h -o noob -lpthread");

  int result = system(command);

  if (result == 0) {
    printf("[info] compilation successful.\n");
    return 0;
  } else {
    printf("[err] compilation failed.\n");
    return 1;
  }
}

void noob_rebuild_yourself(int argc, const char **argv) {
  if (noob_is_outdated("noob.c", "noob") ||
			noob_is_outdated("noob.h", "noob")) {
    printf("[info] rebuilding...\n");
    if (noob_recomp() == 0) {
      noob_build_command *bc = noob_create_build_command(128);

      for (int i = 0; i < argc; i++)
        noob_add_command(bc, argv[i]);

      noob_run_command(bc);

      noob_free_command(bc);
      exit(0);
    }
    exit(1);
  }
}

#endif
