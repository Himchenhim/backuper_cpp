#include "CDirectory.hpp"
#include "exceptions.h"

using std::make_shared;

#define BACKUP_DIR "/.backups/ref_to_backups/"
#define OBJ_DIR "/.backups/obj"


bool IsSubstrInStr(const string & big_str, const string & sub_str){
    return int(big_str.find(sub_str)) != -1;
}

// saving to file
CDirectory::CDirectory(const string & path, const string & name = "") {
    path_of_data_unit = path;
    name_of_data_unit = name;
    // тут мы будем проходиться по нашей директории и дополнять в неё элементы
    AddDataUnits();

    // так же потом после этого посчитаем её хеш (отдельная приватная функция)
    for (const auto & data_unit : data_units)
    {
        if ( data_unit.second->IsFile())
            data_in_file += "file ";
        else
            data_in_file += "tree ";
        data_in_file += (data_unit.second->GetHash() + " ");
        data_in_file += (data_unit.second->GetName() + " ");
        data_in_file += (data_unit.second->GetPath() + "\n");
    }
    hash_of_data_unit = (CalcSha256ForString(data_in_file)).value();
    if (!name.empty())
        // сохраняет только деревья, которые не являются корневыми
        // так как к корневому дереву нам нужно добавить дополнительную информацию
        // и по этому он сохраняется отдельно ДОПОЛНИТЕЛЬНО
        SaveTree();
}


void CDirectory::AddDataUnits() {
    for (const auto & entry : fs::directory_iterator(path_of_data_unit))
    {
        auto filename = entry.path().filename();
        if ( filename.string() == ".backups" || filename.string() == NAME_OF_PROGRAMME)
            continue;

        // если это директория, то запускаем ещё один такой конструктор для директории
        if (fs::is_directory(entry.status()))
            AddDataUnit(filename,make_shared<CDirectory>(path_of_data_unit + "/" + filename.string(),filename.string()));
        else if (fs::is_regular_file(entry.status()))
            // processing of file
            AddDataUnit(filename,make_shared<CFile>(path_of_data_unit + "/" + filename.string(),filename.string()));

    }
}
bool CDirectory::AddDataUnit(string name, const shared_ptr<CDataUnit> & src) {
    // тут мы будем просто добавлять имя + хеш файла
    data_units[move(name)] = src;
    return true;
}
bool CDirectory::SaveTree() const {
    string directory = hash_of_data_unit.substr(0, 2);
    string file = hash_of_data_unit.substr(2);

    string pathToRootDirectory;

    pathToRootDirectory = ( (fs::current_path() /= ".backups/obj") /= directory );
    if ( !fs::exists(pathToRootDirectory))
        fs::create_directory(pathToRootDirectory);
    string filename (pathToRootDirectory+ '/' + file);
    fstream output_fstream;
    output_fstream.open(filename,std::ios_base::out);
    output_fstream << data_in_file;

    return true;
}

bool CDirectory::IsFile() const { return false;}
bool CDirectory::IsDirectory() const {return true;}
bool CDirectory::IsLink() const {return false;}

