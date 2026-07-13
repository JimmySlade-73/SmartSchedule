#include "persistence/PersistenceManager.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum class Headers {
    Semester,
    Name,
    Id,
    Modality,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    TotalHeaders
};

void PersistenceManager::loadData(const std::string& path,
    int sem, int name, int id, int mod, int mon, int tues,
    int wed, int thur, int fri, int sat, int sun)
{
    std::vector<int> index_headers(static_cast<int>(Headers::TotalHeaders), -1);
    index_headers.at(static_cast<int>(Headers::Semester))   = sem;
    index_headers.at(static_cast<int>(Headers::Name))       = name;
    index_headers.at(static_cast<int>(Headers::Id))         = id;
    index_headers.at(static_cast<int>(Headers::Modality))   = mod;
    index_headers.at(static_cast<int>(Headers::Monday))     = mon;
    index_headers.at(static_cast<int>(Headers::Tuesday))    = tues;
    index_headers.at(static_cast<int>(Headers::Wednesday))  = wed;
    index_headers.at(static_cast<int>(Headers::Thursday))   = thur;
    index_headers.at(static_cast<int>(Headers::Friday))     = fri;
    index_headers.at(static_cast<int>(Headers::Saturday))   = sat;
    index_headers.at(static_cast<int>(Headers::Sunday))     = sun;
    extractPDF(path);
    std::string txt = "../resources/data/horario.txt";
    std::ifstream file(txt);
    if (!file.is_open()) {
        std::cerr << "Error; No se pudo abrir el archivo " << txt << std::endl;
        return;
    }

    std::string headers;
    std::getline(file, headers);
    
    // std::vector<std::string> headers({"semester", "name", "id", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat", "Sun", "not", "modality"});
    std::string line;
    while(std::getline(file, line)) {
        if (line == headers || line.empty()) continue;

        std::stringstream ss(line);
        std::vector<std::string> temp;
        
        std::string text;
        while(ss >> text) {
            temp.push_back(text);
        }
        for (int i = 0, i < Headers::TotalHeaders) {

        }
    }
}