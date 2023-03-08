//
// Created by jacob_howard on 2/27/2023.
//

#include "ini.h"
// Gets the project directory from the ini file
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
// Gets the csv path from the ini file
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
// Writes the username to the ini file
void ini::write_user(std::string user_name) {
    // Clears user_name value before writing
    clear_value("user_name");

    std::fstream file(settings_path, std::ios::in | std::ios::out);

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    std::string key = "user_name";
    std::size_t pos = content.find(key + "=");

    if (pos != std::string::npos) {
        //std::string new_value = "new_value";
        std::size_t end_pos = content.find_first_of("\r\n", pos);
        content.replace(pos + key.length() + 1, end_pos - pos - key.length() - 1,
                        user_name);
    }

    file.seekp(0, std::ios::beg);
    file << content;
    file.close();
}
// Clears the specified value in the ini file
void ini::clear_value(std::string key)
{
    //std::string key = "my_key";
    //std::string ini_file = "example.ini";
    std::string line;
    std::string output;
    bool found_key = false;

    std::ifstream input_file(settings_path);
    while (std::getline(input_file, line)) {
        if (line.substr(0, key.length()) == key) {
            found_key = true;
            // Replace existing value with an empty string
            line = key + "=";
        }
        output += line + "\n";
    }
    input_file.close();

    if (!found_key) {
        // If key not found, add it with an empty value
        output += key + "=\n";
    }

    std::ofstream output_file(settings_path);
    output_file << output;
    output_file.close();
}
// Gets the current user from the ini
std::string ini::get_ini_value(std::string set_key)
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
    std::string user_name;
    while (std::getline(ss, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == set_key) {
                //std::cout << "Input path: " << value << std::endl;
                user_name = value;
                break;
            }
        }
    }

    // Close the file
    settings_file.close();
    return user_name;
}