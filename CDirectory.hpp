#pragma once
#include "CDataUnit.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


class CDirectory : public CDataUnit{
private:
    set < shared_ptr<CDataUnit> > data_units;
    string data_in_file;

    // может быть всё что угодно, что унаследованно от CDataUnit
    // пользуемся этой функцией ТОЛЬКО при инициализации директории
    void AddDataUnits();
    bool AddDataUnit(const shared_ptr<CDataUnit> & src);
    bool SaveTree(bool first_directory) const;
public:
    CDirectory(string name, bool first_directory);
    ~CDirectory();
    bool IsFile() const override;
    bool IsDirectory() const override;
    bool IsLink() const override;
};