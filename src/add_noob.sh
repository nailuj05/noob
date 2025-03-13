#!/bin/bash

# --------------------------------------------------------------- #
# add this script to your path or as a function to your shell so  #
# you can easily copy noob.h from the repo to where you need it   #
# --------------------------------------------------------------- #

sdir=$(dirname $0)
cdir=$(pwd)
noobh="noob.h"
noobc="noob.c"

if [[ -f "$sdir/$noobh" ]]; then
		cp "$sdir/$noobh" "$cdir/$noobh"
		cp "$sdir/$noobc" "$cdir/$noobc"
		echo "added noob to $cdir"
else
		echo "noob.h not found in $sdir"
fi
