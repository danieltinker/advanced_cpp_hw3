# run competition (all maps, all Algos, 1 GM from /sos )
  ./simulator_315634022 --competition \
  game_maps_folder=../maps \
  game_manager=../GameManager/sos/GameManager_315634022.so \
  algorithms_folder=../Algorithm/sos \
  num_threads=1 \
  --verbose


# run competition (all maps, all Algos, 1 GM from my compilation )
    ./simulator_315634022 --competition \
  game_maps_folder=../maps \
  game_manager=../GameManager/GameManager_315634022.so \
  algorithms_folder=../Algorithm/sos \
  num_threads=1 \
  --verbose


# run comparative (all GMs, 2 Algos, 1 Map from my folder )
./simulator_315634022 --comparative \
  game_map=../maps/example.txt \
  game_managers_folder=../GameManager/sos/ \
  algorithm1=../Algorithm/sos/Algorithm_315634022.so \
  algorithm2=../Algorithm/sos/Algorithm_322996059_211779582_2.so \
  num_threads=1

# run comparative (all GMs, 2 Algos, 1 Map from others )
./simulator_315634022 --comparative \
  game_map=../maps/example.txt \
  game_managers_folder=../GameManager/GameManagers-sos/ \
  algorithm1=../Algorithm/Algorithms-sos/Algorithm_322213836_212054837.so \
  algorithm2=../Algorithm/Algorithms-sos/Algorithm_322573304_322647603.so \
  num_threads=1

# How To Run [from root directory]:
1. make clean && make

Usage: simulator_<ID> <-comparative|-competition>
       game_map=<file> | game_maps_folder=<dir>
       game_managers_folder=<dir> | game_manager=<file>
       algorithm1=<so> algorithm2=<so> | algorithms_folder=<dir>
       [num_threads=<N>] [--verbose]

# Competition Mode:
./simulator_315634022 \
  --competition \
  game_maps_folder=../maps/ \
  game_manager=../GameManager/sos/libGameManager_315634022.so \
  algorithms_folder=../Algorithm/sos \
  num_threads=4 \
  --verbose
# Comparative Mode:
./simulator_315634022 \
  --comparative \
  game_map=../maps/example.txt \
  game_managers_folder=../GameManager/sos/ \                   
  algorithm1=../Algorithm/sos/libAlgorithm_315634022.so \
  algorithm2=../Algorithm/sos/libAlgorithmAlt_315634022.so \
  num_threads=4 \
  --verbose



./simulator_315634022 --comparative \
  game_map=../maps/example.txt \
  game_managers_folder=../GameManager/sos/ \
  algorithm1=../Algorithm/sos/libAlgorithm_315634022.so \
  algorithm2=../Algorithm/sos/libAlgorithmAlt_315634022.so \
  num_threads=4 \
  --verbose


  ./simulator_315634022 --competition \
  game_maps_folder=../maps \
  game_manager=../GameManager/sos/GameManager_315634022.so \
  algorithms_folder=../Algorithm/sos \
  num_threads=4 \
  --verbose





  #!/bin/bash

# Debug script for .so loading issues
echo "=== Debugging .so file loading issues ==="
echo

# 1. Check if files exist and permissions
echo "1. Checking file existence and permissions:"
SO_FOLDER="../Algorithm/sos"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "File: $so_file"
        ls -la "$so_file"
        echo "  File type: $(file "$so_file")"
        echo
    fi
done

echo "2. Checking architecture compatibility:"
echo "Your system:"
uname -m
echo "Your .so files:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "  $so_file:"
        file "$so_file" | grep -E "(x86-64|i386|ARM|aarch64)"
    fi
done
echo

echo "3. Checking shared library dependencies:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "Dependencies for $so_file:"
        if command -v ldd &> /dev/null; then
            ldd "$so_file" 2>/dev/null || echo "  ldd failed - might not be a valid shared library"
        elif command -v otool &> /dev/null; then
            # macOS
            otool -L "$so_file" 2>/dev/null || echo "  otool failed - might not be a valid shared library"
        fi
        echo
    fi
done

echo "4. Checking for missing symbols (if nm is available):"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "Symbols in $so_file:"
        if command -v nm &> /dev/null; then
            nm -D "$so_file" 2>/dev/null | head -10 || echo "  nm failed"
        fi
        echo
    fi
done

echo "5. Test loading with dlopen directly:"
cat > test_dlopen.c << 'EOF'
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <path_to_so>\n", argv[0]);
        return 1;
    }
    
    // Clear any existing errors
    dlerror();
    
    // Try to load the library
    void *handle = dlopen(argv[1], RTLD_LAZY);
    
    if (!handle) {
        printf("dlopen failed: %s\n", dlerror());
        return 1;
    }
    
    printf("Successfully loaded: %s\n", argv[1]);
    dlclose(handle);
    return 0;
}
EOF

echo "Compiling test program..."
gcc -o test_dlopen test_dlopen.c -ldl 2>/dev/null || {
    echo "Failed to compile test program (gcc not available or compilation failed)"
    echo "You can compile it manually: gcc -o test_dlopen test_dlopen.c -ldl"
}

if [ -f test_dlopen ]; then
    echo "Testing each .so file:"
    for so_file in "$SO_FOLDER"/*.so; do
        if [ -f "$so_file" ]; then
            echo "Testing $so_file:"
            ./test_dlopen "$so_file"
            echo
        fi
    done
fi

echo "6. Checking C++ ABI compatibility (if c++filt available):"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "C++ symbols in $so_file:"
        if command -v nm &> /dev/null && command -v c++filt &> /dev/null; then
            nm -D "$so_file" 2>/dev/null | grep "T " | head -5 | c++filt || echo "  No C++ symbols or tools not available"
        fi
        echo
    fi
done

# Cleanup
rm -f test_dlopen test_dlopen.c

echo "=== Debug complete ==="
echo
echo "Common fixes:"
echo "1. If architecture mismatch: Recompile .so files on the same system"
echo "2. If missing dependencies: Install missing libraries or copy them"
echo "3. If permission issues: chmod +r or chmod +x the .so files"
echo "4. If C++ ABI issues: Ensure all .so files compiled with same compiler/version"
echo "5. If symbol issues: Check that required functions are exported"