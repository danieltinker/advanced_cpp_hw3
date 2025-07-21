#pragma once

#include <iostream>
#include <string>

namespace UserCommon_315634022 {

inline void debug(const std::string& msg) {
    if (cfg.verbose) {
    std::cerr << "[DEBUG] " << msg << std::endl;
    }
}

} // namespace UserCommon_315634022



// # Check what your working .so exports
// nm -D ../GameManager/sos/GameManager_315634022.so | grep "T "

// # Check what their .so files export  
// nm -D ../Algorithm/sos/Algorithm_322573304_322647603.so | grep "T "

// # Compare architectures
// file ../GameManager/sos/GameManager_315634022.so
// file ../Algorithm/sos/Algorithm_322573304_322647603.so

// # Check dependencies
// ldd ../Algorithm/sos/Algorithm_322573304_322647603.so  # Linux
// otool -L ../Algorithm/sos/Algorithm_322573304_322647603.so  # macOS