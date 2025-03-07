#include "noob.h"

int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

  if (noob_has_flag(argc, argv, "-d")) {
    printf("debug build\n");
  }

  noob_cmd *bc = noob_create_build_cmd(12);
  noob_add_cmd(bc, "echo");
  noob_add_cmd(
      bc,
      "\"hey noob, this is where you would construct your build cmds\"");
	
  void *t = noob_run_cmd_async(bc);

	noob_join_async(t);

  noob_free_cmd(bc);

  return 0;
}
