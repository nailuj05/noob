#ifndef NOOB_H
#define NOOB_H

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>

// --------- //
//   utils   //
// --------- //

typedef struct noob_string_s {
  char *buf;
  size_t capacity;
} noob_string;

noob_string *noob_string_create(size_t length) {
	if (length == 0) length = 64;
  noob_string *bc = (noob_string *)malloc(sizeof(noob_string));
  if (bc == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->buf = (char *)calloc(length + 1, 1);

  if (bc->buf == NULL) {
    printf("[err] buy more ram\n");
    exit(1);
  }

  bc->capacity = length;

  return bc;
}

void noob_string_fit(noob_string *str, size_t blen) {
	size_t alen = str ? strlen(str->buf) : 0;
	
  if (alen + blen > str->capacity) {
		size_t new_capacity = alen > blen ? alen * 2 : (alen + blen) * 2;
    char *t = realloc(str->buf, new_capacity + 1);
		
    if (t == NULL) {
        printf("[err] buy more ram\n");
        exit(1);
    }

    str->buf = t;
    str->capacity = new_capacity;
  }
}

void noob_string_append(noob_string *str, const char *astr) {
  size_t blen = astr ? strlen(astr) : 0;

	noob_string_fit(str, blen);
	
  strcat(str->buf, astr);
}

noob_string *noob_string_create_from(const char* init) {
	noob_string* s = noob_string_create(strlen(init) + 1);
	noob_string_append(s, init);
	return s;
}

int noob_string_replace(noob_string *str, char *replace, char *with) {
	size_t replace_l = strlen(replace);
	size_t with_l = strlen(with);

	noob_string_fit(str, with_l);

	// search string, copy rest, replace string with replacement, concat rest
	char *p = strstr(str->buf, replace);
 	if (p != NULL) {
		size_t rest_l = strlen(p + replace_l);
		char *rest = (char *)malloc(rest_l + 1);

		memcpy(rest, p + replace_l, rest_l+1);
		*p = '\0';
		strcat(str->buf, with);
		strcat(str->buf, rest);
		
		free(rest);
		return 1;
	}
	return 0;
}

int noob_has_flag(int argc, const char **argv, const char *flag) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], flag) == 0)
      return i;
  }
  return 0;
}

noob_string *noob_file_read(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) {
		printf("[err] failed to open file\n");
		exit(1);
	}

	
	fseek(file, 0, SEEK_END);
	size_t length = (size_t)ftell(file);
	rewind(file);

	noob_string *f = noob_string_create(length);
	fread(f->buf, 1, length, file);
	f->buf[length] = '\0';
	
	fclose(file);
	return f;
}

int noob_file_write(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("[err] failed to open file\n");
        exit(1);
    }

    size_t length = fwrite(content, sizeof(char), strlen(content), file);
    if (length < strlen(content)) {
			printf("[err] failed to write file %s\n", filename);
        fclose(file);
        exit(1);
    }

		printf("[info] file %s written\n", filename);
    fclose(file);
    return 0;
}

// Calculate the number of varargs so we dont overflow and dont need a NULL as the last arg
#define COUNT_ARGS(...)  (sizeof((const char*[]){__VA_ARGS__}) / sizeof(const char*))
#define noob_help(ac, av, ...) \
    _noob_help(ac, av, COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

int _noob_help(int ac, const char* av[], int c, const char* first, ...) {
	if (noob_has_flag(ac,av,"-h") ||
			noob_has_flag(ac,av,"--help") ||
			noob_has_flag(ac,av,"help")) {
		
		printf("[help] usage and information for this noob build system.\n");

		va_list args;
		va_start(args, first);
		for(int i = 0; i < c; ++i) {
			printf("\t%s\n", first);
			first = va_arg(args, const char*);
		}
		va_end(args);

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

// ------------ //
// build system //
// ------------ //

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

void *_noob_run_cmd(void *p) {
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

  int res = pthread_create(t, NULL, _noob_run_cmd, bc);

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
  printf("[cmd] %s\n", cmd);
  int result = system(cmd);

  if (result == 0) {
    return 0;
  } else {
    printf("[err] cmd failed.\n");
    return 1;
  }
}

// ---------------- //
// rebuild yourself //
// ---------------- //

char *_noob_realpath(const char *path) {
	char *rpath = malloc(1024);
  
	if (realpath(path, rpath) == NULL) {
		printf("[err] realpath failed\n");
		exit(1);
	}

	return rpath;
}

char *_noob_set_wdir(const char *argv0) {
	char *path = _noob_realpath(argv0);
	char *rpath = malloc(1024);
	strcpy(rpath, path);
	char *dir = dirname(path);

	if (chdir(dir) != 0) {
		perror("[err] chdir failed\n");
		exit(1);
	}

	free(path);
	return rpath;
}

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

int _noob_recomp() {
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
	char *rpath = _noob_set_wdir(argv[0]);
	
  if (noob_is_outdated("noob.c", "noob") ||
      noob_is_outdated("noob.h", "noob")) {
    printf("[info] rebuilding...\n");
    if (_noob_recomp() == 0) {
      noob_string *bc = noob_string_create(128);

			noob_string_append(bc, rpath);
			free(rpath);
      for (int i = 1; i < argc; i++) {
				noob_string_append(bc, " ");
        noob_string_append(bc, argv[i]);
			}

      noob_run_cmd(bc);

      noob_string_free(bc);
      exit(0);
    }
    exit(1);
  }
}

#endif
