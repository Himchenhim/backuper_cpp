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


int init_time_mode(){
    if (!fs::exists(fs::current_path() /=".backups/time_mode"))
    {
        fstream time_mode_file;
        time_mode_file.open(fs::current_path() /=".backups/time_mode", std::ios::out);
        time_mode_file << 0;
        time_mode_file.close();
        return 0;
    }
    fstream time_mode_file;
    time_mode_file.open(fs::current_path() /=".backups/time_mode", std::ios::in);
    string number;
    time_mode_file >> number;

    return stoi(number);
}

void SaveInfoToScheduleMode(int mode, const std::chrono::time_point<std::chrono::system_clock> &time_after_backup){
    fstream schedule_file;
    schedule_file.open(fs::current_path()/=".backups/time_mode", std::ios::out);
    std::chrono::system_clock::duration dtn = time_after_backup.time_since_epoch();
    schedule_file << mode << "\n"
            <<  (dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den );
    schedule_file.close();
}


void deleteDirectoryContents(const string& dir_path)
{
    for (const auto& entry : fs::directory_iterator(dir_path))
        fs::remove_all(entry.path());
}



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

void CreateBackupOnTime(){
    cout << "Saving backup on time!\n"
            "To load your backup, you should reload programme" << endl;

    time_t now = time(0);
    tm *ltm = localtime(&now);

    string y = std::to_string(1900+ltm->tm_year);
    string m = std::to_string(1+ltm->tm_mon);
    string d = std::to_string(ltm->tm_mday);

    make_shared<CBackup>(d+"-"+m+"-"+y);
}


void ReturnBackup(vector <shared_ptr<CBackup>> & all_backups, const string & name) {
    for ( const auto & x : all_backups){
        if (x->GetName() == name){
            try {
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

