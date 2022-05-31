#pragma once
#include "CDataUnit.hpp"
#include <fstream>
#include <filesystem>


using std::ifstream;
namespace  fs = std::filesystem;

class CFile : public CDataUnit
{
public:

    // в каждом конструкторе так же нужно вызывать функцию, для подсчёта хеша
    // при конструировании сразу же сохраняем наш объект в папку objects
    CFile (string name_of_file);
    CFile (string name_of_file, string hash);
    ~CFile(){}

    bool IsFile() const override;
    bool IsDirectory() const override;
    bool IsLink() const override;

};
