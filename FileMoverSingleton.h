//
// Created by laptop on 5/29/22.
//

#pragma once

#include "AbsolutePath.h"

class FileMoverSingleton {
public:
    static void move(const AbsolutePath& from, const AbsolutePath& to);
};
