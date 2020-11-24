#!/bin/bash

# build on remote machine

touch AMBuilder

DEST='/home/nom/TRASH/alliedmodders/thirdparty/SMExt-Unwind'

rsync -arvz -e "ssh -p 943" ./ nom@nom-nom-nom.us:${DEST} --delete --exclude build --exclude '.git' --exclude $(basename $0)
ssh -p 943 -t nom@nom-nom-nom.us "cd ${DEST}/build && ambuild"
