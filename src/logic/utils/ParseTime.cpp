#include "logic/utils/ParseTime.hpp"

std::optional<TimeBlock> parseTime(const Day& day, const std::string& str)
{
    if (str == ".") { return std::nullopt; }

    std::string delimeter = "..";
    size_t pos = str.find(delimeter);

    if (pos == std::string::npos) { return std::nullopt; }

    try {
        int start = std::stoi(str.substr(0, pos));
        int end = std::stoi(str.substr(pos + delimeter.length()));
        return TimeBlock{day, start, end};
    } catch (...) { return std::nullopt; }
}

bool haveCollision(TimeBlock block1, TimeBlock block2)
{
    if (block1.day != block2.day) return false;
    if ((block2.start <= block1.start && block1.start < block2.end) ||
        (block2.start < block1.end && block1.end <= block2.end) ||
        (block1.start <= block2.start && block2.start < block1.end)
    ) return true;
    return false;
}

bool haveCollision(Subject sub1, Subject sub2)
{
    if (sub1.name == sub2.name) return true;
    for (auto& block1 : sub1.schedule) {
        for (auto& block2 : sub2.schedule) {
            if (haveCollision(block1, block2)) return true;
        }
    }
    return false;
}