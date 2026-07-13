#pragma once
#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


struct Matter {
    int id;
    int semester;
    std::string name;
    std::string semesterTag;
};

inline void to_json(json& j, const Matter m) {
    j = {
        {"id",          m.id},
        {"semester",    m.semester},
        {"name",        m.name},
        {"semesterTag", m.semesterTag}
    };
}

inline void from_json(const json& j, Matter m) {
    m.id          = j["id"];
    m.semester    = j["semester"];
    m.name        = j["name"];
    m.semesterTag = j["semesterTag"];
}

class SetMatter {
private:
    std::vector<Matter> storage;
    std::vector<std::string> exeptionals_semesters;
    int current_id = 0;

public:
    auto begin() { return storage.begin(); }
    auto end()   { return storage.end();   }
    auto begin() const { return storage.begin(); }
    auto end()   const { return storage.end();   }

    void setStorage(const std::vector<Matter>& value) { storage = value; }
    void setExeptionalsSemesters(const std::vector<std::string>& value) { exeptionals_semesters = value; }
    void setCurrentId(const int& value) { current_id = value; }

    std::vector<Matter> getStorage() const { return storage; }
    std::vector<std::string> getExeptionalsSemesters() const { return exeptionals_semesters; }
    int getCurrentId() const { return current_id; }
    std::optional<Matter> getById(int id);

    int insert(std::string name, std::string semester);
};

inline void to_json(json& j, const SetMatter& set) {
    j = {
        {"storage", set.getStorage()},
        {"exeptionals_semesters", set.getExeptionalsSemesters()},
        {"current_id", set.getCurrentId()}
    };
}

inline void from_json(const json& j, SetMatter& set) {
    set.setStorage(j["storage"].get<std::vector<Matter>>());
    set.setExeptionalsSemesters(j["exeptionals_semesters"].get<std::vector<std::string>>());
    set.setCurrentId(j["current_id"]);
    
}