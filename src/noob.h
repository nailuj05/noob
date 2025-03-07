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

typedef struct noob_build_cmd_s {
  char *cmd;
  size_t length;
} noob_cmd;

noob_cmd *noob_create_build_cmd(size_t cmdLength) {
  noob_cmd *bc = (noob_cmd *)malloc(sizeof(noob_cmd));
  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->cmd = (char *)malloc(sizeof(char) * cmdLength);

  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->length = cmdLength;

  return bc;
}

void noob_add_cmd(noob_cmd *bc, const char *cmd) {
  size_t alen = strlen(cmd);
  size_t blen = strlen(bc->cmd);

  if (alen + blen + 1 > bc->length) {
		size_t new_size = (alen + blen) * 2;
		char* t = realloc(bc->cmd, new_size);
		if(t) {
			bc->cmd = t;
		} else {
			printf("[err] buy more ram\n");
			exit(1);
		}
  }
  strcat(bc->cmd, cmd);
  strcat(bc->cmd, " ");
}

int noob_run_cmd(noob_cmd *bc) {
	printf("[cmd] %s\n", bc->cmd);
  int result = system(bc->cmd);

  if (result == 0) {
    printf("[info] cmd executed.\n");
    return 0;
  } else {
    printf("[err] cmd failed.\n");
    return 1;
  }
}

void * noob_run_cmd_v(void *p) {
	noob_cmd *bc = (noob_cmd*)p;
	
	printf("[cmd] %s\n", bc->cmd);
  int result = system(bc->cmd);

  if (result == 0) {
    printf("[info] async cmd executed.\n");
  } else {
    printf("[err] cmd failed.\n");
  }
	return NULL;
}

void* noob_run_cmd_async(noob_cmd *bc) {
	pthread_t *t = (pthread_t *)malloc(sizeof(pthread_t));

	if(t == NULL) {
		printf("[err] buy more ram\n");
		exit(1);
	}

	int res = pthread_create(t, NULL, noob_run_cmd_v, bc);

	if (res != 0) {
		printf("[err] thread error encountered with code %d\n", res);
		exit(1);
	}

	return t;
}

void noob_join_async(void* t) {
	pthread_join(*(pthread_t *)t, NULL);
	free(t);
	printf("[info] async cmd joined\n");
}

void noob_free_cmd(noob_cmd *bc) {
  if (bc != NULL) {
    free(bc->cmd);
    free(bc);
  }
}

int noob_build_n_run(const char *cmd) {
  int result = system(cmd);

  if (result == 0) {
    return 0;
  } else {
    printf("[err] cmd failed.\n");
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
  char cmd[256];

  if ((system("cc -fsyntax-only noob.c noob.h")) != 0) {
    printf("[info] recompilation failed due to errors.\n");
    return 1;
  }

  if ((system("rm -rf noob")) != 0) {
    printf("[info] removing failed.\n");
    return 0;
  }

  snprintf(cmd, sizeof(cmd), "cc noob.c noob.h -o noob -lpthread");

  int result = system(cmd);

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
      noob_cmd *bc = noob_create_build_cmd(128);

      for (int i = 0; i < argc; i++)
        noob_add_cmd(bc, argv[i]);

      noob_run_cmd(bc);

      noob_free_cmd(bc);
      exit(0);
    }
    exit(1);
  }
}

#endif
