//
// Created by jacob_howard on 2/27/2023.
//

#ifndef FILELOCKCPP_INI_H
#define FILELOCKCPP_INI_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

class ini {
public:
    //Returns project directory path from settings.ini file
    fs::path get_project_directory();
    //Returns csv path from settings.ini file
    std::string get_csv_path();
    //Write user to ini file
    void write_user(std::string user_name);
    //Gets user if available
    std::string get_ini_value(std::string set_key);
    //Clear a value in ini file
    void clear_value(std::string key);

private:
    // Specify the path to the settings.ini file
    fs::path settings_path = "C:\\dev\\FileLockCPP\\FileLockCPP_Settings.ini";
};


#endif //FILELOCKCPP_INI_H
