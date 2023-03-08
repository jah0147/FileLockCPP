//
// Created by jacob_howard on 2/27/2023.
//

#ifndef FILELOCKCPP_MAIN_H
#define FILELOCKCPP_MAIN_H

//Inlcudes
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <tuple>
//File used to create functions that will be ran in main.cpp

//Classes
//csv CSV;
//file_status FileStatus;
//ini INI;
//Global vars
//fs::path dir_path = INI.get_project_directory();
//std::string csv_path = INI.get_csv_path();
//Generates/Adds files to CSV File
#include "file_status.h"
file_status FS;
void checkFileStatus(std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> locked_file_status)
{
    // Call the locked_file_status() function to get the locked filenames and filepaths
    auto [locked_filename, locked_filepath, checked_out_name] = locked_file_status; //CSV.locked_file_status("file_list.csv");

    //Comment or uncomment print loops as needed
    // Print the contents of the locked_filename vector
    std::cout << "Locked filenames:" << std::endl;
    for (const auto& filename : locked_filename) {
        std::cout << "- " << filename << std::endl;
    }
    // Print the contents of the locked_filepath vector
    std::cout << "Locked filepaths:" << std::endl;
    for (const auto& filepath : locked_filepath) {
        std::cout << "- " << filepath << std::endl;
    }
    // Print the contents of the checked_out_name vector
    std::cout << "Checked-out names:" << std::endl;
    for (const auto& name : checked_out_name) {
        std::cout << "- " << name << std::endl;
    }
}

//Currently not in use.
//Will be used later to allow a user to lock/unlock a file
//Will allow user to input their name so it auto-fills csv file when user locks a file
//Will add date from current PC clock to check-out time automatically
//Will allow a user to unlock a file and auto-remove name and date
//Current functions - Allows a user to select a file and toggle writable/read-only modes
//void toggleFileStatus(std::string filepath)
//{
//    //Allow the user to select a file via the windows file explorer to toggle read/write
//    //std::string filePath = FileStatus.selectFile();
//    if (filepath.empty()) { //If the user did not select a file, we stop the program
//        std::cout << "No file selected.\n";
//        //return 0;
//    }
//
//    //Takes the user selected file and toggles the attribute
//    bool success = FileStatus.toggle_file_status(filepath);
//    if (success) { //If the file was successfully toggled
//        std::cout << "File attribute toggled successfully.\n";
//    } else {
//        std::cout << "Error toggling file attribute.\n";
//    }
//}



#endif //FILELOCKCPP_MAIN_H
