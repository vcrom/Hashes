#include "md5.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

struct ProgramArgs
{
    std::string data;
    bool is_file_path = true;
    bool verbose = false;
};

void print_help(const std::string &name)
{
    std::cerr
            << "Usage " << name[0] << "file_path [options]\n"
            << "\t -h                Prints this help\n"
            << "\t [-f] <file path>  File to hash\n"
            << "\t -s <string>       String to hash\n"
            << "\t -v                Verbose output\n"
            << std::endl;
}

std::tuple<bool, ProgramArgs> parse_args(int argc, char * argv[])
{
    ProgramArgs args;
    if(argc < 2) return {false, args};
    bool data_setted = false;
    for(size_t i = 1; i < argc; ++i)
    {
        std::string option = argv[i];
        if(option == "-h") return {false, args};
        else if(option == "-v") args.verbose = true;
        else if((option == "-f" || option == "-s") && !data_setted)
        {
            args.data = argv[++i];
            data_setted = true;
            if(option == "-f") args.is_file_path = true;
            else args.is_file_path = false;
        }
        else if(i == 1)
        {
            data_setted = true;
            args.data = option;
        }
        else
        {
            return {false, args};
        }
    }
    return {data_setted, args};
}

int main(int argc, char * argv[])
{
    auto args = ProgramArgs();
    auto parse_suceed = false;
    std::tie(parse_suceed, args) = parse_args(argc, argv);
    if (parse_suceed)
    {
        std::vector<uint8_t> data;
        if(args.is_file_path)
        {
            std::ifstream input( args.data, std::ios::binary);
            if(!input.is_open())
            {
                std::cerr << "Failed to open file: " << args.data << std::endl;
                return 1;
            }
            if(args.verbose)
                std::cout << "hashing file " << args.data << std::endl;
            data = std::vector<uint8_t> (
                std::istreambuf_iterator<char>(input),
                std::istreambuf_iterator<char>());
        }
        else
        {
            if(args.verbose)
                std::cout << "Hashing string \"" << args.data << "\"" << std::endl;
            data = std::vector<uint8_t>(args.data.begin(), args.data.end());
        }

        std::unique_ptr<HashingAlgorithm> hasher = std::make_unique<Md5> ();
        hasher->hash_data(data);
        std::cout << hasher->get_hash() << std::endl;
    }
    else
    {
        print_help(argv[0]);
    }
    return 0;
}

