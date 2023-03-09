#include "csv.h"
#include <string>
#include <vector>
#include "tuple"
#include <unordered_set>
#include <iostream>
#include "filesystem"
#include <ctime>
#include <iomanip>

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
} else if (fs::is_directory(status)) {

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

