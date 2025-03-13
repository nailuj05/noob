#ifndef NOOB_H
#define NOOB_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// utils

typedef struct noob_string_s {
  char *buf;
  size_t length;
} noob_string;

noob_string *noob_string_create(size_t length) {
  noob_string *bc = (noob_string *)malloc(sizeof(noob_string));
  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->buf = (char *)malloc(sizeof(char) * length);

  if (bc->buf == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->length = length;

  return bc;
}

void noob_string_append(noob_string *str, const char *astr) {
  size_t alen = strlen(astr);
  size_t blen = strlen(str->buf);

  if (alen + blen + 1 > str->length) {
    size_t new_size = (alen + blen) * 2;
    char *t = realloc(str->buf, new_size);
		//printf("[internal] realloced\n");
    if (t) {
      str->buf = t;
    } else {
      printf("[err] buy more ram\n");
      exit(1);
    }
  }
	
  strcat(str->buf, astr);
}

noob_string *noob_string_create_from(const char* init) {
	noob_string* s = noob_string_create(strlen(init));
	noob_string_append(s, init);
	return s;
}

int noob_has_flag(int argc, const char **argv, const char *flag) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0)
      return 1;
  }
  return 0;
}

void noob_ensure_dir(const char *path) {
	noob_string *s = noob_string_create_from("mkdir -p ");
	noob_string_append(s, path);
  if (system(s->buf) != 0) {
		printf("[err] mkdir failed");
		exit(1);
  }
}

// build system

int noob_run_cmd(noob_string *bc) {
  printf("[cmd] %s\n", bc->buf);
  int result = system(bc->buf);

  if (result == 0) {
    printf("[info] cmd executed.\n");
    return 0;
  } else {
    printf("[err] cmd failed.\n");
    return 1;
  }
}

void *noob_run_cmd_v(void *p) {
  noob_string *bc = (noob_string *)p;

  printf("[cmd] %s\n", bc->buf);
  int result = system(bc->buf);

  if (result == 0) {
    //printf("[info] async cmd executed.\n");
  } else {
    printf("[err] cmd failed.\n");
  }
  return NULL;
}

void *noob_run_cmd_async(noob_string *bc) {
  pthread_t *t = (pthread_t *)malloc(sizeof(pthread_t));

  if (t == NULL) {
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

void noob_join_async(void *t) {
  pthread_join(*(pthread_t *)t, NULL);
  free(t);
  printf("[info] async cmd joined\n");
}

void noob_string_free(noob_string *bc) {
  if (bc != NULL) {
    free(bc->buf);
    free(bc);
  }
}

int noob_run(const char *cmd) {
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
    printf("[err] error getting file stats. file might not exist\n");
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
      noob_string *bc = noob_string_create(128);

      for (int i = 0; i < argc; i++)
        noob_string_append(bc, argv[i]);

      noob_run_cmd(bc);

      noob_string_free(bc);
      exit(0);
    }
    exit(1);
  }
}

#endif
