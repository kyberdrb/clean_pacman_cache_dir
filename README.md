# Clear Pacman's Cache Directory

A utility to delete the contents of the pacman's cache directory `/var/cache/pacman/pkg/` and leave in mentioned directory only package files, that the locally installed packages were installed from.

The packages that are listed next to `IgnorePkg` option in the pacman's configuration file - by default at `/etc/pacman.conf` are excluded from deletion. Package files that belong to the ignored packages and deviate from the locally installed version of installed packages need to be deleted manually.

## Build

### CLion `Release` build

1. Generate makefiles

        $ /opt/clion/bin/cmake/linux/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/opt/clion/bin/ninja/linux/ninja -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release

2. Compile the binary

        $ /opt/clion/bin/cmake/linux/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release --target clean_pacman_cache_dir

### Generic CMake `Release` build (when CLion utilities are not available or accessible)

    $ # Install 'cmake' and 'ninja'
    $ sudo pacman --sync --refresh --refresh --needed cmake ninja

    $ /usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -G Ninja -S /home/laptop/git/kyberdrb/clean_pacman_cache_dir -B /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release
    $ /usr/bin/cmake --build /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release --target clean_pacman_cache_dir

## Usage

1. Check the contents of the pacman's cache directory

        ls -1 /var/cache/pacman/pkg | less

2. Move package file in versions for other than the locally installed package version into a separate directory by executing

        $ cd /home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-release
        $ sudo ./clean_pacman_cache_dir

3. Verify contents of directories

        ls -1 /var/cache/pacman/pkg | less
        du -sh /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED | less
        ls -1 /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED | less

4. Delete the directory with collected package files
   
    - Less destructive - possibility to restore packages if needed and delete the rest later

          $ sudo chown --recursive /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED --reference "${HOME}"
          $ sudo move /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED ~/Downloads
          $ gio trash ~/Downloads/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED
        
    - More destructive - delete all files for other pakcage version immediately

          $ sudo rm -r /var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED

5. Verify the contents of the pacman's cache directory, which will now contain only files for locally installed packages

        ls -1 /var/cache/pacman/pkg | less

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

1. Add each filename of every file in pacman's cache directory into a map<PackageFile, PackageWithPartialFilename>, with PackageFile being the filename of the file (without absolute path - only the filename), and PackageWithPartialFilename being empty/null
2. Find all locally installed packages, form a partial filename as prefix by compounding the filename, version and architecture of the package in the format "PACKAGE_NAME-PACKAGE_VERSION-PACKAGE_ARCHITECTURE", and searching for all keys with such prefix in PackageFile. For all found keys assign for the value of PackageWithPartialFilename the compound package filename.
   - When no package file was found a given package, report it?/download it only to the pacmans cache dir? (with alpm? or from archive.archlinux.com by building the URL and webscraping all links that partially match with the compound package filename and downloading all files from the links with `curl` and adding the package filenames manually to the map so that the package files for the locally installed package will be preserved?)
3. Go through each package filename in the map and move to a separate directory FORMER_PACKAGE_VERSIONS such package files that have their values of PackageWithPartialFilename empty/null.

- lookup and handle packages within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges

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
- `string`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+tokenize+string&ia=web
    - https://www.geeksforgeeks.org/tokenizing-a-string-cpp/
    - https://duckduckgo.com/?q=c%2B%2B+check+if+number&t=ffab&ia=web
    - https://www.tutorialspoint.com/how-to-check-if-input-is-numeric-in-cplusplus
    - https://duckduckgo.com/?q=c%2B%2B+string+clear&t=ffab&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+reverse+string&ia=web
    - https://www.journaldev.com/35816/reverse-string-c-plus-plus
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+find+occurrence+character&ia=web
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+substring+extract+index+position&ia=web
    - https://www.educba.com/c-plus-plus-substring/
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+string+substr&ia=web
    - https://www.cplusplus.com/reference/string/string/substr/
- vector
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+vector+contains&ia=web&iax=qa
    - https://stackoverflow.com/questions/3450860/check-if-a-stdvector-contains-a-certain-object#3450906
- `std::filesystem`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+std+filesystem+remove+directory&ia=web
    - https://en.cppreference.com/w/cpp/filesystem/remove
    - https://techoverflow.net/2019/04/21/how-to-recursively-delete-directory-using-c17-filesystem-library/
- own projects
    - duplicate_finder - TODO add Github link
    - EmployeeManagementSystem - TODO add Github link
- C++ specific
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+member+vairable+member+function+with+the+same+name&ia=web&iax=qa
    - https://duckduckgo.com/?q=c%2B%2B+find+longest+prefix+match+string&t=ffab&ia=web
    - https://duckduckgo.com/?q=c%2B%2B+string+compare+longest+match+prefix&t=ffab&ia=web&iax=qa
    - https://stackoverflow.com/questions/22615838/string-matching-computing-the-longest-prefix-suffix-array-in-kmp-algorithm#22616474
    - https://docs.microsoft.com/en-us/cpp/cpp/delegating-constructors?view=msvc-170
- `libarchive`
    - https://duckduckgo.com/?t=ffab&q=c%2B%2B+libarchive&ia=web&iax=qa
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
