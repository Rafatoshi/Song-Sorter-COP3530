// Catch2 tests for quickSort (Ricardo).
//
// Quicksort isn't stable, so tied songs can be in any order. The tests check
// that view counts are in descending order, that no song was lost or changed,
// and that the view counts match std::sort.

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "QuickSort.h"
#include "Song.h"

namespace {

std::vector<unsigned long long> viewCounts(const std::vector<Song>& songs) {
    std::vector<unsigned long long> counts;
    counts.reserve(songs.size());
    for (const Song& s : songs) {
        counts.push_back(s.viewCount);
    }
    return counts;
}

bool isMostToLeast(const std::vector<Song>& songs) {
    for (std::size_t i = 1; i < songs.size(); ++i) {
        if (songs[i - 1].viewCount < songs[i].viewCount) {
            return false;
        }
    }
    return true;
}

// Same songs (title + views), ignoring order.
bool sameSongs(const std::vector<Song>& a, const std::vector<Song>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    auto key = [](const Song& s) { return std::make_pair(s.viewCount, s.title); };
    std::vector<std::pair<unsigned long long, std::string>> ka, kb;
    for (const Song& s : a) ka.push_back(key(s));
    for (const Song& s : b) kb.push_back(key(s));
    std::sort(ka.begin(), ka.end());
    std::sort(kb.begin(), kb.end());
    return ka == kb;
}

// Reference ordering: std::sort, most views first.
std::vector<unsigned long long> referenceViewCounts(std::vector<Song> songs) {
    std::sort(songs.begin(), songs.end(),
              [](const Song& a, const Song& b) { return a.viewCount > b.viewCount; });
    return viewCounts(songs);
}

std::vector<Song> randomSongs(std::size_t count, unsigned long long maxViews,
                              unsigned seed) {
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<unsigned long long> dist(0, maxViews);
    std::vector<Song> songs;
    songs.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        songs.push_back({"Song " + std::to_string(i), dist(rng)});
    }
    return songs;
}

// Runs quickSort on a copy and checks order, contents, and the reference.
void checkQuickSort(const std::vector<Song>& original) {
    std::vector<Song> sorted = original;
    quickSort(sorted);

    REQUIRE(sorted.size() == original.size());
    REQUIRE(isMostToLeast(sorted));
    REQUIRE(sameSongs(sorted, original));
    REQUIRE(viewCounts(sorted) == referenceViewCounts(original));
}

}  // namespace

TEST_CASE("quickSort: empty list stays empty", "[quicksort]") {
    std::vector<Song> songs;
    quickSort(songs);
    REQUIRE(songs.empty());
}

TEST_CASE("quickSort: one song is unchanged", "[quicksort]") {
    std::vector<Song> songs = {{"Only Song", 42}};
    quickSort(songs);
    REQUIRE(songs.size() == 1);
    REQUIRE(songs[0].title == "Only Song");
    REQUIRE(songs[0].viewCount == 42);
}

TEST_CASE("quickSort: two songs", "[quicksort]") {
    SECTION("out of order gets swapped") {
        std::vector<Song> songs = {{"Low", 1}, {"High", 2}};
        quickSort(songs);
        REQUIRE(songs[0].title == "High");
        REQUIRE(songs[1].title == "Low");
    }
    SECTION("already in order stays") {
        std::vector<Song> songs = {{"High", 2}, {"Low", 1}};
        quickSort(songs);
        REQUIRE(songs[0].title == "High");
        REQUIRE(songs[1].title == "Low");
    }
}

TEST_CASE("quickSort: small mixed list gives exact order", "[quicksort]") {
    std::vector<Song> songs = {
        {"C", 300}, {"A", 1000}, {"E", 5}, {"B", 750}, {"D", 20},
    };
    quickSort(songs);
    REQUIRE(viewCounts(songs) ==
            std::vector<unsigned long long>{1000, 750, 300, 20, 5});
    REQUIRE(songs[0].title == "A");
    REQUIRE(songs[4].title == "E");
}

TEST_CASE("quickSort: duplicate view counts", "[quicksort]") {
    SECTION("a few ties") {
        checkQuickSort({{"A", 50}, {"B", 10}, {"C", 50}, {"D", 30},
                        {"E", 10}, {"F", 50}, {"G", 30}});
    }
    SECTION("every song has the same view count") {
        std::vector<Song> songs;
        for (int i = 0; i < 1000; ++i) {
            songs.push_back({"Same " + std::to_string(i), 777});
        }
        checkQuickSort(songs);
    }
    SECTION("10,000 songs with only 5 distinct view counts") {
        checkQuickSort(randomSongs(10000, 4, 7));
    }
}

TEST_CASE("quickSort: already sorted most-to-least", "[quicksort]") {
    std::vector<Song> songs;
    for (int i = 10000; i > 0; --i) {
        songs.push_back({"Song " + std::to_string(i),
                         static_cast<unsigned long long>(i)});
    }
    checkQuickSort(songs);
}

TEST_CASE("quickSort: sorted least-to-most (reverse order)", "[quicksort]") {
    std::vector<Song> songs;
    for (int i = 1; i <= 10000; ++i) {
        songs.push_back({"Song " + std::to_string(i),
                         static_cast<unsigned long long>(i)});
    }
    checkQuickSort(songs);
}

TEST_CASE("quickSort: random lists match std::sort", "[quicksort]") {
    SECTION("500 songs") {
        checkQuickSort(randomSongs(500, 1000000000ULL, 1));
    }
    SECTION("10,000 songs") {
        checkQuickSort(randomSongs(10000, 1000000000ULL, 2));
    }
    SECTION("100,000 songs") {
        checkQuickSort(randomSongs(100000, 1000000000ULL, 3));
    }
    SECTION("extreme view counts (0 and max)") {
        std::vector<Song> songs = randomSongs(1000, 1000000ULL, 4);
        songs.push_back({"Zero", 0});
        songs.push_back({"Max", ~0ULL});
        checkQuickSort(songs);
    }
}
