#pragma once

#include <cstdio>


#ifdef DEBUG_LOG_ENABLED
#define DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...) (void(0))
#endif
