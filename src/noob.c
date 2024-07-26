#include "noob.h"

int main(int argc, const char **argv) {
  RebuildYourself(argc, argv);

  if (HasFlag(argc, argv, "DEBUG")) {
    printf("Debug build\n");
  }

  BuildCommand *bc = CreateBuildCommand(128);
  AddCommand(bc, "echo");
  AddCommand(
      bc,
      "\"Hello Noob, this is where you would construct your build commands\"");

  RunCommand(bc);

  FreeBuildCommand(bc);

  return 0;
}
