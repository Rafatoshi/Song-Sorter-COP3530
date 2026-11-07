#ifndef SONG_H
#define SONG_H

#include <string>

// Shared data format used by every teammate.
struct Song {
    std::string title;
    unsigned long long viewCount;
};

#endif
