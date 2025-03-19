#!/bin/bash

./_build.sh hello_world
./_build.sh return_deadbebe

cp bin/*.exec ../data/exec
