// TODO (All): compare how long each sorting algorithm takes.
// Give both algorithms copies of the SAME original dataset.
// Compare results against std::sort as a reference.

#include "Benchmark.h"
#include "MergeSort.h"
#include "QuickSort.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

namespace {

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

// Check sorting order and make sure no songs were lost or changed.
bool matchesReference(const std::vector<Song>& result,
                      const std::vector<Song>& original,
                      const std::vector<Song>& reference)
{
    return isSortedMostToLeast(result) &&
           sameSongs(result, original) &&
           sameSongs(result, reference);
}

} // namespace

BenchmarkResults benchmarkSorts(const std::vector<Song>& original)
{
    // Give each algorithm a fresh copy of the same unsorted data.
    std::vector<Song> mergeSongs = original;
    std::vector<Song> quickSongs = original;
    std::vector<Song> standardSongs = original;

    using Clock = std::chrono::steady_clock;

    // Measure Merge Sort.
    auto start = Clock::now();
    mergeSort(mergeSongs);
    auto end = Clock::now();

    const double mergeTime =
        std::chrono::duration<double, std::milli>(end - start).count();

    // Measure Quick Sort.
    start = Clock::now();
    quickSort(quickSongs);
    end = Clock::now();

    const double quickTime =
        std::chrono::duration<double, std::milli>(end - start).count();

    // Measure the reference sort.
    start = Clock::now();

    std::sort(standardSongs.begin(), standardSongs.end(),
              [](const Song& a, const Song& b)
              {
                  return a.viewCount > b.viewCount;
              });

    end = Clock::now();

    const double standardTime =
        std::chrono::duration<double, std::milli>(end - start).count();

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

