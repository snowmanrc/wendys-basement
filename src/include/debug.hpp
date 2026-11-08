#pragma once

#include <cstdio>

// comment out the next line to turn off debug mode
#define DEBUG_MODE_ENABLED


#ifdef DEBUG_MODE_ENABLED
#define DEBUG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...) (void(0))
#endif
