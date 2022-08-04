//
// Created by laptop on 8/3/22.
//

#pragma once

#include <memory>
#include <string>

class Paths {
public:
    static const Paths& get();
    friend std::unique_ptr<Paths> std::make_unique<Paths>();

    const std::string& getLogFileDir() const;

    const std::string& getPacmanCacheDirPackageDir() const;

    const std::string& getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText() const;

    const std::string& getPikaurUserCacheDir() const;
    const std::string& getPikaurUserCacheDirBuildDir() const;
    const std::string& getPikaurUserCacheDirPackageDir() const;

    const std::string& getPikaurSystemCacheDir() const;
    const std::string& getPikaurSystemCacheDirAurReposDir() const;
    const std::string& getPikaurSystemCacheDirBuildDir() const;
    const std::string& getPikaurSystemCacheDirPackageDir() const;

private:
    const std::string currentUserHomeDir;

    const std::string logFileDir;

    const std::string pacmanCacheDirPackageDir;
    const std::string pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText;

    const std::string pikaurUserCacheDir;
    const std::string pikaurUserCacheDirBuildDir;
    const std::string pikaurUserCacheDirPackageDir;

    const std::string pikaurSystemCacheDir;
    const std::string pikaurSystemCacheDirAurReposDir;
    const std::string pikaurSystemCacheDirBuildDir;
    const std::string pikaurSystemCacheDirPackageDir;

    Paths();

    std::string findHomeDirOfCurrentUser() const;
};
