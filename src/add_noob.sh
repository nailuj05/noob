#!/bin/bash

# --------------------------------------------------------------- #
# add this script to your path or as a function to your shell so  #
# you can easily copy noob.h from the repo to where you need it   #
# --------------------------------------------------------------- #

sdir=$(dirname $0)
cdir=$(pwd)
noob="noob.h"

if [[ -f "$sdir/$noob" ]]; then
		cp "$sdir/$noob" "$cdir/$noob"
		echo "added noob.h to $cdir"
else
		echo "noob.h not found in $cdir"
fi
