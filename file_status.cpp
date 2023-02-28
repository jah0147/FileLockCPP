//
// Created by jacob_howard on 2/23/2023.
//
#include "file_status.h"
#include "csv.h"

//Function that allows the user to select a file via the windows explorer
std::string file_status::selectFile()
{
    OPENFILENAME ofn; // Declare an OPENFILENAME struct to hold the file selection dialog options
    char szFile[260] = { 0 }; // Create a buffer to hold the selected file path
    ZeroMemory(&ofn, sizeof(ofn)); // Initialize the struct to zero
    ofn.lStructSize = sizeof(ofn); // Set the size of the struct
    ofn.lpstrFile = szFile; // Set the buffer for the selected file path
    ofn.nMaxFile = sizeof(szFile); // Set the maximum size of the buffer
    ofn.lpstrFilter = "All Files (*.*)\0*.*\0"; // Set the file type filter for the dialog
    ofn.lpstrTitle = "Select a file"; // Set the title of the dialog
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Set options for the dialog
    if (GetOpenFileName(&ofn) == FALSE) {
        return ""; // User canceled the dialog
    }
    return std::string(szFile);
}

//Function that toggles the attribute of a file to writable or read-only
bool file_status::toggle_file_status(const std::string& filePath)
{
    DWORD fileAttributes = GetFileAttributes(filePath.c_str()); // Get the attributes of the selected file
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) { // Check if the attributes could not be retrieved
        return false;
    }
    // Toggle read-only attribute
    if (fileAttributes & FILE_ATTRIBUTE_READONLY) { // Check if the read-only attribute is set
        // File is read-only, so make it writable
        if (SetFileAttributes(filePath.c_str(), fileAttributes & ~FILE_ATTRIBUTE_READONLY) == FALSE) { // Clear the read-only attribute and check if the operation failed
            return false;
        }
    } else {
        // File is writable, so make it read-only
        if (SetFileAttributes(filePath.c_str(), fileAttributes | FILE_ATTRIBUTE_READONLY) == FALSE) { // Set the read-only attribute and check if the operation failed
            return false;
        }
    }

    return true;
}
//Function that sets a file to read-only
bool file_status::make_readonly(const std::string &filePath)
{
    DWORD fileAttributes = GetFileAttributes(filePath.c_str()); // Get the attributes of the selected file
    if (SetFileAttributes(filePath.c_str(), fileAttributes | FILE_ATTRIBUTE_READONLY) == FALSE) { // Set the read-only attribute and check if the operation failed
        return false;
    }
    return true;
}
//Function that sets a file to writable
bool file_status::make_writable(const std::string &filePath) {
    DWORD fileAttributes = GetFileAttributes(filePath.c_str()); // Get the attributes of the selected file
    // File is read-only, so make it writable
    if (SetFileAttributes(filePath.c_str(), fileAttributes & ~FILE_ATTRIBUTE_READONLY) == FALSE) { // Clear the read-only attribute and check if the operation failed
        return false;
    }
    return true;
}