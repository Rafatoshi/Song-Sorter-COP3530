// Times merge sort, quicksort, and std::sort on copies of the same dataset,
// then checks each result against std::sort.

#include "Benchmark.h"
#include "MergeSort.h"
#include "QuickSort.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace {

// Number of runs averaged per sort. One run on 500 songs is too short to
// time reliably.
const int kTimingRuns = 5;

using Clock = std::chrono::steady_clock;

// Compare complete song records, ignoring their order.
bool sameSongs(const std::vector<Song>& first,
               const std::vector<Song>& second)
{
    if (first.size() != second.size())
    {
        return false;
    }

    using SongKey = std::pair<std::string, unsigned long long>;

    std::vector<SongKey> firstKeys;
    std::vector<SongKey> secondKeys;

    firstKeys.reserve(first.size());
    secondKeys.reserve(second.size());

    for (const Song& song : first)
    {
        firstKeys.push_back({song.title, song.viewCount});
    }

    for (const Song& song : second)
    {
        secondKeys.push_back({song.title, song.viewCount});
    }

    std::sort(firstKeys.begin(), firstKeys.end());
    std::sort(secondKeys.begin(), secondKeys.end());

    return firstKeys == secondKeys;
}

// Check that songs are sorted from highest to lowest view count.
bool isSortedMostToLeast(const std::vector<Song>& songs)
{
    for (std::size_t i = 1; i < songs.size(); ++i)
    {
        if (songs[i - 1].viewCount < songs[i].viewCount)
        {
            return false;
        }
    }

    return true;
}

// Check that two lists have the same view counts in the same positions.
// Titles are not compared, because songs with equal views may legitimately
// appear in a different order (quicksort is not stable).
bool sameViewOrder(const std::vector<Song>& first,
                   const std::vector<Song>& second)
{
    if (first.size() != second.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < first.size(); ++i)
    {
        if (first[i].viewCount != second[i].viewCount)
        {
            return false;
        }
    }

    return true;
}

// Check sorting order and make sure no songs were lost or changed.
bool matchesReference(const std::vector<Song>& result,
                      const std::vector<Song>& original,
                      const std::vector<Song>& reference)
{
    return isSortedMostToLeast(result) &&
           sameSongs(result, original) &&
           sameViewOrder(result, reference);
}

// Runs sortFunction kTimingRuns times, each on a fresh copy of original.
// Returns the average time in milliseconds and leaves the last sorted copy
// in sortedOut so it can be checked for correctness.
template <typename SortFunction>
double averageTime(const std::vector<Song>& original,
                   SortFunction sortFunction,
                   std::vector<Song>& sortedOut)
{
    double totalTime = 0.0;

    for (int run = 0; run < kTimingRuns; ++run)
    {
        sortedOut = original; // copying is not timed

        auto start = Clock::now();
        sortFunction(sortedOut);
        auto end = Clock::now();

        totalTime += std::chrono::duration<double, std::milli>(end - start).count();
    }

    return totalTime / kTimingRuns;
}

} // namespace

BenchmarkResults benchmarkSorts(const std::vector<Song>& original)
{
    std::vector<Song> mergeSongs;
    std::vector<Song> quickSongs;
    std::vector<Song> standardSongs;

    // Measure Merge Sort.
    const double mergeTime = averageTime(
        original, [](std::vector<Song>& songs) { mergeSort(songs); }, mergeSongs);

    // Measure Quick Sort.
    const double quickTime = averageTime(
        original, [](std::vector<Song>& songs) { quickSort(songs); }, quickSongs);

    // Measure the reference sort.
    const double standardTime = averageTime(
        original,
        [](std::vector<Song>& songs)
        {
            std::sort(songs.begin(), songs.end(),
                      [](const Song& a, const Song& b)
                      {
                          return a.viewCount > b.viewCount;
                      });
        },
        standardSongs);

    // Check the results after timing is complete.
    const bool mergeCorrect =
        matchesReference(mergeSongs, original, standardSongs);

    const bool quickCorrect =
        matchesReference(quickSongs, original, standardSongs);

    return {
        mergeTime,
        quickTime,
        standardTime,
        mergeCorrect,
        quickCorrect
    };
}
