# Clear Pacman's Cache Directory

... and leave only the latest installedVersion of each package (maybe except the ignored ones in '/etc/pacman.conf'?)

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

Internal structural representation

Iterate all localy installed packages from the local DB and save each entry into a hashmap `installedPackages` in an instance of class `InstalledPackages`

    class PackagesAndTheirFiles

    installedPackages
    |-key: "accounts-qml-module"             ('std::string'/class 'PackageName'/std::unique_ptr<PackageName>)
      |-value:                                 (std::unique_ptr<Package>)
        |-locallyInstalledVersion: 0.7-4         (std::string/Version/std::unique_ptr<Version>)
        |-architecture: x86_64                   (std::string/Architecture/std:unique_ptr<Architecture>)
        |-packageVersionsWithRelatedDownloadedFiles (std::map<???, std::vector<???>>)
          |-key: 0.7-4                                (std::string/std::unique_ptr<Version>)
            |-accounts-qml-module-0.7-4-x86_64.pkg.tar.zst (std::vector<std::string/std::unique_ptr<PackageRelatedDownloadedFile>>
    |-key: "accountsservice"                 (std::unique_ptr<PackageName>)
      |-value:                                 (std::unique_ptr<Package>)
        |-locallyInstalledVersion: 22.08.8-1     (std::unique_ptr<Version>)
        |-architecture: x86_64                   (std:unique_ptr<Architecture>)
        |-packageVersionsWithRelatedDownloadedFiles (std::map<std::unique_ptr<Version> std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>>)
          |-key: 0.6.55-3                             (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-0.6.55-3-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |
          |-22.04.62-2                                (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-22.04.62-2-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |   |-accountsservice-22.04.62-2-x86_64.pkg.tar.zst.sig (std::unique_ptr<PackageRelatedDownloadedFile>)
          |
          |-22.08.8-1                                 (std::unique_ptr<Version>)
          | |-value:                                    (std::vector<std::unique_ptr<PackageRelatedDownloadedFile>>)
          |   |-accountsservice-22.08.8-1-x86_64.pkg.tar.zst (std::unique_ptr<PackageRelatedDownloadedFile>)
          |   |-accountsservice-22.08.8-1-x86_64.pkg.tar.zst.sig (std::unique_ptr<PackageRelatedDownloadedFile>)
    |-key: "acl"
      |-value:                   
        |-...

## Algorithm

- **Is it true that after each word of package name in its filename comes a delimiter (likely a dash '-') followed by a number?**  
**So I can iterate the package filename until I hit a number and then go back two characters (or remove/pop_back last two characters) and get the package name? Which will then serve me as a key to the `map` and the actual filename as the value?**
- **Not every installed  package has its downloaded package that it was installed from**  
  **but every downloaded package has its installed  package.**

1. iterate through all locally installed packages with `libalpm` library _(1195 entries)_
   1. create an instance of `package` with its `installedVersion`
   
          // inside the iteration loop
          std::string installedVersion = alpm_pkg_get_version(pkg);
          auto package = std::make_unique<Package>(installedVersion);

   2. save each `package` to the packages with the name of the package being a text key and the `package` instance being the value (`map`?)

          // structure declaration - before the iteration loop
          std::map<
              std::string              // package name
              std::unique_ptr<Package> // package instance
          > packages;
   
          ...

          // inside the iteration loop
          packages.emplace_back(alpm_pkg_get_name(pkg), std::move(package));
   
2. iterate through all files of the pacman's cache directory with the `std::filesystem` library _(7284/7293 files)_
   1. save the name of the file to a variable `filename`
   2. save the extension of the file to a variable `extension`
   3. if the extension equals to ".part"
      1. add the `filename` to the set of `packageFilesDesignatedForDeletion`

         // structure declaration - before the iteration loop
         std::set<
             std::string              // package name
         > packageFilesDesignatedForDeletion;

         ...

         // inside the iteration loop
         packageFilesDesignatedForDeletion.emplace(filename);
      2. continue
   4. initialize a text variable `packageName` to empty text
   5. fill the `packageName` variable by iterating through `filename` by characters one-by-one

          while the character is not a number  
              append the character to the `packageName`

   6. Iterate the rest of the filename and character-by-character fill the variable `downloadedPackageVersion`
   7. Extract the package `installedVersion` from the `downloadedPackageVersion` with a regular expression that searches for each architecture `-(x86_64|any).pkg.*` and replaces matching text (the trailing one with architecture and extensions) with an empty text `""` leaving only the package installedVersion and save it back to the variable `downloadedPackageVersion`
   8. Use the `packageName` to find the matching key in the `installedPackages` collection
      1. When the key is found (and thus the package is locally installed on the system), add a new entry to `packageVersionsWithRelatedDownloadedFiles` using the `downloadedPackageVersion` a key
      2. When the key is missing (`nullptr`) (and thus the package had been uninstalled from the system), add the `filename` to the collection `packageFilesDesignatedForDeletion`
3. Iterate all entries in `packageFilesDesignatedForDeletion`
   1. Move all entries into a separate directory `PACKAGE_CEMETERY` inside the pacman's cache directory
4. Iterate all entries in `installedPackages`  
   1. For each `installedPackage`
      1. if the package is in the list of `IgnoredPkgs` in the pacman's configuration file - likely in `/etc/pacman.conf` (parse or read with `libalpm`?)
         1. continue (skip the deletion of the package - the packages are ignored for good reasons - either they have a strong systemic effect on the system and may cause malfunction of the system, or they are licensed with other than open-source license which may after update stop functioning)
      2. call `moveAllPackageFilesInNonlocalVersionToSeparateDirectory`, where the destination directory will be provided as a parameter?/as an attribute in the class `PackagesAndTheirFiles`?
5. Run the algorithm again to check whether each installed package has exactly one package file that matches the local version of the installed package, and with an optional signature file.
   
---

The version of the algorithm with a tokenization

    1. find all non-alphanumeric characters and save them into `dividers` - a data structure that will hold only one unique copy `(set`?)
    2. tokenize the filename by all characters in `dividers`, i.e. iterate the filename text by characters one-by-one and
       1. save the tokens , i.e. alphanumeric words, and the character they are divided with, into a resizable array (`vector`?) - because number of tokens for each file may be different. For example
    3. initialize a text variable `cumulativePackageName` to empty text
    4. iterate the resizable array of tokens
       1. append current token to the `cumulativePackageName`
       2. check if the `packages` contains and entry that is exactly matching the key stored in `cumulativePackageName`
          1. if yes, add current file to the collection of files for matching `package`

## Sources

- libalpm - library of the Arch Linux Package Manager
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
- map
    - https://duckduckgo.com/?q=c%2B%2B+map+find+key+containing+pattern+substring&t=ffab&ia=web
    - https://stackoverflow.com/questions/9349797/partial-match-for-the-key-of-a-stdmap
    - http://www.cplusplus.com/reference/map/map/lower_bound/
    - to be continued...
- own projects
    - duplicate_finder - TODO add Github link
    - EmployeeManagementSystem - TODO add Github link
