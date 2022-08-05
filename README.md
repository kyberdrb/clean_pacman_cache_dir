# Clear Package Manager's Cache Directories

a.k.a CPMCD - **C**lean **P**ackage **M**anager's **C**ache **D**irectories

A utility to delete the contents of the pacman's and pikaur's cache directories and leave in mentioned directory only package files, that the locally installed packages were installed from.

The packages that are listed next to `IgnorePkg` option in the pacman's configuration file - by default at `/etc/pacman.conf` are excluded from deletion. Package files that belong to the ignored packages and deviate from the locally installed version of installed packages are excluded from automatic deletion and need to be deleted manually if needed.

**Maybe it could be useful to make a backup before you run this utility. It may save you precious time when something goes wrong. I tested it on my machine. I don't know yours. Proceed with caution.**

## Quick Start Guide

1. **Clone** the repository

        mkdir "${HOME}/git"
        cd "${HOME}/git"
        git clone https://github.com/kyberdrb/clean_pacman_cache_dir.git
        cd clean_pacman_cache_dir

1. **Install dependencies**

        sudo pacman --sync --refresh --refresh --needed cmake ninja

1. **Build** the program:

    1. Generate makefiles

            /usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -G Ninja -S "${HOME}/git/clean_pacman_cache_dir" -B "${HOME}/git/clean_pacman_cache_dir/cmake-build-release"

        _[OPTIONAL ALTERNATIVE] Clion alternative with its bundled set utilities - just for reference; I prefer using the utilities of the system; skip if you already executed the command above_

            /opt/clion/bin/cmake/linux/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/opt/clion/bin/ninja/linux/ninja -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release

    1. Compile the binary executable

            /usr/bin/cmake --build "${HOME}/git/clean_pacman_cache_dir/cmake-build-release" --target clean_pacman_cache_dir

        _[OPTIONAL ALTERNATIVE] Clion alternative with its bundled set utilities - just for reference; skip if you executed the command above_

            /opt/clion/bin/cmake/linux/bin/cmake --build "${HOME}/git/clean_pacman_cache_dir/cmake-build-release" --target clean_pacman_cache_dir

1. **[OPTIONAL but recommended]**
**Check** the contents of the pacman's cache directory

        find "/var/cache/pacman/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort | less
        ls --color=auto --group-directories-first -1 /var/cache/pacman/pkg/ | less
        du -sh /var/cache/pacman/pkg/
        df -h

1. **Run** the compiled binary with elevated priviledges

        time sudo ./cmake-build-release/clean_pacman_cache_dir

1. **Verify** the log file printed as the last line of the output in the terminal

        less file.log

1. **Verify** contents of directories

    In `/var/cache/pacman/pkg/` will be only package files related only to locally installed version for every package

        find "/var/cache/pacman/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort | less
        ls --color=auto --group-directories-first -1 /var/cache/pacman/pkg/ | less

    In `/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED` will be package files for other versions, build directories and general package manager's safely deletable files

        du -sh /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED
        find "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort | less
        ls --color=auto --group-directories-first -1 /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED | less

1. Delete the directory with collected package files

    - Less destructive - throwing deletion candidate files to _trash_ - less trust, more caution, slower, more complicated - possibility to restore packages if needed and delete the rest later

          $ sudo chown --recursive /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED --reference "${HOME}"
          $ sudo move /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED ~/Downloads
          $ gio trash ~/Downloads/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED

    - More destructive - deleting entire directory with deletion candidate files related to packages - more trust, less caution, faster, simpler

          $ sudo rm -r /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED

1. Verify the contents of the pacman's cache directory, which will now contain only files for locally installed packages, together with amount of free space on the partition

        find "/var/cache/pacman/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort | less
        ls --color=auto --group-directories-first -1 /var/cache/pacman/pkg/ | less
        df -h

## Contributing

See [CONTRIBUTING](CONTRIBUTING.md) and [CODE_OF_CONDUCT](CODE_OF_CONDUCT) documents.

## Sources

- `libalpm` - library of the Arch Linux Package Manager - the `pacman`
    - https://duckduckgo.com/?q=libalpm&ia=web
    - https://duckduckgo.com/?q=libalpm+list+packages&ia=web
    - https://man.archlinux.org/man/libalpm.3
    - https://man.archlinux.org/man/libalpm.3.de
    - https://man.archlinux.org/man/libalpm_packages.3.en
    - https://archlinux.org/pacman/
    - https://gitlab.archlinux.org/pacman/pacman/-/blob/master/NEWS
    - https://gitlab.archlinux.org/pacman/pacman
    - https://bugs.archlinux.org/index.php?project=3
    - https://duckduckgo.com/?t=ffab&q=libalpm+example&ia=web
    - https://code.toofishes.net/pacman/doc/group__alpm__api__databases.html
- `std::map`
    - https://duckduckgo.com/?q=c%2B%2B+map+find+key+containing+pattern+substring&t=ffab&ia=web
    - https://stackoverflow.com/questions/9349797/partial-match-for-the-key-of-a-stdmap
    - http://www.cplusplus.com/reference/map/map/lower_bound/
    - https://duckduckgo.com/?q=c%2B%2B+custom+object+std+map+key+keys&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key
    - https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key#1102720
    - https://www.techiedelight.com/use-custom-objects-keys-std-map-cpp/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+change+comparator+at+runtime&ia=web
    - https://stackoverflow.com/questions/5733254/how-can-i-create-my-own-comparator-for-a-map
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+custom+object+class+key+error%3A+no+match+for+%E2%80%98operator%3C%E2%80%99&ia=web
    - https://stackoverflow.com/questions/22248284/error-no-match-for-operator-in-x-y-when-trying-to-insert-in-two-map
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+custom+object+class+unique+ptr+key+error%3A+no+match+for+%27operator%3C%27&ia=web
    - https://duckduckgo.com/?t=ffab&q=Using+std%3A%3Aunique_ptr+inside+a+map+as+a+key&ia=web
    - https://stackoverflow.com/questions/29887185/using-stdunique-ptr-inside-a-map-as-a-key
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+smart+pointer+map+key&ia=web
    - https://stackoverflow.com/questions/26252698/smart-pointers-as-map-key
        - using smart pointers as a key in `std::map` - specialize template for `less` functor in the `std` namespace for my custom class, so that the `map` uses the specialized version of `less` to compare the smart pointers with custom objects, in order to search in it by key. **The `less` specialization is NOT necessary if the keys in the `map` will be only inserted and iterated through (and never looked up by key with `find`).**
    - https://duckduckgo.com/?q=c%2B%2B+map+get+key++custom+object+value&t=ffab&ia=web
    - https://www.lonecpluspluscoder.com/2015/08/13/an-elegant-way-to-extract-keys-from-a-c-map/
    - https://www.geeksforgeeks.org/c-map-key-user-define-data-type/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+custom+object+key+find&ia=web
    - https://stackoverflow.com/questions/53402737/c-using-stdfind-in-a-map-where-the-key-is-a-custom-class
        - using custom comparator at runtime  to looking up elements in the `map` - with `std::find_if` or `std::any_of` with a custom predicate
    - - https://duckduckgo.com/?q=c%2B%2B+map+find+key+beginning+with+substring+partial+match+prefix&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/9349797/partial-match-for-the-key-of-a-stdmap/57905386#57905386
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+map+lower_bound&ia=web
    - https://www.cplusplus.com/reference/map/map/lower_bound/
- C++20 SPACESHIP OPERATOR `<=>` (for comparison of custom objects - also for the keys in a `std::map` of a custom type)
    - https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key/70319881#70319881
        - specific answer for C++20 for a `map` with keys of custom type from: https://stackoverflow.com/questions/1102392/how-can-i-use-stdmaps-with-user-defined-types-as-key
    - https://duckduckgo.com/?q=c%2B%2B+20+spaceship+operator+map+key+custom+object+class&t=ffab&ia=web&iax=qa
    - http://modernescpp.com/index.php/c-20-more-details-to-the-spaceship-operator
    - https://stackoverflow.com/questions/20168173/when-using-stdmap-should-i-overload-operator-for-the-key-type#20168253
- `std::set`
    - https://www.cplusplus.com/reference/set/set/
    - https://www.cplusplus.com/reference/set/set/find/
    - https://duckduckgo.com/?q=c%2B%2B+set+smart+pointer+find&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/25878437/c-find-in-set-of-pointers
        - `set::find` with transparent comparator functor heterogenous lookup with `std::less`
            - but the transparent comparator functor didn't worked in my case - the 'non-transparent' - the duplicitly specified one did - in the case of organizing the elements in `std::set` in descending order with

                ```
                'std::greater<std::unique_ptr<PackageName>>'
                ```

        - `set::find` with `const_cast` to find a `const` pointer in a `std::set` of non-const elements of pointer type
    - https://duckduckgo.com/?q=c%2B%2B+set+transparent+comparator+greater&t=ffab&ia=web
    - https://stackoverflow.com/questions/17737132/transparent-operator-functors
    - https://thecodeteacher.com/question/40532/c++---What-are-transparent-comparators
    - https://duckduckgo.com/?t=ffab&q=std+set+custom+comparator&ia=web&iax=qa
    - https://stackoverflow.com/questions/2620862/using-custom-stdset-comparator
        - https://stackoverflow.com/questions/2620862/using-custom-stdset-comparator/46128321#46128321 - **Using `set::find` with provided comparator as a second template parameter at `std::set` initialization to enable finding an element of custom type in `std::set`**
            - Lambda comparator C++20 solution

                ```
                auto cmp = [](int a, int b) { return ... };
                std::set<int, decltype(cmp)> s;
                ```

            - Lambda comparator C++11 solution

                ```
                auto cmp = [](int a, int b) { return ... };
                std::set<int, decltype(cmp)> s(cmp);
                ```

            - Non-member Function comparator

                ```
                bool cmp(int a, int b) {
                    return ...;
                }
                ```

                and use it at `std::set` initialization either as

                ```
                std::set<int, decltype(cmp)*> s(cmp);
                ```

                or as

                ```
                std::set<int, decltype(&cmp)> s(&cmp);
                ```

            - Comparator class

                ```
                struct cmp {
                    bool operator() (int a, int b) const {
                        return ...
                    }
                };

                std::set<int, cmp> s;
                ```

            - Non-member Function comparator wrapped in `std::integral_constant` - _https://en.cppreference.com/w/cpp/types/integral_constant_

                Define comparator function

                ```
                bool cmp(int a, int b) {
                    return ...;
                }
                ```

                Wrap the function into `std::integral_constant`

                ```
                #include <type_traits>
                using Cmp = std::integral_constant<decltype(&cmp), &cmp>;
                ```

                Use the wrapped comparator function at `std::set` initialization

                ```
                std::set<int, Cmp> set;
                ```

    - https://stackoverflow.com/questions/2620862/using-custom-stdset-comparator/56457701#56457701 - **Using `set::find` with provided comparator as overloaded `operator<` or specialized `std::less` - both in various forms - to enable finding an element of custom type in `std::set`**
        - specializing `std::less` functor as comparator (with optional overload of `operator<`) for ascending order, i. e. `a-z` and `A-Z`
        - overloading `operator<` as a public `friend`/member/`std` function
        - specializing `std::greater` functor as comparator (with optional overload of `operator<`) for descending order, i. e. `Z-A` and `z-a`
        - overloading the _spaceship_ operator `operator<=>` (C++20>)
    - https://stackoverflow.com/questions/11185223/stdset-select-less-or-greater-comparator-at-runtime#11185365
    - https://stackoverflow.com/questions/23377400/creating-a-c-stl-set-containing-custom-objects
    - https://duckduckgo.com/?q=c%2B%2B+std+set+descending&t=ffab&ia=web
    - https://stackoverflow.com/questions/22591645/c-stdmap-items-in-descending-order-of-keys
    - https://cplusplus.com/forum/general/205795/
    - https://en.cppreference.com/w/cpp/utility/functional/greater
        - use `using namespace` i. e. `using` directive only in `.cpp` files
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+custom+objects+operator+comparator&ia=web
    - https://www.reddit.com/r/cpp_questions/comments/p66t83/stdset_of_pointers_with_custom_comparator_and/
    - https://www.fluentcpp.com/2017/06/09/search-set-another-type-key/
        - Some **sets store object that embed their own keys**, that is to say that **such objects have a subpart that is to be considered as a key, like an ID for example, while the [entire] object itself is to be considered as a value.**
    - https://www.reddit.com/r/cpp/comments/5yljrp/functors_are_not_dead_the_double_functor_trick/
    - https://stackoverflow.com/search?tab=Relevance&pagesize=50&q=%5bc%2b%2b%5d%20std%3a%3aset%20smart%20pointer
    - https://stackoverflow.com/questions/18563508/does-stdset-still-sort-when-elements-are-wrapped-in-smart-pointers
    - https://usaco.guide/silver/custom-cpp-stl?lang=cpp - C++ Sets with Custom Comparators
    - http://neutrofoton.github.io/blog/2016/12/30/c-plus-plus-set-with-custom-comparator/ - C++ Set With Custom Comparator
    - https://thispointer.com/stdset-tutorial-part-1-set-usage-details-with-default-sorting-criteria/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+doesn%27t+find&ia=web
    - https://www.cplusplus.com/reference/set/set/find/
    - https://www.cplusplus.com/reference/set/set/
    - https://www.cplusplus.com/reference/set/set/set/
    - https://stackoverflow.com/questions/14288825/setfind-doesnt-find
    - https://stackoverflow.com/questions/14288825/setfind-doesnt-find/14288925#14288925
        - **strict weak ordering**
            - For all x: x < x is never true, everything should be equal to itself
            - If x < y then y < x cannot be true
            - If x < y and y < z then x < z, the ordering should be transitive
            - If x == y and y == z then x == z, equality should be transitive
        - **`std::tie` for interval comparisons**
    - https://duckduckgo.com/?t=ffab&q=strict+weak+ordering+c%2B%2B&ia=web
    - https://stackoverflow.com/questions/1293231/stl-ordering-strict-weak-ordering
    - https://stackoverflow.com/questions/1293231/stl-ordering-strict-weak-ordering/35773903#35773903
        - **The whole point of limiting the [STL] library to a less-than operator [`operator<`] is that all of the logical operators can be [derived from this one mentioned operator - `operator<` in order to conform to the strict weak ordering]:**
            - `<(a, b): (a < b)`
            - `<=(a, b): !(b < a)`
            - `==(a, b): !(a < b) && !(b < a)`
            - `!=(a, b): (a < b) || (b < a)`
            - `>(a, b): (b < a)`
            - `>=(a, b): !(a < b)`

        This works as long as your provided operator meets the conditions of a strict weak ordering. The standard <= and >= operators do not.

    - https://www.quora.com/What-is-strict-weak-ordering-in-C++-sort/answer/Adam-Helps?srid=XiLw&share=d2cd7d8a
    - https://www.quora.com/What-is-strict-weak-ordering-in-C++-sort
    - https://en.wikipedia.org/wiki/Weak_ordering#Axiomatizations
    - https://stackoverflow.com/questions/67357282/how-does-comparator-in-a-set-works-with-functor-in-c
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std%3A%3Aset+with+unique_ptr&ia=web
    - https://stackoverflow.com/questions/46458514/stdset-of-unique-ptr-range-insert
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+set+custom+objects+operator%3C&ia=web&iax=qa
    - https://stackoverflow.com/questions/2620862/using-custom-stdset-comparator
    - https://duckduckgo.com/?q=c%2B%2B+std+set+custom+objects+friend+operator%3C&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/19871647/how-do-i-insert-objects-into-stl-set#19871726
    - https://stackoverflow.com/questions/19871647/how-do-i-insert-objects-into-stl-set/19871721#19871721
    - https://duckduckgo.com/?q=c%2B%2B+no+matching+function+call+set+find+comparator&t=ffab&ia=web
    - https://stackoverflow.com/questions/21609490/stdfind-error-no-matching-function
    - https://duckduckgo.com/?t=ffab&q=std%3A%3Afind+set%3A%3Afind&ia=web
    - https://stackoverflow.com/questions/1701067/how-to-check-that-an-element-is-in-a-stdset
    - https://stackoverflow.com/questions/1701067/how-to-check-that-an-element-is-in-a-stdset/54197839#comment1576902_1701083
        - this is specific for sets and maps [i.e. datastructures implemented with RB Tree - for logarithmic - log(n) - complexity for `insert`/`push`/`emplace`, `delete` and `find` functions].
        `vector`s, `list`s etc. don't have a find member function [sorted vector can have `binary_search` to get log(n) complexity for `find`, but other datastructures - mainly explicitly defined and unsorted - have O(n) complexity].
    - https://stackoverflow.com/questions/1701067/how-to-check-that-an-element-is-in-a-stdset/1701855#1701855 - check if an element exists in `std::set` with `count` function
    - https://stackoverflow.com/questions/1701067/how-to-check-that-an-element-is-in-a-stdset/54197839#54197839 - check if an element exists in `std::set` with `contains` (C++20>) function
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+comparator+functor&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+pointers+smart+unique+comparator&ia=web
    - https://stackoverflow.com/questions/31562548/set-of-pointers-with-custom-comparator
    - https://duckduckgo.com/?q=c%2B%2B+set+custom+less+specialization+pointers+unique&t=ffab&ia=web
    - https://stackoverflow.com/questions/63213832/ordering-in-stdset-of-unique-pointers
    - https://duckduckgo.com/?q=c%2B%2B+ordering+less+set+of+unique+pointers+comparator+functor&t=ffab&ia=web
    - https://www.codegrepper.com/code-examples/cpp/c%2B%2B+custom+comparator+for+elements+in+set
    - https://duckduckgo.com/?q=c%2B%2B+std+binary_search+set+operator&t=ffab&ia=web&iax=about
    - https://stackoverflow.com/questions/18406479/binary-search-with-stdpair-using-a-custom-operator
    - https://en.cppreference.com/w/cpp/algorithm/binary_search
        - ways of finding an element of custom type in `std::set`
            - Returning an Iterator to an element
                - `set::find`
                - `std::find`
                - `std::find_if`
            - Returning a boolean whether the k
                - `std::count`
                - `std::contains` (C++20>)
                - `std::any_of`
                - `std::binary_search`
    - https://en.cppreference.com/w/cpp/algorithm/find
    - https://github.com/kyberdrb/clean_pacman_cache_dir/commit/e872520bef41d017ac70cc56e14ae76726efb7aa
    - https://github.com/kyberdrb/clean_pacman_cache_dir/tree/e872520bef41d017ac70cc56e14ae76726efb7aa
        - Simplifying lookup for ignored package with `std::any_of` and a *Predicate*
    - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/e872520bef41d017ac70cc56e14ae76726efb7aa/PackageComparatorPredicate.h
        - Simplifying lookup for ignored package with `std::any_of` and a *Predicate*
    - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/e872520bef41d017ac70cc56e14ae76726efb7aa/main.cpp
        - Simplifying lookup for ignored package with `std::any_of` and a *Predicate*
- `regex`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+replace+regex+patern&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+sed+regex+replace+equivalent&ia=web
    - https://www.softwaretestinghelp.com/regex-in-cpp/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+regex_replace+example&ia=web
    - https://www.cplusplus.com/reference/regex/regex_replace/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+regex_search&ia=web
    - https://cplusplus.com/reference/regex/regex_search/
    - https://regex101.com/
    - https://duckduckgo.com/?t=ffab&q=regex+match+from+backwards&ia=web
    - https://stackoverflow.com/questions/1103149/non-greedy-reluctant-regex-matching-in-sed
- `std::string`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+tokenize+string&ia=web
    - https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
    - https://duckduckgo.com/?q=c%2B%2B+check+if+number&t=ffab&ia=web
    - https://www.tutorialspoint.com/how-to-check-if-input-is-numeric-in-cplusplus
    - https://duckduckgo.com/?q=c%2B%2B+string+clear&t=ffab&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+reverse+string&ia=web
    - https://www.journaldev.com/35816/reverse-string-c-plus-plus
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+find+occurrence+character&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+getAbsolutePath+substring+extract+index+position&ia=web
    - https://www.educba.com/c-plus-plus-substring/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+substr&ia=web
    - https://www.cplusplus.com/reference/string/string/substr/
