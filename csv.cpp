#include "csv.h"
#include <string>
#include <vector>
#include "tuple"
#include <unordered_set>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <cstdlib>

#include "colorCodes.h"
 // for setw() and left
//Our classes
#include "file_status.h"
namespace fs = std::filesystem;

//Generates/Reads a CSV file based on a specified directory
void csv::generate_file_list_csv(const fs::path& dir_path, const std::string& csv_filename) {
    // check if CSV file already exists
    bool csv_exists = fs::exists(csv_filename);

    // open CSV file for writing in append mode
    std::ofstream csv_file(csv_filename, std::ios::app);

    // if CSV file does not exist, write headers to CSV file
    if (!csv_exists) {
    csv_file << "Name,Path,Type,Status,Checked Out By,Date\n";
    }

    // read existing entries into a set
    std::unordered_set<std::string> existing_entries;
    if (csv_exists) {
        std::ifstream existing_csv_file(csv_filename);
        std::string line;
        // skip header row
        std::getline(existing_csv_file, line);
        while (std::getline(existing_csv_file, line)) {
            std::istringstream iss(line);
            std::string name, path, type;
            // parse the CSV row into name, path, and type
            std::getline(std::getline(std::getline(iss, name, ','), path, ','), type);
            // add path to set of existing entries
            existing_entries.insert(path);
        }
    }

    // recursively iterate over directory entries and write names, paths, and types to CSV file
    for (auto &entry: fs::recursive_directory_iterator(dir_path)) {
        // get the file status of the current entry
        const fs::file_status status = entry.status();

        // check if entry is a regular file or a directory
        if (fs::is_regular_file(status)) {
            // write name, path, and type to CSV file if entry is not already in CSV file
            const std::string path_str = entry.path().string();
            if (existing_entries.find(path_str) == existing_entries.end()) {
            csv_file << entry.path().filename().string() << ",";
            csv_file << path_str << ",";
            csv_file << "File\n";
            existing_entries.insert(path_str);
            }
        }
        else if (fs::is_directory(status)) {

            // check if directory contains files
            bool has_files = false;
            for (auto &sub_entry: fs::directory_iterator(entry.path())) {
            if (fs::is_regular_file(sub_entry.status())) {
            has_files = true;
            break;
            }
            }

            // if directory contains files, list them instead of the directory
            if (has_files) {
                for (auto &sub_entry: fs::directory_iterator(entry.path())) {
                if (fs::is_regular_file(sub_entry.status())) {
                    // write name, path, and type to CSV file if entry is not already in CSV file
                    const std::string path_str = sub_entry.path().string();
                    if (existing_entries.find(path_str) == existing_entries.end()) {
                        csv_file << sub_entry.path().filename().string() << ",";
                        csv_file << path_str << ",";
                        csv_file << "File\n";
                        existing_entries.insert(path_str);
                    }
                }
                }
            } else {
                // write name, path, and type of other types of entries to CSV file if entry is not already in CSV file
                const std::string path_str = entry.path().string();
                if (existing_entries.find(path_str) == existing_entries.end()) {
                    csv_file << entry.path().filename().string() << ",";
                    csv_file << path_str << ",";
                    if (fs::is_symlink(status)) {
                        csv_file << "Symlink\n";
                    } else if (fs::is_socket(status)) {
                        csv_file << "Socket\n";
                    } else if (fs::is_fifo(status)) {
                        csv_file << "Named pipe\n";
                    } else {
                        csv_file << "Other\n";
                    }
                existing_entries.insert(path_str);
                }
            }
        }
    }
}

//Should remove all .git files from csv file
void csv::remove_git_files_from_csv(const std::string& csv_file) {
    std::ifstream fin(csv_file);
    std::ofstream fout("temp.csv"); // create temporary file

    std::string line;
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        bool found_git = false;
        while (std::getline(ss, cell, ',')) {
            if (col == 1 && cell.find(".git") != std::string::npos) {
                found_git = true;
                break;
            }
            col++;
        }
        if (!found_git) {
            fout << line << "\n";
        }
    }

    fin.close();
    fout.close();
    std::remove(csv_file.c_str()); // remove original file
    std::rename("temp.csv", csv_file.c_str()); // rename temporary file
}

