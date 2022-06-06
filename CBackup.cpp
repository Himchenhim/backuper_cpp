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
using std::dynamic_pointer_cast;


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

    // create file and after that add contents as for directory and after that write to backup file some extra info
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
}

void CBackup::ReturnBackupToDirectory() {
    root_directory->Restore();
}

void ShowChanges(vector<string> & created, vector<string> & modified, set<string> & deleted, bool backups){
    bool flag = true;
    cout << "--------------------------------------------" << endl;
    cout << "New files and directories:" << endl;
    for (const auto & x : created) {
        cout << x << endl;
        flag = false;
    }
    cout << "--------------------------------------------" << endl;
    cout << "Modified files and directories:" << endl;
    for (const auto & x : modified){
        cout << x << endl;
        flag = false;
    }
    cout << "--------------------------------------------" << endl;
    cout << "Deleted files and directories:" << endl;
    for (const auto & x : deleted){
        cout << x << endl;
        flag = false;
    }
    cout << "--------------------------------------------" << endl;

    if (flag){
        if (backups)
            cout << "Backups the same!" << endl;
        else
            cout << "Directory is the same as backup!" << endl;
    }

}



void CompareDirectories(const shared_ptr<CDirectory> & old_dir,
                        const shared_ptr<CDirectory> & new_dir,
                        vector<string> &created,
                        vector<string> &modified,
                        set<string> &deleted, string path){

    set <string> tmp_deleted = old_dir->GetAllDataUnits();;
    vector <shared_ptr<CDataUnit>> new_all_units_in_directory = new_dir->GetAllDataUnitsPtr();
    path += old_dir->GetName() + "/";

    for (const auto & new_data_unit : new_all_units_in_directory){
        if (new_data_unit->IsFile())
        {
            try {
                shared_ptr<CDataUnit> old_ptr_on_data_unit =
                        old_dir->FindDataUnitInDirectory(new_data_unit->GetName());

                if (!old_ptr_on_data_unit->IsFile()) {
                    created.push_back(path+new_data_unit->GetName());
                    continue;
                }
                shared_ptr<CFile> old_ptr_on_file = dynamic_pointer_cast<CFile>(old_ptr_on_data_unit);

                if (old_ptr_on_file->GetHash() != new_data_unit->GetHash())
                    modified.push_back(path+new_data_unit->GetName());
                tmp_deleted.erase(new_data_unit->GetName());
            }
            catch (const FileException & e) {
                created.push_back(path+new_data_unit->GetName());
            }
        }
        else if (new_data_unit->IsDirectory())
        {
            try {

                shared_ptr<CDataUnit> old_ptr_on_data_unit
                        = old_dir->FindDataUnitInDirectory(new_data_unit->GetName());

                if (old_ptr_on_data_unit->IsDirectory()) {
                    // continue to work with that
                    shared_ptr<CDirectory> old_ptr_on_directory = dynamic_pointer_cast<CDirectory>( old_ptr_on_data_unit);

                    // need to dive into directory ...
                    CompareDirectories(dynamic_pointer_cast<CDirectory>(new_data_unit),
                                       old_ptr_on_directory,
                                       created,
                                       modified,
                                       deleted,path);

                    tmp_deleted.erase(new_data_unit->GetName()+"/");
                }else {
                    created.push_back(path+new_data_unit->GetName()+"/");
                    continue;
                }
            }
            catch (const FileException & e) {
                created.push_back(path + new_data_unit->GetName() + "/");
            }
        }
    }

    // inserting to delete list
    for (const auto & x : tmp_deleted)
        deleted.insert(x);
}


