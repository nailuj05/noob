# noob
no (original) build. Bootstrapped build system in C.

Inspired by tsodings [nob](https://github.com/tsoding/nobuild) in name and idea.

## basic principle

To build even a complex C project you should only need a C Compiler and nothing else, no make, cmake or even shell scripting needed.

noob aims to be a build system you write yourself in C. Within the main function of noob.c you'll start out with the RebuildYourself step.

Afterwards you construct the build steps for your main project (calling your compiler, linker, ...).

Rebuild yourself step:

![RebuildYourself](docs/RebuildYourself.png)

Executing the build system will rebuild itself if needed, after a first compilation, no manual recompilation is needed.

Any changes made to the noob.c file will automatically be build and then executed. 