// Function to read a CSV file and return 3 vectors of locked file information
std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> csv::locked_file_status(const std::string& filename) {
    // Create empty vectors to store the locked filenames, filepaths, and checked-out names
    std::vector<std::string> locked_filename;
    std::vector<std::string> locked_filepath;
    std::vector<std::string> checked_out_name;

    // Open the CSV file for reading
    std::ifstream file(filename);

    // Check if the file was successfully opened
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    // Ignore the first row
    std::string first_line;
    std::getline(file, first_line);

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Parse the line into individual columns
        std::vector<std::string> columns;
        std::istringstream iss(line);
        std::string column;
        while (std::getline(iss, column, ',')) {
            columns.push_back(column);
        }

        // Check if the 4th column is non-empty
        if (columns.size() >= 4 && !columns[3].empty()) {
            // Store the values of the first, second, and fifth columns in separate vectors
            locked_filename.push_back(columns[0]);
            locked_filepath.push_back(columns[1]);
            checked_out_name.push_back(columns[4]);
        }
    }

    // Return a tuple of vectors containing the locked filenames, filepaths, and checked-out names
    return std::make_tuple(locked_filename, locked_filepath, checked_out_name);
}

//Function to read a CSV file and return 2 vectors of unlocked file information
std::pair<std::vector<std::string>, std::vector<std::string>> csv::unlocked_file_status(const std::string& filename) {
    // Create empty vectors to store the unlocked filenames and filepaths
    std::vector<std::string> unlocked_filename;
    std::vector<std::string> unlocked_filepath;

    // Open the CSV file for reading
    std::ifstream file(filename);

    // Check if the file was successfully opened
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Parse the line into individual columns
        std::vector<std::string> columns;
        std::istringstream iss(line);
        std::string column;
        while (std::getline(iss, column, ',')) {
            columns.push_back(column);
        }

        // Check if the 4th column is empty
        if (columns.size() < 4 || columns[3].empty()) {
            // Store the values of the first and second columns in separate vectors
            unlocked_filename.push_back(columns[0]);
            unlocked_filepath.push_back(columns[1]);
        }
    }

    // Return a pair of vectors containing the unlocked filenames and filepaths
    return std::make_pair(unlocked_filename, unlocked_filepath);
}