void CompareBackups(const CBackup & old_backup, const CBackup & new_backup){
    vector <string> created;
    vector <string> modified;
    // when we find file/directory in second directory, we will delete from 'deleted' that file/directory
    set <string> deleted = old_backup.root_directory->GetAllDataUnits();;
    vector <shared_ptr<CDataUnit>> new_all_units_in_directory = new_backup.root_directory->GetAllDataUnitsPtr();


    for (const auto & new_data_unit : new_all_units_in_directory){
        if (new_data_unit->IsFile())
        {
            try {
                shared_ptr<CDataUnit> old_ptr_on_data_unit =
                      old_backup.root_directory->FindDataUnitInDirectory(new_data_unit->GetName());

                if (!old_ptr_on_data_unit->IsFile()) {
                    created.push_back(new_data_unit->GetName());
                    continue;
                }
                shared_ptr<CFile> old_ptr_on_file = dynamic_pointer_cast<CFile>(old_ptr_on_data_unit);

                if (old_ptr_on_file->GetHash() != new_data_unit->GetHash())
                      modified.push_back(new_data_unit->GetName());
                deleted.erase(new_data_unit->GetName());
            }
            catch (const FileException & e) {
                created.push_back(new_data_unit->GetName());
            }
        }else if (new_data_unit->IsDirectory())
        {
            try {

                shared_ptr<CDataUnit> old_ptr_on_data_unit
                    = old_backup.root_directory->FindDataUnitInDirectory(new_data_unit->GetName());

                if (old_ptr_on_data_unit->IsDirectory()) {

                    // continue to work with that
                    shared_ptr<CDirectory> old_ptr_on_directory = dynamic_pointer_cast<CDirectory>( old_ptr_on_data_unit);

                    // need to dive into directory ...
                    CompareDirectories(dynamic_pointer_cast<CDirectory>(new_data_unit),
                            old_ptr_on_directory,
                            created,
                            modified,
                            deleted, "");

                    deleted.erase(new_data_unit->GetName()+"/");
                }else {
                    created.push_back(new_data_unit->GetName()+"/");
                    continue;
                }
            }
            catch (const FileException & e) {
                created.push_back(new_data_unit->GetName() + "/");
            }
        }
    }

    ShowChanges(created,modified,deleted,true);

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

        // try to find that name in our root directory
        if (fs::is_directory(path_to_file)){
            try{

                shared_ptr<CDataUnit> ptr_on_data_unit =directory->FindDataUnitInDirectory(filename);

                if (!ptr_on_data_unit->IsDirectory())
                {
                    created.push_back(path+filename+'/');
                    continue;
                }

                shared_ptr<CDirectory> ptr_on_dir = dynamic_pointer_cast<CDirectory>(directory->FindDataUnitInDirectory(filename));

                tmp_deleted.erase(filename+"/");
                CompareDirectoryWithActualState(ptr_on_dir,created,modified,deleted,path);

            }
            catch(const FileException & e){

                created.push_back(path + filename+"/");

            }


        }
        else if (fs::is_regular_file(path_to_file)) {
            try {
                shared_ptr<CDataUnit> tmp_data_unit = directory->FindDataUnitInDirectory(filename);

                if (!tmp_data_unit->IsFile()){
                    created.push_back(filename);
                    continue;
                }
                shared_ptr <CFile> tmp_file = dynamic_pointer_cast<CFile>(tmp_data_unit);
                // compare their hashes
                // regardless it is directory or file
                if (hash_to_file != tmp_file->GetHash())
                    modified.push_back(path+filename);
                tmp_deleted.erase(filename);

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

        if (fs::is_directory(path)){
            // если он нашёл её, но она оказывается НЕ директория
            try {

                shared_ptr<CDataUnit> ptr_on_data_unit = backup.root_directory->FindDataUnitInDirectory(filename);

                if (!ptr_on_data_unit->IsDirectory()) {
                    created.push_back(filename);
                    continue;
                }

                shared_ptr<CDirectory> ptr_on_directory =
                        std::dynamic_pointer_cast<CDirectory>(backup.root_directory->FindDataUnitInDirectory(filename));

                deleted.erase(filename + "/");

                CompareDirectoryWithActualState(ptr_on_directory, created, modified, deleted, "");
            }
            catch( const FileException & e){
                created.push_back(filename+"/");
            }
        }
        else if (fs::is_regular_file(path)) {
            try {
                shared_ptr<CDataUnit> tmp_data_unit = backup.root_directory->FindDataUnitInDirectory(filename);

                if (! tmp_data_unit->IsFile() ){
                    created.push_back(filename+"/");
                    continue;
                }

                // compare their hashes
                // regardless it is directory or file
                if (CalcSha256ForFile(path) != tmp_data_unit->GetHash() )
                    modified.push_back(filename);
                deleted.erase(filename);
            }

            // if file was not found
            catch (const FileException &e) {
                created.push_back(filename);
            }
        }
    }

    ShowChanges(created,modified,deleted,false);

}

void CBackup::ShowBackup() {
    root_directory->Print(0);
}

CBackup::CBackup(const CBackup & src) = default;

const CBackup & FindBackup(vector <shared_ptr<CBackup>> & all_backups, const string & name_of_backup)
{
    for (const auto & x : all_backups)
    {
        if (x->GetName() == name_of_backup)
            return *x;
    }
    throw InputException("There are no backup with that name!");
}









