#include "SongData.h"

#include <random>
#include <string>

// Generates a requested number of songs.
// Each song receives a unique title and a random view count.
std::vector<Song> generateSongs(std::size_t count)
{
    std::vector<Song> songs;

    // Reserve space to avoid repeated memory allocations.
    songs.reserve(count);

    // Fixed seed makes generated data reproducible.
    std::mt19937_64 generator(3530);

    // Generate view counts between 0 and 1 billion.
    std::uniform_int_distribution<unsigned long long> distribution(
        0ULL, 1000000000ULL
    );

    // Generate the requested number of songs.
    for (std::size_t i = 0; i < count; ++i)
    {
        Song newSong;

        // Give every song a unique title.
        newSong.title = "Song " + std::to_string(i + 1);

        // Assign a random view count.
        newSong.viewCount = distribution(generator);

        // Add the song to the dataset.
        songs.push_back(newSong);
    }

    return songs;
}