void csv::unlock_users_files(const std::string& filename, const std::string& check_name)
{
    file_status FS_CSV; //Initialte file_status

    std::string filepath_unlock = "";

    // Open the file for reading
    std::ifstream file(filename);
    if (!file) {
        // If we couldn't open the file, print an error message and return
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Declare variables to hold each line of the file, the headers, and a flag indicating if a match was found
    std::string line;
    std::vector<std::string> headers;
    bool found_match = false;

    // Read the file line by line
    while (std::getline(file, line)) {
        // Parse each line by splitting it into fields separated by commas
        std::stringstream ss(line);
        std::string field;

        std::vector<std::string> fields;
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // If we're still reading the header row, save the headers for reference
        if (headers.empty()) {
            headers = fields;
        }
        else {
            // Check if the fields vector is large enough to access columns 4-6, and if any of these columns are empty
            if (fields.size() > 3 && !fields[3].empty() && fields.size() > 4 && !fields[4].empty() && fields.size() > 5 && !fields[5].empty()) {
                // Convert the value in column 5 to uppercase for better comparison
                std::string column_name = set_str_upper(fields[4]);
                std::string check_name_up = set_str_upper(check_name);
                // Check if the check_name matches the value in column 5
                if (column_name == check_name_up) {
                    // If there's a match, save the value in column 2 to the filepath_unlock variable and make the file writable
                    filepath_unlock = fields[1];
                    FS_CSV.make_writable(filepath_unlock);
                }
            }
        }
    }

    // If no match was found, print a message indicating so
//    if (!found_match) {
//        std::cout << "No match found for check_name: " << check_name << std::endl;
//    }
}

// Prints the locked files and the users who have them locked
void csv::print_locked_file_status()
{
    // Call the locked_file_status() function to get the locked filenames and filepaths
    auto [locked_filename, locked_filepath, checked_out_name] = csv::locked_file_status(csv_path);

    // Print the header
    std::cout <<ANSI_COLOR_YELLOW;
    std::cout << "[Locked Filenames]";
    std::cout <<ANSI_COLOR_RESET;
    std::cout << "                                 ";
    std::cout <<ANSI_COLOR_GREEN;
    std::cout << "[Checked out by]" << std::endl;
    std::cout <<ANSI_COLOR_RESET;
    // Print the separator line
    std::cout << std::string(67, '-') << std::endl;

    // Loop over the locked filenames and print each filename and user
    for (size_t i = 0; i < locked_filename.size(); ++i) {
        std::cout << "- ";
        std::cout <<ANSI_COLOR_YELLOW;
        std::cout << locked_filename[i];
        std::cout <<ANSI_COLOR_RESET;
        std::cout << "  ";
        std::cout << std::string(47 - locked_filename[i].length(), '-');
        std::cout << "  ";
        std::cout <<ANSI_COLOR_GREEN;
        std::cout << checked_out_name[i] << std::endl;
        std::cout <<ANSI_COLOR_RESET;
    }

//    for (const auto& filename : locked_filename) {
//        std::cout << "- " << filename;
//        std::cout << "\t Checked-Out by: " << checked_out_name << std::endl;
//    }
    // Print the contents of the locked_filepath vector
//    std::cout << "Locked filepaths:" << std::endl;
//    for (const auto& filepath : locked_filepath) {
//        std::cout << "- " << filepath << std::endl;
//    }
    // Print the contents of the checked_out_name vector
//    std::cout << "Checked-out names:" << std::endl;
//    for (const auto& name : checked_out_name) {
//        std::cout << "- " << name << std::endl;
//    }
}

// Prints the locked files of set user
void csv::print_usr_locked_file_status() {
    // Call the locked_file_status() function to get the locked filenames and filepaths
    auto [locked_filename, locked_filepath, checked_out_name] = csv::locked_file_status(csv_path);

    std::string user_name = INI_CSV.get_ini_value("user_name"); //gets users set username

    std::string usr_up = set_str_upper(user_name);

    // Print the header
    std::cout << ANSI_COLOR_YELLOW << "[Locked Filenames]" << ANSI_COLOR_RESET << "                                 "
              << ANSI_COLOR_GREEN << "[Checked out by]" << std::endl << ANSI_COLOR_RESET;
    // Print the separator line
    std::cout << std::string(67, '-') << std::endl;

    // Loop over the locked filenames and print each filename and user that matches the given user name
    for (size_t i = 0; i < locked_filename.size(); ++i) {
        std::string checked_out_name_Up = set_str_upper(checked_out_name[i]);

        if (checked_out_name_Up == usr_up) {
            std::cout << "- " << ANSI_COLOR_YELLOW << locked_filename[i] << ANSI_COLOR_RESET << "  "
                      << std::string(47 - locked_filename[i].length(), '-') << "  "
                      << ANSI_COLOR_GREEN << checked_out_name[i] << std::endl << ANSI_COLOR_RESET;
        }
    }

    // Print the contents of the locked_filepath vector
    // ...
    // Print the contents of the checked_out_name vector
    // ...
}

//Check if user owns file
bool csv::is_user_owner(std::string filepath, std::string file, std::string user)
{
    bool is_owner = true;

    // Open the file for reading
    std::ifstream input_file(filepath);
    if (!input_file) {
        // If we couldn't open the file, print an error message and return false
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return false;
    }

    // Declare variables to hold each line of the file, the headers, and a flag indicating if a match was found
    std::string line;
    std::vector<std::string> headers;

    // Read the file line by line
    while (std::getline(input_file, line)) {
        // Parse each line by splitting it into fields separated by commas
        std::stringstream ss(line);
        std::string field;

        std::vector<std::string> fields;
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // If we're still reading the header row, save the headers for reference
        if (headers.empty()) {
            headers = fields;
        }
        else {
            // Check if the file matches the value in column 2 (which is index 1 since indices start at 0)
            if (fields.size() > 1 && fields[1] == file) {
                // Set names to all upper case for better comparison
                std::string column_name;
                if (fields.size() > 4 && !fields[4].empty()) {
                    column_name = set_str_upper(fields[4]);
                }
                // Set names to all upper case for better comparison
                std::string user_up;
                user_up = set_str_upper(user);

                // If there's a match, check if the 5th column (which is index 4) is empty
                if (fields.size() > 4) {
                    std::string column_5 = fields[4];
                    if (column_5.empty() || column_5 == "") {
                        is_owner = true; // If the 5th column is empty, set is_owner to true
                    }
                    else if (column_name == user_up) {
                        is_owner = true; // If the 5th column contains the user, set is_owner to true
                    }
                    else {
                        is_owner = false; // Otherwise, set is_owner to false
                    }
                    break; // Stop reading the file since we found a match
                }
            }
        }
    }

    return is_owner;
}

// Write lock data to csv file
void csv::write_lock_data(const std::string& filepath, const std::string& filename, const std::string& user_name) {
    std::string line;
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::vector<std::string> headers;
    bool found_match = false;
    std::vector<std::vector<std::string>> rows;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        std::vector<std::string> fields;
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (headers.empty()) {
            headers = fields;
        }
        else {
            if (fields.size() > 1 && fields[1] == filename) {
                fields.resize(headers.size(), ""); // make sure the row has the correct number of columns

                if (fields.size() < 4) {
                    fields.resize(4, "");
                }
                if (fields.size() < 5) {
                    fields.resize(5, "");
                }
                if (fields.size() < 6) {
                    fields.resize(6, "");
                }

                fields[3] = "Locked";
                fields[4] = user_name;
                std::time_t t = std::time(nullptr);
                std::tm tm = *std::localtime(&t);
                std::ostringstream oss;
                oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                fields[5] = oss.str();
                rows.push_back(fields);
                found_match = true;
            }
            else {
                rows.push_back(fields);
            }
        }
    }

    file.close();

    if (!found_match) {
        std::cout << "No match found for filename: " << filename << std::endl;
        return;
    }

    std::ofstream output_file(filepath);
    if (!output_file) {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
        return;
    }

    output_file << headers[0];
    for (int i = 1; i < headers.size(); i++) {
        output_file << "," << headers[i];
    }
    output_file << std::endl;

    for (int i = 0; i < rows.size(); i++) {
        output_file << rows[i][0];
        for (int j = 1; j < rows[i].size(); j++) {
            output_file << "," << rows[i][j];
        }
        output_file << std::endl;
    }

    output_file.close();
}

