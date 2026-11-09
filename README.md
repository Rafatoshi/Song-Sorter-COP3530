# Song-Sorter-COP3530
COP3530 group project comparing Merge Sort and Quick Sort to organize songs by popularity.

## Build and run
Requires CMake 3.14+ and a C++17 compiler. Catch2 v3 is downloaded automatically.

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

- Program: `build/SongSorters` (Visual Studio: `build\Release\SongSorters.exe`)
- Tests: `build/tests` (Visual Studio: `build\Release\tests.exe`)

Data sets: 500, 10,000, and 100,000 generated songs.
