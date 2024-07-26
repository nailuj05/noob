# noob
no (original) build. bootstrapped build system in c.

inspired by tsodings [nob](https://github.com/tsoding/nobuild) in name and idea.

## example
```C
// noob.c
#include "noob.h"

int main() {
  RebuildYourself();
  
  BuildCommand *bc = CreateBuildCommand(128);
  // do all your build logic here
  AddCommand(bc, "gcc main.c -o main");
  RunCommand(bc);
  FreeBuildCommand(bc);

  return 0;
}
```

## basic principle

to build even a complex c project you should only need a c compiler and nothing else, no make, cmake or even shell scripting needed.

noob aims to be a build system you write yourself in c. within the main function of noob.c you'll start out with the rebuildyourself step.

afterwards you construct the build steps for your main project (calling your compiler, linker, ...).

rebuild yourself step:

![RebuildYourself](docs/RebuildYourself.png)

executing the build system will rebuild itself if needed, after a first compilation, no manual recompilation is needed.

any changes made to the noob.c file will automatically be build and then executed. 
