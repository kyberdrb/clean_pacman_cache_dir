CLion: Reload CMake project

    /usr/bin/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug

CLion: Reload CMake project & Build 'Release' version with CMake

    /usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release

CLion: Reload CMake project & Build 'Debug-Coverage' version with CMake

    /usr/bin/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -DCMAKE_CXX_FLAGS=--coverage -DCMAKE_C_FLAGS=--coverage -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug-coverage

CLion: Reload CMake project & Build 'Release-Coverage' version with CMake

    /usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release-coverage

---

CLion: Clean 'Debug' version

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug --target clean

CLion: Clean 'Release' version

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release --target clean

CLion: Clean 'Debug-Coverage' version

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug-coverage --target clean

CLion: Clean 'Release-Coverage' version

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release-coverage --target clean

---

CLion: Build 'Debug' version with CMake

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug --target clean_pacman_cache_dir

CLion: Build 'Release' version with CMake

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release --target clean_pacman_cache_dir

CLion: Build 'Debug-Coverage' version with CMake

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug-coverage --target clean_pacman_cache_dir

CLion: Build 'Release-Coverage' version with CMake

    /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release-coverage --target clean_pacman_cache_dir

---

Valgrind check (after 'Debug' build)

    /usr/bin/valgrind --tool=memcheck --xml=yes --xml-file=/tmp/clion-valgrindd2088354-9a74-4f97-bc95-8c07006ab859/valgrind_output_%p --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug/clean_pacman_cache_dir

---

Profiling 'Debug' version

Prerequisites (adjusting runtime environment)

    - Temportarily [PREFERRED] (on-demand / ad-hoc - more secure, less convenient: changes need to be reaplied after every reboot/login):

            sudo sh -c 'echo 1 > /proc/sys/kernel/perf_event_paranoid'
            sudo sh -c 'echo 0 >/proc/sys/kernel/kptr_restrict'

    - Permanently (less secure, more convenient: changes are persistent and restored after each reboot/login):

            sudo sh -c 'echo kernel.perf_event_paranoid=1 >> /etc/sysctl.d/99-perf.conf'
            sudo sh -c 'echo kernel.kptr_restrict=0 >> /etc/sysctl.d/99-perf.conf'
            sudo sh -c 'sysctl --system'

Record profiling for a 'Debug' version of the executable:

    /usr/bin/perf record --freq=997 --call-graph dwarf -q -o /tmp/clion18322752102670779839perf /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug/clean_pacman_cache_dir
    /usr/bin/perf record --freq=997 --call-graph dwarf --quiet --output="/tmp/clion18322752102670779839perf" /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug/clean_pacman_cache_dir
    /usr/bin/perf record --freq=997 --call-graph dwarf --output="/tmp/clion18322752102670779839perf" /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug/clean_pacman_cache_dir

Interpret profiling recording:

    perf report --input /tmp/clion18322752102670779839perf
    perf report --sort time,overhead,sym --input /tmp/clion18322752102670779839perf

The utility will output a table of functions sorted by the percentual amount of total runtime. The functions with the longest compute time are listed at the top. These are the ones we might want to optimize.
