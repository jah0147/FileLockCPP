//
// Created by jacob_howard on 2/23/2023.
//

#ifndef FILELOCKCPP_CSV_H
#define FILELOCKCPP_CSV_H
#pragma once
#include <string>
#include <filesystem>
#include <list>

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
/**
 * @brief Generates vectors that store file information if they are listed as locked
 *
 * @param filename The filename/path of the CSV file.
 */
    std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> locked_file_status(const std::string& filename);
/**
 * @brief Generates vectors that store file information if they are listed as unlocked
 *
 * @param filename The filename/path of the CSV file.
 */
    std::pair<std::vector<std::string>, std::vector<std::string>> unlocked_file_status(const std::string& filename);
private:

};


#endif //FILELOCKCPP_CSV_H
