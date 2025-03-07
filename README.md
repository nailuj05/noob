# noob
no (original) build. bootstrapped build system in c.

inspired by tsodings [nob](https://github.com/tsoding/nobuild) in name and idea.

## example
```C
// noob.c
#include "noob.h"

int main(int argc, const char * argv) {
  noob_rebuild_yourself(argc, argv);
  
  noob_cmd *bc = noob_create_build_command(128);
  // do all your build logic here
  noob_add_cmd(bc, "gcc main.c -o main");
  noob_run_cmd(bc);
  noob_free_cmd(bc);

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

any changes made to the noob.c file will automatically be build and then executed, argc and argv are passed along aswell. 

## additional features

to make life a bit easier when constructing your build i also implemented functions that might help you do just that.

**noob_has_flag()**
detects whether a flag (`./noob DEBUG`) was passed.
```C
int main(int argc, const char **argv) {
  noob_rebuild_yourself(argc, argv);

  if (noob_has_flag(argc, argv, "DEBUG")) {
    printf("Debug build\n");
    // do some stuff here //
  } else {
    // do some other stuff here // 
  }
  // maybe even do some stuff here // 
}
```

**noob_is_outdated()**

<todo>

**noob_run_cmd_asynci()**

<todo>

**noob_build_n_run()**

<todo>
