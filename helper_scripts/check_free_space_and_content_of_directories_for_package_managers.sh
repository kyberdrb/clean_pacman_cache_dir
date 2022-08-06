#!/bin/sh

set -x

sudo printf "%s"

echo "AVAILABLE SPACE ON PARTITIONS"
echo "----------------------------------------------------------------------"
echo

df --human-readable

echo
echo "SIZES OF DIRECTORIES"
echo "----------------------------------------------------------------------"
echo

du -sh "/var/cache/pacman/pkg/"
du -sh "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/"

du -sh "${HOME}/.cache/pikaur/"
du -sh "${HOME}/.cache/pikaur/build/"
du -sh "${HOME}/.cache/pikaur/pkg/"

sudo du -sh "$(sudo realpath "/var/cache/pikaur")"
sudo du -sh "/var/cache/pikaur/aur_repos/"
sudo du -sh "/var/cache/pikaur/build/"
sudo du -sh "/var/cache/pikaur/pkg/"

echo
echo "======================================================================"
echo

echo "PACMAN - PACKAGE CACHE DIRECTORIES"
echo "----------------------------------------------------------------------"
echo
echo "/var/cache/pacman/pkg/"
find "/var/cache/pacman/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/"
find "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"

echo
echo "PIKAUR - USER HOME CACHE DIRECTORIES"
echo "----------------------------------------------------------------------"
echo
echo "${HOME}/.cache/pikaur/"
find "${HOME}/.cache/pikaur/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "${HOME}/.cache/pikaur/build/"
find "${HOME}/.cache/pikaur/build/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "${HOME}/.cache/pikaur/pkg/"
find "${HOME}/.cache/pikaur/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo

echo "PIKAUR - SYSTEM CACHE DIRECTORIES"
echo "----------------------------------------------------------------------"
echo
echo "/var/cache/pikaur"
sudo find "$(sudo realpath "/var/cache/pikaur")" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "/var/cache/pikaur/aur_repos/"
sudo find "/var/cache/pikaur/aur_repos/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "/var/cache/pikaur/build/"
sudo find "/var/cache/pikaur/build/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
echo "======================================================================"
echo
echo "/var/cache/pikaur/pkg/"
sudo find "/var/cache/pikaur/pkg/" -mindepth 1 -maxdepth 1 -printf "%y: %p\n" | sort
echo
