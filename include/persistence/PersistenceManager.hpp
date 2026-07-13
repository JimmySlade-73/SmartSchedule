#pragma once
#include <optional>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class PersistenceManager {
public:
    void loadData(const std::string& path, int sem, int name, int id, int mod, int mon, int tues, int wed, int thur, int fri, int sat, int sun);

    template<typename T>
    void save(const T& it, const std::string& path) {
        std::cout << "Saving to: "
                  << std::filesystem::absolute(path)
                  << '\n';

        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Could not create file: " << path << std::endl;
            return;
        }
        json data = it;
        file << data.dump(4);
    }

    template<typename T>
    void load(T& it, const std::string& path) {
        std::cout << "Loading from: "
                  << std::filesystem::absolute(path)
                  << '\n';

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << path << std::endl;
            return;
        }
        json data = json::parse(file);
        it = data.get<T>();
    }

    bool extractPDF(const std::string& pdfPath) {
        std::string python = "C:\\Users\\Slade\\AppData\\Local\\Programs\\Python\\Python314\\python.exe";
        std::string script = "../include/persistence/utils/LoadFromPDF.py";
        std::string cmd = python + " " + script + " \"" + pdfPath + "\"";
        int result = std::system(cmd.c_str());
        return result == 0;
    }

};