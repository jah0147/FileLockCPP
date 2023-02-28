//
// Created by jacob_howard on 2/27/2023.
//

#ifndef FILELOCKCPP_MAIN_H
#define FILELOCKCPP_MAIN_H

#define TESTING //comment out when not testing

//Inlcudes
#include <iostream>
#include <string>
#include <Windows.h>
#include "file_status.h"
#include "csv.h"
#include "ini.h"
#include "UI.h"
//File used to create functions that will be ran in main.cpp

//Classes
csv CSV;
file_status FileStatus;
ini INI;
UI ui;
//Global vars
fs::path dir_path = INI.get_project_directory();
std::string csv_path = INI.get_csv_path();
//Console UI
void display_console_ui()
{
    ui.display_ui();
}
//Generates/Adds files to CSV File
void generateCSV()
{

    // specify the directory path and CSV file name
    //fs::path dir_path = "C:\\Users\\jacob_howard\\CLionProjects\\FileLockCPP\\testDir"; //Dir path
    //std::string csv_filename = "file_list.csv";

    // generate the file list CSV
    CSV.generate_file_list_csv(dir_path, csv_path);
}
//Checks the file status of all files and stores them in vectors. Should print the status of all files
void checkFileStatus()
{
    // Call the locked_file_status() function to get the locked filenames and filepaths
    auto [locked_filename, locked_filepath, checked_out_name] = CSV.locked_file_status("file_list.csv");

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
//void toggleFileStatus()
//{
//    //Allow the user to select a file via the windows file explorer to toggle read/write
//    std::string filePath = FileStatus.selectFile();
//    if (filePath.empty()) { //If the user did not select a file, we stop the program
//        std::cout << "No file selected.\n";
//        //return 0;
//    }
//
//    //Takes the user selected file and toggles the attribute
//    bool success = FileStatus.toggle_file_status(filePath);
//    if (success) { //If the file was successfully toggled
//        std::cout << "File attribute toggled successfully.\n";
//    } else {
//        std::cout << "Error toggling file attribute.\n";
//    }
//}

//Sets file status to read only based on CSV file
void setFileStatusReadOnly()
{
    // Call the locked_file_status() function to get the locked filenames and filepaths
    auto [locked_filename, locked_filepath, checked_out_name] = CSV.locked_file_status("file_list.csv");

    bool setFileReadOnly = false;
    for (const auto& filepath : locked_filepath) {
        setFileReadOnly = FileStatus.make_readonly(filepath);
        //std::cout << "Set " << filename << " to Read-Only" << std::endl;
    }

}

//Sets a file to writable based on CSV file
void setFileStatusWritable()
{
    // Call the locked_file_status() function to get the locked filenames and filepaths
    std::pair<std::vector<std::string>, std::vector<std::string>> result = CSV.unlocked_file_status("file_list.csv");
    //locked_filenames = result.first; //use result.first for filename and result.second for path
    std::vector<std::string> unlocked_filepath = result.second;

    bool setFileReadOnly = false;
    for (const auto& filepath : unlocked_filepath) {
        setFileReadOnly = FileStatus.make_writable(filepath);
        //std::cout << "Set " << filename << " to Writable" << std::endl;
    }
}


#endif //FILELOCKCPP_MAIN_H
