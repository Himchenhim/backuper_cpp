#pragma once
#include "CDataUnit.hpp"

// TREE
class CDirectory : public CDataUnit{
private:
    set < shared_ptr<CDataUnit> > data_units;
public:
    CDirectory(string name);
    ~CDirectory();
    bool AddDataUnitToDirectory(shared_ptr<CDataUnit> src); // может быть всё что угодно, что унаследованно от CDataUnit
    bool SaveTreeToFile(); // будем так же хранить в /2_цифры_хеша/38_цифр и там будет само наше дерево
};