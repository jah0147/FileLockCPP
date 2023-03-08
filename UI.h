//
// Created by jacob_howard on 2/28/2023.
//
#include "string"
#include "iostream"
#ifndef FILELOCKCPP_UI_H
#define FILELOCKCPP_UI_H


class UI {
public:
    void display_ui();
    void display_home_menu();
    void display_set_user();
    void display_locked_files();
    void display_help();

private:
    //Used to prin the art in the UI
    std::string textArt = "\n"
                          "███████╗██╗██╗     ███████╗██╗      █████╗  █████╗ ██╗  ██╗ █████╗ ██████╗ ██████╗ \n"
                          "██╔════╝██║██║     ██╔════╝██║     ██╔══██╗██╔══██╗██║ ██╔╝██╔══██╗██╔══██╗██╔══██╗\n"
                          "█████╗  ██║██║     █████╗  ██║     ██║  ██║██║  ╚═╝█████═╝ ██║  ╚═╝██████╔╝██████╔╝\n"
                          "██╔══╝  ██║██║     ██╔══╝  ██║     ██║  ██║██║  ██╗██╔═██╗ ██║  ██╗██╔═══╝ ██╔═══╝ \n"
                          "██║     ██║███████╗███████╗███████╗╚█████╔╝╚█████╔╝██║ ╚██╗╚█████╔╝██║     ██║     \n"
                          "╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝ ╚════╝  ╚════╝ ╚═╝  ╚═╝ ╚════╝ ╚═╝     ╚═╝     ";
    //quick function that prints art
    void inline display_text_art()
    {
        std::cout << textArt << std::endl;
    }
};


#endif //FILELOCKCPP_UI_H
