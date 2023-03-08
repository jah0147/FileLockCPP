//
// Created by jacob_howard on 2/28/2023.
//
#include "string"
#include "iostream"
#ifndef FILELOCKCPP_UI_H
#define FILELOCKCPP_UI_H
#include "colorCodes.h"

class UI {
public:
    void display_ui();
    void display_home_menu();
    void display_set_user();
    void display_locked_files();
    void display_help();
    void display_text_art();

private:
    //Used to prin the art in the UI
    std::string textArt = "\n"
                          "███████╗██╗██╗     ███████╗██╗      █████╗  █████╗ ██╗  ██╗ █████╗ ██████╗ ██████╗ \n"
                          "██╔════╝██║██║     ██╔════╝██║     ██╔══██╗██╔══██╗██║ ██╔╝██╔══██╗██╔══██╗██╔══██╗\n"
                          "█████╗  ██║██║     █████╗  ██║     ██║  ██║██║  ╚═╝█████═╝ ██║  ╚═╝██████╔╝██████╔╝\n"
                          "██╔══╝  ██║██║     ██╔══╝  ██║     ██║  ██║██║  ██╗██╔═██╗ ██║  ██╗██╔═══╝ ██╔═══╝ \n"
                          "██║     ██║███████╗███████╗███████╗╚█████╔╝╚█████╔╝██║ ╚██╗╚█████╔╝██║     ██║     \n"
                          "╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝ ╚════╝  ╚════╝ ╚═╝  ╚═╝ ╚════╝ ╚═╝     ╚═╝     ";
};


#endif //FILELOCKCPP_UI_H
