#pragma once
#include <vector>
#include <optional>
#include <string>
#include "data/Subject.hpp"
#include <algorithm>

class ScheduleEngine {
private:
    std::vector<Subject> all_records;
    std::vector<int> selected;
    std::vector<std::string> completed_matters;

public:
    ScheduleEngine() = default;

    void loadData(const std::string& path);
    std::vector<TableEntry*> getAll();
    std::vector<int> getSelected() { return selected; }
    std::vector<std::string> getCompletedMatters() { return completed_matters; }
    void selectByIndex(int index) { if (index >= 0) selected.push_back(index); }
    void unselectByIndex(int ind) { std::erase_if(selected, [&ind](const int& i){ return ind == i; }); }
    void completeMatter(int ind) { if (ind >= 0) completed_matters.push_back(all_records.at(ind).name); }
    std::vector<int> getFiltred();
    std::vector<std::vector<std::string>> getSchedule(int start, int end);
    void quitMatterByIndex(int ind)
        { if (ind >= 0) completed_matters.erase(completed_matters.begin() + ind); }
    void save(const std::string& path)
    void load(const std::string& path)
};