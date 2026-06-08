#include "data/SubjectTable.hpp"
#include <algorithm>

void SubjectTable::addRecord(const Subject& subject)
{
    records.push_back(subject);
}

std::optional<Subject> SubjectTable::findByClassId(const std::string& classId) const {
    auto it = std::find_if(records.begin(), records.end(), [&](const Subject& s) {
        return s.id == classId;
    });

    if (it != records.end()) {
        return *it;
    }

    return std::nullopt;
}