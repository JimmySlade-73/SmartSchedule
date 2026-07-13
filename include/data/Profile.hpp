#pragma once
#include <string>
#include <vector>

class Profile {
private:
    std::vector<int> selected;
    std::vector<int> completed_matters;

public:
    void setSelected(std::vector<int> value)  { selected = value; }
    void setCompleted(std::vector<int> value) { completed_matters = value; }

    std::vector<int> getSelected()  const { return selected; }
    std::vector<int> getCompleted() const { return completed_matters; }

    void addSelectedByIndex(int value)  { selected.push_back(value); }
    void addCompletedByIndex(int value) { completed_matters.push_back(value); }

    void removeSelectedByIndex(int ind)  { std::erase_if(selected, [&ind](const int& i){ return ind == i; }); }
    void removeCompletedByIndex(int ind) { completed_matters.erase(completed_matters.begin() + ind); }
};

inline void to_json(json& j,const Profile& p) {
    j = {
        {"selected",  p.getSelected() },
        {"completed", p.getCompleted()}
    };
}

inline void from_json(const json& j, Profile& p) {
    p.setCompleted(j["completed"].get<std::vector<int>>());
    p.setSelected(j["selected"].get<std::vector<int>>());
}