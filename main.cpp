#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>

#include <tbb/parallel_for.h>
// #include <tbb/parallel_for_each.h>
#include <tbb/concurrent_hash_map.h>

struct MyBody {
    void operator()(const std::string& line, tbb::concurrent_hash_map<std::string, long long>& cmap ) {
        
    }
};

std::vector<std::string> get_lines(const std::string& input) {
    std::ifstream input_file(input);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open input_file: " << input << std::endl;
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    input_file.close();

    return std::move(lines);
}

tbb::concurrent_hash_map<std::string, long long> calculate_frequency_internal(const std::vector<std::string>& lines) {
    tbb::concurrent_hash_map<std::string, long long> cmap;
    tbb::concurrent_hash_map<std::string, long long>::accessor accessor;
    tbb::parallel_for(0, static_cast<int>(lines.size()), 1, [&cmap, &accessor, &lines](long long i){
        const auto& line = lines[i];
        if (cmap.find(accessor, line)) {
            accessor->second++;
        } else {
            cmap.emplace(accessor, line, 1);
        }
    });

    return std::move(cmap);
}

void put_frequencies(const std::string& output, const tbb::concurrent_hash_map<std::string, long long>& cmap) {
    std::ofstream output_file(output);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open output_file: " << output << std::endl;
        return;
    }

    for (const auto &elem : cmap) {
        output_file << elem.first << " " << elem.second << std::endl;
    }
    
    output_file.close();
}

void calculate_frequency(const std::string& input, const std::string& output) {
    auto lines = get_lines(input);
    if (lines.empty()) {
        std::cerr << "Aborting output" << input << std::endl;
        return;
    }
    
    auto cmap = calculate_frequency_internal(lines);

    put_frequencies(output, cmap);
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