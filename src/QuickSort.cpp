#include "QuickSort.h"

#include <utility>  // std::swap

// Quicksort (written from scratch) that orders songs from MOST views to
// LEAST views.
//
// Design choices:
//  - Median-of-three pivot: look at the first, middle, and last songs of the
//    range and use the median view count as the pivot. This avoids the O(n^2)
//    worst case on lists that are already sorted or reverse-sorted.
//  - Hoare partition scheme: two indices walk toward each other and swap
//    out-of-place songs. Unlike Lomuto, it splits runs of EQUAL view counts
//    roughly in half, so lists with many ties (common for view counts)
//    still sort in O(n log n).
//  - Recurse on the smaller side, loop on the larger side. This keeps the
//    recursion depth at O(log n), so even 10,000+ songs can't overflow the
//    call stack.
//
// Note: quicksort is NOT stable. Songs with the same view count can come out
// in any order relative to each other. Only the view-count order is
// guaranteed.

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
    // Copy the pivot VALUE: the pivot song itself may be swapped during the loop.
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
