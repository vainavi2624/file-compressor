# File-compressor
A file compressing tool based on Huffman Coding Algorithm

### Project Explanation: Huffman Compression and Decompression System

#### Introduction
The project implements a file compression and decompression system based on the Huffman coding algorithm. Huffman coding is a widely-used technique for lossless data compression that assigns variable-length codes to input characters based on their frequencies. This project provides a practical implementation of Huffman coding to compress and decompress files, making it an essential tool for efficient data storage.

#### Architecture
The project is designed to handle both compression and decompression tasks using a command-line interface. It consists of several key components:
1. **HuffmanNode Structure**: Represents a node in the Huffman tree with attributes for the character, its frequency, and pointers to left and right child nodes.
2. **Priority Queue**: Used to build the Huffman tree by processing nodes in order of their frequency.
3. **Huffman Tree Construction**: Builds the Huffman tree from the input data and generates Huffman codes.
4. **Compression and Decompression Functions**: Handles reading, processing, and writing data to and from files.

#### Functionalities
1. **Huffman Tree Construction**: 
   - **Character Frequency Counting**: Counts the frequency of each character in the input file.
   - **Tree Building**: Constructs the Huffman tree using a priority queue based on character frequencies.
   - **Code Generation**: Generates Huffman codes by traversing the Huffman tree.

2. **File Compression**:
   - **Input Reading**: Reads data from the specified file and builds the Huffman tree.
   - **Code Writing**: Writes Huffman codes to the output file.
   - **Data Compression**: Converts the input data into a compressed format using the generated Huffman codes and writes it to the output file.

3. **File Decompression**:
   - **Code Reading**: Reads Huffman codes from the compressed file.
   - **Data Decompression**: Decodes the compressed data using the Huffman codes and writes the decompressed data to a new file.

#### Implementation
1. **Data Structures**:
   - **HuffmanNode**: Defines the structure of nodes in the Huffman tree with a constructor for initialization and an overloaded `>` operator for priority queue operations.
   - **Priority Queue**: Used to build the Huffman tree by storing nodes in order of frequency.

2. **Functions**:
   - **generateCodes**: Recursively traverses the Huffman tree to create a mapping of characters to their corresponding Huffman codes.
   - **buildHuffmanTree**: Constructs the Huffman tree from character frequencies and generates codes.
   - **compressFile**: Reads the input file, builds the Huffman tree, compresses the data, and writes the result to the output file.
   - **decompressFile**: Reads the compressed file, reconstructs the Huffman tree, decompresses the data, and writes the result to a new file.

3. **Main Function**:
   - Handles user input to determine whether to compress or decompress a file and invokes the corresponding function based on the user's choice.

#### Challenges
1. **File Handling**: Ensuring correct file operations, including handling binary data and generating appropriate output file names.
2. **Error Handling**: Managing cases where files cannot be opened, or input data is invalid.
3. **Code Efficiency**: Efficiently managing memory and processing large files to avoid performance bottlenecks.

#### Conclusion
This project demonstrates a practical application of Huffman coding for file compression and decompression. The system effectively compresses and decompresses files by constructing a Huffman tree, generating variable-length codes, and handling file I/O operations. Despite the challenges in managing file operations and ensuring code efficiency, the project provides a solid implementation of Huffman coding, offering valuable insights into data compression techniques.
