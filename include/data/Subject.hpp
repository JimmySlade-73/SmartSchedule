#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "data/TimeBlock.hpp"

struct TableEntry {
    virtual ~TableEntry() = default;
};

struct Subject : TableEntry {
    std::string id;
    int matter_id;
    std::string modality;
    std::vector<TimeBlock> schedule;

    std::vector<std::string> getScheduleFields() const {
        std::vector<std::string> day_fields(static_cast<int>(Day::TotalDays), "");

        for (const TimeBlock& b : schedule) {
            day_fields.at(static_cast<int>(b.day)) =
                std::to_string(b.start) + " - " + std::to_string(b.end);
        }
        return day_fields;
    }
};

inline void to_json(json& j, const Subject& s) {
    j = {
        {"id",          s.id},
        {"matter_id",   s.matter_id},
        {"modality",    s.modality},
        {"schedule",    s.schedule}
    };
}

inline void from_json(const json& j, Subject& s) {
    s.id                 = j["id"];
    s.matter_id          = j["matter_id"];
    s.modality           = j["modality"];
    s.schedule           = j["schedule"].get<std::vector<TimeBlock>>();
}