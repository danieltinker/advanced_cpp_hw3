#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include "ActionRequest.h"
/*
  Utility: parse lines like "Key = Value" and convert Value → size_t.
*/
namespace arena {
inline bool parseKeyValue(const std::string& line,
                          const std::string& key,
                          std::size_t& outVar)
{
    auto pos = line.find('=');
    if (pos == std::string::npos) return false;
    std::string lhs = line.substr(0, pos);
    std::string rhs = line.substr(pos + 1);

    auto trim = [](std::string& s) {
        std::size_t a = 0;
        while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
        std::size_t b = s.size();
        while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
        s = s.substr(a, b - a);
    };
    trim(lhs);
    trim(rhs);
    if (lhs != key) return false;
    try {
        outVar = static_cast<std::size_t>(std::stoul(rhs));
        return true;
    } catch (...) {
        return false;
    }
}

/*
  Convert an ActionRequest enum to exactly the string the output log expects.
*/
inline std::string actionToString(ActionRequest a) {
    switch (a) {
        case ActionRequest::MoveForward:   return "MoveForward";
        case ActionRequest::MoveBackward:  return "MoveBackward";
        case ActionRequest::RotateLeft90:  return "RotateLeft90";
        case ActionRequest::RotateRight90: return "RotateRight90";
        case ActionRequest::RotateLeft45:  return "RotateLeft45";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::Shoot:         return "Shoot";
        case ActionRequest::GetBattleInfo: return "GetBattleInfo";
        case ActionRequest::DoNothing:     return "DoNothing";
        default:                                   return "DoNothing";
    }
}
}