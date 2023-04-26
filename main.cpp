#include <iostream>
#include <filesystem>

#include "csv.h"

int main(int argc, char* argv[]) {
    if(argc <= 2){
        std::cout << "not enough arguments: path to file must be given" << '\n';
        return 1;
    }
    double asz = sizeof(argv) / sizeof(*argv);
    if(asz > 2){
        std::cout << "too many arguments given" << '\n';
        return 1;
    }
    bool exists = std::filesystem::exists(argv[1]);
    if(!exists){
        std::cout << "the file " << argv[1] << " does not exist" << '\n';
        return 1;
    }
    std::string cmd = argv[2];
    if(cmd != "print" && cmd != "sum" &&
    cmd != "median" && cmd != "mean"
    && cmd != "std"){
        std::cout << argv[2] <<"is an invalid command" << '\n';
        return 1;
    } else {
        std::string file = argv[1];
        Csv csv(file);
        csv.printRes(cmd);
    }
    return 0;
}