#include "CDirectory.hpp"
#include "CFile.hpp"
#include "hashes.hpp"

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
        data_in_file += (data_unit->GetHash() + " ");
        data_in_file += (data_unit->GetName() + "\n");
    }
    hash_of_data_unit = (CalcSha256ForString(data_in_file)).value();

    // и запишем в файл (отдельная приватная функция)
    SaveTree(first_directory);

}
CDirectory::~CDirectory() {}

void CDirectory::AddDataUnits() {
    for (const auto & entry : fs::directory_iterator(name_of_data_unit))
    {
        auto filename = entry.path().filename();

        // если это директория, то запускаем ещё один такой конструктор для директории
        if (fs::is_directory(entry.status()))
        {
            AddDataUnit(make_shared<CDirectory>(fs::current_path()/=filename, false));
            // обработка директории
        }else if (fs::is_regular_file(entry.status())){
            // обработка файла
            AddDataUnit(make_shared<CFile>(filename.c_str()));
        }
    }
}
bool CDirectory::AddDataUnit(const shared_ptr<CDataUnit> & src) {
    // тут мы будем просто добавлять имя + хеш файла
    data_units.insert(src);
    return true;
}
bool CDirectory::SaveTree(bool first_directory) const {
    string directory = hash_of_data_unit.substr(0, 2);
    string file = hash_of_data_unit.substr(2);

    std::cout << hash_of_data_unit << std::endl;
    std::cout << directory << std::endl;
    std::cout << file << std::endl;

    string pathToRootDirectory;
    if (first_directory) {
        // создание директории
        pathToRootDirectory = ( (fs::current_path() /= ".backups/ref_to_backups")  /= directory);
    }
    else {
         pathToRootDirectory = ( (fs::current_path() /= ".backups/obj")  /= directory);
    }
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



