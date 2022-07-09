#ifndef SPECTRE_OPEN_FILE
#define SPECTRE_OPEN_FILE

#include <fstream>
#include <vector>
#include <string>

inline std::vector<char> readFile(const char* filepath){
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if(file){
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> outBuffer(fileSize);
        file.seekg(0);
        file.read(outBuffer.data(), fileSize);
        file.close();
        return outBuffer;
    }
    else{
        std::string error("No such file :");
        error += filepath;
        throw std::runtime_error(error.data());
    }
}

#endif