- `std::vector`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+contains&ia=web&iax=qa
    - https://stackoverflow.com/questions/3450860/check-if-a-stdvector-contains-a-certain-object#3450906 - **search vector of objects or unique pointers with `find_if` and a custom predicate:**
        - **either a comparator as functor encapsulated in a separate class/file - encapsulated - the class the objects of the vector are elements of is left as-is - useful when the class file is read-only, reusable, easier to understand; increases number of files in a project...**
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects/6939290#6939290
        - **...or a lambda function - disposable function - fewer lines of code; maybe more complicated to understand...**
        - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/6bb11e5fb9ba9d3f39a7084a3033bc805b89c829/Packages.cpp#L122
    - https://duckduckgo.com/?t=ffab&q=find+custom+object+vector&ia=web
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects/6939278#6939278 - **...or search a vector of objects or of unique pointers with `find` and by overloading the equality operator as a public friend function for the type of elements the vector holds - uses only the class features, `vector` featres and C++ features only - all encapsulated in one class with only internal comparing mechanism - no external comparator mechanisms**
    - https://stackoverflow.com/questions/3450860/check-if-a-stdvector-contains-a-certain-object#3450906 - **search vector of objects or unique pointers with `find_if` and a custom predicate:**
        - **either a comparator as functor encapsulated in a separate class/file - encapsulated - the class the objects of the vector are elements of is left as-is - useful when the class file is read-only, reusable, simpler client (caller) code; increases number of files in a project...**
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects/6939290#6939290
        - **...or a lambda function - disposable function - fewer lines of code; maybe more complicated to understand...**
    - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/6bb11e5fb9ba9d3f39a7084a3033bc805b89c829/Packages.cpp#L122
    - https://duckduckgo.com/?t=ffab&q=find+custom+object+vector&ia=web
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects
    - https://stackoverflow.com/questions/6939129/how-to-use-stdfind-stdfind-if-with-a-vector-of-custom-class-objects/6939278#6939278 - **...or search a vector of objects or of unique pointers with `find` and by overloading the equality operator as a public friend function for the type of elements the vector holds - uses only the class features, `vector` featres and C++ features only - all encapsulated in one class with only internal comparing mechanism - no external comparator mechanisms**
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+find+if+predicate&ia=web
    - https://stackoverflow.com/questions/14437825/using-stdfind-with-a-predicate
    - https://stackoverflow.com/questions/14437825/using-stdfind-with-a-predicate/34709047#34709047 - **`any_of` with lambda predicate**
    - https://stackoverflow.com/questions/14437825/using-stdfind-with-a-predicate/14437961#14437961 - **`find_if` with lambda predicate**
    - https://stackoverflow.com/questions/22660448/using-predicate-in-find-if
    - https://stackoverflow.com/questions/22660448/using-predicate-in-find-if/22660600#22660600 - **`find_if` with comparator functor**
    - https://stackoverflow.com/questions/22660448/using-predicate-in-find-if/22660643#22660643 - **`find_if` with either lambda or comparator functor**
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+find_if+predicate+struct&ia=web
    - https://stackoverflow.com/questions/14225932/search-for-a-struct-item-in-a-vector-by-member-data - **_read entire thread for finding and removing elements in a vector with lambdas, comparator functors, and templated comparator functors_**
    - https://stackoverflow.com/questions/14225932/search-for-a-struct-item-in-a-vector-by-member-data/14226007#14226007 - **`find_if` with templated comparator functor**
    - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/6bb11e5fb9ba9d3f39a7084a3033bc805b89c829/Packages.cpp#L123
    - https://github.com/kyberdrb/clean_pacman_cache_dir/commit/d0515a7c532c5a667e2d7b87a59d19c509569958
      https://github.com/kyberdrb/clean_pacman_cache_dir/blob/d0515a7c532c5a667e2d7b87a59d19c509569958/Packages.cpp#L99
    - https://github.com/kyberdrb/clean_pacman_cache_dir/blob/d0515a7c532c5a667e2d7b87a59d19c509569958/PackageNamesEqualityComparator.h
- `std::filesystem`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+filesystem+remove+directory&ia=web
    - https://en.cppreference.com/w/cpp/filesystem/remove
    - https://techoverflow.net/2019/04/21/how-to-recursively-delete-directory-using-c17-filesystem-library/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+memory+leak+filesystem&ia=web
        - `rename()` in `std::filesystem` leads to memory leak (thanks Valgrind ;) ) if we don't catch the `filesystem_error` exception which is thrown when we don't have the permissions to rename/move a file or when the source file doesn't exist
    - Possible memory leak in filesystem::copy() #66  - https://github.com/boostorg/filesystem/issues/66
    - https://stackoverflow.com/questions/36019855/memory-leak-in-directory-iterator-of-boost-filesystem
- own projects
    - duplicate_finder - https://github.com/kyberdrb/duplicate_finder
        - iterating through files
    - EmployeeManagementSystem - https://github.com/kyberdrb/EmployeeManagementSystem
        - _Singleton_ design pattern implementation from `ID_Pool` class implementation
- UML / PlantUML
    - https://duckduckgo.com/?t=ffab&q=uml+abstract+class&ia=web
    - https://www.guru99.com/uml-class-diagram.html
    - https://duckduckgo.com/?q=uml+abstract+method&t=ffab&ia=web
    - https://stackoverflow.com/questions/12636575/uml-class-diagram-is-this-how-to-write-abstract-method-and-property
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+pure+virtual+class&ia=web
    - https://plugins.jetbrains.com/plugin/7017-plantuml-integration
    - https://plantuml.com/class-diagram
    - https://duckduckgo.com/?t=ffab&q=plantuml+interface&ia=web
    - https://blog.jetbrains.com/dotnet/2020/10/06/create-uml-diagrams-using-plantuml/
    - https://duckduckgo.com/?t=ffab&q=plantuml+comment&ia=web
    - https://plantuml.com/commons
    - https://stackoverflow.com/questions/61398067/override-method-in-uml
    - https://duckduckgo.com/?t=ffab&q=plantuml+line+break&ia=web
    - https://forum.plantuml.net/13687/can-use-line-brake-in-note-generated-from-variable
    - How to use PlantUML with Markdown - https://gist.github.com/noamtamim/f11982b28602bd7e604c233fbe9d910f
    - https://forum.plantuml.net/3720/how-do-you-change-the-font-used-for-monospaced-style-text
        - PlantUML monospaced text

        ```
        ""this is monospaced""
        ```
    - https://forum.plantuml.net/977/change-a-distance-between-classes
        - Increase/Decrease distance between classes/entities in PlantUML diagrams with

            ```
            skinparam nodesep 10
            skinparam ranksep 20
            ```
    - https://duckduckgo.com/?t=ffab&q=plantuml+adjust+label+position&ia=web
    - https://stackoverflow.com/questions/45103284/plantuml-define-relative-position-of-components
    - https://stackoverflow.com/questions/45103284/plantuml-define-relative-position-of-components/67149595#67149595
        - Specify preferred arrow direction in a relationship between entities with

            ```
            -l->
            -r->
            -u->
            -d->
            ```

            or

            ```
            -lelft->
            -right->
            -up->
            -down->
            ```

    - https://graphviz.org/
        - Graphviz - The graphics engine that PlantUML uses
    - https://duckduckgo.com/?t=ffab&q=plantuml+relative+label+position&ia=web
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path/48735216#48735216
        - Typical workarounds that I've seen or used include:
            - adding hidden lines a -[hidden]- b
            - extending the length of a line a --- b (more dashes, longer line)
            - specifying preferred direction of lines (a -left- b)
            - swapping association ends (a -- b → b -- a)
            - changing the order of definitions (the order does matter... sometimes)
            - adding empty nodes with background/border colors set to Transparent
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path/48735216#comment101108492_48735216
        - `together` keyword for groupping entities

            ```
            together group1 { class a class b }  together group2 {  class c  class d }  group1 -[hidden]- group2
            ```
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path/48804742#48804742
        - changing line type

            ```
            skinparam linetype polyline
            skinparam linetype ortho
            ```
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path/61795202#61795202
        - To make a connection less important in the layout, use `[norank]`

            ```
            a -[norank]-> b
            ```
    - https://stackoverflow.com/questions/48712801/how-to-correct-plantuml-line-path/48740023#48740023
        - so for some reason adding the duplicated command both ways solves it.

            ```
            order -down- publisher
            publisher -[hidden]up- order
            ```
    - PlantUML: Label on relations - https://plantuml.com/class-diagram#4a705b44651caa59
    - PlantUML: Notes and stereotypes - https://plantuml.com/class-diagram#59c91a18bcc97bb0
    - https://duckduckgo.com/?q=aggregation+association+lifetime&t=ffab&ia=web
    - https://github-wiki-see.page/m/sukhoi1/Patterns-UML/wiki/000.-UML-Object-Lifetime%3A-Aggregation%2C-Composition%2C-Association
    - https://github.com/sukhoi1/Patterns-UML/wiki/000.-UML-Object-Lifetime:-Aggregation%2C-Composition%2C-Association
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+uml+reference+member+association+aggregation&ia=web
    - https://stackoverflow.com/questions/25779394/c-association-aggregation-and-composition
    - https://stackoverflow.com/questions/25779394/c-association-aggregation-and-composition/25914787#25914787
        - **I'm going to ignore Aggregation. It is not a very clearly defined concept and in my opinion it causes more confusion than it is worth. Composition and Association are quite enough, Craig Larman told me so. It might not be the answer your instructor was looking for but it is unlikely to be implemented in C++ any differently to Association anyway.**
    - https://stackoverflow.com/questions/9640885/uml-aggregation-vs-association
    - https://stackoverflow.com/questions/9640885/uml-aggregation-vs-association/9643017#9643017
        - **Rumbaugh's statement is the most telling and Uncle Bob's good advice. As I've said elsewhere, Aggregation is semantically so weak as to offer nothing practically beneficial. It only has one valid corner case (acyclicity of recursive relationships) however few people know and understand that. So you end up having to point out in comments anyway.**
        **I just don't use it. And have never felt any loss. Stick with simple binary associations and focus on what really matters - getting the cardinality and naming right.** You'll get far more from that than trying to decide the undecidable association vs. aggregation.
        - ---
        - **Composition** - the lifetime/lifecycle of an instance created from a class and the member variable/attribute within the instance are bound,
        i. e. when the instance gets freed from the memory, the member variable gets freed as well,
        e. g. an `unique_ptr` member or a raw pointer member that is deleted in the destructor of the encapsulating class
        - **Association** - the lifetime/lifecycle of an instance created from a class and the member variable/attribute within the instance are independent,
        i. e. when the instance gets freed from the memory, the member variable gets freed as well,
        e. g.
            - a `weak_ptr` member
            - or a reference member
            - or a pointer member that is ommitted from deletion in the destructor of the encapsulating class
        - ---
    - https://stackoverflow.com/questions/9640885/uml-aggregation-vs-association/9641001#9641001
        - Maybe this can help you ...

            > The difference is one of implication. Aggregation denotes whole/part relationships whereas associations do not. **However**, there is not likely to be much difference in the way that the two relationships are implemented. That is, **it would be very difficult to look at the code and determine whether a particular relationship ought to be aggregation or association. For this reason, it is pretty safe to ignore the aggregation relationship altogether.**
            **[So the implementation of the _aggregeation_ and _association_ in the code is very similar, if not identical, therefore we can safely ommit aggregation and use only composition and association to make it easier for ourselves and others to understand our class diagrams.]**

            [Robert C. Martin | UML]
    - https://duckduckgo.com/?q=plantuml+invisible+node+layout&t=ffab&ia=web
    - https://dzone.com/articles/plantuml-pleasantness-layout-elements-with-hidden
    - https://duckduckgo.com/?t=ffab&q=plantuml+invisible+class&ia=web
    - https://stackoverflow.com/questions/56447433/plantuml-how-to-control-the-alignment-of-classes-of-the-same-rank
    - https://www.w3schools.com/charsets/ref_utf_arrows.asp
    - https://duckduckgo.com/?q=plantuml+inner+class&t=ffab&iar=images&iax=images&ia=images&iai=http%3A%2F%2Fwww.plantuml.com%2Fplantuml%2Fpng%2FIyv9B2vMIClFD-HAXZ6Dk41y1MrTNJ0F0000
    - https://forum.plantuml.net/3655/representing-inner-class-or-containement-in-class-diagram
        - Representing inner class / nested class in PlantUML
    - https://forum.plantuml.net/3655/representing-inner-class-or-containement-in-class-diagram?show=3656#a3656
- C++ specific
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+member+vairable+member+function+with+the+same+name&ia=web&iax=qa
    - https://duckduckgo.com/?q=c%2B%2B+find+longest+prefix+match+string&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+string+compare+longest+match+prefix&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/22615838/string-matching-computing-the-longest-prefix-suffix-array-in-kmp-algorithm#22616474
    - https://docs.microsoft.com/en-us/cpp/cpp/delegating-constructors?view=msvc-170
    - https://duckduckgo.com/?q=c%2B%2B+member+non+member+friend+function&t=ffab&ia=web
    - https://www.geeksforgeeks.org/difference-between-friend-function-and-member-function-in-cpp/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+return+string+by+value&ia=web
    - https://stackoverflow.com/questions/31406585/what-is-better-return-stdstring-by-value-or-by-constant-reference
    - https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization
    - https://en.wikipedia.org/wiki/Side_effect_(computer_science)
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+return+string+by+value+const+reference&ia=web
    - https://stackoverflow.com/questions/13507698/return-stdstring-as-const-reference
    - https://stackoverflow.com/questions/13507698/return-stdstring-as-const-reference/13507917#13507917
        - The problem of deciding how to return a non-trivial object from some sort of a container is actually non-trivial:

            1. If the class from which you return your value imposes any sort of constraint on the object, you can't return a non-const reference because it would loose the possibility to enforce its invariants [i. e. `discarding qualifiers`]. Clearly, returning an object by non-const reference is only viable if object the member function is called on is also non-const.

            ...

            3. Returning an object by value may incur a significant cost for copying the object.
        - If you class is further a viable monitor you definitely want to return the object by value because otherwise the object can be mutated before the caller had any chance to copy it.
        - Basically, none of the choices is ideal. When in doubt, I return by value unless the object is known to be expensive to copy in which case I might return by const&.

    - https://stackoverflow.com/questions/13507698/return-stdstring-as-const-reference/13507934#13507934
        - Returning by reference or const reference has no speed difference - both are very fast as they just return a reference to the original object, no copying is involved.
    - https://stackoverflow.com/questions/13507698/return-stdstring-as-const-reference/13510644#13510644
        - The most common thing to do here would be to return the value as a const-reference, then you can use a reference or copy the value as necessary
    - https://stackoverflow.com/questions/13507698/return-stdstring-as-const-reference/13507736#13507736
        - Return it as a reference. If a copy is needed, it certainly can be made from that reference.
    - https://stackoverflow.com/questions/10231349/are-the-days-of-passing-const-stdstring-as-a-parameter-over
        - On my PC, using g++ 4.6.1, I got these results :
            - variable by reference: 1000000000 iterations -> time elapsed: 2.25912 sec
            - variable by value: 1000000000 iterations -> time elapsed: 27.2259 sec
            - literal by reference: 100000000 iterations -> time elapsed: 9.10319 sec
            - literal by value: 100000000 iterations -> time elapsed: 8.62659 sec
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+converting+constructor&ia=web
    - https://en.cppreference.com/w/cpp/language/move_constructor
    - https://duckduckgo.com/?q=c%2B%2B+move+class+member+variable&t=ffab&ia=web&iax=qa
    - https://duckduckgo.com/?t=ffab&q=clang+tidy+c%2B%2B+constructor+must+explicitly+initialize+the+reference+member&ia=web&iax=qa
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+initialize+member+const+reference+to+empty+value&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+reference_wrapper&t=ffab&ia=web
    - https://duckduckgo.com/?t=ffab&q=using+directive+vs+declaration&ia=web
    - https://www.programming4beginners.com/tutorial/chapter13/using-Directive-and-using-Declaration
    - https://duckduckgo.com/?t=ffab&q=std%3A%3Atie&ia=web
    - https://en.cppreference.com/w/cpp/utility/tuple/tie
    - https://duckduckgo.com/?q=c%2B%2B+std+binary_search+vector+unique&t=ffab&ia=web
    - https://stackoverflow.com/questions/6919405/mystical-restriction-on-stdbinary-search?noredirect=1&lq=1
        - **the type of the searched value passed to `std::binary_search` needs to matche the type of elements that the `std::set`, or any other datastructure it iterates through,  holds**
    - https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
    - https://en.cppreference.com/w/cpp/utility/functional/bind
    - https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
    - https://duckduckgo.com/?t=ffab&q=std%3A%3Afor_each&ia=web
    - https://en.cppreference.com/w/cpp/algorithm/for_each
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+parameter+argument&ia=web
    - https://www.geeksforgeeks.org/difference-between-argument-and-parameter-in-c-c-with-examples/
    - https://stackoverflow.com/questions/2522299/c-catch-blocks-catch-exception-by-value-or-reference
    - https://stackoverflow.com/questions/2522299/c-catch-blocks-catch-exception-by-value-or-reference/2522311#2522311
        - The standard practice for exceptions in C++ is ...

            > Throw by value, catch by reference
            [ideally by `const` reference:
            `const` for assurance for immutability
            and `reference` for resolving derived exceptions from `std::exception` inheritance]

            in order to accurately catch the derived exceptions, not only their base classes
    - https://stackoverflow.com/questions/2522299/c-catch-blocks-catch-exception-by-value-or-reference/2522311#comment2519675_2522311
        - To this I would add: **always catch [exceptions] by const reference, and ensure your exception types have const-correct accessors.**
    - https://duckduckgo.com/?q=c%2B%2B+memory+profiler+consumption&t=ffab&ia=webs
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+no+matching+function+for+call+to+swap&ia=web
    - https://stackoverflow.com/questions/28761802/sort-no-matching-function-for-call-to-swap/28761803#28761803
        - It turns out it's a very simple problem, but not very obvious to spot (and the error message doesn't do a very good job in helping out either):

            Remove the `const` declaration on run() - voilá.

            [I was modifying a member variable in a `const` member function. Removing the `const` ness of the function allowed the compiler to modify member variables, i. e. calling the `swap` function]
    - https://duckduckgo.com/?q=c%2B%2B+custom+own+exceptions&t=ffab&ia=web&iax=qa
    - http://peterforgacs.github.io/2017/06/25/Custom-C-Exceptions-For-Beginners/
    - https://dotnettutorials.net/lesson/exception-handling-in-cpp/
    - https://cplusplus.com/doc/tutorial/exceptions/
    - **Detecting home directory path**
        - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+home+directory&ia=web
        - How do you get there home directory - https://cplusplus.com/forum/beginner/24235/
        - https://stackoverflow.com/questions/42696260/how-to-get-user-home-directory-on-windows
        - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+home+directory+linux&ia=web
        - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
        - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux#comment10000952_2910392
        - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux#comment64750158_2910377
        - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux/2910392#2910392
        - https://duckduckgo.com/?q=c%2B%2B+getuid+geteuid+sudo&t=ffab&ia=web
        - https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-geteuid-get-effective-user-id
        - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+user+behind+sudo&ia=web
        - https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo
        - my own intuition: adding `-laudit` to the linker options because of compile/linking time error "undefined reference to 'audit_getloginuid()'"
    - **Converting current time to human readable format**
        - https://duckduckgo.com/?t=ffab&q=c%2B%2B+date+time+timestamp&ia=web
        - with `ctime` header - C-style: https://www.softwaretestinghelp.com/date-and-time-in-cpp/
        - with `chrono` header - C++ style (preferred): https://stackoverflow.com/questions/58603446/convert-timestamp-to-the-formatted-date-time-using-c
        - https://duckduckgo.com/?q=c%2B%2B+chrono+date+time+timestamp&t=ffab&ia=web
        - https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+stringstream+as+member&ia=web
    - https://cplusplus.com/reference/sstream/stringstream/stringstream/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+stringstream+initialize&ia=web&iax=qa
    - https://cplusplus.com/reference/sstream/stringstream/
    - initializing `stringstream` - short answer: https://stackoverflow.com/questions/21924156/how-to-initialize-a-stdstringstream/21924311#21924311
    - initializing `stringstream` - long answer: https://stackoverflow.com/questions/21924156/how-to-initialize-a-stdstringstream#21924263
    - https://duckduckgo.com/?t=ffab&q=stringstream+clear&ia=web
    - clearing/resetting/reinitializing `stringstream` - https://stackoverflow.com/questions/2848087/how-to-clear-stringstream
