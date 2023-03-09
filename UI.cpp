//
// Created by jacob_howard on 2/28/2023.
//
#include <iostream>
#include <string>
#include <conio.h>
#include "csv.h"
#include "UI.h"
#include "ini.h"
#include "file_status.h"

ini INI_UI;
csv CSV_UI;
file_status FS_UI;

std::string csv_path = INI_UI.get_ini_value("csv_path");
std::string user_name = INI_UI.get_ini_value("user_name");
// Main UI displaying function - Displays all UI based on cases
void UI::display_ui()
{
    system("cls");
    char input;
    bool running = true;
    display_home_menu();

    while (running) {

        //Set input to a key listener
        input = _getch();
        std::cout << input << "\n";

        switch (input) {
            case '1': //Set User
            display_set_user();
            //Lock files since user may have changed
            FS_UI.set_csv_files_readOnly(CSV_UI.locked_file_status(csv_path));
            // Set files that need to be read only to writable
            FS_UI.set_csv_files_writable(CSV_UI.unlocked_file_status(csv_path));
            // Set files that need to be read only to read only
            //setFileStatusReadOnly(CSV.locked_file_status(csv_path));
            FS_UI.set_csv_files_readOnly(CSV_UI.locked_file_status(csv_path));
            user_name = INI_UI.get_ini_value("user_name");
            //unlock the users files
            CSV_UI.unlock_users_files(csv_path, user_name);

            // Clear console screen
            system("cls");
            display_home_menu(); //We should display the home menu after
                break;
            case '2': //Display a list of all locked files
                // Clear console screen
                system("cls");
                //Print art
                display_text_art();
                std::cout <<std::endl<<"\nPress " <<ANSI_COLOR_RED<< "[Q] " <<ANSI_COLOR_RESET<< "to exit this menu\n" << std::endl;
                //Display all the locked files
                display_locked_files();
                input = _getch(); //Listens for user key-click
                //Waits until user exits
                while (input != 'q'){
                    input = _getch(); //Listens for user key-click
                }
                //clear screen
                system("cls");
                //display home menu
                display_home_menu();
                break;
            case '3': //Lock or unlock files
                // Clear console screen
                system("cls");
                // Print art
                display_text_art();
                std::cout << std::endl;
                std::cout << "[L] " <<ANSI_COLOR_YELLOW<< "Lock a File\n"
                          <<         ANSI_COLOR_RESET
                          << "[U] " <<ANSI_COLOR_GREEN<< "Unlock a File\n"
                          << ANSI_COLOR_RESET
                          << "\n"
                          << "[Q] " <<ANSI_COLOR_RED<< "Exit this Menu\n"
                          << ANSI_COLOR_RESET;
                while ((input != 'l') && (input !='u') && (input !='q')){ //waits until user inputs a correct key
                    input = _getch(); //Listens for user key-click
                }
                if (input == 'l')
                {
                    //Use lock file function
                    //Let user select file
                    //Get filepath of selected file
                    //Check CSV for row of filepath
                    //If file does not exist, notify user
                    //If file exists, check if a user has file checked out
                    //If yes,
                    //If no, lock file
                    //test
                    std::string user_name = INI_UI.get_ini_value("user_name");
                    std::string file = FS_UI.selectFile();
                    bool is_owner = CSV_UI.is_user_owner(csv_path,
                                                         file,
                                                         user_name);
                    if (is_owner) //No one has file locked or user already has file locked
                    {
                        CSV_UI.write_lock_data(csv_path,file,user_name); //Writes data to csv file
                        //FS_UI.make_readonly(file); //Makes selected file read only (not needed as user owns file)
                    } else // We cannot lock file for user
                    {
                        std::cout << "not owner";
                    }
                } else if (input == 'u')
                {
                    std::string user_name = INI_UI.get_ini_value("user_name");
                    std::string file = FS_UI.selectFile();
                    bool is_owner = CSV_UI.is_user_owner(csv_path,
                                                         file,
                                                         user_name);
                    if (is_owner) //No one has file locked or user already has file locked
                    {
                        CSV_UI.clear_lock_data(csv_path,file); //Writes data to csv file
                        //FS_UI.make_writable(file); //Makes selected file writable (not needed as user owns file)
                    } else // We cannot lock file for user
                    {
                        std::cout << "not owner";
                    }
                }
                //If user exits this menu
                display_home_menu();
                break;
            case 'h': //Help
                // Clear console screen
                system("cls");
                display_help();
                break;
            case 'q':
                running = false;
                break;
            default:
                // Clear console screen
                system("cls");
                // Print art
                display_text_art();
                std::cout << "Invalid input. Enter H for help.\n";
                break;
        }
    }
}
// Displays the home menu
void UI::display_home_menu() {
    system("cls");
    // Set console window size
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rect = {0, 0, 82, 22}; // 80 columns x 30 rows
    SetConsoleWindowInfo(console, TRUE, &rect);

    // Set console output encoding to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    //Print initial interface
    display_text_art();
    //Print the current user from the ini file
    std::string current_user = INI_UI.get_ini_value("user_name");
    std::cout << "Current User: "<<ANSI_COLOR_GREEN<<current_user<<ANSI_COLOR_RESET<<"\n"<< std::endl;

    std::cout << "[1] Set User\n"
              << "[2] Display Locked Files\n"
              << "[3] Lock/Unlock Files\n"
              << "\n"
              << "[H] " <<ANSI_COLOR_YELLOW<< "Help\n"
              << ANSI_COLOR_RESET
              << "[Q] " <<ANSI_COLOR_RED<< "Quit\n";
}
// Promts the user to set their username
void UI::display_set_user() {
    std::string user_name;
    // Clear console screen
    system("cls");
    // Print art
    display_text_art();
    std::cout << std::endl;
    std::cout << "Please enter your " <<ANSI_COLOR_GREEN<< "First " <<ANSI_COLOR_RESET<< "and "<<ANSI_COLOR_GREEN<< "Last " <<ANSI_COLOR_RESET<< "name: ";
    std::getline(std::cin, user_name);
    INI_UI.write_user(user_name);
}

//Display locked files
void UI::display_locked_files()
{
    CSV_UI.print_locked_file_status();
}

// Displays help menu
void UI::display_help()
{
    display_text_art();
    std::cout << "Help documentation:\n"
              << "1. Set User - sets the username.\n"
              << "\t - This will allow the program to unlock files\n"
              << "\t   that belong to the set user.\n"
              << "\n"
              << "2. Display Locked Files - Displays all files that are locked.\n"
              << "\t\t This also displays who has the file checked-out.\n"
              << "\n"
              << "3. Lock/Unlock Files - Allows a user to select a file via\n"
              << "\t\t  the windows file explorer to lock or unlock.\n"
              << "\n"
              << "H. Help - displays this help documentation\n"
              << "\n"
              << "Q. Quit - Exits the program.\n"
              << "\t  - Will also exit other menus if not currently at the main menu.\n";
}

void UI::display_text_art() {
    std::cout <<ANSI_COLOR_YELLOW<< textArt <<ANSI_COLOR_RESET<< std::endl;
}