// Clears lock data (i.e. user unlocks a file if it belongs to them. Use the is_user_owner func to check)
void csv::clear_lock_data(const std::string& filepath, const std::string& filename) {
    std::string line;
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    bool found_match = false;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        std::vector<std::string> fields;
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (headers.empty()) {
            headers = fields;
        }
        else {
            if (fields.size() > 1 && fields[1] == filename) {
                if (fields.size() > 3) {
                    fields[3].clear();
                }
                if (fields.size() > 4) {
                    fields[4].clear();
                }
                if (fields.size() > 5) {
                    fields[5].clear();
                }
                rows.push_back(fields);
                found_match = true;
            }
            else {
                rows.push_back(fields);
            }
        }
    }

    file.close();

    if (!found_match) {
        std::cout << "No match found for filename: " << filename << std::endl;
        return;
    }

    std::ofstream output_file(filepath);
    if (output_file.is_open()) {
        output_file << headers[0];
        for (int i = 1; i < headers.size(); i++) {
            output_file << "," << headers[i];
        }
        output_file << std::endl;
        for (const auto& row : rows) {
            output_file << row[0];
            for (int i = 1; i < row.size(); i++) {
                output_file << "," << row[i];
            }
            output_file << std::endl;
        }
        output_file.close();
    }
    else {
        std::cerr << "Failed to open file for writing: " << filepath << std::endl;
    }
}

