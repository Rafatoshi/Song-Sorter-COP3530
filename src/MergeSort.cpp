#include "MergeSort.h"

using namespace std;

// TODO (Rafael): implement mergeSort and any helper functions.
// Use merge sort; order songs from most views to least views.

// combine two sorted sections
static void mergeParts(vector<Song>& songs, vector<Song>& temp, int left, int middle, int right) {
    int i = left;
    int j = middle + 1;
    int k = left;

    while (i < middle && j <= right) {
        if (songs[i].viewCount >= songs[j].viewCount) {
            temp[k] = songs[i];
            i++;
        }
        else {
            temp[k] = songs[j];
            j++
        }
        k++;
    }

    // copy any missing song after first loop
    while (i <= middle) {
        temp[k] = songs[i];
        i++;
        k++;
    }

    for (int index = left; index <= right; index++) {
        songs[index] = temp[index];
    }
}

// divide the vector and sort each section recursively
static void mergeSortRange(vector<Song>& songs, vector<Song>& temp, int left, int right) {
    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;

    mergeSortRange(songs, temp, left, middle);
    mergeSortRange(songs, temp, middle + 1, right);

    mergeParts(songs, temp, left, middle, right);
}

// sort songs by view count
void mergeSort(vector<Song>& songs) {
    if (songs.size() <= 1) {
        return;
    }

    int size = songs.size();
    vector<Song> temp(size);

    mergeSortRange(songs, temp, 0, size - 1);
}