- Polymorhism, Inheritance and STL (mainly `std::set`)
    - https://duckduckgo.com/?q=c%2B%2B+find+derived+vector&t=ffab&ia=web
    - https://stackoverflow.com/questions/11889178/c-can-vectorbase-contain-objects-of-type-derived#11889242
    - https://duckduckgo.com/?q=c%2B%2B+find+derived+pointer+base+vector&t=ffab&ia=web
    - https://stackoverflow.com/questions/14945787/how-to-reference-derived-objects-in-a-vector-of-pointers-to-base-class-objects
    - https://stackoverflow.com/questions/18223036/i-want-a-vector-of-derived-class-pointers-as-base-class-pointers
    - https://duckduckgo.com/?q=c%2B%2B+vector+of+pointers+to+base+class+std%3A%3Afind&t=ffab&ia=web
    - https://stackoverflow.com/questions/14003932/vector-of-pointers-of-the-base-class
    - https://stackoverflow.com/questions/22760062/stdfind-for-vector-containing-pointers
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+pointers+base+class+find&ia=web
    - https://stackoverflow.com/questions/32596893/c-using-stdfind-on-a-set-of-class-pointers
    - https://stackoverflow.com/questions/4070304/how-can-i-find-an-element-in-a-set-which-contains-pointers-to-the-elements
    - https://www.google.com/search?q=c%2B%2B+set+base+pointers+find&source=hp&ei=J5igYuqxHMqI8gKV3Zy4Bw&iflsig=AJiK0e8AAAAAYqCmN7g2axxDLUnZLfmeIUMzCSLIT7h8&ved=0ahUKEwiq4cDN7534AhVKhFwKHZUuB3cQ4dUDCAY&uact=5&oq=c%2B%2B+set+base+pointers+find&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABOgsIABCABBCxAxCDAToICAAQgAQQsQM6CAgAELEDEIMBOhEILhCABBCxAxCDARDHARCjAjoRCC4QgAQQsQMQgwEQxwEQ0QM6BQgAEIAEOgYIABAeEBY6CAgAEB4QDxAWOgQIABATOggIABAeEBYQEzoKCAAQHhAPEBYQEzoKCAAQHhAWEAoQEzoICCEQHhAWEB06BAghEBU6CgghEB4QDxAWEB06BwghEAoQoAFQAFiVP2C8QGgAcAB4AIABggGIAd8SkgEFMTYuMTCYAQCgAQE&sclient=gws-wiz
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+base+pointers+find&ia=web
    - https://www.google.com/search?q=c%2B%2B+find+element+derived&ei=TpigYpLoM4HYsAem3ay4Cw&start=10&sa=N&ved=2ahUKEwiSx6Tg7534AhUBLOwKHaYuC7cQ8NMDegQIARBM&biw=1918&bih=971
        - **one can find an element in a `std::set` of polymorphic instances, i.e. of pointers to the base class, when the type of the instance is assigned to a variable, that is of the same type that the type of instances in the container - in the case of inheritance, when the `std::set` holds pointers to the common (abstract/virtual) base class, the instance in the `find` function also needs to be of the common (abstract/virtual) base class type**
    - vectors and derived classes - https://cplusplus.com/forum/general/55651/
    - vectors and derived classes - https://cplusplus.com/forum/general/55651/#msg299317
        - ... derived classes in arrays or vectors of base classes is one of the most useful properties of polymorphism. However, the object can't just be of the type, rather it needs to be a pointer of the the base class. The concept of polymorphism is that a pointer of a base class can point to any class that derives from it.
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+set+override+operator%3C+base+derived&ia=web
    - https://stackoverflow.com/questions/40347810/overriding-an-operator-to-use-the-derived-class-in-c
    - https://www.google.com/search?q=c%2B%2B+set+override+operator%3C+base+derived&source=hp&ei=U5mgYoDeBMTDgQaO1JyABA&iflsig=AJiK0e8AAAAAYqCnY9cGJB1OliQwhVMn4awjT21i2pbH&ved=0ahUKEwjA06_c8J34AhXEYcAKHQ4qB0AQ4dUDCAY&uact=5&oq=c%2B%2B+set+override+operator%3C+base+derived&gs_lcp=Cgdnd3Mtd2l6EANQAFgAYPsBaABwAHgAgAGaAYgBmgGSAQMwLjGYAQCgAQKgAQE&sclient=gws-wiz
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+no+matching+function+call+to+std+set+find+derived+base&ia=web
    - https://www.google.com/search?q=c%2B%2B+no+matching+function+call+to+std+set+find+derived+base&source=hp&ei=OJqgYsXjH6iFxc8P_L-HiAc&iflsig=AJiK0e8AAAAAYqCoSMGeKoHqmyxM9M8hOQrZE3EWaVsr&ved=0ahUKEwjF3-PJ8Z34AhWoQvEDHfzfAXEQ4dUDCAY&uact=5&oq=c%2B%2B+no+matching+function+call+to+std+set+find+derived+base&gs_lcp=Cgdnd3Mtd2l6EANQAFgAYOoCaABwAHgAgAF_iAF_kgEDMC4xmAEAoAECoAEB&sclient=gws-wiz
    - https://www.reddit.com/r/cpp_questions/comments/c0vz4s/no_matching_function_for_call/
    - https://duckduckgo.com/?q=c%2B%2B+vector+of+polymorphic+objects&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+vector+of+polymorphic+objects+find&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+set+of+polymorphic+objects&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+set+of+polymorphic+objects+find&t=ffab&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+polymorphic+unique_ptr&ia=web
    - https://stackoverflow.com/questions/13288394/unique-ptr-and-polymorphism
    - https://duckduckgo.com/?t=ffab&q=dynamic_cast+unique_ptr&ia=web
    - https://stackoverflow.com/questions/11002641/dynamic-casting-for-unique-ptr
    - https://duckduckgo.com/?q=c%2B%2B+polymorphism+inheritance+delegate+delegating+constructors&t=ffab&ia=web
    - https://arne-mertz.de/2015/08/new-c-features-inherited-and-delegating-constructors/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+inheriting+constructors&ia=web&iax=qa
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+inheriting+constructors&ia=web&iax=qa
    - https://dotnettutorials.net/lesson/how-cpp-constructors-called-in-inheritance/
    - https://dotnettutorials.net/lesson/how-cpp-constructors-called-in-inheritance/
    - https://dotnettutorials.net/lesson/access-specifiers-in-cpp/
    - http://cplusplus.bordoon.com/inheriting_constructors.html
    - https://stackoverflow.com/questions/20029883/do-we-inherit-constructors-in-c-whats-is-exact-definition-of-inheriting
- `Singleton` design pattern in C++
    - EmployeeManagementSystem - https://github.com/kyberdrb/EmployeeManagementSystem
        - First _Singleton_ design pattern implementation from `ID_Pool` class implementation `ID_Pool.h` and `ID_Pool.cpp` files
    - https://duckduckgo.com/?q=make_unique+singleton&t=ffab&ia=web
    - https://stackoverflow.com/questions/50995599/make-unique-doesnt-compile-for-creating-a-singleton-instance
    - https://stackoverflow.com/questions/50995599/make-unique-doesnt-compile-for-creating-a-singleton-instance/50995962#50995962
    - https://stackoverflow.com/questions/50995599/make-unique-doesnt-compile-for-creating-a-singleton-instance/50996628#50996628
    - https://stackoverflow.com/questions/33905030/how-to-make-stdmake-unique-a-friend-of-my-class
    - https://stackoverflow.com/questions/33905030/how-to-make-stdmake-unique-a-friend-of-my-class/33905304#33905304
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+ld+undefined+reference+to
    - https://stackoverflow.com/questions/14557657/linker-error-c-undefined-reference
    - https://www.geeksforgeeks.org/static-data-members-c/
    - https://duckduckgo.com/?q=static+class+design+pattern&t=ffab&ia=web
    - https://stackoverflow.com/questions/15178769/singleton-design-pattern-vs-static-class
    - https://stackoverflow.com/questions/15178769/singleton-design-pattern-vs-static-class/47002325#47002325
