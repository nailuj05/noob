#include "noob.h"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

  if (noob_has_flag(argc, argv, "-d")) {
    printf("debug build\n");
  }

  noob_build_command *bc = noob_create_build_command(12);
  noob_add_command(bc, "echo");
  noob_add_command(
      bc,
      "\"hey noob, this is where you would construct your build commands\"");
	
  void *t = noob_run_command_async(bc);

	noob_join_async(t);

  noob_free_command(bc);

  return 0;
}
