#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Input{
private:
    std::string path;
public:
    enum InputType{
        VERTEX_COLOR,
        UV,
        TEXTURE
    };
};

void parseMaterial(std::string buffer){
    size_t pos1 = buffer.find_first_of("SpectreMaterialVersion");
    int versionMajor = 1;
    int versionMinor = 0;
    if(pos1 != std::string::npos){
        size_t pos2 = buffer.find_first_of('.', pos1);
        versionMajor = atoi(buffer.substr(pos1 + 23, pos2 - pos1 - 23).data());
        pos1 = buffer.find_first_of(')', pos2);
        versionMinor = atoi(buffer.substr(pos2 + 1, pos1 - pos2).data());
    }

    std::cout << "Version(" << versionMajor << '.' << versionMinor << ")\n";

    pos1 = buffer.find_first_of("Input");
    size_t pos2 = buffer.find_first_of("{", pos1);

    pos1 = buffer.find_first_of("}", pos2);
    if(buffer.find_first_of("{", pos2) < pos1){

    }

}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "No input material file specified !" << std::endl;
        return 1;
    }
    for(uint32_t i = 1; i < argc; i++){
        std::fstream fs(argv[i], std::ios::in);
        if(!fs){
            std::cout << "Could not open file " << argv[i] << " !" << std::endl;
            return 2;
        }
        char* buffer;
        fs.seekg(0, fs.end);
        int length = fs.tellg();
        fs.seekg(0, fs.beg);

        buffer = new char[length];
        fs.read(buffer, length);

        parseMaterial(buffer);
        delete[] buffer;
    }
    return 0;
}