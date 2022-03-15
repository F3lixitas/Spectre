#include <fstream>
#include <vector>


std::vector<char> readFile(const char* filepath){
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
        throw std::runtime_error("No such file!");
    }
}