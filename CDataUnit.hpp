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

class CDataUnit{

public:

    // move-нуть потом нейм
    CDataUnit(string name);
    // удаляем hash_of_file, если он существует
    ~CDataUnit();

    virtual bool IsFile() const = 0;
    virtual bool IsDirectory() const = 0;
    virtual bool IsLink() const = 0;

    virtual  CDataUnit * Clone() const = 0;
    // return number of copied bytes
    virtual unsigned int CopyUnit() const = 0;

protected:
    string name_of_data_unit;
    // для каждого наследуемого типа данных свой метод вычисления хеша
    string hash_of_data_unit;
};



