//
// Created by jacob_howard on 2/28/2023.
//
#include <iostream>
#include <string>
#include <conio.h>
#include "UI.h"
#include "main.h"

void UI::display_ui()
{
    char input;
    bool running = true;

    while (running) {
        std::cout << "Enter a command: ";
        input = _getch();
        std::cout << input << "\n";

        switch (input) {
            case '1':
                //startFunction();
                break;
            case '2':
                //stopFunction();
                break;
            case '3':
                display_help();
                break;
            case '4':
                running = false;
                break;
            default:
                std::cout << "Invalid input. Enter 3 for help.\n";
                break;
        }
    }
}

void UI::display_help()
{
    std::cout << "Help documentation:\n"
              << "1. Start - starts the program\n"
              << "2. Stop - stops the program\n"
              << "3. Help - displays this help documentation\n"
              << "4. Quit - exits the program\n";
}