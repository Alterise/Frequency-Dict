#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#include <tbb/parallel_for.h>
#include <tbb/parallel_sort.h>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>

tbb::concurrent_vector<std::string> get_lines(const std::string& input) {
    std::ifstream input_file(input);

    if (!input_file.is_open()) {
        std::cerr << "Failed to open input_file: " << input << std::endl;
        return {};
    }

    tbb::concurrent_vector<std::string> lines;
    std::string line;
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    input_file.close();

    return std::move(lines);
}

tbb::concurrent_vector<std::pair<std::string, long long>> calculate_frequency_internal(const tbb::concurrent_vector<std::string>& lines) {
    tbb::concurrent_hash_map<std::string, long long> cmap;
    tbb::parallel_for(0, static_cast<int>(lines.size()), 1, [&cmap, &lines](long long i) {
        tbb::concurrent_hash_map<std::string, long long>::accessor accessor;
        auto line = lines[i];

        std::transform(line.begin(), line.end(), line.begin(), [](char c) { 
            c = std::tolower(c);
            if (c >= 'a' && c <= 'z') {
                return c;
            } else {
                return ' ';
            }
        });

        std::string::const_iterator start = line.begin();
        std::string::const_iterator end = line.end();
        std::string::const_iterator next = std::find(start, end, ' ');

        std::string result;

        while (next != end) {
            if (next - start > 1) {
                result = std::string(start, next);
                if (cmap.find(accessor, result)) {
                    accessor->second++;
                } else {
                    cmap.insert({result, 1});
                }
            }
            

            start = next + 1;
            next = std::find(start, end, ' ');
        }
    });

    tbb::concurrent_vector<std::pair<std::string, long long>> result_lines;

    for (const auto& elem : cmap) {
        result_lines.push_back(elem);
    }
    
    tbb::parallel_sort(result_lines.begin(), result_lines.end(), [](const std::pair<std::string, long long>& lhs, 
                                                                    const std::pair<std::string, long long>& rhs) {
        if (lhs.second > rhs.second) {
            return true;
        } else if (lhs.second == rhs.second && lhs.first < rhs.first) {
            return true;
        } else {
            return false;
        }
    });

    return std::move(result_lines);
}

void put_frequencies(const std::string& output, const tbb::concurrent_vector<std::pair<std::string, long long>>& lines) {
    std::ofstream output_file(output);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open output_file: " << output << std::endl;
        return;
    }

    for (const auto &[word, count] : lines) {
        output_file << count << " " << word << std::endl;
    }
    
    output_file.close();
}

void calculate_frequency(const std::string& input, const std::string& output) {
    auto lines = get_lines(input);
    if (lines.empty()) {
        std::cerr << "Aborting output: " << input << std::endl;
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