- General programming
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+parameter+argument&ia=web
- CMake, LLVM toolchain, `clang`, cross-compiling
    - https://stackoverflow.com/questions/53879422/how-to-set-g-compile-options-in-clion
    - ---
    - https://duckduckgo.com/?t=ffab&q=cmake+options+o2+optimize&ia=web&iax=qa
    - https://stackoverflow.com/questions/41264827/setting-optimization-settings-in-visual-studio-through-cmake
    - ---
    - https://duckduckgo.com/?t=ffab&q=clang+undfined+reference+to&ia=web
    - https://duckduckgo.com/?t=ffab&q=undefined+reference+to+%60std%3A%3Abasic_ifstream%3Cchar%2C+std%3A%3Achar_traits%3Cchar%3E+%3E%3A%3Abasic_ifstream()%27&ia=web
    - https://stackoverflow.com/questions/22972545/undefined-reference-with-clang-with-o2
    - https://stackoverflow.com/questions/21689228/c-undefined-reference-using-fstream/21689297#21689297
        - **i. e. not `clang` but `clang++` instead**
    - https://duckduckgo.com/?q=llvm+c%2B%2B17&ia=web
    - https://wiki.archlinux.org/title/LLVM
    - https://wiki.archlinux.org/title/GNU#Toolchain
    - https://duckduckgo.com/?q=clion+lld+clang+linker&t=ffab&ia=web
    - https://intellij-support.jetbrains.com/hc/en-us/community/posts/360000394670-How-can-I-configure-LLVM-Clang-6-0-with-CLION-2018-1?page=1#community_comment_360000166850
    - https://intellij-support.jetbrains.com/hc/en-us/community/posts/206606735-Using-Clang-With-CLion-on-Windows?page=1#community_comment_115000631284
    - https://stackoverflow.com/questions/56447526/how-to-add-linker-flags-in-clion
    - https://duckduckgo.com/?t=ffab&q=clion+clang+toolchain+c%2B%2B&ia=web
    - https://duckduckgo.com/?t=ffab&q=clang+c%2B%2B+compiler&ia=web
    - https://duckduckgo.com/?t=ffab&q=cmake+pass+build+options+to+clang&ia=web
    - https://stackoverflow.com/questions/41775000/correctly-passing-command-line-arguments-to-clang-llvm
    - https://duckduckgo.com/?q=cmake+clang-13%3A+warning%3A+-lalpm%3A+%27linker%27+input+unused+%5B-Wunused-command-line-argument%5D&t=ffab&ia=web
    - https://stackoverflow.com/questions/38855817/cmake-import-lpthread-and-others
    - https://duckduckgo.com/?t=ffab&q=cmake+link+libraries&ia=web
    - https://duckduckgo.com/?t=ffab&q=cmake+detect+compiler&ia=web
    - https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
    - https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang/10055571#10055571
    - [-DCMAKE_LINKER=/usr/bin/lld - Google search](https://www.google.com/search?q=-DCMAKE_LINKER%3D%2Fusr%2Fbin%2Flld&source=hp&ei=gqRpYsKhBYHMaLyUpsgO&iflsig=AHkkrS4AAAAAYmmykhO9pSPpbch5vF3LJzwnIaT37LmD&ved=0ahUKEwjC6sfTh7X3AhUBJhoKHTyKCekQ4dUDCAc&uact=5&oq=-DCMAKE_LINKER%3D%2Fusr%2Fbin%2Flld&gs_lcp=Cgdnd3Mtd2l6EANQAFgAYN4CaABwAHgAgAFbiAFbkgEBMZgBAKABAqABAQ&sclient=gws-wiz)
    - [clion lld - Google search](https://www.google.com/search?q=clion+lld&source=hp&ei=yaRpYvv2K8Oaa9XqrGA&iflsig=AHkkrS4AAAAAYmmy2RVSzGzk6d7nbtiNWhma98T2As1F&ved=0ahUKEwi7_9v1h7X3AhVDzRoKHVU1CwwQ4dUDCAc&uact=5&oq=clion+lld&gs_lcp=Cgdnd3Mtd2l6EAMyBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeOgsIABCABBCxAxCDAToFCAAQgAQ6CAgAEIAEELEDOhEILhCABBCxAxCDARDHARCjAjoRCC4QgAQQsQMQgwEQxwEQ0QM6CAguEIAEELEDOg4ILhCABBCxAxDHARCvAToLCC4QsQMQgwEQ1AI6DgguEIAEELEDEIMBENQCOgUILhCABDoRCC4QgAQQsQMQgwEQxwEQrwE6CAguEIAEENQCOgQIABANOgYIABANEB46CAgAEA0QChAeUABYiQlgrwtoAHAAeACAAckBiAGVCZIBBTEuNy4xmAEAoAEB&sclient=gws-wiz)
    - https://blog.jetbrains.com/clion/2020/12/setting-up-clang-on-windows/
    - [cmake linux exe cross compile - Google search](https://www.google.com/search?q=cmake+linux+exe+cross+compile&source=hp&ei=TalpYuO3I8eN9u8P_bWMmAQ&iflsig=AHkkrS4AAAAAYmm3XaHxBQwantog6U8KFOhmtmJ9BY5b&ved=0ahUKEwijkvCcjLX3AhXHhv0HHf0aA0MQ4dUDCAc&uact=5&oq=cmake+linux+exe+cross+compile&gs_lcp=Cgdnd3Mtd2l6EAMyBQghEKABMgUIIRCgATIFCCEQoAEyBQghEKABMggIIRAWEB0QHlAAWABg_ANoAHAAeACAAZoBiAGaAZIBAzAuMZgBAKABAqABAQ&sclient=gws-wiz)
    - https://stackoverflow.com/questions/63178407/cmake-compile-in-linux-execute-in-windows
        - it appears that I need to use a `mingw-w64` for cross-compiling a binary for Windows on a system with Linux/UNIX
    - [clion cmake compile in linux execute in windows exe cross compile - Google search](https://www.google.com/search?q=clion+cmake+compile+in+linux+execute+in+windows+exe+cross+compile&ei=TqlpYuzFOJKB9u8P8dGlqAs&ved=0ahUKEwjspMKdjLX3AhWSgP0HHfFoCbUQ4dUDCA4&uact=5&oq=clion+cmake+compile+in+linux+execute+in+windows+exe+cross+compile&gs_lcp=Cgdnd3Mtd2l6EAM6BwgAEEcQsANKBAhBGABKBAhGGABQph1Y0Vlg3WVoAXABeACAAa8BiAGeIJIBBDcuMjmYAQCgAQHIAQjAAQE&sclient=gws-wiz)
- Linux/UNIX
    - https://duckduckgo.com/?t=ffab&q=extract+single+file+from+tar&ia=web
    - https://www.cyberciti.biz/faq/linux-unix-extracting-specific-files/
    - https://wiki.archlinux.org/title/Arch_Linux_Archive
        - example of a URL for a specific package: `https://archive.archlinux.org/packages/a/a52dec/`
    - https://duckduckgo.com/?q=less+case+sensitive+search+command+line&t=ffab&ia=web
    - https://unix.stackexchange.com/questions/116395/less-searches-are-always-case-insensitive#116401
        - case sensitive search is enabled in `less` by default
        - for case _insensitive_ search use `less -i` or `less --ignore-case` or even `less --IGNORE_CASE`
    - https://duckduckgo.com/?t=ffab&q=open+man+page+in+custom+editor&ia=web
- CLion
    - https://duckduckgo.com/?t=ffab&q=clion+sudo&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+ninja+optimize+o2+clion&t=ffab&ia=web
    - https://blog.jetbrains.com/clion/2019/10/clion-2019-3-eap-ninja-cmake-generators/
    - https://duckduckgo.com/?t=ffab&q=clion+lldb+error%3A+unknown+option%3A+--interpreter%3Dmi2&ia=web
  - https://intellij-support.jetbrains.com/hc/en-us/community/posts/360000394670-How-can-I-configure-LLVM-Clang-6-0-with-CLION-2018-1
- terminal
    - https://duckduckgo.com/?t=ffab&q=copy+bash+output+with+color+formatting&ia=web
    - https://askubuntu.com/questions/394228/bash-colored-copy-paste
    - https://duckduckgo.com/?t=ffab&q=embed+html+to+markdown&ia=web&iax=qa
    - **finally I solved it by marking the text in the `xfce4-terminal`, right clicked on it and from the context menu I selected `Copy as HTML`. That copied the output as HTML formatted text with Markdown interpretted as code block with colors**
    - https://duckduckgo.com/?t=ffab&q=clion+%2Fetc%2Fsysctl.d%2F99-perf.conf&ia=web
    - https://www.jetbrains.com/help/clion/cpu-profiler.html#Prerequisites
    - https://duckduckgo.com/?t=ffab&q=read+perf+profiling+file+log&ia=web
    - https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/monitoring_and_managing_system_status_and_performance/recording-and-analyzing-performance-profiles-with-perf_monitoring-and-managing-system-status-and-performance
    - https://duckduckgo.com/?t=ffab&q=gcov+testing+interpretation&ia=web
    - https://www.linuxtoday.com/blog/analyzing-code-coverage-with-gcov/
    - https://stackoverflow.com/questions/20315388/terminal-find-directories-last-instead-of-first
    - https://stackoverflow.com/questions/20315388/terminal-find-directories-last-instead-of-first/20325502#20325502
- `libarchive`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+libarchive&ia=web&iax=qa

## Result

    PACKAGE FILENAME                                                              PACKAGE NAME                              DOWNLOADED PACKAGE VERSION   INSTALLED PACKAGE VERSION   ARE VERSIONS MATCHING?   KEEP THE PACKAGE?

    accounts-qml-module-0.7-4-x86_64.pkg.tar.zst                                  accounts-qml-module                       0.7-4                        0.7-4                       YES                      YES
    accountsservice-0.6.55-3-x86_64.pkg.tar.zst                                   accountsservice                           0.6.55-3                     22.08.8-1                   NO                       NO
    accountsservice-22.04.62-2-x86_64.pkg.tar.zst                                 accountsservice                           22.04.62-2                   22.08.8-1                   NO                       NO
    accountsservice-22.04.62-2-x86_64.pkg.tar.zst.sig                                                                                                                                                         NO
    accountsservice-22.08.8-1-x86_64.pkg.tar.zst                                  accountsservice                           22.08.8-1                    22.08.8-1                   YES                      YES
    accountsservice-22.08.8-1-x86_64.pkg.tar.zst.sig                                                                                                                                                          YES
    acl-2.3.0-1-x86_64.pkg.tar.zst
    acl-2.3.1-1-x86_64.pkg.tar.zst
    acl-2.3.1-2-x86_64.pkg.tar.zst
    acl-2.3.1-2-x86_64.pkg.tar.zst.sig
    adobe-source-code-pro-fonts-2.038ro+1.058it+1.018var-1-any.pkg.tar.zst
    adobe-source-han-sans-otc-fonts-2.004-1-any.pkg.tar.zst
    adobe-source-han-sans-otc-fonts-2.004-1-any.pkg.tar.zst.sig
    adobe-source-han-serif-otc-fonts-2.001-1-any.pkg.tar.zst
    adobe-source-han-serif-otc-fonts-2.001-1-any.pkg.tar.zst.sig
    adwaita-icon-theme-40.0-1-any.pkg.tar.zst
    adwaita-icon-theme-40.1.1-1-any.pkg.tar.zst
    adwaita-icon-theme-40rc-1-any.pkg.tar.zst
    adwaita-icon-theme-41.0-1-any.pkg.tar.zst
    adwaita-icon-theme-41.0-1-any.pkg.tar.zst.sig

---

Internal structural representation - a coarse draft

Iterate all localy installed packages from the local DB and save each entry into a hashmap `installedPackages` in an instance of class `InstalledPackages`

    class PackagesAndTheirFiles

    installedPackages
    |-key: "accounts-qml-module"             ('std::string'/class 'PackageName'/std::unique_ptr<PackageName>)
      |-value:                                 (std::unique_ptr<Package>)
        |-locallyInstalledVersion: 0.7-4         (std::string/Version/std::unique_ptr<Version>)
        |-architecture: x86_64                   (std::string/Architecture/std:unique_ptr<Architecture>)
        |-packageVersionsWithTheirRelatedDownloadedFiles (std::map<???, std::vector<???>>)
          |-key: 0.7-4                                (std::string/std::unique_ptr<Version>)
            |-accounts-qml-module-0.7-4-x86_64.pkg.tar.zst (std::vector<std::string/std::unique_ptr<PackageRelatedDownloadedFile>>
    |-key: "accountsservice"                 (std::unique_ptr<PackageName>)
      |-value:                                 (std::unique_ptr<Package>)
        |-locallyInstalledVersion: 22.08.8-1     (std::unique_ptr<Version>)
        |-architecture: x86_64                   (std:unique_ptr<Architecture>)
        |-packageVersionsWithTheirRelatedDownloadedFiles (std::map<std::unique_ptr<Version> std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>>)
          |-key: 0.6.55-3                             (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-0.6.55-3-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |
          |-key: 22.04.62-2                                (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-22.04.62-2-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |   |-accountsservice-22.04.62-2-x86_64.pkg.tar.zst.sig (std::unique_ptr<PackageRelatedDownloadedFile>)
          |
          |-key: 22.08.8-1                                 (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-22.08.8-1-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |   |-accountsservice-22.08.8-1-x86_64.pkg.tar.zst.sig (std::unique_ptr<PackageRelatedDownloadedFile>)
    |-key: "acl"
      |-value:
        |-...

- class `PackageName` will use overloaded less-than-operator i.e. `operator<`, because the class is only used as a key in a `installedPackages` map, in order to indicate that the class is used as a key
- class `Version` will use custom comparator functor class `VersionComparator`, because its used as a key in a map `packageVersionsWithTheirRelatedDownloadedFiles` and as a field in the `PackagesAndTheirFiles` to indicate variability of usage of the class
  - I'll use `operator<` from standard string, because the `alpm_pkg_vercmp` function from `alpm.h` ignores package release versions, and I want to do a complete comparison of Versions
- `ignoredPackageNamesInTextFormat` - keep package files that are listed next to `IgnorePkg` option in the pacman's configuration file

## Algorithms

### Algorithm 1 - Composite

- **Is it true that after each word of package filename in its filename comes a delimiter (likely a dash '-') followed by a number?**
**So I can iterate the package filename until I hit a number and then go back two characters (or remove/pop_back last two characters) and get the package filename? Which will then serve me as a key to the `map` and the actual filename as the value?**
- **Not every installed  package has its downloaded package that it was installed from**
  **but every downloaded package has its installed  package.**

1. iterate through all locally installed packages with `libalpm` library _(1195 entries)_
   1. create an instance of `package` with its `locallyInstalledVersion`

          // inside the iteration loop
          std::string locallyInstalledVersion = alpm_pkg_get_version(pkg);
          auto package = std::make_unique<Package>(locallyInstalledVersion);

   2. save each `package` to the packages with the filename of the package being a text key and the `package` instance being the value (`map`?)

          // structure declaration - before the iteration loop
          std::map<
              std::string              // package filename
              std::unique_ptr<Package> // package instance
          > packages;

          ...

          // inside the iteration loop
          packages.emplace_back(alpm_pkg_get_name(pkg), std::move(package));

2. iterate through all files of the pacman's cache directory with the `std::filesystem` library _(7284/7293 files)_
   1. save the filename of the file to a variable `filename`
   2. save the extension of the file to a variable `extension`
   3. if the extension equals to ".part"
      1. add the `filename` to the set of `packageFilesDesignatedForDeletion`

             // structure declaration - before the iteration loop
             std::set<
                 std::string              // package filename
             > packageFilesDesignatedForDeletion;

             ...

             // inside the iteration loop
             packageFilesDesignatedForDeletion.emplace(filename);
      2. continue
   4. Build the regular expression pattern with the help of the `Architecture` class that accumulated all available versions from the previous loop.
   5. shorten the `filename` only to package filename and version by removing the trailing text with a regular expression, and save the result into a variable `packageNameAndVersion`
   6. separate the package filename and version by tokenizing the `packageNameAndVersion` by dash `-` delimiter into `tokens` vector through `token` string
   7. initialize a text variables `packageName` and `packageVersion` to empty text
   8. iterate `tokens`

           For each token
               if the first character of the token is not a number
                   append the token to the packageName
                   continue
               append the token to the packageVersion
   9. remove the dash `-` at the end of `packageName` and `packageVersion`
   10. Use the `packageName` to find the matching key in the `installedPackages` collection
When the key is found (and thus the package is locally installed on the system), add a new entry to `packageVersionsWithTheirRelatedDownloadedFiles` using the `downloadedPackageVersion` a key
   11. When the key is missing (`nullptr`) (and thus the package had been uninstalled from the system), add the `filename` to the collection `packageFilesDesignatedForDeletion`
6. Iterate all entries in `packageFilesDesignatedForDeletion`
   1. Move all entries into a separate directory `PACKAGE_CEMETERY` inside the pacman's cache directory
7. Iterate all entries in `installedPackages`
   1. For each `installedPackage`
      1. if the package is in the list of `IgnoredPkgs` in the pacman's configuration file - likely in `/etc/pacman.conf` (parse or read with `libalpm`?)
         1. continue (skip the deletion of the package - the packages are ignored for good reasons - either they have a strong systemic effect on the system and may cause malfunction of the system, or they are licensed with other than open-source license which may after update stop functioning)
      2. call `moveAllPackageFilesInNonlocalVersionToSeparateDirectory`, where the destination directory will be provided as a parameter?/as an attribute in the class `PackagesAndTheirFiles`?
8. Run the algorithm again to check whether each installed package has exactly one package file that matches the local version of the installed package, and with an optional signature file.

### Algorithm 2 - Tokenization

The version of the algorithm with a tokenization

    1. find all non-alphanumeric characters and save them into `dividers` - a data structure that will hold only one unique copy `(set`?)
    2. tokenize the filename by all characters in `dividers`, i.e. iterate the filename text by characters one-by-one and
       1. save the tokens , i.e. alphanumeric words, and the character they are divided with, into a resizable array (`vector`?) - because number of tokens for each file may be different. For example
    3. initialize a text variable `cumulativePackageName` to empty text
    4. iterate the resizable array of tokens
       1. append current token to the `cumulativePackageName`
       2. check if the `packages` contains and entry that is exactly matching the key stored in `cumulativePackageName`
          1. if yes, add current file to the collection of files for matching `package`

### Algorithm 3 - Compound Substring Key Find with Partial Match

1. Add each filename of every file in pacman's cache directory into a map<ExtendedInstallationPackageFile, PackageWithPartialFilename>, with ExtendedInstallationPackageFile being the filename of the file (without absolute path - only the filename), and PackageWithPartialFilename being empty/null
2. Find all locally installed packages, form a partial filename as prefix by compounding the filename, version and architecture of the package in the format "PACKAGE_NAME-PACKAGE_VERSION-PACKAGE_ARCHITECTURE", and searching for all keys with such prefix in ExtendedInstallationPackageFile. For all found keys assign for the value of PackageWithPartialFilename the compound package filename.
   - When no package file was found a given package, report it?/download it only to the pacmans cache dir? (with alpm? or from archive.archlinux.com by building the URL and webscraping all links that partially match with the compound package filename and downloading all files from the links with `curl` and adding the package filenames manually to the map so that the package files for the locally installed package will be preserved?)
3. Go through each package filename in the map and move to a separate directory FORMER_PACKAGE_VERSIONS such package files that have their values of PackageWithPartialFilename empty/null.

- lookup and handle packages within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges

## Notes

- '2.0.1' is the last working version - but without the fix for printing partially downloaded files

- regex for main.cpp to search for variables to encapsulate in classes: `[std::|\.]find`
    - for 'Package' class at line with

          auto matchingPackage = installedPackages.find(packageWithInferredName);

        - try out all possible combinations how to search in a `std::set` of unique pointers:
            - `std::find` (with overloaded equality operator - direct and dereferenced unique pointer comparison)
            - `std::find_if` (with comparator in separate struct as predicate; with lambda as predicate - direct and dereferenced unique pointer comparison)
            - `std::any_of` (with comparator in separate struct as predicate; with lambda as predicate - direct and dereferenced unique pointer comparison)
    - for line 153 (on upstream branch `main`): integrate the 'PackageName' class to the 'Package' class by composition (maybe)

### Ways of finding an element of custom type in a `std::vector`

- when comparing two unique pointers without dereferencing, i. e. comparing unique pointers directly without '*' or '->',
  only the public equality operator friend function is used

- seems like the public member equality operator (or any operator defined as a member function) is used only when the unique pointer objects (or any other pointer to object) are dereferenced on the both sides of the comparison

- looks like defining any operator as a `public` `friend` (i. e. non-member) function with `const` parameters is the most compatible option: trading of compatibility and stable functionality in multiple context on the one hand with encapsulation on the other.

- std::find
    - public friend function - all params const (the only one that worked for me for smart pointers)
    - TODO overloading global `std::operator==`
- std::find_if
    - direct comparison in main
        - lambda
            - public friend function - all params const (the only one that works)
        - comparator
            - direct comparison in comparator
                - public friend function - all params const
    - dereferenced comparison
        - lambda
            - public friend function - all params non-const
            - public friend function - all params const
            - public member const function - const parameter
            - public member non-const function - const parameter
            - public member const function - non-const parameter
            - public member non-const function - non-const param
        - comparator
            - dereferenced comparison in comparator
                - public friend function - all params non-const
                - public friend function - all params const
                - public member const function - const parameter
                - public member non-const function - const parameter
                - public member const function - non-const parameter
                - public member non-const function - non-const param
- std::any_of
    - direct comparison in main
        - lambda
            - public friend function - all params const (the only one that works)
        - comparator
            - direct comparison in comparator
    - dereferenced comparison
        - lambda
            - public friend function - all params non-const
            - public friend function - all params const
            - public member const function - const parameter
            - public member non-const function - const parameter
            - public member const function - non-const parameter
            - public member non-const function - non-const param
        - comparator
            - dereferenced comparison in comparator
                - public friend function - all params non-const
                - public friend function - all params const
                - public member const function - const parameter
                - public member non-const function - const parameter
                - public member const function - non-const parameter
                - public member non-const function - non-const param
- TODO `std::binary_search`
    - TODO friend `operator<`
    - TODO member `operator<`
    - TODO `std::operator<`
    - TODO `std::less` - for ascending lexicographic order
    - TODO `std::greater` - for descending lexicographic order
    - TODO custom comparator

Example code

```
IgnoredPackageName.h

// FOR DIRECT COMPARISON OF SMART POINTERS

    // Fails to find an element in a vector of unique pointers, because the parameters are not constant
    //  because the third parameter in 'std::find' takes a reference to a constant type 'const _Tp& __val'
    //  and in this function are defined parameters as references to the type of element the vector holds
    //  not as reference to the constant element type that the vector holds,
    //  therefore the types for parameters for the equality operator in the class
    //  and the type of the third parameter in the 'std::find'
    //  need to match in their constant modifiers in order to accurately find an element in the vector,
    //  even though the type of elements the vector holds are NOT constant
//    friend bool operator==(std::unique_ptr<IgnoredPackageName>& oneIgnoredPackageName, std::unique_ptr<IgnoredPackageName>& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName->name == anotherIgnoredPackageName->name;
//    }

    // Fails to find an element in a vector of unique pointers, because the 'std::find' needs to have access to the
    //  internal structure for the type of elements it compares
    //  therefore the equality operator needs to be declared as public 'friend' i.e. nonmember function
    //  even though it is defined in the namespace of this class
//    bool operator==(const std::unique_ptr<IgnoredPackageName>& otherIgnoredPackageName) const {
//        return this->name == otherIgnoredPackageName->name;
//    }

    // Successfully finds an element in a vector of unique pointers because
    //  1. the 'std::find' function has access to the internal structure of the elements that it compares
    //     for equality with the equality operator 'operator=='
    //  2. and the types for parameters for the equality operator in the class
    //     and the type of the third parameter in the 'std::find'
    //     are matching in their constant modifiers for the type of elements the vector holds
    friend bool operator==(const std::unique_ptr<IgnoredPackageName>& oneIgnoredPackageName, const std::unique_ptr<IgnoredPackageName>& anotherIgnoredPackageName) {
        return oneIgnoredPackageName->name == anotherIgnoredPackageName->name;
    }

// FOR COMPARISON OF DEREFERENCED (SMART) POINTERS (I. E. COMPARING OBJECTS not pointers to objects nor pointers to pointers)

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    friend bool operator==(IgnoredPackageName& oneIgnoredPackageName, IgnoredPackageName& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName.name == anotherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    friend bool operator==(const IgnoredPackageName& oneIgnoredPackageName, const IgnoredPackageName& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName.name == anotherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(const IgnoredPackageName& otherIgnoredPackageName) const {
//        return this->name == otherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(IgnoredPackageName& otherIgnoredPackageName) const {
//        return this->name == otherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(const IgnoredPackageName& otherIgnoredPackageName) {
//        return this->name == otherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(IgnoredPackageName& otherIgnoredPackageName) {
//        return this->name == otherIgnoredPackageName.name;
//    }
```

```
main.cpp

// FOR LAMBDA PREDICATE

        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(std::move(packageNameCopy));
        bool isPackageNameIgnored =
                std::find_if(
                        ignoredPackageNames.begin(),
                        ignoredPackageNames.end(),
                        [&ignoredPackageNameCandidate](const std::unique_ptr<IgnoredPackageName>& ignoredPackageName) {
                            return ignoredPackageNameCandidate == ignoredPackageName;
//                            return *ignoredPackageNameCandidate == *ignoredPackageName;
                        }
                ) != ignoredPackageNames.end();

//        bool isPackageNameIgnored =
//                std::any_of(
//                        ignoredPackageNames.begin(),
//                        ignoredPackageNames.end(),
//                        [&ignoredPackageNameCandidate](const std::unique_ptr<IgnoredPackageName>& ignoredPackageName) {
//                            return ignoredPackageNameCandidate == ignoredPackageName;
////                            return *ignoredPackageNameCandidate == *ignoredPackageName;
//                        }
//                );

// FOR COMAPRATOR PREDICATE...

        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(std::move(packageNameCopy));
        bool isPackageNameIgnored =
                std::find_if(
                        ignoredPackageNames.begin(),
                        ignoredPackageNames.end(),
                        IgnoredPackageNamesEqualityComparator(ignoredPackageNameCandidate)
                ) != ignoredPackageNames.end();

//        bool isPackageNameIgnored =
//                std::any_of(
//                        ignoredPackageNames.begin(),
//                        ignoredPackageNames.end(),
//                        IgnoredPackageNamesEqualityComparator(ignoredPackageNameCandidate)
//                );

// ...AND THE COMPARATOR

#pragma once

#include "IgnoredPackageName.h"

// Comparator for std::map with unique_ptr keys didn't work for std::vector with unique_ptr elements...
//struct IgnoredPackageNamesEqualityComparator {
//    bool operator()(const PackageName& firstPackageName, const PackageName& secondPackageName) const {
//        return firstPackageName == secondPackageName;
//    }
//};

// ...using Predicate instead (pre C++11 solution)
struct IgnoredPackageNamesEqualityComparator {
    const std::unique_ptr<IgnoredPackageName>& ignoredPackageName;

    explicit IgnoredPackageNamesEqualityComparator(const std::unique_ptr<IgnoredPackageName>& packageName) :
        ignoredPackageName(packageName)
    {}

    bool operator()(const std::unique_ptr<IgnoredPackageName>& otherPackageName) const {
//        return (ignoredPackageName == otherPackageName);
        return (*ignoredPackageName == *otherPackageName);
    }
};
```

### Ways of finding an element of custom type in a `std::set`

- Client: `main.cpp`
- Comparator: lambda/`PackageComparator.h`/internal comparison logic in overloaded `operator<` or specialized `std::less` function (the default function/functor that `std::set` uses to compare elements)
- Element of custom type in `std::set`: Package (`Package.h`)

PACKAGE (INSTANCE OF CUSTOM TYPE) TO FIND

    // main.cpp

    #include "Package.h"

    <~snip~>

    std::string inferredPackageNameAsText = packageNameAndVersion;
    auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageNameAsText));

STRATEGIES TO FIND A PACKAGE (AN INSTANCE OF CUSTOM TYPE)

- `set::find` - passing unique ptr ref

    ```
    // main.cpp

    std::set<std::unique_ptr<Package>> installedPackages{};

    <~snip~>

    auto matchingPackage = installedPackages.find(packageWithInferredName);
    ```

    - overloading `operator<`
        - public friend `operator<` with all const params of reference type to const `unique_ptr` with dereferenced comparison by `->` within the operator function **without** specialized `std::less`

            ```
            // Package.h

            friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name < anotherPackage->name;
            }
            ```

        - public friend `operator<` with all const params of reference type to const `unique_ptr` with dereferenced comparison by `->` within the operator function **with** specialized `std::less` (**unnecessary** to specialize 'std::less' - the public friend `operator<` in mentioned format is enough to ensure ordering of elements in the `std::set`)

                  // Package.h

                  // WORKS for direct comparison with and without overloading 'std::less' funcion
                  friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                      return onePackage->name < anotherPackage->name;
                  }

                  // Doesn't work - compiles but fails to find the searched for element at runtime
                  //    friend bool operator<(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
                  //        return onePackage->name < anotherPackage->name;
                  //    }

                  // Doesn't work - compiles but fails to find the searched for element at runtime
                  //    bool operator<(const std::unique_ptr<Package>& package) const {
                  //        // TODO maybe replace the 'getName()' function with only fields?
                  //
                  //        assert(this->getName() == this->name);
                  //        assert(this->getName() == Package::name);
                  //        assert(this->name == Package::name);
                  //
                  //        return this->getName() < package->getName();
                  //        //        return this->name < package->getName();
                  //        //        return Package::name < package->getName();
                  //        //        return this->name < package->name;
                  //        //        return Package::name < package->name;
                  //    }

            And then specialize the `std::less` function outside of the class as a non-member function, e. g. under the class block in the header file

                // Package.h

                // overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
                namespace std {
                    template<>
                    struct less<unique_ptr<Package>> {
                        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
                            return onePackage < anotherPackage;
                        }
                    };
                }

        - overloading the global `std::operator<` outside (e.g. under) the class definition:

            ```
            // Package.h

            namespace std {
                inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->getName() < anotherPackage->getName();
                }
            }
            ```

    - defining a custom comparator

        and passing the custom comparator as a second template argument at initialization

        - comparator functor class added as a second template parameter at 'std::set' construction - direct comparison of unique ptrs passed by reference - public friend operator< with all const params of reference type to const unique_ptr (with specialized 'std::less' with dereference by `->` or without specialized 'std::less' with defined public friend `operator<` with all const params of reference type to unique_ptr<Package> - see examples above)

            ```
            // main.cpp - changing only 'std::set' declaration leaving 'set::find' as is

            #include "PackageComparator.h"

            <~snip~>

            std::set<std::unique_ptr<Package>, PackageComparator> installedPackages;

            <~snip~>

            auto matchingPackage = installedPackages.find(packageWithInferredName);
            ```

            Define the separate custom comparator:

            ```
            // PackageComparator.h

            #pragma once

            #include "Package.h"

            struct PackageComparator {
                bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
                    // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
                    return onePackage < anotherPackage;
                }
            };
            ```

            Define comparator function for matching compared types for the class, that the element type in `std::set` is defined in:

            ```
            // Package.h

            friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name < anotherPackage->name;
            }
            ```

            Or overload the global `std::operator==` outside (e.g. under) the class definition:

            ```
            // Package.h
            namespace std {
                inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->getName() < anotherPackage->getName();
                }
            }
            ```

        - TODO comparator functor class - directly passed unique ptrs - dereferenced comparison with `*`

            Doesn't work with overloaded `std::operator<` as `inline bool operator<(const Package& onePackage, Package& anotherPackage)` - `error: no match for ‘operator<’ (operand types are ‘Package’ and ‘Package’)`

        - TODO comparator functor class - directly passed unique ptrs - dereferenced comparison with `->` with an accessor function

        - comparator lambda added as a second template parameter at 'std::set' construction - direct comparison of unique ptrs passed by reference - public friend operator< with all const params of reference type to const unique_ptr without specialized 'std::less'

                // main.cpp - changing 'std::set' declaration

                auto packageComparatorLambda = [](const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage < anotherPackage;
                };
                std::set<std::unique_ptr<Package>, decltype(packageComparatorLambda)> installedPackages(packageComparatorLambda);

          Define public friend `operator<` for the comparison of directly passed unique pointers

                // Package.h

                friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->name < anotherPackage->name;
                }

        - TODO comparator lambda - comparison of dereferenced unique pointers by `*` + public friend/member `operator<`
        - TODO comparator lambda - comparison of dereferenced unique pointers by `->`

        - comparator function added as a second template parameter at 'std::set' construction - direct comparison of unique ptrs passed by reference - public friend operator< with all const params of reference type to const unique_ptr without specialized 'std::less'

              // main.cpp - changing 'std::set' declaration

              std::set<std::unique_ptr<Package>, decltype(PackageComparatorFunction)*> installedPackages(comparePackages);
              //std::set<std::unique_ptr<Package>, decltype(PackageComparatorFunction)*> installedPackages; // C++20 and newer

          or

              std::set<std::unique_ptr<Package>, decltype(&PackageComparatorFunction)> installedPackages(&comparePackages);
              //std::set<std::unique_ptr<Package>, decltype(&PackageComparatorFunction)> installedPackages;  // C++20 and newer

          leaving 'set::find' as is

              auto matchingPackage = installedPackages.find(packageWithInferredName);

          Define comparator function outside of the class block, e. g. underneath as a non-member function at the end of the header file

              // Package.h

              inline bool comparePackages(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                  return onePackage < anotherPackage;
              }

          Then define public friend `operator<` for the direct unique pointer comparison

              // Package.h

              friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                  return onePackage->name < anotherPackage->name;
              }

        - TODO comparator function - comparison of dereferenced unique pointers by `*` + public friend/member `operator<`
        - TODO comparator function - comparison of dereferenced unique pointers by `->`

        - specialized 'std::less' with dereferenced comparison within by `->` - **without** public friend `operator<` - the standalone `std::less` function is enough to find a matching element in `std::set` that holds elements of custom type when lookup is performed with specialized `find` function `set::find` instead of other methods [O(log(n) vs O(n)]. The `std::less` functor relies on the public friend/member `operator<` of the another dereferenced type that are given as return values from the accessor methods.

                // Package.h

                // overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
                namespace std {
                    template<>
                    struct less<unique_ptr<Package>> {
                        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
                            return onePackage->getName() < anotherPackage->getName();
                        }
                    };
                }

    - specializing `std::less`
        - directly passed unique pointer to `set::find` with specialized 'std::less' with direct comparison

        - directly passed unique pointer to `set::find` with specialized 'std::less' with dereferenced comparison by '\*' **with** public friend/member operator< with specialized 'std::less' with dereferenced comparison by '\*'

            Specialize the `std::less` function with parameter types matching the type of elemenents the `std::set` holds

                // Package.h

                // overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
                namespace std {
                    template<>
                    struct less<unique_ptr<Package>> {
                        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
                            return *onePackage < *anotherPackage;
                        }
                    };
                }

            Choose one of the belowmentioned functions to overload `operator<` as friend or member function with parameter types matching the type of dereferenced smart pointer.

            All of the belowmentioned functions work for dereferenced comparison together with overloaded 'std::less' funcion for cutom type or with custom comparator without 'std::less' specialization

                // Package.h

                bool operator<(const Package& package) const {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                    return this->getName() < package.getName();
                //        return this->name < package.getName();
                //        return Package::name < package.getName();
                //        return this->name < package.name;
                //        return Package::name < package.name;
                }

                bool operator<(const Package& package) {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                //        return this->getName() < package.getName();
                    return this->name < package.getName();
                //        return Package::name < package.getName();
                //        return this->name < package.name;
                //        return Package::name < package.name;
                }

                bool operator<(Package& package) const {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                //        return this->getName() < package.getName();
                //        return this->name < package.getName();
                //        return Package::name < package.getName();
                    return this->name < package.name;
                //        return Package::name < package.name;
                }


                bool operator<(Package& package) {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                //        return this->getName() < package.getName();
                //        return this->name < package.getName();
                //        return Package::name < package.getName();
                //        return this->name < package.name;
                    return Package::name < package.name;
                }

                friend bool operator<(Package& onePackage, Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

        - TODO directly passed unique pointer to `set::find` with specialized 'std::less' with dereferened comparison with `->` and an accessor function

    - `std::integral_constant` as comparator

- `std::find`

        // main.cpp

        std::set<std::unique_ptr<Package>> installedPackages{};

    - **_passing unique pointer directly - direct comparison of unique pointers_** (allegedly slower that the specialized 'find' function 'std::set::find' - O(log(n)) vs O(n), and, in fact, the progrm intuitively takes longer, althogh I didn't measure it exactly).

        ```
        // main.cpp

        auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), packageWithInferredName);
        ```

        The mentioned `std::find` function finds the element by overloading the `operator==` as a public friend function with all params of refrence type to unique pointer to Package (or any element of custom type in general) (assuming the element with given key or element feature compared in the `operator==` function exist in the `std::set`)

        ```
        // Package.h

        //   WORKS for direct comparison in 'std::find', 'std::find_if', 'std::any_of'
        friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
            return onePackage->name == anotherPackage->name;
        }

        // Doesn't work
        friend bool operator==(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
            return onePackage->name == anotherPackage->name;
        }

        // Doesn't work
        bool operator==(const std::unique_ptr<Package>& package) const {
            // TODO maybe replace the 'getName()' function with only fields?
            return this->getName() == package->getName();
        //    return this->name == package.getName();
        //    return this->name < package.name;
        //    return Package::name == package.getName();
        //    return Package::name == package.name;
        }
        ```

        Or overload the global `std::operator==` outside (e.g. under) the class definition:

        ```
        // Package.h

        namespace std {
            inline bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
                return onePackage->getName() == anotherPackage.getName();
            }
        }
        ```

    - **_passing dereferenced unique pointer - comparison of unique pointer and the passed underlying instance the uniqe pointer holds_**

        ```
        // main.cpp

        auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), *packageWithInferredName);
        ```

        This version of `std::find` works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage)' in 'Package.h'

        ```
        // Package.h

        // WORKS only for 'std::find'
        friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
            return onePackage->name == anotherPackage.name;
        }

        // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
        //    friend bool operator==(std::unique_ptr<Package>& onePackage, Package& anotherPackage) {
        //        return onePackage->name == anotherPackage.name;
        //    }

        // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
        //    bool operator==(const Package& otherPackage) const {
        //        return this->name == otherPackage.name;
        //    }

        // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
        //    bool operator==(Package& otherPackage) {
        //        return this->name == otherPackage.name;
        //    }
        ```

        Or overload the global `std::operator==` outside (e.g. under) the class definition:

        ```
        // Package.h

        namespace std {
            inline bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
                return onePackage->getName() == anotherPackage.getName();
            }
        }
        ``

- `std::find_if`

    ```
    // main.cpp

    std::set<std::unique_ptr<Package>> installedPackages{};
    ```

    - `std::find_if` with lambda comparator
        - lambda with direct comparison

            ```
            // main.cpp

            auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return packageWithInferredName == currentInstalledPackage;
                    }
            );
            ```

            Works only with `friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)` in 'Package.h'

            ```
            // Package.h

            //   WORKS for direct comparison in 'std::find', 'std::find_if', 'std::any_of'
            friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name == anotherPackage->name;
            }
            ```

            Or overload the global `std::operator==` outside (e.g. under) the class definition:

            ```
            // Package.h

            namespace std {
                inline bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->getName() == anotherPackage->getName();
                }
            }
            ```

        - lambda with dereferenced comparison by `*`

            ```
            // main.cpp

            auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return *packageWithInferredName == *currentInstalledPackage;
                    }
            );
            ```

            Works only with any of the `operator==` mentioned below

            ```
            // Package.h

            friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(Package& onePackage, const Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(Package& onePackage, Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            bool operator==(const Package& otherPackage) const {
                return this->name == otherPackage.name;
            }

            bool operator==(const Package& otherPackage) {
                return this->name == otherPackage.name;
            }

            bool operator==(Package& otherPackage) const {
                return this->name == otherPackage.name;
            }

            bool operator==(Package& otherPackage) {
                return this->name == otherPackage.name;
            }
            ```

            Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, const Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘Package’ and ‘Package’)`

        - lambda with dereferenced comparison by `->` - delegating comparison

            ```
            // main.cpp

            auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return packageWithInferredName->getName() == currentInstalledPackage->getName();
                    }
            );
            ```

        - passing dereferenced unique pointer to lambda in `std::find_if` - **COULDN'T MAKE IT WORK**

            ```
            auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const Package& currentInstalledPackage) {
                        return packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
            //            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
            //            return packageWithInferredName->getName() == currentInstalledPackage->getName();
                    }
            );
            ```

            - `error: binding reference of type ‘Package&’ to ‘const Package’ discards qualifiers` when a value from first function is declared as `const` and this value is passed through a parameter to another function which take the same parameter type without const
            - when forgetting to add `const`ness of the second parameter of public friend `operator==` function or of first parameer of public member `operator==` function
            - or
            - `error: no match for call to ‘(main()::<lambda(const Package&)>) (const std::unique_ptr<Package>&)’`
            - `note:   no known conversion for argument 1 from ‘const std::unique_ptr<Package>’ to ‘const Package&’`
            - Possible solution would be to define an implicit?/(explicit? casted to `Package&` with `static_cast?`) conversion constructor `Package::Package(const std::unique_ptr<Package> uniquePtr)` but that would only slow things down and make the code less readable and expressive. Let's keep it simple ;)

    - `std::find_if` with comparator predicate

        ```
        // main.cpp

        #include "PackageComparatorPredicate.h"
        ```

        - passing unique pointer to comparator predicate directly

            ```
            // main.cpp

            auto matchingPackage = std::find_if(
                    installedPackages.begin(),
                    installedPackages.end(),
                    PackageComparatorPredicate(packageWithInferredName));
            ```

            - comparator predicate with direct comparison

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package == otherPackage);
                    }
                };

                ```

                For direct comparison of unique pointers is effective only function that overloads the `operator==` as public friend function with all `const` params (public friend `operator==` without const parameters and any of the public member function for `operator==` don't find anything):

                ```
                // Package.h

                friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->name == anotherPackage->name;
                }
                ```

                Or overload the global `std::operator==` outside (e.g. under) the class definition:

                ```
                // Package.h

                namespace std {
                    inline bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return onePackage->getName() == anotherPackage->getName();
                    }
                }
                ```

            - comparator predicate with dereferenced comparison by `*`

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (*(this->package) == *otherPackage);
                    }
                };

                ```

                For comparison of dereferenced unique pointers use one of the belowmentioned functions to overloading `operator==`

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                bool operator==(const Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(const Package& otherPackage) {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) {
                    return this->name == otherPackage.name;
                }
                ```

                Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, const Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘Package’ and ‘Package’)`

            - comparator predicate with dereferenced comparison by `->` - delegating comparison to the return type of the function we're sending the message to, so we need to overload `operator==` in the class that the type is defined in or that the type refers to. For the concrete implementations of overloaded `operator==` for particular types see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package->getName() == otherPackage->getName());
                    }
                };

                ```

        - passing dereferenced unique pointer to comparator predicate

            ```
            // main.cpp

            auto matchingPackage = std::find_if(
                    installedPackages.begin(),
                    installedPackages.end(),
                    PackageComparatorPredicate(*packageWithInferredName));
            ```

            - comparator predicate with direct comparison - comparison of dereferenced member variable (instance with features to search by) with smart pointer

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return this->package == otherPackage;
                    }
                };
                ```

                The overloaded `operator==` needs to be implemented as `const` function with all `const` operands (parameters)

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->name == anotherPackage.name;
                }

                bool operator==(const std::unique_ptr<Package>& otherPackage) const {
                    return this->name == otherPackage->name;
                }
                ```

                Or overload the global `std::operator==` outside (e.g. under) the class definition:

                ```
                // Package.h

                namespace std {
                    inline bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return onePackage->getName() == anotherPackage.getName();
                    }
                }
                ```

            - comparator predicate with comparison of member variable of reference type (instance with features to search by) with dereferenced smart pointer

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package == *otherPackage);
                    }
                };
                ```

                The comparator predicate in this case uses the `operator==` with operand types `const Package` and `Package`. When `const` is added to the second parameter of the friend `operator==` overload as well (or the single parameter from member overload function), everything works as intended. Problems occured during compilation, when I ommitted the `const` qualifier for mentioned operands at mentioned position which produces an error `error: binding reference of type ‘Package&’ to ‘const Package’ discards qualifiers`

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                bool operator==(const Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }
                ```

                Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘Package’)`

            - comparator predicate with comparison of member variable of reference type (instance with features to search by) with smart pointer - both dereferenced to call an accessor function (`->` for unique pointer, `.` for reference) to delegating the comparison from the original reference type to the type returned by the accessor function. The returned type needs to have the `operator==` overloaded instead of the original type. For the concrete implementations of overloaded `operator==` for particular use cases see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package.getName() == otherPackage->getName());
                    }
                };
                ```

            - comparator predicate with dereferenced comparison by `->` - delegating comparison to the return type of the function we're sending the message to, so we need to overload `operator==` in the class that the type is defined in or that the type refers to. For the concrete implementations of overloaded `operator==` for particular types see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package->getName() == otherPackage->getName());
                    }
                };
                ```

