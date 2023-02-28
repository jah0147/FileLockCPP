#ifndef FILE_ATTRIBUTE_UTILS_H
#define FILE_ATTRIBUTE_UTILS_H

#include <string>
#include <Windows.h>

class file_status {
public:

    //Function that allows the user to select a file via the windows explorer
    std::string selectFile();
    //Function that toggles the attribute of a file to writable or read-only
    static bool toggle_file_status(const std::string& filePath);

    static bool make_readonly(const std::string& filePath);
    static bool make_writable(const std::string& filePath);
};

#endif // FILE_ATTRIBUTE_UTILS_H