// Function to commit and push the csv file to gitlab
std::pair<std::string, std::string> csv::commit_and_push_CSV(const std::string& filePath, const std::string& userName) {
    const std::string &commitMessage = " Locked/Unlocked files and updated CSV file";

    // Check if file exists
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return std::make_pair("", "");
    }

    // Read file contents
    std::ifstream inputFile(filePath);
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string fileContents = buffer.str();

    // Get the current commit hash for the file
    std::string commitHash = "";
    std::string command = "git log -n 1 --pretty=format:%H -- " + filePath;
    FILE *pipe = _popen(command.c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL) {
                commitHash += buffer;
            }
        }
        _pclose(pipe);
        commitHash = commitHash.substr(0, 7); // Get the first 7 characters of the commit hash
    }

    // Write file contents to disk
    std::ofstream outFile("temp.csv");
    outFile << fileContents;
    outFile.close();

    // Commit and push the file
    command = "git add " + filePath + " && git commit -m \"" + userName + " " + commitMessage +
              "\" && git push > output.txt 2>&1";
    system(command.c_str());

    // Read the output of the push command from the file
    if (std::filesystem::exists("output.txt")) {
        std::ifstream outputFile("output.txt");
        std::stringstream outputBuffer;
        outputBuffer << outputFile.rdbuf();
        std::string output = outputBuffer.str();
        outputFile.close();

        // Check if the push was successful
        if (output.find("Everything up-to-date") == std::string::npos &&
            output.find("error: failed to push") != std::string::npos) {
            // Print the error message
            std::cerr << output << std::endl;

            // Fetch and pull the latest changes
            command = "git fetch origin " + filePath + " && git checkout origin/master " + filePath;
            pipe = _popen(command.c_str(), "r");
            std::string fetchOutput = "";
            if (pipe) {
                char buffer[128];
                while (!feof(pipe)) {
                    if (fgets(buffer, 128, pipe) != NULL) {
                        fetchOutput += buffer;
                    }
                }
                _pclose(pipe);
            }
            if (fetchOutput.find("Updates were rejected because the remote contains work that you do") !=
                std::string::npos) {
                // Print the error message
                std::cerr << fetchOutput << std::endl;

                // Fetch the latest changes
                command = "git fetch origin " + filePath;
                pipe = _popen(command.c_str(), "r");
                std::string fetchOutput = "";
                if (pipe) {
                    char buffer[128];
                    while (!feof(pipe)) {
                        if (fgets(buffer, 128, pipe) != NULL) {
                            fetchOutput += buffer;
                        }
                    }
                    _pclose(pipe);
                }

                // Merge the changes
                command = "git merge origin/master " + filePath + " > merge_output.txt 2>&1";
                system(command.c_str());

                // Read the output of the merge command from the file
                if (std::filesystem::exists("merge_output.txt")) {
                    std::ifstream mergeFile("merge_output.txt");
                    std::stringstream mergeBuffer;
                    mergeBuffer << mergeFile.rdbuf();
                    std::string mergeOutput = mergeBuffer.str();
                    mergeFile.close();

                    // Check if the merge was successful
                    if (mergeOutput.find("Automatic merge failed") != std::string::npos ||
                        mergeOutput.find("CONFLICT") != std::string::npos) {
                        // Print the merge conflict in a user-friendly way
                        std::string remoteConflict = "";
                        std::string localConflict = "";
                        std::string mergeConflict = "";
                        std::string line;
                        std::ifstream remoteFile("file_list.csv.orig");
                        std::ifstream localFile("file_list.csv");
                        while (std::getline(remoteFile, line) && std::getline(localFile, line)) {
                            if (line.find("<<<<<<<") != std::string::npos) {
                                remoteConflict += line + "\n";
                                std::getline(remoteFile, line);
                                while (line.find("=======") == std::string::npos) {
                                    remoteConflict += line + "\n";
                                    std::getline(remoteFile, line);
                                }
                                std::getline(remoteFile, line);
                                while (line.find(">>>>>>>") == std::string::npos) {
                                    //skip the conflicting code in the remote file
                                    std::getline(remoteFile, line);
                                }
                            } else if (line.find("=======") != std::string::npos) {
                                localConflict += line + "\n";
                                std::getline(localFile, line);
                                while (line.find(">>>>>>>") == std::string::npos) {
                                    localConflict += line + "\n";
                                    std::getline(localFile, line);
                                }
                            } else {
                                //no conflict, write the line to the output file
                                outFile << line << std::endl;
                            }
                        }

                        //write any remaining lines in the local file
                        while (std::getline(localFile, line)) {
                            localConflict += line + "\n";
                        }

                        //write any remaining lines in the remote file
                        while (std::getline(remoteFile, line)) {
                            remoteConflict += line + "\n";
                        }

                        //close the files
                        remoteFile.close();
                        localFile.close();
                        outFile.close();

                        //return the conflict strings
                        return std::make_pair(localConflict, remoteConflict);
                    }
                }
            }
            return std::make_pair("", "");
        } else {
            return std::make_pair("", "");
        }
    }
}


        void print_merge_conflicts(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("<<<<<<<") != std::string::npos ||
            line.find("=======") != std::string::npos ||
            line.find(">>>>>>>") != std::string::npos) {
            // This line is part of a merge conflict, so print it out in a more readable way
            std::cout << line << std::endl;
        }
    }

    file.close();
}

// Helper function to check if a Git repository exists in the given directory
//bool check_git_repository(const std::string& directory) {
//    std::string command = "git rev-parse --git-dir";
//    FILE* pipe = _popen(command.c_str(), "r");
//    std::string output = "";
//    if (pipe) {
//        char buffer[128];
//        while (!feof(pipe)) {
//            if (fgets(buffer, 128, pipe) != NULL) {
//                output += buffer;
//            }
//        }
//        _pclose(pipe);
//    }
//    return (output.find(".git") != std::string::npos);
//}
