// Menu-driven console program: load songs, benchmark merge sort vs quicksort
// vs std::sort, and print the first N sorted songs.

#include <iostream>
#include <limits>
#include <vector>

#include "Song.h"
#include "SongData.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "Benchmark.h"

using namespace std;

// Reads an int. On bad input, clears the error and returns -1.
int readInt()
{
    int value;

    if (cin >> value)
    {
        return value;
    }

    if (cin.eof())
    {
        return -1;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return -1;
}

void loadSongs(vector<Song>& songs, size_t count)
{
    songs = generateSongs(count);
    cout << count << " songs loaded.\n";
}

int main()
{
    vector<Song> songs;
    int choice = 0;

    while (choice != 6)
    {
        cout << "\n===== SONG SORTERS =====\n";
        cout << "1. Load 500 songs\n";
        cout << "2. Load 10000 songs\n";
        cout << "3. Load 100000 songs\n";
        cout << "4. Run benchmarks\n";
        cout << "5. Display sorted songs\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";

        choice = readInt();

        // stop at end of input
        if (!cin)
        {
            break;
        }

        if (choice == 1)
        {
            loadSongs(songs, 500);
        }
        else if (choice == 2)
        {
            loadSongs(songs, 10000);
        }
        else if (choice == 3)
        {
            loadSongs(songs, 100000);
        }
        else if (choice == 4)
        {
            if (songs.empty())
            {
                cout << "Load songs first.\n";
            }
            else
            {
                BenchmarkResults results = benchmarkSorts(songs);

                cout << "\n===== BENCHMARK RESULTS (" << songs.size()
                     << " songs, average of 5 runs) =====\n";

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
        else if (choice == 5)
        {
            if (songs.empty())
            {
                cout << "Load songs first.\n";
            }
            else
            {
                cout << "\n1. Display Merge Sort results\n";
                cout << "2. Display Quick Sort results\n";
                cout << "Enter choice: ";

                int sortChoice = readInt();

                if (sortChoice != 1 && sortChoice != 2)
                {
                    cout << "Invalid choice.\n";
                    continue;
                }

                cout << "How many songs to display? ";
                int n = readInt();

                if (n <= 0)
                {
                    cout << "Invalid number.\n";
                    continue;
                }

                vector<Song> sortedSongs = songs;

                if (sortChoice == 1)
                {
                    mergeSort(sortedSongs);
                }
                else
                {
                    quickSort(sortedSongs);
                }

                cout << "\n===== TOP " << n << " SONGS =====\n";

                for (size_t i = 0;
                     i < static_cast<size_t>(n) && i < sortedSongs.size();
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
        else if (choice == 6)
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
