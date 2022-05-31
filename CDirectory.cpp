#include "CDirectory.hpp"
#include "CFile.hpp"
#include "hashes.hpp"
#include <chrono>

using std::make_shared;



CDirectory::CDirectory(string name, bool first_directory) {
    name_of_data_unit = move(name);
    // тут мы будем проходиться по нашей директории и дополнять в неё элементы
    AddDataUnits();
    // так же потом после этого посчитаем её хеш (отдельная приватная функция)

    for (const auto & data_unit : data_units)
    {
        if ( data_unit->IsFile())
            data_in_file += "file ";
        else
            data_in_file += "tree ";
        data_in_file += (data_unit->GetHash() + " ");
        data_in_file += (data_unit->GetName() + "\n");
    }
    hash_of_data_unit = (CalcSha256ForString(data_in_file)).value();
    std::cout << data_in_file;

    // и запишем в файл (отдельная приватная функция)
    if (!first_directory)
        // сохраняет только деревья, которые не являются корневыми
        // так как к корневому дереву нам нужно добавить дополнительную информацию
        SaveTree();

}
CDirectory::~CDirectory() {}

void CDirectory::AddDataUnits() {
    for (const auto & entry : fs::directory_iterator(name_of_data_unit))
    {
        auto filename = entry.path().filename();
        if ( filename.string() == ".backups" || filename.string() == NAME_OF_PROGRAMME)
            continue;
        std::cout << filename.string() << std::endl;

        // если это директория, то запускаем ещё один такой конструктор для директории
        if (fs::is_directory(entry.status()))
        {
            std::cout <<  ( (fs::current_path()/=name_of_data_unit)/=filename) << std::endl;
            AddDataUnit(make_shared<CDirectory>(( (fs::current_path()/=name_of_data_unit)/=filename), false));
            // обработка директории
        }else if (fs::is_regular_file(entry.status())){
            // обработка файла
            std::cout <<  (fs::current_path()/=filename) << std::endl;
            AddDataUnit(make_shared<CFile>(fs::current_path()/=filename));
        }
    }
}
bool CDirectory::AddDataUnit(const shared_ptr<CDataUnit> & src) {
    // тут мы будем просто добавлять имя + хеш файла
    data_units.insert(src);
    return true;
}
bool CDirectory::SaveTree() const {
    string directory = hash_of_data_unit.substr(0, 2);
    string file = hash_of_data_unit.substr(2);

    std::cout << hash_of_data_unit << std::endl;
    std::cout << directory << std::endl;
    std::cout << file << std::endl;

    string pathToRootDirectory;

    pathToRootDirectory = ( (fs::current_path() /= ".backups/obj") /= directory );
    if ( !fs::exists(pathToRootDirectory))
        fs::create_directory(pathToRootDirectory);
    string filename (pathToRootDirectory+ '/' + file);
    std::fstream output_fstream;
    output_fstream.open(filename,std::ios_base::out);
    output_fstream << data_in_file;

    return true;
}

bool CDirectory::IsFile() const { return false;}
bool CDirectory::IsDirectory() const {return true;}
bool CDirectory::IsLink() const {return false;}