// reading from file
// for root directory
CDirectory::CDirectory(fstream & sstream) {
    path_of_data_unit = fs::current_path();
    string type_of_data_unit, hash_of_data_unit, name_of_data_unit, path_of_data_unit;
        size_t number_of_records;
        sstream >> number_of_records;

        for (size_t i = 0; i < number_of_records; ++i) {

            // о том, что или файл poskozeny или число записей неправильное
            try {
                sstream >> type_of_data_unit;
                sstream >> hash_of_data_unit;
                sstream >> name_of_data_unit;
                sstream >> path_of_data_unit;
            }
            catch (const std::exception & e)
            {
                std::cout << "Problems with data files!" << std::endl;
                std::cout << e.what() << std::endl;
            }

            if (type_of_data_unit == "file")
                data_units.insert({name_of_data_unit,make_shared<CFile>(path_of_data_unit,name_of_data_unit, hash_of_data_unit)});
            else {
                fstream new_stream;
                string tree_file = fs::current_path().string() + OBJ_DIR + "/"
                                   + hash_of_data_unit.substr(0,2) + "/" + hash_of_data_unit.substr(2);
                new_stream.open(tree_file, std::ios::in);

                data_units.insert({name_of_data_unit,make_shared<CDirectory>(new_stream, path_of_data_unit, name_of_data_unit)});

                new_stream.close();
            }
        }

}
// for not root directories
CDirectory::CDirectory(fstream & sstream, string path, string name) {

    path_of_data_unit = move(path);
    name_of_data_unit = move(name);

    string type_of_data_unit, hash_of_data_unit, name_of_data_unit, path_of_data_unit;
    while (sstream >> type_of_data_unit)
    {
        try {
            sstream >> hash_of_data_unit;
            sstream >> name_of_data_unit;
            sstream >> path_of_data_unit;
        }
        catch (const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
        if (type_of_data_unit == "file")
            data_units.insert({name_of_data_unit,make_shared<CFile>(path_of_data_unit,
                                                 name_of_data_unit,
                                                 hash_of_data_unit)});
        if (type_of_data_unit == "tree") {
            fstream new_stream;
            string tree_file = fs::current_path().string() + OBJ_DIR + "/"
                               + hash_of_data_unit.substr(0,2) + "/" + hash_of_data_unit.substr(2);
            new_stream.open(tree_file, std::ios::in);

            data_units.insert({name_of_data_unit, make_shared<CDirectory>(new_stream,
                                                      path_of_data_unit,
                                                      name_of_data_unit)});

            new_stream.close();
        }
    }


}

void CDirectory::Restore() const  {

    // creating of folder
    if ( !name_of_data_unit.empty() && !fs::exists(path_of_data_unit) )
        fs::create_directory(path_of_data_unit);
    else
        for (const auto& entry : fs::directory_iterator(path_of_data_unit)){
            if (!IsSubstrInStr(entry.path().string(),".backups") &&
                !IsSubstrInStr(entry.path().string(),NAME_OF_PROGRAMME) )
                fs::remove_all(entry.path());
        }

    // enumeration of folder
    for( const auto & x : data_units){
        if (x.second->IsDirectory()){
            x.second->Restore();
        }
        if (x.second->IsFile()){
            x.second->Restore();
        }
    }
}
string CDirectory::FindDataUnit(const string &name) const {
    return data_units.count(name) == 0 ? "" : data_units.find(name)->second->GetHash() ;
}

shared_ptr<CDataUnit> CDirectory::FindDataUnitInDirectory(const string &name) {
        if ( data_units.count(name) == 0 )
            throw FileException("Not Find!");
        return data_units.find(name)->second;
}

set<string> CDirectory::GetAllDataUnits() {
    set<string> name_data_units;
    for ( const auto & x : data_units){
        if ( x.second->IsFile())
            name_data_units.insert(x.first);
        else
            name_data_units.insert(x.first+"/");
    }
    return name_data_units;
}

vector<shared_ptr<CDataUnit>> CDirectory::GetAllDataUnitsPtr() {
    vector<shared_ptr<CDataUnit>> ptr_data_units;
    for ( const auto & x : data_units)
        ptr_data_units.push_back(x.second);

    return ptr_data_units;
}

void CDirectory::ShowAllUnits()
{
    for (const auto & x : data_units)
        std::cout << x.first << " -- name " << std::endl;
}

void CDirectory::Print(size_t level) const {
    for (const auto & x : data_units)
    {
        if (x.second->IsDirectory())
        {
            auto lead = string(level * 3, '-');
            std::cout << lead << x.first << std::endl;
            x.second->Print(level+1);
        }else
            x.second->Print(level);
    }
}

CDirectory::~CDirectory() = default;
