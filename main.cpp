//#include "main.h"
#include "UI.h"
#include "csv.h"
#include "ini.h"
#include "file_status.h"

int main()
{
UI ui;
csv CSV;
ini INI;
file_status FS;
//file_status FileStatus;
fs::path dir_path = INI.get_project_directory();
std::string csv_path = INI.get_ini_value("csv_path");
std::string user_name = INI.get_ini_value("user_name");

std::cout << "Generating CSV File. This could take some time..." << std::endl;
CSV.generate_file_list_csv(dir_path, csv_path);

// Check the file status
    //checkFileStatus(CSV.locked_file_status(csv_path));
// Set files that need to be read only to read only
    //setFileStatusReadOnly(CSV.locked_file_status(csv_path));
    FS.set_csv_files_readOnly(CSV.locked_file_status(csv_path));
// Set files writable that need to be
    FS.set_csv_files_writable(CSV.unlocked_file_status(csv_path));
//Set users files to writable
CSV.unlock_users_files(csv_path, user_name);
// Display the UI
ui.display_ui();

    return 0;
}
