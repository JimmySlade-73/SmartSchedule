#include "data/SetMatter.hpp"
#include <algorithm>

std::optional<Matter> SetMatter::getById(int id)
{
    for (auto& m : storage) {
        if (m.id == id) {
            return m;
        }
    }
    return std::nullopt;
}

int SetMatter::insert(std::string name, std::string semester)
{
    int s;
    try {
        s = std::stoi(semester);
    } catch (...) {
        auto ex = std::find_if(exeptionals_semesters.begin(), exeptionals_semesters.end(),
            [&](const std::string& exeptional){ return exeptional == semester; }
        );
        if (ex == exeptionals_semesters.end()) {
            exeptionals_semesters.push_back(semester);
            ex = std::prev(exeptionals_semesters.end());
        }
        int distance = std::distance(exeptionals_semesters.begin(), ex);
        s = 11 + distance;
    }

    Matter matter;
    matter.name = name;
    matter.semester = s;
    matter.semesterTag = semester;

    auto it = std::find_if(storage.begin(), storage.end(),
        [&](const Matter& m){ return m.name == matter.name && m.semester == matter.semester; }
    );
    if (it != storage.end()) {
        return it->id;
    }
    matter.id = current_id++;
    storage.push_back(matter);
    return storage.back().id;
}