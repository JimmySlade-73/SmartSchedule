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

struct TableEntry {
    virtual std::vector<std::string> getFields() const = 0;
    virtual ~TableEntry() = default;
};

struct Subject : TableEntry {
    int semester;
    std::string semesterTag;
    std::string id;
    std::string name;
    std::string modality;
    std::vector<TimeBlock> schedule;

    static std::vector<std::string> getHeaders() {
        return {"semestre", "id", "name", "modality", "L", "M", "M", "J", "V", "S", "D"};
    }

    static std::vector<std::string> getHeadersSchedule() {
        return {"Hour", "L", "M", "M", "J", "V", "S", "D"};
    }

    std::vector<std::string> getFields() const override {
        std::vector<std::string> day_fields(static_cast<int>(Day::TotalDays), "");

        for (const TimeBlock& b : schedule) {
            day_fields.at(static_cast<int>(b.day)) =
                std::to_string(b.start) + " - " + std::to_string(b.end);
        }

        std::vector<std::string> fields = { semesterTag, id, name, modality };
        fields.insert(fields.end(), day_fields.begin(), day_fields.end());
        return fields;
    }
};

inline void to_json(json& j, const Subject& s) {
    j = {
        {"semester",    s.semester},
        {"semesterTag", s.semesterTag},
        {"id",          s.id},
        {"name",        s.name},
        {"modality",    s.modality},
        {"schedule",    s.schedule}
    };
}

inline void from_json(const json& j, Subject& s) {
    s.semester    = j["semester"];
    s.semesterTag = j["semesterTag"];
    s.id          = j["id"];
    s.name        = j["name"];
    s.modality    = j["modality"];
    s.schedule    = j["schedule"].get<std::vector<TimeBlock>>();
}