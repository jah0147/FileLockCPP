#ifndef FILE_ATTRIBUTE_UTILS_H
#define FILE_ATTRIBUTE_UTILS_H

#include <string>
#include <Windows.h>
#include <vector>
#include <tuple>

class file_status {
public:

    //Function that allows the user to select a file via the windows explorer
    std::string selectFile();
    //Function that toggles the attribute of a file to writable or read-only
    static bool toggle_file_status(const std::string& filePath);

    static bool make_readonly(const std::string& filePath);

    bool make_writable(std::string& filePath);
    //Sets file status to read only based on CSV file
    void set_csv_files_readOnly(std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> locked_file_status);
    //Sets a file to writable based on CSV file
    void set_csv_files_writable(std::pair<std::vector<std::string>, std::vector<std::string>> unlocked_file_status);
};

#endif // FILE_ATTRIBUTE_UTILS_H
