#include "CBackup.hpp"
#include "CDirectory.hpp"
#include "texts.hpp"
#include "hashes.hpp"
#include "exceptions.h"
#include <filesystem>
#include <chrono>


namespace fs = std::filesystem;
using std::make_shared;
using std::shared_ptr;


CBackup::CBackup(string name) : name_of_backup(move(name)){
    root_directory = make_shared<CDirectory>(fs::current_path().c_str(),"");
    hash_of_data = root_directory->hash_of_data_unit;
    hash_of_backup = CalcSha256ForString(root_directory->data_in_file + " " + name_of_backup + " " + hash_of_backup ).value();
    SaveBackup();
}

bool CBackup::SaveBackup()  {
    string directory = hash_of_backup.substr(0,2);
    string path_to_dir = ( (fs::current_path() /= ".backups/ref_to_backups/") += directory );
    if ( !fs::exists(path_to_dir))
        fs::create_directory(path_to_dir);

    // создать файл и напихать туда контента из data а потом добавить своего
    string file = hash_of_backup.substr(2);
    string file_location = path_to_dir + '/' + file;

    std::fstream output_fstream;
    output_fstream.open(file_location,std::ios_base::out);

    // тут нужно подсчитать сколько раз '\n' встречается в тексте
    size_t number_of_str = number_of_repeating(root_directory->data_in_file, "\n");
    output_fstream << number_of_str << "\n" << root_directory->data_in_file;
    auto now_time = std::chrono::system_clock::now();
    time_of_backup = std::chrono::system_clock::to_time_t(now_time);
    // adding additional info about Backup
    output_fstream << name_of_backup << " " << hash_of_backup << " " << hash_of_data << " " << time_of_backup;
    output_fstream.close();
    return true;
}

CBackup::CBackup(std::fstream & input_stream) {

    root_directory = make_shared<CDirectory> (input_stream);
    input_stream >> name_of_backup >> hash_of_backup >> hash_of_data >> time_of_backup;
    cout << name_of_backup << endl;
}

void CBackup::ReturnBackupToDirectory(const string &path) {
    root_directory->Restore();
}

// -- new file/directory - если путь данного файла не найдён в старом бэкапе
// -- modified file - если путь данного файла найден, но хэщ разный
// -- deleted file - если данный файл был в старом бэкапе, а сейчас его нет

void CompareBackups(const CBackup &old_backup, const CBackup &new_backup){

}

void ShowChanges(vector<string> & created, vector<string> & modified, set<string> & deleted){
    cout << "--------------------------------------------" << endl;
    cout << "New files and directories:" << endl;
    for (const auto & x : created)
        cout << x << endl;

    cout << "--------------------------------------------" << endl;
    cout << "Modified files and directories:" << endl;
    for (const auto & x : modified)
        cout << x << endl;

    cout << "--------------------------------------------" << endl;
    cout << "Deleted files and directories:" << endl;
    for (const auto & x : deleted)
        cout << x << endl;


}

void CompareDirectoryWithActualState(const shared_ptr<CDirectory> & directory,
                                     vector<string> &created,
                                     vector<string> &modified,
                                     set<string> &deleted,
                                     string path){ // jack/robert
    set<string> tmp_deleted = directory->GetAllDataUnits();
    path += directory->GetName() + "/";
    for (const auto & entry : fs::directory_iterator(fs::current_path() /= path ))
    {
        // for getting hash
        string path_to_file = entry.path().string();
        string filename = entry.path().filename().string();
        string hash_to_file = CalcSha256ForFile(path_to_file).value();
        cout << "filename: " << path+filename << endl;

        // try to find that name in our root directory
        if (fs::is_directory(path_to_file)){
            if ( tmp_deleted.find(filename) == tmp_deleted.end() ) {
                created.push_back(path + filename+"/");
            }
            else
            {
                shared_ptr<CDirectory> ptr_on_directory =
                        std::dynamic_pointer_cast<CDirectory>(directory->FindDataUnitInDirectory(filename));
                tmp_deleted.erase(filename);
                CompareDirectoryWithActualState(ptr_on_directory,created,modified,deleted,path);
            }
        }
        else if (fs::is_regular_file(path_to_file)) {
            try {
                shared_ptr<CDataUnit> tmp_data_unit = directory->FindDataUnitInDirectory(filename);
                // compare their hashes
                // regardless it is directory or file
                if (hash_to_file == tmp_data_unit->GetHash() )
                {
                    tmp_deleted.erase(filename);
                }else {
                    tmp_deleted.erase(filename);
                    modified.push_back(path+filename);
                }
            }
                // if file was not found
            catch (const FileException &e) {
                created.push_back(path+filename);
            }
        }
    }

    // konec
    for (const auto & x : tmp_deleted)
        deleted.insert(path+x );
}