- `std::any_of`

    ```
    // main.cpp

    std::set<std::unique_ptr<Package>> installedPackages{};
    ```

    - `std::any_of` with lambda comparator
        - lambda with direct comparison

            ```
            // main.cpp

            bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return packageWithInferredName == currentInstalledPackage;
                    }
            );
            ```

            Works only with `friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)` in 'Package.h'

            ```
            // Package.h

            //   WORKS for direct comparison in 'std::find', 'std::any_of', 'std::any_of'
            friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name == anotherPackage->name;
            }
            ```

            Or overload the global `std::operator==` outside (e.g. under) the class definition:

            ```
            // Package.h

            namespace std {
                inline bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->getName() == anotherPackage->getName();
                }
            }
            ```

        - lambda with dereferenced comparison by `*`

            ```
            // main.cpp

            bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return *packageWithInferredName == *currentInstalledPackage;
                    }
            );
            ```

            Works only with any of the `operator==` mentioned below

            ```
            // Package.h

            friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(Package& onePackage, const Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            friend bool operator==(Package& onePackage, Package& anotherPackage) {
                return onePackage.name == anotherPackage.name;
            }

            bool operator==(const Package& otherPackage) const {
                return this->name == otherPackage.name;
            }

            bool operator==(const Package& otherPackage) {
                return this->name == otherPackage.name;
            }

            bool operator==(Package& otherPackage) const {
                return this->name == otherPackage.name;
            }

            bool operator==(Package& otherPackage) {
                return this->name == otherPackage.name;
            }
            ```

            Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, const Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘Package’ and ‘Package’)`

        - lambda with dereferenced comparison by `->` - delegating comparison

            ```
            // main.cpp

            bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
                        return packageWithInferredName->getName() == currentInstalledPackage->getName();
                    }
            );
            ```

        - passing dereferenced unique pointer to lambda in `std::any_of` - **COULDN'T MAKE IT WORK**

            ```
            bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
                    [&packageWithInferredName](const Package& currentInstalledPackage) {
                        return packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
            //            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
            //            return packageWithInferredName->getName() == currentInstalledPackage->getName();
                    }
            );
            ```

            - `error: binding reference of type ‘Package&’ to ‘const Package’ discards qualifiers` when a value from first function is declared as `const` and this value is passed through a parameter to another function which take the same parameter type without const
            - when forgetting to add `const`ness of the second parameter of public friend `operator==` function or of first parameer of public member `operator==` function
            - or
            - `error: no match for call to ‘(main()::<lambda(const Package&)>) (const std::unique_ptr<Package>&)’`
            - `note:   no known conversion for argument 1 from ‘const std::unique_ptr<Package>’ to ‘const Package&’`
            - Possible solution would be to define an implicit?/(explicit? casted to `Package&` with `static_cast?`) conversion constructor `Package::Package(const std::unique_ptr<Package> uniquePtr)` but that would only slow things down and make the code less readable and expressive. Let's keep it simple ;)

    - `std::any_of` with comparator predicate

        ```
        // main.cpp

        #include "PackageComparatorPredicate.h"
        ```

        - passing unique pointer to comparator predicate directly

            ```
            // main.cpp

            bool isPackageWithInferredNameFoundAsTest = std::any_of(
                    installedPackages.begin(),
                    installedPackages.end(),
                    PackageComparatorPredicate(packageWithInferredName));
            ```

            - comparator predicate with direct comparison

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package == otherPackage);
                    }
                };

                ```

                For direct comparison of unique pointers is effective only function that overloads the `operator==` as public friend function with all `const` params (public friend `operator==` without const parameters and any of the public member function for `operator==` don't find anything):

                ```
                // Package.h

                friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->name == anotherPackage->name;
                }
                ```

                Or overload the global `std::operator==` outside (e.g. under) the class definition:

                ```
                // Package.h

                namespace std {
                    inline bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return onePackage->getName() == anotherPackage->getName();
                    }
                }
                ```

            - comparator predicate with dereferenced comparison by `*`

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (*(this->package) == *otherPackage);
                    }
                };

                ```

                For comparison of dereferenced unique pointers use one of the belowmentioned functions to overloading `operator==`

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                bool operator==(const Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(const Package& otherPackage) {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) {
                    return this->name == otherPackage.name;
                }
                ```

                Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, const Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘Package’ and ‘Package’)`

            - comparator predicate with dereferenced comparison by `->` - delegating comparison to the return type of the function we're sending the message to, so we need to overload `operator==` in the class that the type is defined in or that the type refers to. For the concrete implementations of overloaded `operator==` for particular types see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package->getName() == otherPackage->getName());
                    }
                };

                ```

        - passing dereferenced unique pointer to comparator predicate

            ```
            // main.cpp

            bool isPackageWithInferredNameFoundAsTest = std::any_of(
                    installedPackages.begin(),
                    installedPackages.end(),
                    PackageComparatorPredicate(*packageWithInferredName));
            ```

            - comparator predicate with direct comparison - comparison of dereferenced member variable (instance with features to search by) with smart pointer

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return this->package == otherPackage;
                    }
                };
                ```

                The overloaded `operator==` needs to be implemented as `const` function with all `const` operands (parameters)

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                    return onePackage->name == anotherPackage.name;
                }

                bool operator==(const std::unique_ptr<Package>& otherPackage) const {
                    return this->name == otherPackage->name;
                }
                ```

                Or overload the global `std::operator==` outside (e.g. under) the class definition:

                ```
                // Package.h

                namespace std {
                    inline bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return onePackage->getName() == anotherPackage.getName();
                    }
                }
                ```

            - comparator predicate with comparison of member variable of reference type (instance with features to search by) with dereferenced smart pointer

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package == *otherPackage);
                    }
                };
                ```

                The comparator predicate in this case uses the `operator==` with operand types `const Package` and `Package`. When `const` is added to the second parameter of the friend `operator==` overload as well (or the single parameter from member overload function), everything works as intended. Problems occured during compilation, when I ommitted the `const` qualifier for mentioned operands at mentioned position which produces an error `error: binding reference of type ‘Package&’ to ‘const Package’ discards qualifiers`

                ```
                // Package.h

                friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                friend bool operator==(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name == anotherPackage.name;
                }

                bool operator==(const Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }

                bool operator==(Package& otherPackage) const {
                    return this->name == otherPackage.name;
                }
                ```

                Doesn't work with overloaded `std::operator==` as `inline bool operator==(const Package& onePackage, Package& anotherPackage)` - `error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘Package’)`

            - comparator predicate with comparison of member variable of reference type (instance with features to search by) with smart pointer - both dereferenced to call an accessor function (`->` for unique pointer, `.` for reference) to delegating the comparison from the original reference type to the type returned by the accessor function. The returned type needs to have the `operator==` overloaded instead of the original type. For the concrete implementations of overloaded `operator==` for particular use cases see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const Package& package;

                    explicit PackageComparatorPredicate(const Package& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package.getName() == otherPackage->getName());
                    }
                };
                ```

            - comparator predicate with dereferenced comparison by `->` - delegating comparison to the return type of the function we're sending the message to, so we need to overload `operator==` in the class that the type is defined in or that the type refers to. For the concrete implementations of overloaded `operator==` for particular types see mentioned examples in this section.

                ```
                // PackageComparatorPredicate.h

                #pragma once

                #include "Package.h"

                struct PackageComparatorPredicate {
                    const std::unique_ptr<Package>& package;

                    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
                            package(packageToFind)
                    {}

                    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
                        return (this->package->getName() == otherPackage->getName());
                    }
                };
                ```

