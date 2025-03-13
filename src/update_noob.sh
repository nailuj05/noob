#!/bin/bash

# --------------------------------------------------------------- #
# add this script to your path or as a function to your shell so  #
# you can easily update noob.h from the repo to where you need it #
# --------------------------------------------------------------- #

sdir=$(dirname $0)
cdir=$(pwd)
noobh="noob.h"

if [[ -f "$sdir/$noob" ]]; then
		cp "$sdir/$noobh" "$cdir/$noobh"
		echo "updated noob.h in $cdir"
else
		echo "noob.h not found in $sdir"
fi
