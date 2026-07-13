#pragma once
#include <vector>
#include <optional>
#include <string>
#include <algorithm>
#include "data/Subject.hpp"
#include "data/SetMatter.hpp"
#include "data/Profile.hpp"
#include "persistence/PersistenceManager.hpp"

class ScheduleEngine {
private:
    SetMatter all_matter;
    std::vector<Subject> all_records;
    
    Profile profile;
    PersistenceManager persistence;

public:
    ScheduleEngine() = default;

    void loadData(const std::string& path);
    void debugCMD_General();
    std::vector<TableEntry*> getAll();
    std::vector<std::vector<std::string>> getAllStr();
    std::vector<std::string> getHeaders() { return {"Sem.", "id", "name", "Mod.", "L", "M", "M", "J", "V", "S", "D"}; }
    std::vector<std::string> getHeadersSchedule() { return {"Hour", "L", "M", "M", "J", "V", "S", "D"}; }
    std::vector<int> getSelected() { return profile.getSelected(); }
    std::vector<std::string> getCompletedMatters();
    
    void selectByIndex(int index) { if (index >= 0) profile.addSelectedByIndex(index); }
    void unselectByIndex(int ind) { profile.removeSelectedByIndex(ind); }
    void completeMatter(int ind) { if (ind >= 0) profile.addCompletedByIndex(all_records.at(ind).matter_id); }
    std::vector<int> getFiltred();
    std::vector<std::vector<std::string>> getSchedule(int start, int end);
    void quitMatterByIndex(int ind)
        { if (ind >= 0) profile.removeCompletedByIndex(ind); }
    void save(const std::string& path);
    void load(const std::string& path);
};