- `std::binary_search`
    - `std::binary_search` with `std::set` inicialized without custom comparator as a second template parameter - using the default `std::less` and `operator<` for comparing and sorting elements of custom type

    ```
    // main.cpp

    std::set<std::unique_ptr<Package>> installedPackages{};
    ```

    - directly passing unique pointer to binary search

        ```
        // main.cpp

        bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName);
        ```

        - public friend `operator<` with all const args

            Works even with `installedPackages` set initialized as `std::set<std::unique_ptr<Package>> installedPackages{};` with only default comparator (without custom comparator). Binary search works only on ordered datastructures. The order is attained by the overloaded `operator<` as a public friend function with all const params, which gets called at insertion to the `std::set` - `emplace()`/`push_back()`. That`s why the lookup actually finds a matching element of custom type in `std::set`

            ```
            // Package.h

            friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name < anotherPackage->name;
            }
            ```

        - overloaded `std::operator<` - globally overloaded `operator<`

            According to `Clang-Tidy` "Modification of 'std' namespace can result in undefined behavior", so going to test it out.

            - overloaded `std::operator<` - globally overloaded `operator<` - with dereferenced comparison by `->` and an accessor function

                ```
                // Package.h

                namespace std {
                    inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return onePackage->getName() < anotherPackage->getName();
                    }
                }
                ```

            - overloaded `std::operator<` - globally overloaded `operator<` - with dereferenced comparison by `*`

                ```
                // Package.h

                namespace std {
                    inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                        return *onePackage < *anotherPackage;
                    }
                }
                ```

                Choose one of the functions for overloading `operator<` from below:

                ```
                // Package.h

                bool operator<(const Package& package) const {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                    return this->getName() < package.getName();
                //    return this->name < package.getName();
                //    return Package::name < package.getName();
                //    return this->name < package.name;
                //    return Package::name < package.name;
                }

                bool operator<(const Package& package) {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                    return this->getName() < package.getName();
                    return this->name < package.getName();
                    return Package::name < package.getName();
                    return this->name < package.name;
                    return Package::name < package.name;
                }

                bool operator<(Package& package) const {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                //    return this->getName() < package.getName();
                //    return this->name < package.getName();
                //    return Package::name < package.getName();
                    return this->name < package.name;
                //    return Package::name < package.name;
                }

                bool operator<(Package& package) {
                    // TODO maybe replace the 'getName()' function with only fields?

                    assert(this->getName() == this->name);
                    assert(this->getName() == Package::name);
                    assert(this->name == Package::name);

                //    return this->getName() < package.getName();
                //    return this->n- Detecting home directory path
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+home+directory&ia=web
    - How do you get there home directory - https://cplusplus.com/forum/beginner/24235/
    - https://stackoverflow.com/questions/42696260/how-to-get-user-home-directory-on-windows
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+home+directory+linux&ia=web
    - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
    - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux#comment10000952_2910392
    - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux#comment64750158_2910377
    - https://stackoverflow.com/questions/2910377/get-home-directory-in-linux/2910392#2910392
    - https://duckduckgo.com/?q=c%2B%2B+getuid+geteuid+sudo&t=ffab&ia=web
    - https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-geteuid-get-effective-user-id
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+get+user+behind+sudo&ia=web
    - https://stackoverflow.com/questions/10272784/how-do-i-get-the-users-real-uid-if-the-program-is-run-with-sudo
    - my own intuition: adding `-laudit` to the linker options because of compile/linking time error "undefined reference to 'audit_getloginuid()'"
- Converting current time to human readable format
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+date+time+timestamp&ia=web
    - with `ctime` header - C-style: https://www.softwaretestinghelp.com/date-and-time-in-cpp/
    - with `chrono` header - C++ style (preferred): https://stackoverflow.com/questions/58603446/convert-timestamp-to-the-formatted-date-time-using-c
    - https://duckduckgo.com/?q=c%2B%2B+chrono+date+time+timestamp&t=ffab&ia=web
    - https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchronoame < package.getName();
                //    return Package::name < package.getName();
                //    return this->name < package.name;
                    return Package::name < package.name;
                }

                friend bool operator<(Package& onePackage, Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(const Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(const Package& onePackage, Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }

                friend bool operator<(Package& onePackage, const Package& anotherPackage) {
                    return onePackage.name < anotherPackage.name;
                }
                ```

        - public friend `operator<` with all const params of reference type to const `unique_ptr` with dereferenced comparison by `->` within the operator function **with** specialized `std::less` (**unnecessary** to specialize 'std::less' - the public friend `operator<` in mentioned format is enough to ensure ordering of elements in the `std::set`)

            Standalone specialized `std:less` functor doesn't find an element of custom type for `std::binary_search`, even though the functor is specialized for the custom type the `std::set` holds.

            ```
            // Package.h

            friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name < anotherPackage->name;
            }

            <~snip~>

            namespace std {
                template<>
                struct less<unique_ptr<Package>> {
                    bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
                        return onePackage < anotherPackage;
                    }
                };
            }
            ```

        - specializing `std::less` with dereferenced comparison - **COULDN'T MAKE IT WORK** with `operator<` in any version

            ```
            // Package.h

            bool operator<(const Package& package) const {
                // TODO maybe replace the 'getName()' function with only fields?

                assert(this->getName() == this->name);
                assert(this->getName() == Package::name);
                assert(this->name == Package::name);

                return this->getName() < package.getName();
            //    return this->name < package.getName();
            //    return Package::name < package.getName();
            //    return this->name < package.name;
            //    return Package::name < package.name;
            }

            bool operator<(const Package& package) {
                // TODO maybe replace the 'getName()' function with only fields?

                assert(this->getName() == this->name);
                assert(this->getName() == Package::name);
                assert(this->name == Package::name);

                return this->getName() < package.getName();
                return this->name < package.getName();
                return Package::name < package.getName();
                return this->name < package.name;
                return Package::name < package.name;
            }

            bool operator<(Package& package) const {
                // TODO maybe replace the 'getName()' function with only fields?

                assert(this->getName() == this->name);
                assert(this->getName() == Package::name);
                assert(this->name == Package::name);

            //    return this->getName() < package.getName();
            //    return this->name < package.getName();
            //    return Package::name < package.getName();
                return this->name < package.name;
            //    return Package::name < package.name;
            }

            bool operator<(Package& package) {
                // TODO maybe replace the 'getName()' function with only fields?

                assert(this->getName() == this->name);
                assert(this->getName() == Package::name);
                assert(this->name == Package::name);

            //    return this->getName() < package.getName();
            //    return this->name < package.getName();
            //    return Package::name < package.getName();
            //    return this->name < package.name;
                return Package::name < package.name;
            }

            friend bool operator<(Package& onePackage, Package& anotherPackage) {
                return onePackage.name < anotherPackage.name;
            }

            friend bool operator<(const Package& onePackage, const Package& anotherPackage) {
                return onePackage.name < anotherPackage.name;
            }

            friend bool operator<(const Package& onePackage, Package& anotherPackage) {
                return onePackage.name < anotherPackage.name;
            }

            friend bool operator<(Package& onePackage, const Package& anotherPackage) {
                return onePackage.name < anotherPackage.name;
            }

            <~snip~>

            namespace std {
                template<>
                struct less<unique_ptr<Package>> {
                    bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
                        return *onePackage < *anotherPackage;
                    }
                };
            }
            ```

    - directly passing unique pointer to binary search with custom comparator

        ```
        // main.cpp

        #include "PackageComparator.h"

        <~snip~>

        bool packageWithInferredNameIsMissingAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName, PackageComparator());
        ```

        - direct comparison of unique pointers in comparator

            ```
            #pragma once

            #include "Package.h"

            struct PackageComparator {
                bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
                    // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
                    return onePackage < anotherPackage;
                }
            };
            ```

            ```
            // Package.h

            friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
                return onePackage->name < anotherPackage->name;
            }
            ```

        - **comparison of dereferenced unique pointers in comparator by `*` and `->` with an accessor method didn't work with any version of overloaded `operator<`. Maybe because of the mystical `std::binary_search` `ForwardIterator` limitation?** See examples above.

    - passing dereferenced unique pointer to binary search - **COULDN'T MAKE IT WORK** with `operator<` in both required versions

        ```
        // main.cpp

        bool packageWithInferredNameIsMissingAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName);
        ```

        ```
        // Package.h

        // for 'stl_algo.h'
        friend bool operator<(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
            return onePackage.name < anotherPackage->name;
        }

        // for 'predefined_ops.h'
        friend bool operator<(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
            return onePackage->name < anotherPackage.name;
        }
        ```

    - passing dereferenced unique pointer to binary search with custom comparator - **COULDN'T MAKE IT WORK** for any version of comparison in comparator with any version of overloaded `operator<`

        ```
        // main.cpp

        #include "PackageComparator.h"

        <~snip~>

        bool packageWithInferredNameIsMissingAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName, PackageComparator())
        ```

        ```
        // PackageComparator.h

        #pragma once

        #include "Package.h"

        //struct PackageComparator {
        //    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
        //        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
        ////        return onePackage < anotherPackage;
        //
        //        // DEREFERENCED COMPARISON without accessor method - works with
        //        //  - 'bool operator<(const Package& package) const'
        //        //  - 'bool operator<(Package& package)'
        //        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
        ////        return *onePackage < *anotherPackage;
        //
        //        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
        //        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
        //        //  Works out of the box for 'std::string'.
        //        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
        //        return onePackage->getName() < anotherPackage->getName();
        //    }
        //};

        struct PackageComparator {
            bool operator()(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
                // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
                return onePackage < anotherPackage;

                // DEREFERENCED COMPARISON without accessor method - works with
                //  - 'bool operator<(const Package& package) const'
                //  - 'bool operator<(Package& package)'
                //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
        //        return onePackage < *anotherPackage;

                // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
                //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
                //  Works out of the box for 'std::string'.
                //  For custom type at least public friend 'operator<' with all const params needs to be implemented
        //        return onePackage.getName() < anotherPackage->getName();
            }

            bool operator()(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) const {
                // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
                return onePackage < anotherPackage;

                // DEREFERENCED COMPARISON without accessor method - works with
                //  - 'bool operator<(const Package& package) const'
                //  - 'bool operator<(Package& package)'
                //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
        //        return *onePackage < anotherPackage;

                // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
                //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
                //  Works out of the box for 'std::string'.
                //  For custom type at least public friend 'operator<' with all const params needs to be implemented
        //        return onePackage->getName() < anotherPackage.getName();
            }
        };

        ```

        ```
        // Package.h

        // FOR DIRECT COMPARISON
            // for 'stl_algo.h'
        //    friend bool operator<(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
        //        return onePackage.name < anotherPackage->name;
        //    }
        //
        //    // for 'predefined_ops.h'
        //    friend bool operator<(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
        //        return onePackage->name < anotherPackage.name;
        //    }

        // FOR DEREFERENCED COMPARISON without accessor method
        //    bool operator<(const Package& package) const {
        //        // TODO maybe replace the 'getName()' function with only fields?
        //
        //        assert(this->getName() == this->name);
        //        assert(this->getName() == Package::name);
        //        assert(this->name == Package::name);
        //
        //        return this->getName() < package.getName();
        ////        return this->name < package.getName();
        ////        return Package::name < package.getName();
        ////        return this->name < package.name;
        ////        return Package::name < package.name;
        //    }

        //    bool operator<(const Package& package) {
        //        // TODO maybe replace the 'getName()' function with only fields?
        //
        //        assert(this->getName() == this->name);
        //        assert(this->getName() == Package::name);
        //        assert(this->name == Package::name);
        //
        ////        return this->getName() < package.getName();
        //        return this->name < package.getName();
        ////        return Package::name < package.getName();
        ////        return this->name < package.name;
        ////        return Package::name < package.name;
        //    }

        //    bool operator<(Package& package) const {
        //        // TODO maybe replace the 'getName()' function with only fields?
        //
        //        assert(this->getName() == this->name);
        //        assert(this->getName() == Package::name);
        //        assert(this->name == Package::name);
        //
        ////        return this->getName() < package.getName();
        ////        return this->name < package.getName();
        ////        return Package::name < package.getName();
        //        return this->name < package.name;
        ////        return Package::name < package.name;
        //    }

        //    bool operator<(Package& package) {
        //        // TODO maybe replace the 'getName()' function with only fields?
        //
        //        assert(this->getName() == this->name);
        //        assert(this->getName() == Package::name);
        //        assert(this->name == Package::name);
        //
        ////        return this->getName() < package.getName();
        ////        return this->name < package.getName();
        ////        return Package::name < package.getName();
        ////        return this->name < package.name;
        //        return Package::name < package.name;
        //    }

        //    friend bool operator<(Package& onePackage, Package& anotherPackage) {
        //        return onePackage.name < anotherPackage.name;
        //    }

        //    friend bool operator<(const Package& onePackage, const Package& anotherPackage) {
        //        return onePackage.name < anotherPackage.name;
        //    }

        //    friend bool operator<(const Package& onePackage, Package& anotherPackage) {
        //        return onePackage.name < anotherPackage.name;
        //    }

        //    friend bool operator<(Package& onePackage, const Package& anotherPackage) {
        //        return onePackage.name < anotherPackage.name;
        //    }
        ```

    - `std::binary_search` with `std::set` inicialized with custom comparator as a second template parameter - overloading the default `std::less` and `operator<` for comparing and sorting elements of custom type

        ```
        std::set<std::unique_ptr<Package>> installedPackages{}; // WORKS - with at least overloaded public friend 'operator<' with all const params of reference type to constant unique_ptr to Package

        //std::set<std::unique_ptr<Package, PackageComparator>> installedPackages; // doesn't work - using PackageComparator as a second template argument for 'unique_ptr' as default deleter instead of using it as a second template argument for 'set' as a comparator
        //std::set<std::unique_ptr<Package>, PackageComparator> installedPackages; // WORKS - thanks https://www.codegrepper.com/code-examples/cpp/c%2B%2B+custom+comparator+for+elements+in+set

        //auto packageComparator = std::make_unique<PackageComparator>();
        //std::set<std::unique_ptr<Package>, PackageComparator> installedPackages(*packageComparator); // WORKS

        //auto packageComparatorLambda = [](const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
        //    return onePackage < anotherPackage;
        //};
        //std::set<std::unique_ptr<Package>, decltype(packageComparatorLambda)> installedPackages(packageComparatorLambda); // WORKS

        //std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages(comparePackages); // WORKS
        //std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer
        //std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages(&comparePackages); // WORKS
        //std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer
        ```

        **It doesn't matter which one of the initializations for `std::set` I use, the `std::binary_search` doesn't find anything.**

        **`std::binary_search` for `std::set` works only when the class, that the type of elements in `std::set` are defined in, has overloaded `operator<` as a public friend function with all const params of the same type that the elements the `std::set` holds**
        **or with a globally overloaded `std::operator<` with the same mentioned parameter list**

**Entire code excerpts**

```
main.cpp ('std::set' initialization with optional comparator initialization excerpts + finding a package)

<~snip~>

    std::set<std::unique_ptr<Package>> installedPackages{};

    std::set<std::unique_ptr<Package, PackageComparator>> installedPackages; // doesn't work - using PackageComparator as a second template argument for 'unique_ptr' as default deleter instead of using it as a second template argument for 'set' as a comparator
    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages; // WORKS - thanks https://www.codegrepper.com/code-examples/cpp/c%2B%2B+custom+comparator+for+elements+in+set

    auto packageComparator = std::make_unique<PackageComparator>();
    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages(*packageComparator); // WORKS

    auto packageComparatorLambda = [](const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
        return onePackage < anotherPackage;
    };
    std::set<std::unique_ptr<Package>, decltype(packageComparatorLambda)> installedPackages(packageComparatorLambda); // WORKS

    std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages(comparePackages); // WORKS
    std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer
    std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages(&comparePackages); // WORKS
    std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer

    auto packageComparator = std::make_unique<PackageComparator>();
    std::set<std::unique_ptr<Package>> installedPackages(*packageComparator); // still doesn't work

<~snip~>

            std::string inferredPackageNameAsText = packageNameAndVersion;
            auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageNameAsText));

            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // search for the matching package element in the 'installedPackages' by 'packageWithInferredName'
                // 'set::find'
                auto matchingPackage = installedPackages.find(packageWithInferredName);

                // 'std::find' (direct and dereferenced comparison in 'operator==' in class for custom element type)
//                auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), packageWithInferredName);  // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
//                auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), *packageWithInferredName); // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage)' in 'Package.h'

                // 'std::find_if' with lambda (passing directly) - (direct and dereferenced comparison in lambda)
//                auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
//                        [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
//                            return packageWithInferredName == currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' and 'operator==' const member functions in 'Package.h'
////                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // Couldn't make it work
//                auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
//                        [&packageWithInferredName](const Package& currentInstalledPackage) {
//                            return packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
////                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // 'std::find_if' with comparator predicate (directly pass to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                auto matchingPackage = std::find_if(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(packageWithInferredName));

                // 'std::find_if' with comparator predicate (pass dereferenced unique ptr to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                auto matchingPackage = std::find_if(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(*packageWithInferredName));

                // 'std::any_of' with lambda (direct and dereferenced comparison in lambda)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
//                                                                        [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
////                            return packageWithInferredName == currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
//                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // 'std::any_of' with comparator predicate (directly pass to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(packageWithInferredName));

                // 'std::any_of' with comparator predicate (pass dereferenced unique ptr to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(*packageWithInferredName));

                // 'std::binary_search'
                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName);  // WORKS with overloaded public friend 'operator<' with all const args and even with 'installedPackages' set initialized as 'std::set<std::unique_ptr<Package>> installedPackages{};' with only default comparator. Binary search works only on ordered datastructures. The order is attained by the overloaded 'operator<' as a public friend function with all const params, which gets called at insertion to the 'std::set' - `emplace()`/`push_back()`. That's why the lookup actually finds a matching element of custom type in 'std::set'
//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName, PackageComparator());

//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName);
//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName, PackageComparator());

                // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                //  trying to show the values for STL containers and smartpointer instances.
                //  Instead it shows an error message saying "Cannot instantiate printer for default visualizer"
