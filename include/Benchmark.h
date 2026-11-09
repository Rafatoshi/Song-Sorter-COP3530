
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Song.h"
#include <vector>

struct BenchmarkResults
{
    double mergeTime;
    double quickTime;
    double standardTime;

    bool mergeCorrect;
    bool quickCorrect;
};

BenchmarkResults benchmarkSorts(const std::vector<Song>& original);

#endif