void CompareBackupWithActualState(const CBackup & backup) {
    // проходимся по директории и находим в бэкапе файл сравниваем их
    // хэши, если это директория, то ныряем в директорию и смотрим, есть ли ткая же директория в
    // бэкапе, если нет то просто занимаемся выводом файлов и директорий, и возле всех пишем - new
    vector<string> created;
    vector<string> modified;
    set<string> deleted = backup.root_directory->GetAllDataUnits();

    for (const auto & entry : fs::directory_iterator(fs::current_path()))
    {
        //skip
        if (entry.path().filename().string() == ".backups" || entry.path().filename().string() == "bin" )
            continue;

        // for getting hash
        auto path = entry.path().string();
        auto filename = entry.path().filename().string();
        cout << "filename: " << filename << endl;

        // try to find that name in our root directory
        if (fs::is_directory(path)){
            if ( deleted.find(filename) == deleted.end() ) {
                created.push_back(filename + "/");
            }
            else
            {
                shared_ptr<CDirectory> ptr_on_directory = std::dynamic_pointer_cast<CDirectory>(backup.root_directory->FindDataUnitInDirectory(filename));

                cout << "^------------------------------" << endl;
                cout << ptr_on_directory->GetName() << endl;
                ptr_on_directory -> ShowAllUnits();
                cout << "^------------------------------" << endl;

                deleted.erase(filename);
                CompareDirectoryWithActualState(ptr_on_directory,created,modified,deleted,"");
            }
        }
        else if (fs::is_regular_file(path)) {
            try {
                shared_ptr<CDataUnit> tmp_data_unit = backup.root_directory->FindDataUnitInDirectory(filename);
                // compare their hashes
                // regardless it is directory or file
                if (CalcSha256ForFile(path) == tmp_data_unit->GetHash() )
                {
                    deleted.erase(filename);
                }else {
                    deleted.erase(filename);
                    modified.push_back(filename);
                }
            }
                // if file was not found
            catch (const FileException &e) {
                created.push_back(filename);
            }
        }
    }
    ShowChanges(created,modified,deleted);
}

CBackup::CBackup(const CBackup & src)= default;

const CBackup & FindBackup(vector <shared_ptr<CBackup>> & all_backups, const string & name_of_backup)
{
    for (const auto & x : all_backups)
    {
        if (x->GetName() == name_of_backup)
            return *x;
    }
    throw InputException("There are no backup with that name!");
}



//set<shared_ptr<CDataUnit>> deleted;
//set<shared_ptr<CDataUnit>> created;
//set<shared_ptr<CDataUnit>> modified;
//set<shared_ptr<CDataUnit>> same;
//
//for ( const auto & entry : fs::directory_iterator(directory_to_iterate)){
//auto filename = entry.path().string();
//
//if (fs::is_directory(entry.status())){
//
//}else if (fs::is_regular_file(entry.status())){
//// compare by name
//string hash = backup.root_directory->FindDataUnit(filename);
//
//if (hash.empty())
//// add file to -created-
//created.insert(make_shared<CFile>(fs::current_path()/=filename,filename,hash));
//// compare by hash
//else if (hash != CalcSha256ForFile(fs::current_path()/=filename))
//modified.insert(make_shared<CFile>(fs::current_path()/=filename,filename,hash));
//else
//same.insert(make_shared<CFile>(fs::current_path()/=filename,filename,hash));
//
//}
//}





