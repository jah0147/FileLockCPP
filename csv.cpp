#include "csv.h"
#include <fstream>
#include <string>
#include <vector>
#include "tuple"
#include <unordered_set>
#include "filesystem"
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