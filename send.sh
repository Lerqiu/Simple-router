#!/bin/bash

set -e

# rm -rf /tmp/virbian{1..4}
# mkdir /tmp/virbian{1..4}

for x in {1..4}; do
  sshfs -o password_stdin -p 5000$x user@localhost:/home/user/SK /tmp/virbian$x <<< user
done

for x in {1..4}; do
    if [ -d /tmp/virbian$x/router ]
    then
        rm -rf /tmp/virbian$x/router/*
    else
        mkdir /tmp/virbian$x/router
    fi
    cp -r ./* /tmp/virbian$x/router
done


for x in {1..4}; do
  fusermount -u /tmp/virbian$x
done