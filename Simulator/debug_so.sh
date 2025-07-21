#!/bin/bash

echo "=== Troubleshooting Existing .so Files ==="
echo

SO_FOLDER="../Algorithm/sos"
WORKING_SO="../GameManager/sos/GameManager_315634022.so"

# 1. Basic file information
echo "1. Comparing your working .so with coworkers' .so files:"
echo
echo "Your working file:"
if [ -f "$WORKING_SO" ]; then
    ls -la "$WORKING_SO"
    file "$WORKING_SO"
else
    echo "  Working .so not found at $WORKING_SO"
fi

echo
echo "Coworkers' files:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "$(basename "$so_file"):"
        ls -la "$so_file"
        file "$so_file"
        echo
    fi
done

# 2. Architecture compatibility check
echo "2. Architecture compatibility:"
echo "System architecture: $(uname -m)"
echo
if [ -f "$WORKING_SO" ]; then
    echo "Your working .so architecture:"
    file "$WORKING_SO" | grep -E "(x86-64|i386|ARM|aarch64|Mach-O)"
fi

echo "Coworkers' .so architectures:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        echo "  $(basename "$so_file"): $(file "$so_file" | grep -E "(x86-64|i386|ARM|aarch64|Mach-O)" | cut -d: -f2-)"
    fi
done

echo
echo "3. Detailed dlopen error analysis:"

# Create a more detailed dlopen test
cat > detailed_dlopen_test.c << 'EOF'
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

void test_dlopen_modes(const char* filename) {
    printf("\n=== Testing %s ===\n", filename);
    
    // Test different dlopen modes
    const char* modes[] = {"RTLD_LAZY", "RTLD_NOW", "RTLD_LAZY|RTLD_LOCAL", "RTLD_NOW|RTLD_GLOBAL"};
    int mode_flags[] = {RTLD_LAZY, RTLD_NOW, RTLD_LAZY|RTLD_LOCAL, RTLD_NOW|RTLD_GLOBAL};
    
    for (int i = 0; i < 4; i++) {
        dlerror(); // Clear errors
        void *handle = dlopen(filename, mode_flags[i]);
        
        if (handle) {
            printf("✓ SUCCESS with %s\n", modes[i]);
            dlclose(handle);
            return;
        } else {
            printf("✗ FAILED with %s: %s\n", modes[i], dlerror());
        }
    }
    
    // Try with full path
    char fullpath[1024];
    if (realpath(filename, fullpath)) {
        printf("\nTrying with absolute path: %s\n", fullpath);
        dlerror();
        void *handle = dlopen(fullpath, RTLD_LAZY);
        if (handle) {
            printf("✓ SUCCESS with absolute path\n");
            dlclose(handle);
        } else {
            printf("✗ FAILED with absolute path: %s\n", dlerror());
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <so_file1> [so_file2] ...\n", argv[0]);
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        test_dlopen_modes(argv[i]);
    }
    
    return 0;
}
EOF

echo "Compiling detailed dlopen test..."
gcc -o detailed_dlopen_test detailed_dlopen_test.c -ldl 2>/dev/null

if [ -f detailed_dlopen_test ]; then
    echo "Running detailed tests on problematic .so files:"
    ./detailed_dlopen_test "$SO_FOLDER"/Algorithm_322573304_322647603.so "$SO_FOLDER"/Algorithm_322213836_212054837.so
else
    echo "Failed to compile detailed test. Trying simple approach..."
fi

# 4. Check dependencies
echo
echo "4. Checking library dependencies:"

check_dependencies() {
    local so_file="$1"
    echo "Dependencies for $(basename "$so_file"):"
    
    if command -v ldd >/dev/null 2>&1; then
        # Linux
        ldd "$so_file" 2>/dev/null || echo "  ldd failed - not a valid shared library or wrong architecture"
    elif command -v otool >/dev/null 2>&1; then
        # macOS
        otool -L "$so_file" 2>/dev/null || echo "  otool failed - not a valid shared library or wrong architecture"
    else
        echo "  No dependency checker available (ldd/otool)"
    fi
    echo
}

if [ -f "$WORKING_SO" ]; then
    echo "Your working .so dependencies:"
    check_dependencies "$WORKING_SO"
fi

echo "Coworkers' .so dependencies:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        check_dependencies "$so_file"
    fi
done

# 5. Symbol analysis
echo "5. Exported symbols comparison:"

analyze_symbols() {
    local so_file="$1"
    local name="$(basename "$so_file")"
    
    echo "$name symbols:"
    if command -v nm >/dev/null 2>&1; then
        local symbols=$(nm -D "$so_file" 2>/dev/null | grep "T " | wc -l)
        if [ "$symbols" -gt 0 ]; then
            echo "  Exported functions: $symbols"
            echo "  Sample functions:"
            nm -D "$so_file" 2>/dev/null | grep "T " | head -5 | sed 's/^/    /'
        else
            echo "  No exported symbols found or nm failed"
        fi
    else
        echo "  nm not available for symbol analysis"
    fi
    echo
}

if [ -f "$WORKING_SO" ]; then
    echo "Your working .so:"
    analyze_symbols "$WORKING_SO"
fi

echo "Coworkers' .so files:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        analyze_symbols "$so_file"
    fi
done

# 6. File integrity check
echo "6. File integrity check:"
for so_file in "$SO_FOLDER"/*.so; do
    if [ -f "$so_file" ]; then
        local name="$(basename "$so_file")"
        local size=$(stat -c%s "$so_file" 2>/dev/null || stat -f%z "$so_file" 2>/dev/null)
        echo "$name: $size bytes"
        
        # Check if file is readable
        if [ -r "$so_file" ]; then
            echo "  ✓ Readable"
        else
            echo "  ✗ Not readable - permission issue"
        fi
        
        # Try to read first few bytes
        if xxd -l 16 "$so_file" >/dev/null 2>&1; then
            echo "  ✓ File appears intact"
            echo "  Header: $(xxd -l 16 -p "$so_file" | tr -d '\n')"
        else
            echo "  ✗ File might be corrupted"
        fi
        echo
    fi
done

# Cleanup
rm -f detailed_dlopen_test detailed_dlopen_test.c

echo "=== Analysis Complete ==="
echo
echo "Common solutions based on findings:"
echo "1. Architecture mismatch → Ask coworkers to recompile on your system type"
echo "2. Missing dependencies → Install missing libraries or get static build"
echo "3. File corruption → Re-download/copy the .so files"
echo "4. Permission issues → chmod +r *.so"
echo "5. Wrong file format → Verify these are actually shared libraries"
echo "6. C++ ABI issues → All .so files must use same compiler/stdlib version"