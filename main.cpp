#include "main.h"
int main()
{
    //Start of code that requires testing
#ifdef TESTING
    display_console_ui();
#endif
    //Start of code that does not require testing
#ifndef TESTING
    //Generate or read from the CSV file
    generateCSV();
    //Checks the file status of all files
    checkFileStatus();
    //Sets files to read only that need to be
    setFileStatusReadOnly();
    //Sets files to writable that need to be
    setFileStatusWritable();
#endif
    return 0;
}
