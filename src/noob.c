#include "noob.h"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

	if (noob_help(argc, argv, "usage: ./noob", "-d : debug build", "-h : help"))
		exit(0);
	
  if (noob_has_flag(argc, argv, "-d")) {
    printf("debug build\n");
  }

  noob_string *bc = noob_string_create(12);
  noob_string_append(bc, "echo ");
  noob_string_append(bc, "\"hey noob, this is where you would construct your build cmds\"");
	
  void *t = noob_run_cmd_async(bc);
	
	noob_join_async(t);

  noob_string_free(bc);

  return 0;
}
