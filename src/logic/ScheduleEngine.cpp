#include "logic/ScheduleEngine.hpp"
#include "logic/utils/ParseTime.hpp"
#include "data/Subject.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

void ScheduleEngine::loadData(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error; No se pudo abrir el archivo " << path << std::endl;
        return;
    }

    std::string headers;
    std::getline(file, headers);
    
    std::string line;
    while(std::getline(file, line)) {
        if (line == headers || line.empty()) continue;

        std::stringstream ss(line);
        Subject s;
        std::vector<std::string> temp;
        
        std::string text;
        while(ss >> text) {
            temp.push_back(text);
        }

        s.modality = temp.at(temp.size() - 1);

        for (int i = 0; i < static_cast<int>(Day::TotalDays); i++) {
            int index = temp.size() + i - 1 - static_cast<int>(Day::TotalDays);
            std::string schedule_str = temp.at(index);
            if (auto block = parseTime(static_cast<Day>(i), schedule_str)) {
                s.schedule.push_back(*block);
            }
        }

        s.id = temp.at(temp.size() - 2 - static_cast<int>(Day::TotalDays));

        s.semester = (temp.at(0) == "Electiva") ? 11 : std::stoi(temp.at(0));
        s.semesterTag = temp.at(0);

        std::string name("");
        for (int i = 1; i < temp.size() - 2 - static_cast<int>(Day::TotalDays); i++) {
            name += (i > 1 ? " " : "") + temp.at(i);
        }
        s.name = name;
        all_records.push_back(s);
        std::cout << "ID: " << s.id
        << "|Modality: " << s.modality
        << "|Name: " << s.name;
        std::cout << "|Horario: ";
        for (const auto& sch : s.schedule){
            std::cout << sch;
        }
        std:: cout << "|semestre: " << s.semesterTag
        << std::endl;
    }
}

std::vector<TableEntry*> ScheduleEngine::getAll() {
    std::vector<TableEntry*> ptrs;
    for (auto& s : all_records) ptrs.push_back(&s);
    return ptrs;
}

std::vector<int> ScheduleEngine::getFiltred()
{
    std::vector<int> filtred;
    for (int i = 0; i < all_records.size(); i++) {
        bool is_compatible = true;
        for (int index : selected) {
            if (haveCollision(all_records.at(i), all_records.at(index))){
                is_compatible = false;
                break;
            }
        }
        if (is_compatible) filtred.push_back(i);
    }
    for (std::string str : completed_matters) {
        std::erase_if(filtred, [this, &str](const int& n){ return str == all_records.at(n).name; });
    }
    return filtred;
}

std::vector<std::vector<std::string>> ScheduleEngine::getSchedule(int start, int end)
{
    std::vector<std::vector<std::string>> schedule;
    for (int h = start; h < end; h++) {
        std::vector<std::string> hour;
        for (int i = -1;  i < static_cast<int>(Day::TotalDays); i++) {
            std::string str = "";
            if (i == -1) {
                str = std::to_string(h);
                hour.push_back(str);
                continue;
            }
            for (int index : selected) {
                for (auto block : all_records.at(index).schedule) {
                    if (haveCollision(block, {static_cast<Day>(i), h, h + 1})) {
                        
                        str += "";
                        str += all_records.at(index).name;
                    }
                }
            }
            hour.push_back(str);
        }
        schedule.push_back(hour);
    }
    std::cout << "inicia" << std::endl;
    for (auto i: schedule) {
        for (auto j: i) {
            std::cout << j << " | ";
        }
        std::cout << std::endl;
    }
    return schedule;
}

void ScheduleEngine::save(const std::string& path)
{
    std::cout << "Saving to: "
              << std::filesystem::absolute(path)
              << '\n';

    json data = {
        {"all_records"      , all_records},
        {"selected"         , selected},
        {"completed_matters", completed_matters}
    };
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not create file: " << path << std::endl;
        return;
    }
    file << data.dump(4);
}

void ScheduleEngine::load(const std::string& path)
{
    std::cout << "Loading to: "
              << std::filesystem::absolute(path)
              << '\n';
        
    std::ifstream file(path);
    if (!file.is_open()) return;

    json data = json::parse(file);
    all_records       = data["all_records"].get<std::vector<Subject>>();
    selected          = data["selected"].get<std::vector<int>>();
    completed_matters = data["completed_matters"].get<std::vector<std::string>>();
}