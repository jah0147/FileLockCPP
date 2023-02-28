//
// Created by jacob_howard on 2/27/2023.
//

#include "ini.h"

fs::path ini::get_project_directory() {
    // Check if the file exists
    if (!fs::exists(settings_path)) {
        std::cerr << "Settings file not found!" << std::endl;
    }

    // Open the file for reading
    std::ifstream settings_file(settings_path);

    // Check if the file was opened successfully
    if (!settings_file.is_open()) {
        std::cerr << "Failed to open settings file!" << std::endl;
    }

    // Read the contents of the file into a string
    std::stringstream buffer;
    buffer << settings_file.rdbuf();
    std::string contents = buffer.str();

    // Parse the contents of the file
    // In this example, we assume that each line of the file contains a key-value pair separated by an equals sign (=)
    std::stringstream ss(contents);
    std::string line;
    fs::path project_dir_path;
    while (std::getline(ss, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "project_directory") {
                //std::cout << "Input path: " << value << std::endl;
                project_dir_path = value;
                break;
            }
        }
    }

    // Close the file
    settings_file.close();
    return project_dir_path;
}

std::string ini::get_csv_path()
{
    // Check if the file exists
    if (!fs::exists(settings_path)) {
        std::cerr << "Settings file not found!" << std::endl;
    }

    // Open the file for reading
    std::ifstream settings_file(settings_path);

    // Check if the file was opened successfully
    if (!settings_file.is_open()) {
        std::cerr << "Failed to open settings file!" << std::endl;
    }

    // Read the contents of the file into a string
    std::stringstream buffer;
    buffer << settings_file.rdbuf();
    std::string contents = buffer.str();

    // Parse the contents of the file
    // In this example, we assume that each line of the file contains a key-value pair separated by an equals sign (=)
    std::stringstream ss(contents);
    std::string line;
    std::string csv_path;
    while (std::getline(ss, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "csv_path") {
                //std::cout << "Input path: " << value << std::endl;
                csv_path = value;
                break;
            }
        }
    }

    // Close the file
    settings_file.close();
    return csv_path;
}