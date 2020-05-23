#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

struct mvcArgs {
    string input_file;
    string output_file;
    int thresh;
};

std::vector<std::string> split_string(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}