#include "CBackup.hpp"
#include "CDirectory.hpp"
#include "texts.hpp"
#include "hashes.hpp"
#include <filesystem>
#include <chrono>


namespace fs = std::filesystem;
using std::make_shared;
using std::shared_ptr;


CBackup::CBackup(string name) : name_of_backup(move(name)){
    root_directory = make_shared<CDirectory>(fs::current_path().c_str());
    hash_of_data = root_directory->hash_of_data_unit;
    hash_of_backup = CalcSha256ForString(root_directory->data_in_file + name_of_backup + " " + hash_of_backup ).value();
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

}

void CBackup::ReturnBackupToDirectory(const string &path) {
    root_directory->Restore();
}


