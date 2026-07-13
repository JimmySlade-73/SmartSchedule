#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum class Day {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    TotalDays
};

struct TimeBlock {
    Day day;
    int start;
    int end;
};

inline void to_json(json& j, const TimeBlock& b) {
    j = {
        {"day",   static_cast<int>(b.day)},
        {"start", b.start},
        {"end",   b.end}
    };
}
inline void from_json(const json& j, TimeBlock& b) {
    b.day   = static_cast<Day>(j["day"].get<int>());
    b.start = j["start"];
    b.end   = j["end"];
}

inline std::ostream& operator<<(std::ostream& os, const TimeBlock& block) {
    os << "[Dia: " << static_cast<int>(block.day) 
       << " | " << block.start << " - " << block.end << "]";
    return os;
}