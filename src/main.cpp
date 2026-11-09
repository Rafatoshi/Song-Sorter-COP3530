// TODO (All): add the menu-driven console program here.
// Planned options: load songs, run merge sort, run quicksort,
// print the first N songs and runtimes, exit.

#include <iostream>
#include <vector>

#include "Song.h"
#include "SongData.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Benchmark.h"

using namespace std;

int main()
{
    vector<Song> songs;
    int choice = 0;

    while (choice != 5)
    {
        cout << "\n===== SONG SORTERS =====\n";
        cout << "1. Load 500 songs\n";
        cout << "2. Load 10000 songs\n";
        cout << "3. Run benchmarks\n";
        cout << "4. Display sorted songs\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 1)
        {
            songs = generateSongs(500);
            cout << "500 songs loaded.\n";
        }
        else if (choice == 2)
        {
            songs = generateSongs(10000);
            cout << "10000 songs loaded.\n";
        }
        else if (choice == 3)
        {
            if (songs.empty())
            {
                cout << "Load songs first.\n";
            }
            else
            {
                BenchmarkResults results = benchmarkSorts(songs);

                cout << "\n===== BENCHMARK RESULTS =====\n";

                cout << "Merge Sort: "
                     << results.mergeTime << " ms\n";

                cout << "Quick Sort: "
                     << results.quickTime << " ms\n";

                cout << "std::sort: "
                     << results.standardTime << " ms\n";

                cout << "Merge Sort correct: "
                     << (results.mergeCorrect ? "Yes" : "No") << endl;

                cout << "Quick Sort correct: "
                     << (results.quickCorrect ? "Yes" : "No") << endl;
            }
        }
        else if (choice == 4)
        {
            if (songs.empty())
            {
                cout << "Load songs first.\n";
            }
            else
            {
                int sortChoice;

                cout << "\n1. Display Merge Sort results\n";
                cout << "2. Display Quick Sort results\n";
                cout << "Enter choice: ";
                cin >> sortChoice;

                vector<Song> sortedSongs = songs;

                if (sortChoice == 1)
                {
                    mergeSort(sortedSongs);
                }
                else if (sortChoice == 2)
                {
                    quickSort(sortedSongs);
                }
                else
                {
                    cout << "Invalid choice.\n";
                }

                if (sortChoice == 1 || sortChoice == 2)
                {
                    cout << "\n===== TOP 10 SONGS =====\n";

                    for (size_t i = 0;
                         i < 10 && i < sortedSongs.size();
                         i++)
                    {
                        cout << i + 1 << ". "
                             << sortedSongs[i].title
                             << " - "
                             << sortedSongs[i].viewCount
                             << " views\n";
                    }
                }
            }
        }
        else if (choice == 5)
        {
            cout << "Goodbye!\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