//                std::cout << *packageWithInferredName << "\n";

                // if key was NOT found, strip the coumpound package key by one character - or word  from the end and perform lookup again
                // comment out for 'std::any_of' and 'std::binary_search'
                bool packageWithInferredNameIsMissing = matchingPackage == installedPackages.end();

                bool packageWithInferredNameIsMissingAsTest = !isPackageWithInferredNameFoundAsTest;
                assert(packageWithInferredNameIsMissing == packageWithInferredNameIsMissingAsTest);

<~snip~>
```

```
PackageComparator.h

#pragma once

#include "Package.h"

struct PackageComparator {
    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
        return onePackage < anotherPackage;

        // DEREFERENCED COMPARISON without accessor method - works with
        //  - 'bool operator<(const Package& package) const'
        //  - 'bool operator<(Package& package)'
        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
//        return *onePackage < *anotherPackage;

        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
        //  Works out of the box for 'std::string'.
        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
//        return onePackage->getName() < anotherPackage->getName();
    }
};

//struct PackageComparator {
//    bool operator()(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
//        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
////        return onePackage < anotherPackage;
//
//        // DEREFERENCED COMPARISON without accessor method - works with
//        //  - 'bool operator<(const Package& package) const'
//        //  - 'bool operator<(Package& package)'
//        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
////        return onePackage < *anotherPackage;
//
//        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
//        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
//        //  Works out of the box for 'std::string'.
//        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
//        return onePackage.getName() < anotherPackage->getName();
//    }
//
//    bool operator()(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) const {
//        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
////        return onePackage < anotherPackage;
//
//        // DEREFERENCED COMPARISON without accessor method - works with
//        //  - 'bool operator<(const Package& package) const'
//        //  - 'bool operator<(Package& package)'
//        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
////        return *onePackage < anotherPackage;
//
//        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
//        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
//        //  Works out of the box for 'std::string'.
//        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
//        return onePackage->getName() < anotherPackage.getName();
//    }
//};


```

```
Package.h ('operator<' overloads, 'std::less' specializations and 'operator==' overloads)

<~snip~>

// MEMBER OPERATORS

// FOR DIRECT (SMART) POINTER COMPARISON FOR 'SET::FIND'
//  Functions in this section work for dereferenced comparison together with overloaded 'std::less' funcion for cutom type
//  or with custom comparator without 'std::less' overload
//  except the
//  'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
//  which works with and without 'std::less' overload

    // WORKS for direct comparison with and without overloading 'std::less' funcion
//    friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name < anotherPackage->name;
//    }

    // Doesn't work - compiles but fails to find the searched for element at runtime when used as a standalone function
//    friend bool operator<(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name < anotherPackage->name;
//    }

    // Doesn't work - compiles but fails to find the searched for element at runtime
//    bool operator<(const std::unique_ptr<Package>& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//
//        assert(this->getName() == this->name);
//        assert(this->getName() == Package::name);
//        assert(this->name == Package::name);
//
//        return this->getName() < package->getName();
//        //        return this->name < package->getName();
//        //        return Package::name < package->getName();
//        //        return this->name < package->name;
//        //        return Package::name < package->name;
//    }

// FOR DIRECT (SMART) POINTER COMPARISON FOR 'STD::FIND', 'STD::FIND_IF' (lambda or predicate comparator), 'STD::ANY_OF' (lambda or predicate comparator)

//   WORKS for direct comparison in 'std::find', 'std::find_if', 'std::any_of'
//    friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name == anotherPackage->name;
//    }

    // Doesn't work
//    friend bool operator==(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name == anotherPackage->name;
//    }

    // Doesn't work
//    bool operator==(const std::unique_ptr<Package>& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//        return this->getName() == package->getName();
////        return this->name == package.getName();
////        return this->name < package.name;
////        return Package::name == package.getName();
////        return Package::name == package.name;
//    }

// ------------------------------------------------------------------------------------------------
// ================================================================================================
// ------------------------------------------------------------------------------------------------

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR 'SET::FIND'
//  Functions in this section work for dereferenced comparison together with overloaded 'std::less' funcion for cutom type
//  or with custom comparator without 'std::less' overload,
//  but I couldn't make the element lookup work for 'std::binary_search'

//    bool operator<(const Package& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//
//        assert(this->getName() == this->name);
//        assert(this->getName() == Package::name);
//        assert(this->name == Package::name);
//
//        return this->getName() < package.getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
////        return this->name < package.name;
////        return Package::name < package.name;
//    }

//    bool operator<(const Package& package) {
//        // TODO maybe replace the 'getName()' function with only fields?
//
//        assert(this->getName() == this->name);
//        assert(this->getName() == Package::name);
//        assert(this->name == Package::name);
//
////        return this->getName() < package.getName();
//        return this->name < package.getName();
////        return Package::name < package.getName();
////        return this->name < package.name;
////        return Package::name < package.name;
//    }

//    bool operator<(Package& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//
//        assert(this->getName() == this->name);
//        assert(this->getName() == Package::name);
//        assert(this->name == Package::name);
//
////        return this->getName() < package.getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
//        return this->name < package.name;
////        return Package::name < package.name;
//    }

//    bool operator<(Package& package) {
//        // TODO maybe replace the 'getName()' function with only fields?
//
//        assert(this->getName() == this->name);
//        assert(this->getName() == Package::name);
//        assert(this->name == Package::name);
//
////        return this->getName() < package.getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
////        return this->name < package.name;
//        return Package::name < package.name;
//    }

//    friend bool operator<(Package& onePackage, Package& anotherPackage) {
//        return onePackage.name < anotherPackage.name;
//    }

//    friend bool operator<(const Package& onePackage, const Package& anotherPackage) {
//        return onePackage.name < anotherPackage.name;
//    }

//    friend bool operator<(const Package& onePackage, Package& anotherPackage) {
//        return onePackage.name < anotherPackage.name;
//    }

//    friend bool operator<(Package& onePackage, const Package& anotherPackage) {
//        return onePackage.name < anotherPackage.name;
//    }

// FOR 'STD::BINARY_SEARCH' WITH PASSED DEREFERENCED SMART POINTER - COULDN'T MAKE IT WORK

    // for 'stl_algo.h'
//    friend bool operator<(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage.name < anotherPackage->name;
//    }
//
//    // for 'predefined_ops.h'
//    friend bool operator<(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
//        return onePackage->name < anotherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR 'STD::FIND'

    // WORKS only for 'std::find'
//    friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    friend bool operator==(std::unique_ptr<Package>& onePackage, Package& anotherPackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    bool operator==(const Package& otherPackage) const {
//        return this->name == otherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    bool operator==(Package& otherPackage) {
//        return this->name == otherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR LAMBDA COMPARATOR OR PREDICATE COMPARATOR FOR 'STD::FIND_IF', 'STD::ANY_OF' WITH SEARCHED ELEMENT PASSED DIRECTLY

    // WORKS
    // ... but doesn't work for 'std::find'  - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // WORKS - the exact candidate function for comparison with comparator predicate with dereferenced comparison with '*'
//    friend bool operator==(const Package& onePackage, Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // Doesn't work - 'error: binding reference of type ‘Package&’ to ‘const Package’ discards qualifiers'
//    friend bool operator==(Package& onePackage, const Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // Doesn't work for passing searched element to the comparator predicate as dereferenced smart pointer and comparing them directly
    //  - error: passing ‘const Package’ as ‘this’ argument discards qualifiers [-fpermissive]
//    friend bool operator==(Package& onePackage, Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // WORKS
//    bool operator==(const Package& otherPackage) const {
//        return this->name == otherPackage.name;
//    }

//    // Doesn't work - 'error: passing ‘const Package’ as ‘this’ argument discards qualifiers [-fpermissive]'
//    bool operator==(const Package& otherPackage) {
//        return this->name == otherPackage.name;
//    }

    // WORKS
//    bool operator==(Package& otherPackage) const {
//        return this->name == otherPackage.name;
//    }

    // Doesn't work for passing searched element to the comparator predicate as dereferenced smart pointer and comparing them directly
    //  - error: passing ‘const Package’ as ‘this’ argument discards qualifiers [-fpermissive]
//    bool operator==(Package& otherPackage) {
//        return this->name == otherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR LAMBDA COMPARATOR OR PREDICATE COMPARATOR FOR 'STD::FIND_IF', 'STD::ANY_OF' WITH SEARCHED ELEMENT PASSED AS ALREADY DEREFERENCED

    // WORKS for comparing dereferenced smart pointer (i.e. the instance) with directly passed smart pointer in the comparator predicate
//    friend bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’) - missing 'const' for all parameters of function
    //  - PackageComparatorPredicate.h:51:33: error: binding reference of type ‘std::unique_ptr<Package>&’ to ‘const std::unique_ptr<Package>’ discards qualifiers
//    friend bool operator==(Package& anotherPackage, std::unique_ptr<Package>& onePackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’) - missing 'const' for all parameters of function
//    friend bool operator==(const Package& anotherPackage, std::unique_ptr<Package>& onePackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’) - missing 'const' for all parameters of function
//    friend bool operator==(Package& anotherPackage, const std::unique_ptr<Package>& onePackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // WORKS for comparing dereferenced smart pointer (i.e. the instance) with directly passed smart pointer in the comparator predicate
//    bool operator==(const std::unique_ptr<Package>& otherPackage) const {
//        return this->name == otherPackage->name;
//    }

    // Doesn't work - ‘const Package’ is not derived from ‘const std::pair<_T1, _T2>’
    //  the function needs to be 'const'
//    bool operator==(const std::unique_ptr<Package>& otherPackage) {
//        return this->name == otherPackage->name;
//    }

    // Doesn't work - ‘const Package’ is not derived from ‘const std::pair<_T1, _T2>’
    //  the operand needs to be 'const'
//    bool operator==(std::unique_ptr<Package>& otherPackage) const {
//        return this->name == otherPackage->name;
//    }

    // Doesn't work - missing 'const' for parameter
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’)
    //  - PackageComparatorPredicate.h:51:33: error: binding reference of type ‘std::unique_ptr<Package>&’ to ‘const std::unique_ptr<Package>’ discards qualifiers
//    bool operator==(std::unique_ptr<Package>& otherPackage) {
//        return this->name == otherPackage->name;
//    }

<~snip~>

// NON-MEMBER OPERATORS/COMPARATORS - OVERLOADED/SPECIALIZED FUNCTIONS/FUNCTORS FROM 'STD'

// DIRECT COMPARISON WITH SPECIALIZED 'STD::LESS' FUNCTOR FOR 'SET::FIND'

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return onePackage < anotherPackage;
//        }
//    };
//}

// DEREFERENCED COMPARISON WITH SPECIALIZED 'STD::LESS' FUNCTOR FOR 'SET::FIND'

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return onePackage->getName() < anotherPackage->getName();
//        }
//    };
//}

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return *onePackage < *anotherPackage;
//        }
//    };
//}

// 'STD::FIND', 'STD::FIND_IF' - DIRECTLY PASSING AN UNIQUE POINTER

//namespace std {
//    inline bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->getName() == anotherPackage->getName();
//    }
//}

// 'STD::FIND' - PASSING A DEREFERENCED UNIQUE POINTER - DOESN'T WORK FOR 'STD::FIND_IF' with dereferenced unique ptr comparison

//namespace std {
//    inline bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
//        return onePackage->getName() == anotherPackage.getName();
//    }
//}

// 'STD::FIND_IF' - DIRECTLY PASSING A UNIQUE POINTER - DOESN'T WORK FOR COMPARATOR PREDICATE WITH DEREFERENCED COMPARISON
//   error: no match for ‘operator==’ (operand types are ‘Package’ and ‘Package’)

//namespace std {
//    inline bool operator==(const Package& onePackage, const Package& anotherPackage) {
//        return onePackage.getName() == anotherPackage.getName();
//    }
//}

// 'STD::FIND_IF' - PASSING A DEREFERENCED UNIQUE POINTER - DIRECT COMPARISON

//namespace std {
//    inline bool operator==(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->getName() == anotherPackage.getName();
//    }
//}

// 'STD::FIND_IF' - PASSING A DEREFERENCED UNIQUE POINTER - DEREFERENCED COMPARISON WITH '*' - DOESN'T WORK

//namespace std {
//    inline bool operator==(const Package& onePackage, Package& anotherPackage) {
//        return onePackage.getName() == anotherPackage.getName();
//    }
//}

// 'SET::FIND', 'STD::BINARY_SEARCH' - DIRECTLY PASSING AN UNIQUE POINTER

namespace std {
    inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
        return onePackage->getName() < anotherPackage->getName();
    }
}

//namespace std {
//    inline bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return *onePackage < *anotherPackage;
//    }
//}

// 'SET::FIND' - DIRECTLY PASSING AN UNIQUE POINTER - DEREFERENCED COMPARISON IN COMPARATOR - COULDN'T MAKE IT WORK

//namespace std {
//    inline bool operator<(const Package& onePackage, Package& anotherPackage) {
//        return onePackage.getName() == anotherPackage.getName();
//    }
//}

// 'STD::BINARY_SEARCH' - PASSING A DEREFERENCED UNIQUE POINTER

//namespace std {
//    inline bool operator<(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage.getName() < anotherPackage->getName();
//    }
//}
//
//namespace std {
//    inline bool operator<(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
//        return onePackage->getName() < anotherPackage.getName();
//    }
//}

// LAMBDA COMPARATOR FOR SECOND TEMPLATE PARAMETER

//inline bool comparePackages(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//    return onePackage < anotherPackage;
//}
```

```
PackageComparatorPredicate.h

#pragma once

#include "Package.h"

// using Predicate instead (reusable; no modification of the compared class)

// FOR PASSING SMART POINTER TO COMPARATOR PREDICATE DIRECTLY

struct PackageComparatorPredicate {
    const std::unique_ptr<Package>& package;

    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
            package(packageToFind)
    {}

    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
        // DIRECT COMPARISON - works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
        return (this->package == otherPackage);

        // DEREFERENCED COMPARISON without accessor method - works with
        //  - 'friend bool operator==(Package& onePackage, Package& anotherPackage)' (with/without const for parameters)
        //  - and member function 'bool operator==(Package& otherPackage)' (with/without const for function/parameters)
//        return (*(this->package) == *otherPackage);

        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'Package' element to compared field within the 'Package' element
//        return (this->package->getName() == otherPackage->getName());
    }
};

// FOR PASSING DEREFERENCED SMART POINTER TO COMPARATOR PREDICATE

