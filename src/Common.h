#pragma once

#define NODISCARD [[nodiscard]]
#define INDEX_NONE -1

#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define STRINGIFY_INTERNAL(X) #X

#ifndef APP_VERSION
    #define APP_VERSION "0.0.0-dev"
#endif