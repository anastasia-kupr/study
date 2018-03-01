#include <QCoreApplication>
#include <fstream>
#include <iostream>
#include <cmath>

template<typename uint16_t, size_t size>
class BitBufferAbstract {
public:
    BitBufferAbstract() {
        currentBufferLength = sizeof(uint16_t) * 8 * size;
        currentBitOffset = 0;
        currentWordOffset = 0;
        currentLength = 0;

        this->allocateBuffer();
    }

    virtual ~BitBufferAbstract() {
        this->deallocateBuffer();
    }

protected:

    virtual void allocateBuffer() {
        buffer = (uint16_t*)malloc(currentBufferLength / 8);
        memset(buffer, 0, currentBufferLength / 8);
    }

    virtual void deallocateBuffer() {
        free(buffer);
    }

    uint16_t* buffer;

    size_t currentBufferLength;
    size_t currentLength;

    size_t currentBitOffset;
    size_t currentWordOffset;
};

template<typename uint16_t, size_t size>
class BitBufferWriter : public BitBufferAbstract<uint16_t, size> {
public:
    BitBufferWriter(std::string filename) :  BitBufferAbstract<uint16_t, size>() {
        this->openFile(filename);
    }

    ~BitBufferWriter() {
        bufferFlush();
        this->closeFile();
    }

    void write(const uint16_t* raw, const size_t bits) {
        size_t i = 0;
        if (this->currentLength + bits >= this->currentBufferLength) {

            for (; this->currentLength + i <= this->currentBufferLength; i++) {
                bool sourceBit = (*raw >> i) & 1;
                this->buffer[this->currentWordOffset] |= (uint16_t)sourceBit << this->currentBitOffset;
                this->currentBitOffset ++;
                this->currentLength ++;
                if (this->currentBitOffset >= sizeof(uint16_t) * 8) {
                    this->currentBitOffset = 0;
                    this->currentWordOffset ++;
                }
            }

            bufferFlush();
        }

        for (; i < bits; i++) {
            bool sourceBit = (*raw >> i) & 1;
            this->buffer[this->currentWordOffset] |= (uint16_t)sourceBit << this->currentBitOffset;
            this->currentBitOffset ++;
            this->currentLength ++;
            if (this->currentBitOffset >= sizeof(uint16_t) * 8) {
                this->currentBitOffset = 0;
                this->currentWordOffset ++;
            }
        }
    }

    void bufferFlush() {
        size_t wsize = ceil(this->currentLength / 8.0);
        file.write((const char*)this->buffer, wsize);
        file.flush();

        memset(this->buffer, 0, this->currentBufferLength / 8.0);

        this->currentWordOffset = 0;
        this->currentBitOffset = 0;
        this->currentLength = 0;
    }

protected:
    void openFile(std::string filename) {
        file.open(filename.c_str(), std::ios::out | std::ios::binary);
    }

    void closeFile() {
        file.close();
    }

    std::ofstream file;
};

template<typename uint16_t, size_t size>
class BitBufferReader : public BitBufferAbstract<uint16_t, size> {
public:
    BitBufferReader(std::string filename) :  BitBufferAbstract<uint16_t, size>() {
        this->openFile(filename);
        bufferRead();
    }

    ~BitBufferReader() {
        this->closeFile();
    }

    void read(uint16_t* raw, const size_t bits) {
        size_t i = 0;

        if (this->currentLength + bits >= this->currentBufferLength) {

            for (; this->currentLength + i <= this->currentBufferLength; i++) {
                bool sourceBit = (this->buffer[this->currentWordOffset] >> this->currentBitOffset) & 1;
                *raw |= (uint16_t)sourceBit << i;

                this->currentBitOffset ++;
                this->currentLength ++;

                if (this->currentBitOffset >= sizeof(uint16_t) * 8) {
                    this->currentBitOffset = 0;
                    this->currentWordOffset ++;
                }
            }

            bufferRead();
        }

        for (; i < bits; i++) {
            bool sourceBit = (this->buffer[this->currentWordOffset] >> this->currentBitOffset) & 1;
            *raw |= (uint16_t)sourceBit << i;

            this->currentBitOffset ++;
            this->currentLength ++;
            if (this->currentBitOffset >= sizeof(uint16_t) * 8) {
                this->currentBitOffset = 0;
                this->currentWordOffset ++;
            }
        }
    }

protected:
    void bufferRead() {
        memset(this->buffer, 0, this->currentBufferLength / 8);
        file.read((char*)this->buffer, size * sizeof(uint16_t));

        this->currentWordOffset = 0;
        this->currentBitOffset = 0;
        this->currentLength = 0;
    }

    void openFile(std::string filename) {
        file.open(filename.c_str(), std::ios::in | std::ios::binary);
    }

    void closeFile() {
        file.close();
    }

    std::ifstream file;
};

std::vector<int> readDataFile(std::string filename) {
    std::vector<int> result;

    std::ifstream inputFile(filename.c_str());
    while (!inputFile.eof()) {
        int tmp;
        inputFile >> tmp;

        result.push_back(tmp);
    }

    inputFile.close();

    return result;
}

void writeDataFile(std::string filename, std::vector<int> &data) {
    std::vector<int> result;

    std::ofstream outputFile(filename.c_str());
    for (auto && i : data) {
        outputFile << i << " ";
    }

    outputFile.close();
}

int main(int argc, char *argv[])
{

#pragma mark -- writing

    std::vector<int> inputNumbers = readDataFile("data.txt");
    std::vector<int> inputBits = readDataFile("bits.txt");

    auto *writer = new BitBufferWriter<uint16_t, 1>("file.bin");

    for (int i = 0; i < inputNumbers.size(); i++) {
        uint16_t c = inputNumbers[i];
        writer->write(&c, inputBits[i]);
    }

    delete writer;

#pragma mark -- reading


    std::vector<int> outputNumbers;

    auto *reader = new BitBufferReader<uint16_t, 1>("file.bin");

    for (int i = 0; i < inputBits.size(); i++) {
        uint16_t c = 0;
        reader->read(&c, inputBits[i]);
        outputNumbers.push_back(c);
        std::cout<< c << std::endl;
    }

    writeDataFile("data_out.txt", outputNumbers);

    delete reader;

    return 0;
}
