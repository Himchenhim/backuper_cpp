#pragma once
#include "CDataUnit.hpp"
#include <filesystem>
#include <fstream>
#include "CFile.hpp"
#include "hashes.hpp"
#include <chrono>


namespace fs = std::filesystem;
using std::fstream;


class CDirectory : public CDataUnit{
private:
    set < shared_ptr<CDataUnit> > data_units;
    string data_in_file;

    // functions for initialization of directory
    void AddDataUnits();
    bool AddDataUnit(const shared_ptr<CDataUnit> & src);
    bool SaveTree() const;
public:
    CDirectory(const string & path, const string & name);
    CDirectory(fstream & sstream);
    CDirectory(fstream & sstream, string path, string name);
    ~CDirectory();
    void Restore() const override ;
    bool IsFile() const override;
    bool IsDirectory() const override;
    bool IsLink() const override;
    friend class CBackup;
};