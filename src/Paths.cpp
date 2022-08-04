//
// Created by laptop on 8/3/22.
//

#include "Paths.h"

// helpers to detect the home directory path of the current user, even behind 'sudo'
#include <libaudit.h> // for 'audit_getloginuid()' to detect the UID of the user who invoked 'sudo', instead of the 'root' user
#include <pwd.h> // for 'getpwuid()' to get the home directory for the given UID

const Paths& Paths::get() {
    static auto paths = std::make_unique<Paths>();
    return *paths;
}

Paths::Paths() :
        currentUserHomeDir(this->findHomeDirOfCurrentUser()),
        logFileDir(this->currentUserHomeDir + "/.config/cpmcd/logs/"),
        pacmanCacheDirPackageDir("/var/cache/pacman/pkg"),
        pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText(this->pacmanCacheDirPackageDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/"),
        pikaurUserCacheDir(this->currentUserHomeDir + "/.cache/pikaur/"),
        pikaurUserCacheDirBuildDir(this->pikaurUserCacheDir + "build/"),
        pikaurUserCacheDirPackageDir(this->pikaurUserCacheDir + "pkg/"),
        pikaurSystemCacheDir("/var/cache/pikaur"), // might be a symlink, so ommitting the slash '/' at the end of the path
        pikaurSystemCacheDirAurReposDir(this->pikaurSystemCacheDir + "/aur_repos/"),
        pikaurSystemCacheDirBuildDir(this->pikaurSystemCacheDir + "/build/"),
        pikaurSystemCacheDirPackageDir(this->pikaurSystemCacheDir + "/pkg/")
{}

// detect the home directory path of the current user, even behind 'sudo'
std::string Paths::findHomeDirOfCurrentUser() const {
    std::string currentUserHomeDir = getpwuid(audit_getloginuid())->pw_dir;

    if (currentUserHomeDir.empty()) {
        return "/root";
    }

    return currentUserHomeDir;
}

const std::string& Paths::getLogFileDir() const {
    return this->logFileDir;
}

const std::string& Paths::getPacmanCacheDirPackageDir() const {
    return this->pacmanCacheDirPackageDir;
}

const std::string& Paths::getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText() const {
    return this->pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText;
}

const std::string& Paths::getPikaurUserCacheDir() const {
    return this->pikaurUserCacheDir;
}

const std::string& Paths::getPikaurUserCacheDirBuildDir() const {
    return this->pikaurUserCacheDirBuildDir;
}

const std::string& Paths::getPikaurUserCacheDirPackageDir() const {
    return this->pikaurUserCacheDirPackageDir;
}

const std::string& Paths::getPikaurSystemCacheDir() const {
    return this->pikaurSystemCacheDir;
}

const std::string& Paths::getPikaurSystemCacheDirAurReposDir() const {
    return this->pikaurSystemCacheDirAurReposDir;
}

const std::string& Paths::getPikaurSystemCacheDirBuildDir() const {
    return this->pikaurSystemCacheDirBuildDir;
}

const std::string& Paths::getPikaurSystemCacheDirPackageDir() const {
    return this->pikaurSystemCacheDirPackageDir;
}