//struct PackageComparatorPredicate {
//    const Package& package;
//
//    explicit PackageComparatorPredicate(const Package& packageToFind) :
//            package(packageToFind)
//    {}
//
//    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
//        // COMPARISON OF SEARCHED DEREFERENCED MEMBER VARIABLE WITH SMART POINTER
//        //  - works with 'friend bool operator==(Package& onePackage, std::unique_ptr<Package>& anotherPackage)'
////        return this->package == otherPackage;
//
//        // COMPARISON OF MEMBER VARIABLE REFERENCE WITH DEREFERENCED SMART POINTER
//        //  - 'friend bool operator==(Package& onePackage, Package& anotherPackage)' (with/without const for parameters)
//        //  - and member function 'bool operator==(Package& otherPackage)' (with/without const for function/parameters)
////        return (this->package == *otherPackage);
//
//        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'Package' element to compared field within the 'Package' element
//        return (this->package.getName() == otherPackage->getName());
//    }
//};
```

## Ressurection merge log

<pre>[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

Changes not staged for commit:
  (use &quot;git add &lt;file&gt;...&quot; to update what will be committed)
  (use &quot;git restore &lt;file&gt;...&quot; to discard changes in working directory)
	<font color="#AA0000">modified:   clean_pacman_cache_dir-class_diagram.puml</font>

no changes added to commit (use &quot;git add&quot; and/or &quot;git commit -a&quot;)
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
* <font color="#00AA00">encapsulate_container_variables_to_separate_classes</font>
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
  main
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

Changes not staged for commit:
  (use &quot;git add &lt;file&gt;...&quot; to update what will be committed)
  (use &quot;git restore &lt;file&gt;...&quot; to discard changes in working directory)
	<font color="#AA0000">modified:   clean_pacman_cache_dir-class_diagram.puml</font>

no changes added to commit (use &quot;git add&quot; and/or &quot;git commit -a&quot;)
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

Changes not staged for commit:
  (use &quot;git add &lt;file&gt;...&quot; to update what will be committed)
  (use &quot;git restore &lt;file&gt;...&quot; to discard changes in working directory)
	<font color="#AA0000">modified:   clean_pacman_cache_dir-class_diagram.puml</font>

no changes added to commit (use &quot;git add&quot; and/or &quot;git commit -a&quot;)
[laptop@laptop clean_pacman_cache_dir]$ git add -A
[laptop@laptop clean_pacman_cache_dir]$ git commit -m &quot;Update relationships in UML class diagram&quot;
[encapsulate_container_variables_to_separate_classes 92bd00e] Update relationships in UML class diagram
 1 file changed, 62 insertions(+), 60 deletions(-)
[laptop@laptop clean_pacman_cache_dir]$ git push
Enumerating objects: 5, done.
Counting objects: 100% (5/5), done.
Delta compression using up to 2 threads
Compressing objects: 100% (3/3), done.
Writing objects: 100% (3/3), 784 bytes | 784.00 KiB/s, done.
Total 3 (delta 2), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (2/2), completed with 2 local objects.
To github.com:kyberdrb/clean_pacman_cache_dir.git
   2905f8b..92bd00e  encapsulate_container_variables_to_separate_classes -&gt; encapsulate_container_variables_to_separate_classes
[laptop@laptop clean_pacman_cache_dir]$ cm
cmake       cmake-gui   cmp         cmsutil     cmuwmtopbm  cmx2raw     cmx2text    cmx2xhtml
[laptop@laptop clean_pacman_cache_dir]$ ls
<font color="#FFFF55"><b>cmake-build-debug</b></font>
<font color="#FFFF55"><b>cmake-build-release</b></font>
AbsolutePath.cpp
AbsolutePath.h
clean_pacman_cache_dir-class_diagram.puml
clean_pacman_cache_dir.drawio
<font color="#FF55FF"><b>clean_pacman_cache_dir.drawio.png</b></font>
CMakeLists.txt
ExtendedInstallationPackageFile.cpp
ExtendedInstallationPackageFile.h
FileMover.cpp
FileMover.h
Filename.cpp
Filename.h
IgnoredPackageNameComparatorPredicate.h
IgnoredPackageName.cpp
IgnoredPackageName.h
IgnoredPackageNames.cpp
IgnoredPackageNames.h
LICENSE
LocallyInstalledPackage.cpp
LocallyInstalledPackage.h
LocallyInstalledPackages.cpp
LocallyInstalledPackages.h
main.cpp
MatchFinderForPackageFilesToLocallyInstalledPackages.cpp
MatchFinderForPackageFilesToLocallyInstalledPackages.h
MoverOfInstallationPackageFiles.cpp
MoverOfInstallationPackageFiles.h
Package.cpp
Package.h
PackageNameAndVersion.cpp
PackageNameAndVersion.h
PackageName.cpp
PackageName.h
PackageNameMissing.cpp
PackageNameMissing.h
PackageVersion.cpp
PackageVersion.h
PackageWithInferredName.cpp
PackageWithInferredName.h
README.md
Refactoring_Package_due_to_using_a_subset_of_features_for_packageWithInferredName_of_original_Package_type_and_initializing_the_rest_to_undefined_values-Class_diagram.puml
Refactoring_PackageFile_due_to_referencing_of_member_variable_to_temporary_value-Class_diagram.puml
SimpleInstallationPackageFile.cpp
SimpleInstallationPackageFile.h
SimpleInstallationPackageFileType.h
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
* <font color="#00AA00">encapsulate_container_variables_to_separate_classes</font>
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
  main
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ man cmake
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
* <font color="#00AA00">encapsulate_container_variables_to_separate_classes</font>
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
  main
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch encapsulate_container_variables_to_separate_classes
Your branch is up to date with &apos;origin/encapsulate_container_variables_to_separate_classes&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git branc
git: &apos;branc&apos; is not a git command. See &apos;git --help&apos;.
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
* <font color="#00AA00">encapsulate_container_variables_to_separate_classes</font>
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
  main
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git checkout backports
Switched to branch &apos;backports&apos;
Your branch is up to date with &apos;origin/backports&apos;.
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch backports
Your branch is up to date with &apos;origin/backports&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git pull
Already up to date.
[laptop@laptop clean_pacman_cache_dir]$ git merge --no-ff encapsulate_container_variables_to_separate_classes --message=&quot;Encapsulate container variables to separate classes&quot;
Merge made by the &apos;ort&apos; strategy.
 CMakeLists.txt                                           |   2 <font color="#00AA00">+</font><font color="#AA0000">-</font>
 FileMover.cpp                                            |   6 <font color="#00AA00">+</font><font color="#AA0000">-</font>
 IgnoredPackageNameComparatorPredicate.h                  |  22 <font color="#00AA00">+++++++</font>
 IgnoredPackageNames.cpp                                  |  70 <font color="#00AA00">++++++++++++++++++++++</font>
 IgnoredPackageNames.h                                    |  22 <font color="#00AA00">+++++++</font>
 LocallyInstalledPackage.cpp                              |   4 <font color="#00AA00">+</font><font color="#AA0000">-</font>
 LocallyInstalledPackage.h                                |  23 <font color="#00AA00">++++++</font><font color="#AA0000">--</font>
 LocallyInstalledPackages.cpp                             | 143 <font color="#00AA00">+++++++++++++++++++++++++++++++++++++++++++++</font>
 LocallyInstalledPackages.h                               |  49 <font color="#00AA00">++++++++++++++++</font>
 MatchFinderForPackageFilesToLocallyInstalledPackages.cpp | 176 <font color="#00AA00">+++++++++++++++++++++++++++++++++++++++++++++++++++++++</font>
 MatchFinderForPackageFilesToLocallyInstalledPackages.h   |  26 <font color="#00AA00">+++++++++</font>
 MoverOfInstallationPackageFiles.cpp                      |  31 <font color="#00AA00">++++++++++</font>
 MoverOfInstallationPackageFiles.h                        |  22 <font color="#00AA00">+++++++</font>
 Package.h                                                |   8 <font color="#AA0000">---</font>
 PackageNameMissing.cpp                                   |  13 <font color="#00AA00">+++++</font>
 PackageNameMissing.h                                     |  23 <font color="#00AA00">++++++++</font>
 PackageWithInferredName.cpp                              |   2 <font color="#00AA00">+</font><font color="#AA0000">-</font>
 PackageWithInferredName.h                                |   8 <font color="#00AA00">+</font><font color="#AA0000">--</font>
 README.md                                                |  19 <font color="#00AA00">+++++</font><font color="#AA0000">-</font>
 clean_pacman_cache_dir-class_diagram.puml                | 211 <font color="#00AA00">+++++++++++++++++++++++++++++++++++++++++++++++++++++</font><font color="#AA0000">-------------</font>
 main.cpp                                                 | 343 <font color="#00AA00">+++++++++</font><font color="#AA0000">--------------------------------------------------------------------------------------------------</font>
 21 files changed, 838 insertions(+), 385 deletions(-)
 create mode 100644 IgnoredPackageNameComparatorPredicate.h
 create mode 100644 IgnoredPackageNames.cpp
 create mode 100644 IgnoredPackageNames.h
 create mode 100644 LocallyInstalledPackages.cpp
 create mode 100644 LocallyInstalledPackages.h
 create mode 100644 MatchFinderForPackageFilesToLocallyInstalledPackages.cpp
 create mode 100644 MatchFinderForPackageFilesToLocallyInstalledPackages.h
 create mode 100644 MoverOfInstallationPackageFiles.cpp
 create mode 100644 MoverOfInstallationPackageFiles.h
 create mode 100644 PackageNameMissing.cpp
 create mode 100644 PackageNameMissing.h
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch backports
Your branch is ahead of &apos;origin/backports&apos; by 10 commits.
  (use &quot;git push&quot; to publish your local commits)

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git push
Enumerating objects: 1, done.
Counting objects: 100% (1/1), done.
Writing objects: 100% (1/1), 234 bytes | 234.00 KiB/s, done.
Total 1 (delta 0), reused 0 (delta 0), pack-reused 0
To github.com:kyberdrb/clean_pacman_cache_dir.git
   ca15638..9ad61b8  backports -&gt; backports
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch backports
Your branch is up to date with &apos;origin/backports&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git pull
Already up to date.
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch backports
Your branch is up to date with &apos;origin/backports&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
* <font color="#00AA00">backports</font>
  encapsulate_container_variables_to_separate_classes
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
  main
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git checkout main
Switched to branch &apos;main&apos;
Your branch is up to date with &apos;origin/main&apos;.
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch main
Your branch is up to date with &apos;origin/main&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git pull
Already up to date.
[laptop@laptop clean_pacman_cache_dir]$ git tag
1.0.0
1.0.1
2.0.0
2.0.1
2.1.0
2.2.0
2.3.1
2.3.2
2.3.3
2.3.4
2.3.5
2.3.6
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
  encapsulate_container_variables_to_separate_classes
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
* <font color="#00AA00">main</font>
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$ git pull
Already up to date.
[laptop@laptop clean_pacman_cache_dir]$ git merge --no-ff backports --message=&quot;Encapsulate container variables to separate classes&quot;
Auto-merging CMakeLists.txt
CONFLICT (content): Merge conflict in CMakeLists.txt
Auto-merging IgnoredPackageNames.cpp
CONFLICT (add/add): Merge conflict in IgnoredPackageNames.cpp
Auto-merging IgnoredPackageNames.h
CONFLICT (add/add): Merge conflict in IgnoredPackageNames.h
Auto-merging LocallyInstalledPackages.cpp
CONFLICT (add/add): Merge conflict in LocallyInstalledPackages.cpp
Auto-merging LocallyInstalledPackages.h
CONFLICT (add/add): Merge conflict in LocallyInstalledPackages.h
Auto-merging Package.cpp
CONFLICT (content): Merge conflict in Package.cpp
Auto-merging Package.h
CONFLICT (content): Merge conflict in Package.h
CONFLICT (modify/delete): PackageFile.cpp deleted in backports and modified in HEAD.  Version HEAD of PackageFile.cpp left in tree.
CONFLICT (modify/delete): PackageFile.h deleted in backports and modified in HEAD.  Version HEAD of PackageFile.h left in tree.
Auto-merging PackageName.cpp
CONFLICT (add/add): Merge conflict in PackageName.cpp
Auto-merging PackageName.h
CONFLICT (add/add): Merge conflict in PackageName.h
Auto-merging PackageVersion.cpp
CONFLICT (add/add): Merge conflict in PackageVersion.cpp
Auto-merging PackageVersion.h
CONFLICT (add/add): Merge conflict in PackageVersion.h
Auto-merging README.md
CONFLICT (content): Merge conflict in README.md
Auto-merging main.cpp
CONFLICT (content): Merge conflict in main.cpp
Automatic merge failed; fix conflicts and then commit the result.
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch main
Your branch is up to date with &apos;origin/main&apos;.

You have unmerged paths.
  (fix conflicts and run &quot;git commit&quot;)
  (use &quot;git merge --abort&quot; to abort the merge)

Changes to be committed:
	<font color="#00AA00">new file:   AbsolutePath.cpp</font>
	<font color="#00AA00">new file:   AbsolutePath.h</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   FileMover.cpp</font>
	<font color="#00AA00">new file:   FileMover.h</font>
	<font color="#00AA00">new file:   Filename.cpp</font>
	<font color="#00AA00">new file:   Filename.h</font>
	<font color="#00AA00">new file:   IgnoredPackageName.cpp</font>
	<font color="#00AA00">new file:   IgnoredPackageName.h</font>
	<font color="#00AA00">new file:   IgnoredPackageNameComparatorPredicate.h</font>
	<font color="#00AA00">new file:   LICENSE</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.cpp</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.h</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.cpp</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.h</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.cpp</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.h</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.cpp</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.h</font>
	<font color="#00AA00">new file:   PackageNameMissing.cpp</font>
	<font color="#00AA00">new file:   PackageNameMissing.h</font>
	<font color="#00AA00">new file:   PackageWithInferredName.cpp</font>
	<font color="#00AA00">new file:   PackageWithInferredName.h</font>
	<font color="#00AA00">new file:   Refactoring_PackageFile_due_to_referencing_of_member_variable_to_temporary_value-Class_diagram.puml</font>
	<font color="#00AA00">new file:   Refactoring_Package_due_to_using_a_subset_of_features_for_packageWithInferredName_of_original_Package_type_and_initializing_the_rest_to_undefined_values-Class_diagram.puml</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFileType.h</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir-class_diagram.puml</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio.png</font>

Unmerged paths:
  (use &quot;git add/rm &lt;file&gt;...&quot; as appropriate to mark resolution)
	<font color="#AA0000">both modified:   CMakeLists.txt</font>
	<font color="#AA0000">both added:      IgnoredPackageNames.cpp</font>
	<font color="#AA0000">both added:      IgnoredPackageNames.h</font>
	<font color="#AA0000">both added:      LocallyInstalledPackages.cpp</font>
	<font color="#AA0000">both added:      LocallyInstalledPackages.h</font>
	<font color="#AA0000">both modified:   Package.cpp</font>
	<font color="#AA0000">both modified:   Package.h</font>
	<font color="#AA0000">deleted by them: PackageFile.cpp</font>
	<font color="#AA0000">deleted by them: PackageFile.h</font>
	<font color="#AA0000">both added:      PackageName.cpp</font>
	<font color="#AA0000">both added:      PackageName.h</font>
	<font color="#AA0000">both added:      PackageVersion.cpp</font>
	<font color="#AA0000">both added:      PackageVersion.h</font>
	<font color="#AA0000">both modified:   README.md</font>
	<font color="#AA0000">both modified:   main.cpp</font>

[laptop@laptop clean_pacman_cache_dir]$ pwd
/home/laptop/git/kyberdrb/clean_pacman_cache_dir
[laptop@laptop clean_pacman_cache_dir]$ ls -l PackageFile.cpp
-rw-r--r-- 1 laptop users 870 Jul  5 00:31 PackageFile.cpp
[laptop@laptop clean_pacman_cache_dir]$ gio trash PackageFile.cpp PackageFile.h
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch main
Your branch is up to date with &apos;origin/main&apos;.

You have unmerged paths.
  (fix conflicts and run &quot;git commit&quot;)
  (use &quot;git merge --abort&quot; to abort the merge)

Changes to be committed:
	<font color="#00AA00">new file:   AbsolutePath.cpp</font>
	<font color="#00AA00">new file:   AbsolutePath.h</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   FileMover.cpp</font>
	<font color="#00AA00">new file:   FileMover.h</font>
	<font color="#00AA00">new file:   Filename.cpp</font>
	<font color="#00AA00">new file:   Filename.h</font>
	<font color="#00AA00">new file:   IgnoredPackageName.cpp</font>
	<font color="#00AA00">new file:   IgnoredPackageName.h</font>
	<font color="#00AA00">new file:   IgnoredPackageNameComparatorPredicate.h</font>
	<font color="#00AA00">new file:   LICENSE</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.cpp</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.h</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.cpp</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.h</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.cpp</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.h</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.cpp</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.h</font>
	<font color="#00AA00">new file:   PackageNameMissing.cpp</font>
	<font color="#00AA00">new file:   PackageNameMissing.h</font>
	<font color="#00AA00">new file:   PackageWithInferredName.cpp</font>
	<font color="#00AA00">new file:   PackageWithInferredName.h</font>
	<font color="#00AA00">new file:   Refactoring_PackageFile_due_to_referencing_of_member_variable_to_temporary_value-Class_diagram.puml</font>
	<font color="#00AA00">new file:   Refactoring_Package_due_to_using_a_subset_of_features_for_packageWithInferredName_of_original_Package_type_and_initializing_the_rest_to_undefined_values-Class_diagram.puml</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFileType.h</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir-class_diagram.puml</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio.png</font>

Unmerged paths:
  (use &quot;git add/rm &lt;file&gt;...&quot; as appropriate to mark resolution)
	<font color="#AA0000">both modified:   CMakeLists.txt</font>
	<font color="#AA0000">both added:      IgnoredPackageNames.cpp</font>
	<font color="#AA0000">both added:      IgnoredPackageNames.h</font>
	<font color="#AA0000">both added:      LocallyInstalledPackages.cpp</font>
	<font color="#AA0000">both added:      LocallyInstalledPackages.h</font>
	<font color="#AA0000">both modified:   Package.cpp</font>
	<font color="#AA0000">both modified:   Package.h</font>
	<font color="#AA0000">deleted by them: PackageFile.cpp</font>
	<font color="#AA0000">deleted by them: PackageFile.h</font>
	<font color="#AA0000">both added:      PackageName.cpp</font>
	<font color="#AA0000">both added:      PackageName.h</font>
	<font color="#AA0000">both added:      PackageVersion.cpp</font>
	<font color="#AA0000">both added:      PackageVersion.h</font>
	<font color="#AA0000">both modified:   README.md</font>
	<font color="#AA0000">both modified:   main.cpp</font>

[laptop@laptop clean_pacman_cache_dir]$ git add --all
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch main
Your branch is up to date with &apos;origin/main&apos;.

All conflicts fixed but you are still merging.
  (use &quot;git commit&quot; to conclude merge)

Changes to be committed:
	<font color="#00AA00">new file:   AbsolutePath.cpp</font>
	<font color="#00AA00">new file:   AbsolutePath.h</font>
	<font color="#00AA00">modified:   CMakeLists.txt</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   ExtendedInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   FileMover.cpp</font>
	<font color="#00AA00">new file:   FileMover.h</font>
	<font color="#00AA00">new file:   Filename.cpp</font>
	<font color="#00AA00">new file:   Filename.h</font>
	<font color="#00AA00">new file:   IgnoredPackageName.cpp</font>
	<font color="#00AA00">new file:   IgnoredPackageName.h</font>
	<font color="#00AA00">new file:   IgnoredPackageNameComparatorPredicate.h</font>
	<font color="#00AA00">modified:   IgnoredPackageNames.cpp</font>
	<font color="#00AA00">modified:   IgnoredPackageNames.h</font>
	<font color="#00AA00">new file:   LICENSE</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.cpp</font>
	<font color="#00AA00">new file:   LocallyInstalledPackage.h</font>
	<font color="#00AA00">modified:   LocallyInstalledPackages.cpp</font>
	<font color="#00AA00">modified:   LocallyInstalledPackages.h</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.cpp</font>
	<font color="#00AA00">new file:   MatchFinderForPackageFilesToLocallyInstalledPackages.h</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.cpp</font>
	<font color="#00AA00">new file:   MoverOfInstallationPackageFiles.h</font>
	<font color="#00AA00">modified:   Package.cpp</font>
	<font color="#00AA00">modified:   Package.h</font>
	<font color="#00AA00">deleted:    PackageFile.cpp</font>
	<font color="#00AA00">deleted:    PackageFile.h</font>
	<font color="#00AA00">modified:   PackageName.cpp</font>
	<font color="#00AA00">modified:   PackageName.h</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.cpp</font>
	<font color="#00AA00">new file:   PackageNameAndVersion.h</font>
	<font color="#00AA00">new file:   PackageNameMissing.cpp</font>
	<font color="#00AA00">new file:   PackageNameMissing.h</font>
	<font color="#00AA00">modified:   PackageVersion.cpp</font>
	<font color="#00AA00">modified:   PackageVersion.h</font>
	<font color="#00AA00">new file:   PackageWithInferredName.cpp</font>
	<font color="#00AA00">new file:   PackageWithInferredName.h</font>
	<font color="#00AA00">modified:   README.md</font>
	<font color="#00AA00">new file:   Refactoring_PackageFile_due_to_referencing_of_member_variable_to_temporary_value-Class_diagram.puml</font>
	<font color="#00AA00">new file:   Refactoring_Package_due_to_using_a_subset_of_features_for_packageWithInferredName_of_original_Package_type_and_initializing_the_rest_to_undefined_values-Class_diagram.puml</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.cpp</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFile.h</font>
	<font color="#00AA00">new file:   SimpleInstallationPackageFileType.h</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir-class_diagram.puml</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio</font>
	<font color="#00AA00">new file:   clean_pacman_cache_dir.drawio.png</font>
	<font color="#00AA00">modified:   main.cpp</font>

[laptop@laptop clean_pacman_cache_dir]$ git commit --message=&quot;Bring project back to life...&quot; --message=&quot;...Conform to SOLID principles, delegate functionalities to separate classes according to SRP/encapsulation/autonomy for more cohesion, less coupling, making the intentions and meaning of the classes and operations clearer; refactoring to making further planned changes easier to implement with less mental effort&quot;
[main 7006f1b] Bring project back to life...
[laptop@laptop clean_pacman_cache_dir]$ git push
Enumerating objects: 34, done.
Counting objects: 100% (34/34), done.
Delta compression using up to 2 threads
Compressing objects: 100% (12/12), done.
Writing objects: 100% (12/12), 1.94 KiB | 1.94 MiB/s, done.
Total 12 (delta 11), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (11/11), completed with 11 local objects.
To github.com:kyberdrb/clean_pacman_cache_dir.git
   6bb11e5..7006f1b  main -&gt; main
[laptop@laptop clean_pacman_cache_dir]$ git status
On branch main
Your branch is up to date with &apos;origin/main&apos;.

nothing to commit, working tree clean
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
  encapsulate_container_variables_to_separate_classes
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
* <font color="#00AA00">main</font>
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$ git tag --message=&quot;Ressurection&quot; --annotate &quot;3.0.0&quot;
[laptop@laptop clean_pacman_cache_dir]$ git tag
1.0.0
1.0.1
2.0.0
2.0.1
2.1.0
2.2.0
2.3.1
2.3.2
2.3.3
2.3.4
2.3.5
2.3.6
3.0.0
[laptop@laptop clean_pacman_cache_dir]$ git push --tags
Enumerating objects: 1, done.
Counting objects: 100% (1/1), done.
Writing objects: 100% (1/1), 160 bytes | 160.00 KiB/s, done.
Total 1 (delta 0), reused 0 (delta 0), pack-reused 0
To github.com:kyberdrb/clean_pacman_cache_dir.git
 * [new tag]         3.0.0 -&gt; 3.0.0
[laptop@laptop clean_pacman_cache_dir]$ git branch
  backport-encapsulate_ignored_package_name_to_class
  backport-fix_for_package_from_inferred_name
  backport-fix_for_partially_downloaded_package_file
  backports
  encapsulate_container_variables_to_separate_classes
  encapsulate_relating_package_files_to_packages
  find_element_of_custom_type_in_set
* <font color="#00AA00">main</font>
  packageVersion_from_string_to_PackageVersion
  refactoring_Package_class
[laptop@laptop clean_pacman_cache_dir]$
[laptop@laptop clean_pacman_cache_dir]$ date
Tue Jul  5 03:06:57 PM CEST 2022</pre>

## Possible experiments

- `std::set` with custom comparator given as a second template parameter at initialization
- `std::find_if`/`std::any_of`
    - passing unique pointer to `std::any_of` directly
        - lambda
            - public friend function - all params const (the only one that works) - 'std::set' encapsulated in class
        - comparator predicate
            - direct comparison in comparator
                - public friend function - all params const - 'std::set' encapsulated in class

## Testing algorithm on cache directories accessible only for root

Permission modification - switch to user

    sudo chown --dereference laptop:users "$(dirname "$(sudo realpath "/var/cache/pikaur")")"

Permission restoration - back to `root`

    sudo chown --dereference root:root "$(dirname "$(sudo realpath "/var/cache/pikaur")")"

Sources

- https://duckduckgo.com/?q=ls+dir+cannot+access+permission+denied+even+when+owner&t=ffab&ia=web
- https://askubuntu.com/questions/850227/ls-cannot-open-directory-permission-denied
- https://stackoverflow.com/questions/48225618/ls-permission-denied-even-though-i-have-read-access-to-the-directory#48225881
