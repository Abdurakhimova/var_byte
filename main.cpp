#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdint>

std::vector<uint8_t> varbyte_encode(const std::vector<int32_t>& numbers) {
    std::vector<uint8_t> encoded;

    for (const auto& number : numbers) {
        uint32_t value = static_cast<uint32_t>(number);
        do {
            uint8_t byte = value & 0x7F;
            value >>= 7;
            if (value != 0) {
                byte |= 0x80;
            }
            encoded.push_back(byte);
        } while (value != 0);
    }

    return encoded;
}

std::vector<int32_t> varbyte_decode(const std::vector<uint8_t>& encoded) {
    std::vector<int32_t> decoded;
    int32_t current_number = 0;
    int32_t shift = 0;

    for (const auto& byte : encoded) {
        current_number |= (byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            decoded.push_back(current_number);
            current_number = 0;
            shift = 0;
        }
        else {
            shift += 7;
        }
    }

    return decoded;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <mode: a|d>" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    std::string mode(argv[2]);
    if (mode == "a") {
        std::ifstream input_file(filename, std::ios::binary);
        std::vector<int32_t> numbers;

        int32_t number;
        while (input_file.read(reinterpret_cast<char*>(&number), sizeof(int32_t))) {
            numbers.push_back(number);
        }

        input_file.close();

        std::vector<uint8_t> encoded = varbyte_encode(numbers);
        std::ofstream output_file(filename + ".arch", std::ios::binary);

        output_file.write(reinterpret_cast<const char*>(encoded.data()), encoded.size());
        output_file.close();
    }
    else if (mode == "d") {
        std::ifstream input_file(filename, std::ios::binary);
        std::vector<uint8_t> encoded;

        uint8_t byte;
        while (input_file.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t))) {
            encoded.push_back(byte);
        }

        input_file.close();

        std::vector<int32_t> decoded = varbyte_decode(encoded);
        std::ofstream output_file(filename.substr(0, filename.find(".arch")), std::ios::binary);

        for (const auto& number : decoded) {
            output_file.write(reinterpret_cast<const char*>(&number), sizeof(int32_t));
        }

        output_file.close();
    }
    else {
        std::cerr << "Invalid mode. Please use 'a' for archiving or 'd' for dearchiving." << std::endl;
        return 1;
    }

    return 0;
}
