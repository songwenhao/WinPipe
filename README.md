# WinPipe

## Clone codes
git clone https://github.com/songwenhao/WinPipe.git

## Build ##
```
mkdir cmake-out
cd cmake-out

// build share library
cmake -B . -S ..

// build static library
cmake -B . -S .. -DBUILD_SHARED_LIBS=OFF
```

## Specify MSVC Runtime library
see. https://learn.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-170
```
mkdir cmake-out
cd cmake-out

// MTd MT
cmake -B . -S .. -DBUILD_SHARED_LIBS=OFF -DMSVC_STATIC_RUNTIME=ON

// MDd MD
cmake -B . -S .. -DBUILD_SHARED_LIBS=OFF
```