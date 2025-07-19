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
