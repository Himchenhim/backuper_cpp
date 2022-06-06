#include "filesystem_work.hpp"

void init_directory() {
    if ( !fs::exists("./.backups") ){
        fs::create_directory("./.backups");
    }
    if ( !fs::exists("./.backups/obj")){
        fs::create_directory("./.backups/obj");
    }
    if ( !fs::exists("./.backups/ref_to_backups")){
        fs::create_directory("./.backups/ref_to_backups");
    }
}


void deleteDirectoryContents(const string& dir_path)
{
    for (const auto& entry : fs::directory_iterator(dir_path))
        fs::remove_all(entry.path());
}

// изначально, мы проходимся по директории всех коммитов, потом мы запускаем для
// каждой директории свой цикл, для того, что бы собрать информацию о коммитте и
// создать новый объект CBackup. Сложность состоит в том, как правильно инициализировать
// директории и сам <set<shared_ptr<CDataUnit>>> внутри самого CDirectory
void ReadPreviousBackups(vector<shared_ptr<CBackup>> & backups){
    fs::path pathToRef = fs::current_path().string() + "/.backups/ref_to_backups/";

    for (const auto & entry : fs::directory_iterator(pathToRef))
    {
        auto filename = entry.path().filename();
        if ( filename.string() == ".backups" || filename.string() == NAME_OF_PROGRAMME)
            continue;

        // тут мы заходим поочередно в каждую директорию, состоящую из 2ух символов
        string absolute_dir_path = (pathToRef.string() + "/" + filename.string());
        if (fs::is_directory(absolute_dir_path)){

            for ( const auto & entry_in : fs::directory_iterator(absolute_dir_path) ){
                auto filename_in = entry_in.path().filename();
                string file_to_open = ( absolute_dir_path + "/" + filename_in.string());

                // теперь нужно открыть файл и сделать с него объект CBackup
                std::fstream input_stream;
                input_stream.open(file_to_open,std::ios_base::in);

                backups.push_back(make_shared<CBackup>(input_stream));
            }

        }

    }
}
void CreateBackup (vector<shared_ptr<CBackup>> & all_backups, string name){
    // iterator on our new backup
    all_backups.push_back(make_shared<CBackup>( move(name) ));
}


void ReturnBackup(vector <shared_ptr<CBackup>> & all_backups, const string & name) {
    for ( const auto & x : all_backups){
        if (x->GetName() == name){
            try {
                // fstream opened_backup(path_to_file);
                string dir_to_backup = fs::current_path().string();
                x->ReturnBackupToDirectory();
            }
            catch (const std::exception & e){
                cout << e.what() << "\n error in returning backup"<< endl;
            }

        }

    }
}

void ShowBackups(vector< shared_ptr<CBackup>> &backups) {
    if (backups.empty())
        cout << "You don't have backups..." << endl;
    size_t counter = 1;
    for (const auto & x : backups)
    {
        time_t time_of_backup = x->GetTime();
        std::cout << counter << ") name backup: " << x->GetName()
//            <<"\n - hash of backup: " <<  x->GetHashBackup()
            <<"\n - time: " <<ctime(&time_of_backup) << std::endl;
        ++counter;
    }
}

