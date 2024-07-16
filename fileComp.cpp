#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Define HuffmanNode structure
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr)
        : character(c), frequency(f), left(l), right(r) {}

    bool operator>(const HuffmanNode& other) const {
        return frequency > other.frequency;
    }
};

// Function to generate codes by traversing the Huffman tree
void generateCodes(unordered_map<char, string>& codes, HuffmanNode* node, const string& code) {
    if (!node) return;
    if (node->left == nullptr && node->right == nullptr) {
        codes[node->character] = code;
    }
    generateCodes(codes, node->left, code + "0");
    generateCodes(codes, node->right, code + "1");
}

// Function to build the Huffman tree and generate codes
bool buildHuffmanTree(unordered_map<char, string>& codes, const string& input) {
    // Count the frequency of each character
    unordered_map<char, int> charFrequency;
    for (char c : input) {
        charFrequency[c]++;
    }

    if (charFrequency.empty()) {
        return false; // Return false if input is empty
    }

    // Create a priority queue of Huffman nodes
    auto compare = [](HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    };
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(compare)> pq(compare);
    for (const auto& [c, f] : charFrequency) {
        pq.push(new HuffmanNode(c, f));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        pq.push(new HuffmanNode(0, left->frequency + right->frequency, left, right));
    }

    if (pq.empty()) {
        return false; // Return false if pq is empty
    }

    // Traverse the Huffman tree to generate the codes
    HuffmanNode* root = pq.top();
    generateCodes(codes, root, "");
    return true;
}

// Function to compress a file
void compressFile(const string& inputFile) {
    ifstream input(inputFile, ios::binary);
    if (!input.is_open()) {
        cerr << "Error opening input file: " << inputFile << endl;
        return;
    }

    // Derive output file name from input file path
    fs::path inputPath(inputFile);
    string outputFileName = "compressed_" + inputPath.filename().string();
    fs::path outputPath = inputPath.parent_path() / outputFileName;

    ofstream output(outputPath, ios::binary);
    if (!output.is_open()) {
        cerr << "Error opening output file: " << outputPath << endl;
        return;
    }

    string inputData;
    char c;
    while (input.get(c)) {
        inputData += c;
    }

    unordered_map<char, string> huffmanCodes;
    if (!buildHuffmanTree(huffmanCodes, inputData)) {
        cerr << "Error: Failed to build Huffman Tree. Input file may be empty or invalid." << endl;
        return;
    }

    // Write the Huffman codes to the output file
    size_t codeCount = huffmanCodes.size();
    output.write(reinterpret_cast<const char*>(&codeCount), sizeof(size_t));
    for (const auto& [c, code] : huffmanCodes) {
        output.write(&c, sizeof(char));
        size_t codeSize = code.size();
        output.write(reinterpret_cast<const char*>(&codeSize), sizeof(size_t));
        output.write(code.c_str(), code.size());
    }

    // Compress the input data using the Huffman codes
    string compressedData;
    for (char c : inputData) {
        compressedData += huffmanCodes[c];
    }

    // Write the compressed data to the output file
    size_t compressedSize = (compressedData.size() + 7) / 8;
    output.write(reinterpret_cast<const char*>(&compressedSize), sizeof(size_t));
    for (size_t i = 0; i < compressedSize; i++) {
        char byte = 0;
        for (int j = 0; j < 8 && i * 8 + j < compressedData.size(); j++) {
            byte = (byte << 1) | (compressedData[i * 8 + j] - '0');
        }
        output.put(byte);
    }

    if (output.good()) {
        cout << "File compressed successfully: " << outputPath << endl;
    } else {
        cerr << "Error writing to output file: " << outputPath << endl;
    }
}

// Function to decompress a file
void decompressFile(const string& inputFile) {
    ifstream input(inputFile, ios::binary);
    if (!input.is_open()) {
        cerr << "Error opening input file: " << inputFile << endl;
        return;
    }

    // Read the Huffman codes from the input file
    size_t codeCount;
    input.read(reinterpret_cast<char*>(&codeCount), sizeof(size_t));
    unordered_map<string, char> huffmanCodes;
    for (size_t i = 0; i < codeCount; i++) {
        char c;
        input.read(&c, sizeof(char));
        size_t codeSize;
        input.read(reinterpret_cast<char*>(&codeSize), sizeof(size_t));
        string code(codeSize, '\0');
        input.read(&code[0], codeSize);
        huffmanCodes[code] = c;
    }

    // Decompress the data using the Huffman codes
    size_t compressedSize;
    input.read(reinterpret_cast<char*>(&compressedSize), sizeof(size_t));
    string compressedData;
    for (size_t i = 0; i < compressedSize; i++) {
        char byte;
        input.get(byte);
        for (int j = 7; j >= 0; j--) {
            compressedData += (byte & (1 << j)) ? "1" : "0";
        }
    }

    string decompressedData;
    string currentCode;
    for (char bit : compressedData) {
        currentCode += bit;
        if (huffmanCodes.count(currentCode)) {
            decompressedData += huffmanCodes[currentCode];
            currentCode.clear();
        }
    }

    // Derive output file name from input file path
    fs::path inputPath(inputFile);
    string outputFileName = "decompressed_" + inputPath.filename().string();
    fs::path outputPath = inputPath.parent_path() / outputFileName;

    ofstream output(outputPath, ios::binary);
    if (!output.is_open()) {
        cerr << "Error opening output file: " << outputPath << endl;
        return;
    }

    // Write the decompressed data to the output file
    output.write(decompressedData.c_str(), decompressedData.size());

    if (output.good()) {
        cout << "File decompressed successfully: " << outputPath << endl;
    } else {
        cerr << "Error writing to output file: " << outputPath << endl;
    }
}

// Main function to handle user input and file operations
int main() {
    string inputFile;
    char option;

    cout << "Welcome to the File Compressor/Decompressor" << endl;
    cout << "Please enter the input file path( size > 10KB ): ";
    cin >> inputFile;

    cout << "Do you want to (c)ompress or (d)ecompress the file? ";
    cin >> option;

    if (option == 'c') {
        compressFile(inputFile);
    } else if (option == 'd') {
        decompressFile(inputFile);
    } else {
        cout << "Invalid option. Please run the program again." << endl;
    }

    return 0;
}
