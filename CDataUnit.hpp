#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <set>

using std::vector;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::ostream;
using std::set;

#define NAME_OF_PROGRAMME "bin"


class CDataUnit{

public:

    virtual bool IsFile() const = 0;
    virtual bool IsDirectory() const = 0;
    virtual bool IsLink() const = 0;
    virtual void Restore() const = 0;

    string GetHash(){return hash_of_data_unit;}
    string GetName(){return name_of_data_unit;}
    string GetPath(){return path_of_data_unit;}

    virtual void Print(size_t level) const = 0;

protected:
    string name_of_data_unit;
    string path_of_data_unit;
    // для каждого наследуемого типа данных свой метод вычисления хеша
    string hash_of_data_unit;

};




