#include "logic/ScheduleEngine.hpp"
#include "logic/utils/ParseTime.hpp"
#include "data/Subject.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

void ScheduleEngine::loadData(const std::string& path)
{
    persistence.extractPDF(path);
    std::string txt = "../resources/data/horario.txt";
    std::ifstream file(txt);
    if (!file.is_open()) {
        std::cerr << "Error; No se pudo abrir el archivo " << txt << std::endl;
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

        std::string name("");
        for (int i = 1; i < temp.size() - 2 - static_cast<int>(Day::TotalDays); i++) {
            name += (i > 1 ? " " : "") + temp.at(i);
        }
        s.matter_id = all_matter.insert(name, temp.at(0));
        all_records.push_back(s);
    }
}

void ScheduleEngine::debugCMD_General()
{
    for (auto& s : all_records) {
        // print
        auto matter = all_matter.getById(s.matter_id);
        std::cout << "ID: " << s.id
        << "|Modality: " << s.modality
        << "|Name: " << matter->name;
        std::cout << "|Horario: ";
        for (const auto& sch : s.schedule){
            std::cout << sch;
        }
        std::cout << "|semestre: " << matter->semesterTag
        << std::endl;
    }
}

std::vector<TableEntry*> ScheduleEngine::getAll() {
    std::vector<TableEntry*> ptrs;
    for (auto& s : all_records) ptrs.push_back(&s);
    return ptrs;
}

std::vector<std::vector<std::string>> ScheduleEngine::getAllStr()
{
    std::vector<std::vector<std::string>> result;
    for (auto& s : all_records) {
        std::vector<std::string> row;
        auto matter = all_matter.getById(s.matter_id);
        
        row.push_back(matter->semesterTag);
        row.push_back(s.id);
        row.push_back(matter->name);
        row.push_back(s.modality);
        auto day_fields = s.getScheduleFields();
        row.insert(row.end(), day_fields.begin(), day_fields.end());

        result.push_back(row);
    }
    return result;
}

std::vector<std::string> ScheduleEngine::getCompletedMatters()
{
    std::vector<std::string> completed;
    for (auto& id : profile.getCompleted()) {
        completed.push_back(all_matter.getById(id)->name);
    }
    return completed;
}

std::vector<int> ScheduleEngine::getFiltred()
{
    std::vector<int> filtred;
    for (int i = 0; i < all_records.size(); i++) {
        bool is_compatible = true;
        for (int index : profile.getSelected()) {
            if (haveCollision(all_records.at(i), all_records.at(index))){
                is_compatible = false;
                break;
            }
        }
        if (is_compatible) filtred.push_back(i);
    }
    for (int m : profile.getCompleted()) {
        std::erase_if(filtred, [this, &m](const int& n){ return m == all_records.at(n).matter_id; });
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
            for (int index : profile.getSelected()) {
                for (auto block : all_records.at(index).schedule) {
                    if (haveCollision(block, {static_cast<Day>(i), h, h + 1})) {
                        
                        str += "";
                        str += all_matter.getById(all_records.at(index).matter_id)->name;
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
    auto path1 = path + "profile.json";
    persistence.save(profile, path1);
    auto path2 = path + "all_matter.json";
    persistence.save(all_matter, path2);
    auto path3 = path + "all_records.json";
    persistence.save(all_records, path3);
}

void ScheduleEngine::load(const std::string& path)
{
    auto path1 = path + "profile.json";
    persistence.load(profile, path1);
    auto path2 = path + "all_matter.json";
    persistence.load(all_matter, path2);
    auto path3 = path + "all_records.json";
    persistence.load(all_records, path3);
}