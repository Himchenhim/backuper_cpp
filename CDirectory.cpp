#include "CDirectory.hpp"

using std::make_shared;

#define BACKUP_DIR "/.backups/ref_to_backups/"
#define OBJ_DIR "/.backups/obj"


bool IsSubstrInStr(const string & big_str, const string & sub_str){
    return int(big_str.find(sub_str)) != -1;
}


CDirectory::CDirectory(const string & name) {
    name_of_data_unit = name;
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
    if (name != fs::current_path().string())
        // сохраняет только деревья, которые не являются корневыми
        // так как к корневому дереву нам нужно добавить дополнительную информацию
        SaveTree();
}

void CDirectory::AddDataUnits() {
    for (const auto & entry : fs::directory_iterator(name_of_data_unit))
    {
        auto filename = entry.path().filename();
        if ( filename.string() == ".backups" || filename.string() == NAME_OF_PROGRAMME)
            continue;

        // если это директория, то запускаем ещё один такой конструктор для директории
        if (fs::is_directory(entry.status()))
            AddDataUnit(make_shared<CDirectory>(name_of_data_unit + "/" + filename.string()));
        else if (fs::is_regular_file(entry.status()))
            // processing of file
            AddDataUnit(make_shared<CFile>(name_of_data_unit + "/" + filename.string()));

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

CDirectory::CDirectory(fstream & sstream) {

    string type_of_file, hash_of_file, name_of_file;
        size_t number_of_records;
        sstream >> number_of_records;

        for (size_t i = 0; i < number_of_records; ++i) {

            // о том, что или файл poskozeny или число записей неправильное
            try {
                sstream >> type_of_file;
                sstream >> hash_of_file;
                sstream >> name_of_file;
            }
            catch (const std::exception & e)
            {
                std::cout << e.what() << std::endl;
            }

            if (type_of_file == "file")
                data_units.insert(make_shared<CFile>(name_of_file, hash_of_file));
            else {
                fstream new_stream;
                string tree_file = fs::current_path().string() + OBJ_DIR
                                   + hash_of_file.substr(0,2) + "/" + hash_of_file.substr(2);

                new_stream.open(name_of_file, std::ios::in);
                data_units.insert(make_shared<CDirectory>(new_stream, name_of_file));
                new_stream.close();
            }
        }

}
CDirectory::CDirectory(fstream & sstream, string name) {
    name_of_data_unit = std::move(name);
    string type_of_file, hash_of_file, name_of_file;
    while (sstream >> type_of_file)
    {
        try {
            sstream >> hash_of_file;
            sstream >> name_of_file;
        }
        catch (const std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }

        if (type_of_file == "file")
            data_units.insert(make_shared<CFile>(name_of_file, hash_of_file));
        if (type_of_file == "tree") {
            fstream new_stream;
            string tree_file = fs::current_path().string() + OBJ_DIR
                               + hash_of_file.substr(0,2) + "/" + hash_of_file.substr(2);

            new_stream.open(tree_file, std::ios::in);
            data_units.insert(make_shared<CDirectory>(new_stream,name_of_file));
            new_stream.close();
        }
    }


}
void CDirectory::Restore() const  {

    // creating of folder
    if (!fs::exists(name_of_data_unit))
        fs::create_directory(name_of_data_unit);
    else
        for (const auto& entry : fs::directory_iterator(name_of_data_unit)){
            if (!IsSubstrInStr(entry.path().string(),".backups") &&
                !IsSubstrInStr(entry.path().string(),NAME_OF_PROGRAMME) )
                fs::remove_all(entry.path());
        }


    // enumeration of folder
    for( const auto & x : data_units){
        if (x->IsDirectory()){
            x->Restore();
        }
        if (x->IsFile()){
            x->Restore();
        }
    }
}

CDirectory::~CDirectory() {}
