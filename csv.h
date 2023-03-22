//
// Created by jacob_howard on 2/23/2023.
//

#ifndef FILELOCKCPP_CSV_H
#define FILELOCKCPP_CSV_H
#pragma once
#include <string>
#include <filesystem>
#include <list>
#include <ctype.h>
#include "ini.h"

namespace fs = std::filesystem;


class csv {
public:


/**
 * @brief Generates a CSV file with file names and paths from a specified directory.
 *
 * @param dir_path The path to the directory to generate the file list CSV for.
 * @param csv_filename The filename for the generated CSV file.
 */
    void generate_file_list_csv(const fs::path& dir_path, const std::string& csv_filename);

    void remove_git_files_from_csv(const std::string& filePath);
/**
 * @brief Generates vectors that store file information if they are listed as locked
 *
 * @param filename The filename/path of the CSV file.
 */
    static std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> locked_file_status(const std::string& filename);
/**
 * @brief Generates vectors that store file information if they are listed as unlocked
 *
 * @param filename The filename/path of the CSV file.
 */
    std::pair<std::vector<std::string>, std::vector<std::string>> unlocked_file_status(const std::string& filename);
    void unlock_users_files(const std::string& filename, const std::string& check_name);
    void print_locked_file_status();
    void print_usr_locked_file_status();
    std::pair<std::string, std::string> commit_and_push_CSV(const std::string& filePath, const std::string& userName);
    //Checks if the user is the ownder of the file
    bool is_user_owner(std::string filepath, std::string file, std::string user);
    //Writes lock data into csv file
    void write_lock_data(const std::string& filepath, const std::string& filename, const std::string& user_name);
    // Clears lock data (i.e. user unlocks a file if it belongs to them. Use the is_user_owner func to check)
    void clear_lock_data(const std::string& filepath, const std::string& filename);
private:
    //Set Strings Upper Case
    inline std::string set_str_upper(std::string string)
    {
        if (!string.empty()) {
            for (char &c: string) {
                c = toupper(c);
            }
        }
        return string;
    };
    ini INI_CSV;
    std::string csv_path = INI_CSV.get_csv_path();

};


#endif //FILELOCKCPP_CSV_H
