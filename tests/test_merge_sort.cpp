// TODO (Rafael): add Catch2 tests for mergeSort.
#include <catch2/catch_test_macros.hpp>
#include "MergeSort.h"
#include <vector>

using namespace std;
// Cases to check: empty, one song, duplicates, already sorted,
// and descending view-count order.

// empty vector
TEST_CASE("Empty List", "[MergeSort]") {
    vector<Song> songs;

    mergeSort(songs);

    REQUIRE(songs.empty());
}

// one song
TEST_CASE("One song", "[MergeSort]") {
    vector<Song> songs = {{"Song A", 100}};

    mergeSort(songs);

    REQUIRE(songs.size() == 1);

    REQUIRE(songs[0].title == "Song A");
}

// duplicates
TEST_CASE("Duplicates", "[MergeSort]") {
    vector<Song> songs = {
        {"Song A", 90},
        {"Song B", 37},
        {"Song C", 90}
    }

    mergeSort(songs);

    REQUIRE(songs[0].title == "Song A");
    REQUIRE(songs[1].title == "Song C");
    REQUIRE(songs[2].title == "Song B");
}

// already sorted
TEST_CASE("Already sorted", "[MergeSort]") {
    vector<Song> songs = {
        {"Song A", 90},
        {"Song B", 37},
        {"Song C", 10}
    }

    merSort(songs);

    REQUIRE(songs[0].viewCount == 90);
    REQUIRE(songs[1].viewCount == 37);
    REQUIRE(songs[2].viewCount == 10);
}

// descending order
TEST_CASE("Descending order", "[MergeSort]") {
    vector<Song> songs = {
        {"Song A", 20},
        {"Song B", 90},
        {"Song C", 37},
        {"Song D", 1}
    };

    mergeSort(songs);

    REQUIRE(songs[0].title == "Song B");
    REQUIRE(songs[1].title == "Song C");
    REQUIRE(songs[2].title == "Song A");
    REQUIRE(songs[3].title == "Song D");
}