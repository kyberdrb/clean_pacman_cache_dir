#!/bin/sh

set -x

STARTING_TIME="$(date +%Y-%m-%d-%H-%M-%S)"
SCRIPT_DIR="$(dirname "$(readlink --canonicalize "$0")")"

/usr/bin/valgrind --tool=memcheck --xml=yes --xml-file="/tmp/valgrind_output-${STARTING_TIME}.xml" --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no ${SCRIPT_DIR}/cmake-build-debug/clean_pacman_cache_dir > /tmp/clean_pacman_cache_dir-cmake-build-debug-output.log
#/usr/bin/valgrind --tool=memcheck --xml=yes --xml-file="/tmp/valgrind_output-${STARTING_TIME}.xml" --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no ${SCRIPT_DIR}/cmake-build-debug/clean_pacman_cache_dir

#sudo /usr/bin/valgrind --tool=memcheck --xml=yes --xml-file="/tmp/valgrind_output-${STARTING_TIME}.xml" --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no ${SCRIPT_DIR}/cmake-build-debug/clean_pacman_cache_dir

#less "/tmp/valgrind_output-${STARTING_TIME}.xml"

