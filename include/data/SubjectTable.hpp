#pragma once
#include <vector>
#include <string>
#include <optional>
#include "Subject.hpp"

class SubjectTable {
private:
    std::vector<Subject> records;

public:
    SubjectTable() = default;
    void addRecord(const Subject& subject);
    const std::vector<Subject>& getAllRecords() const { return records; };
    std::optional<Subject> findByClassId(const std::string& classId) const;
    void clear() { records.clear(); };
};