#pragma once
#include "CDataUnit.hpp"
#include <filesystem>
#include <fstream>
#include "CFile.hpp"
#include "hashes.hpp"
#include <chrono>
#include <map>

namespace fs = std::filesystem;
using std::map;
using std::fstream;


class CDirectory : public CDataUnit{
private:
    map <string, shared_ptr<CDataUnit> > data_units;
    string data_in_file;

    // functions for initialization of directory
    void AddDataUnits();
    bool AddDataUnit(string name, const shared_ptr<CDataUnit> & src);
    bool SaveTree() const;
public:

    CDirectory(const string & path, const string & name);
    CDirectory(fstream & sstream);
    CDirectory(fstream & sstream, string path, string name);
    ~CDirectory();
    void Restore() const override ;
    set<string> GetAllDataUnits();
    vector<shared_ptr<CDataUnit>> GetAllDataUnitsPtr();
    string FindDataUnit(const string & name) const;

    void Print(size_t level) const;

    shared_ptr<CDataUnit> FindDataUnitInDirectory(const string & name);

    void ShowAllUnits();

    bool IsFile() const override;
    bool IsDirectory() const override;
    bool IsLink() const override;
    friend class CBackup;
};