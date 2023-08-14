#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

std::vector<std::string> get_lines(const std::string& input) {
    std::ifstream input_file(input);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open input_file: " << input << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    input_file.close();

    return std::move(lines);
}

void calculate_frequency_internal(const std::vector<std::string>& lines) {
    
}

void put_frequencies(const std::string& output, void* _) {
    std::ofstream output_file(output);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open output_file: " << output << std::endl;
        return;
    }

    // TODO: 

    output_file.close();
}

void calculate_frequency(const std::string& input, const std::string& output) {
    auto lines = get_lines(input);
    if (lines.empty()) {
        std::cerr << "Aborting output" << input << std::endl;
        return;
    }
    
    calculate_frequency_internal(output, lines);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Use: ./freq [input_file] [output_file]" << std::endl;
        return 0;
    }

    auto start = std::chrono::steady_clock::now();
    calculate_frequency(argv[1], argv[2]);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}