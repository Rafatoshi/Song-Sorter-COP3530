// TODO (All): compare how long each sorting algorithm takes.
// Give both algorithms copies of the SAME original dataset.
// Compare results against std::sort as a reference.
#include "Benchmark.h"
#include "MergeSort.h"
#include "QuickSort.h"

#include <algorithm>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

// Checks whether two sorted lists have the same view counts.
bool sameViewCounts(const vector<Song>& a, const vector<Song>& b)
{
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].viewCount != b[i].viewCount) {
            return false;
        }
    }

    return true;
}

BenchmarkResults benchmarkSorts(const vector<Song>& original)
{
    // Give each algorithm its own copy of the same dataset.
    vector<Song> mergeSongs = original;
    vector<Song> quickSongs = original;
    vector<Song> standardSongs = original;

    // Merge Sort timing.
    auto start = high_resolution_clock::now();
    mergeSort(mergeSongs);
    auto end = high_resolution_clock::now();

    double mergeTime =
        duration<double, milli>(end - start).count();

    // Quick Sort timing.
    start = high_resolution_clock::now();
    quickSort(quickSongs);
    end = high_resolution_clock::now();

    double quickTime =
        duration<double, milli>(end - start).count();

    // Standard sort timing (reference).
    start = high_resolution_clock::now();

    sort(standardSongs.begin(), standardSongs.end(),
         [](const Song& a, const Song& b) {
             return a.viewCount > b.viewCount;
         });

    end = high_resolution_clock::now();

    double standardTime =
        duration<double, milli>(end - start).count();

    // Check both algorithms against the reference.
    bool mergeCorrect = sameViewCounts(mergeSongs, standardSongs);
    bool quickCorrect = sameViewCounts(quickSongs, standardSongs);

    return {
        mergeTime,
        quickTime,
        standardTime,
        mergeCorrect,
        quickCorrect
    };
}
