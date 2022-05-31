#include "filesystem_work.hpp"

void init_directory() {
    if ( !fs::exists("./.backups") ){
        fs::create_directory("./.backups");
    }
    if ( !fs::exists("./.backups/obj")){
        cout << "hera1" << endl;
        fs::create_directory("./.backups/obj");
    }
    if ( !fs::exists("./.backups/ref_to_backups")){
        cout << "hera2" << endl;
        fs::create_directory("./.backups/ref_to_backups");
    }
}

void ReadPreviousBackups(set <shared_ptr<CBackup>> & backups){
    fs::path pathToRef = (fs::current_path() /= ".backups/ref_to_backups/");

    for (const auto & entry : fs::directory_iterator(pathToRef))
    {
        auto filename = entry.path().filename();
        if ( filename.string() == ".backups" || filename.string() == NAME_OF_PROGRAMME)
            continue;
        std::cout << filename.string() << std::endl;

        // если это директория, то запускаем ещё один такой конструктор для директории
        if (fs::is_regular_file(entry.status())) {
            // тут нужно как-то инициализировать BACKUP, потом добавлять его в set<shared_ptr<CBackup>>
            cout << (fs::current_path()/= filename) << endl;
        }
    }
}
uintmax_t ComputeFileSize (const fs::path& pathToCheck)
{
    if (fs::exists(pathToCheck) &&
        fs::is_regular_file(pathToCheck))
    {
        auto err = std::error_code{};
        auto filesize = fs::file_size(pathToCheck, err);
        if ( filesize != static_cast<uintmax_t>(-1))
            return filesize;
    }
    return static_cast<uintmax_t>(-1);
}
void DisplayFileInfo(const fs::directory_entry & entry, string & lead, fs::path &filename)
{
    cout << lead << " " << filename.c_str()<< ", " << ComputeFileSize(entry) << endl;
}
void CreateBackup (set <shared_ptr<CBackup>> & all_backups, string name){
    // iterator on our new backup
    all_backups.insert(make_shared<CBackup>( move(name) ));
}
void ShowBackups(set< shared_ptr<CBackup>> &backups) {
    for (const auto & x : backups)
    {
        std::cout << "name of backup: " << x->GetName() << std::endl;
    }
}