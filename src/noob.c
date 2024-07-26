#include "noob.h"
#include <stdio.h>

int main() {
  RebuildYourself();

  BuildCommand *bc = CreateBuildCommand(128);
  AddCommand(bc, "echo");
  AddCommand(
      bc,
      " \"Hello Noob, this is where you would construct your build commands\"");

  RunCommand(bc);

  FreeBuildCommand(bc);

  return 0;
}
