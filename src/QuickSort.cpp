#include "QuickSort.h"

#include <utility>  // std::swap

// Quicksort, highest view count first.
// The pivot is the median of the first, middle and last songs, which avoids
// the O(n^2) case on already sorted lists. Hoare partition splits runs of equal
// view counts evenly. We recurse on the smaller side and loop on the larger
// side so the recursion depth stays O(log n).
// Not stable: songs with equal views can end up in any order.

namespace {

// Signed index type so "lo - 1" never wraps around like size_t would.
using Index = long long;

// True if song a should come before song b (higher views first).
bool comesBefore(const Song& a, const Song& b) {
    return a.viewCount > b.viewCount;
}

// Orders songs[lo], songs[mid], songs[hi] so that
// songs[lo] >= songs[mid] >= songs[hi] by view count, leaving the median of
// the three at mid. Returns mid.
Index medianOfThree(std::vector<Song>& songs, Index lo, Index hi) {
    Index mid = lo + (hi - lo) / 2;

    if (comesBefore(songs[mid], songs[lo])) std::swap(songs[mid], songs[lo]);
    if (comesBefore(songs[hi], songs[lo]))  std::swap(songs[hi], songs[lo]);
    if (comesBefore(songs[hi], songs[mid])) std::swap(songs[hi], songs[mid]);

    return mid;
}

// Hoare partition on songs[lo..hi] (inclusive).
// Returns p such that every song in [lo, p] has views >= every song in
// [p + 1, hi]. Both sides are non-empty when hi > lo.
Index partition(std::vector<Song>& songs, Index lo, Index hi) {
    Index mid = medianOfThree(songs, lo, hi);
    // copy the pivot value since the pivot song can move during the loop
    const unsigned long long pivot = songs[mid].viewCount;

    Index i = lo - 1;
    Index j = hi + 1;

    while (true) {
        // Find a song on the left that belongs on the right (too few views).
        do { ++i; } while (songs[i].viewCount > pivot);
        // Find a song on the right that belongs on the left (too many views).
        do { --j; } while (songs[j].viewCount < pivot);

        if (i >= j) {
            return j;
        }
        std::swap(songs[i], songs[j]);
    }
}

void quickSortRange(std::vector<Song>& songs, Index lo, Index hi) {
    while (lo < hi) {
        Index p = partition(songs, lo, hi);

        // Recurse on the smaller half, keep looping on the larger half.
        if (p - lo < hi - p) {
            quickSortRange(songs, lo, p);
            lo = p + 1;
        } else {
            quickSortRange(songs, p + 1, hi);
            hi = p;
        }
    }
}

}  // namespace

void quickSort(std::vector<Song>& songs) {
    if (songs.size() < 2) {
        return;
    }
    quickSortRange(songs, 0, static_cast<Index>(songs.size()) - 1);
}
