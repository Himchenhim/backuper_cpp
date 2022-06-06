#pragma once
#include "CDataUnit.hpp"
#include <fstream>
#include <filesystem>


using std::ifstream;
namespace  fs = std::filesystem;

class CFile : public CDataUnit
{

public:

    // for every constructor we need call function, which calculate hash
    // and we save object of CFile class to folder ./.backups/obj/2_hash_symbols/38_hash_symbols
    CFile (string path_of_file, string name_of_file);
    CFile (string path_of_file, string name_of_file, string hash_of_file);
    void Restore() const override ;

    ~CFile(){}

    bool IsFile() const override;
    bool IsDirectory() const override;
    bool IsLink() const override;

    void Print(size_t level) const override;

};
