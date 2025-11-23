generate build directory

```
cmake -S . -B build
```

build (the executable)

```
cmake --build build
```

run

```
./build/src/main
```

or

```
.\build\bin\cms.exe
```

clean

```
rm -rf build
```

or

delete the build directory

shorthands

clean and generate build dir

```
rm -rf build && cmake -S . -B build
```

build and run

```
cmake --build build && ./build/bin/cms
```

benchmarking:

1. generate benchmark datasets
	```bash
	gcc benchmark/generate_data.c -O2 -o benchmark/generate-sequential && ./benchmark/generate-sequential 5000 data/P3_7-CMS-sequential.txt sequential
	gcc benchmark/generate_data.c -O2 -o benchmark/generate-random && ./benchmark/generate-random 5000 data/P3_7-CMS-random.txt random
	```

2. point the build at the sequential dataset
	- edit `include/config.h`
	- change `#define DB_FILE` to `"data/P3_7-CMS-sequential.txt"`

3. build and benchmark the sequential run
	```bash
	cmake --build build && hyperfine --warmup 3 "./build/bin/cms < benchmark/cmds.txt"
	```

4. repeat for the random dataset
	- switch `DB_FILE` in `include/config.h` to `"data/P3_7-CMS-random.txt"`
	- rerun step 3 (rebuild + hyperfine) to capture the random benchmark numbers

