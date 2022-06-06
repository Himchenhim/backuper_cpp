#include "CDataUnit.hpp"
#include "exceptions.h"
#include "CBackup.hpp"
#include "texts.hpp"
#include "filesystem_work.hpp"
#include <filesystem>
#include <map>
#include <utility>
#include <vector>
#include "event_scheduler.hpp"
#include <climits>


using std::cout;
using std::endl;
using std::cin;
using std::isdigit;
using std::stoi;
using std::invalid_argument;
using std::map;
using std::exception;
using std::vector;


namespace fs = std::filesystem;
namespace txt =  text;

void ShowStartMenu(const string & text_before_menu="") {

    printf("\x1b[1;1H\x1b[2J");

    if (!text_before_menu.empty())
        cout << text_before_menu << endl;
    cout << txt::reminder;
}



bool NameExistsInBackups(vector<shared_ptr<CBackup>> & backups,const string & name)
{
    for (const auto & x : backups)
        if ( name == x->GetName())
            return true;
    return false;
}
string HashExistInBackup(vector<shared_ptr<CBackup>> & all_backups,const string & name)
{
    for ( auto name_it = all_backups.begin(); name_it != all_backups.end(); ++name_it)
        if ( (*name_it)->GetName() == name)
            for (const auto & x : all_backups)
                if (x->GetHashData() == (*name_it)->GetHashData() && !(x->GetName() == name))
                    return x->GetName();

    return "";
}

bool check_valid_input_main_functions(string & input, int & input_number){
    try {
        input_number = std::stoi(input);
    }catch (const invalid_argument & err) {
        cout << "Programme does not support working with name of number (e.g 'first', 'one')" << endl;
        return false;
    }

    if (input_number < 0 || input_number > 6) {
        cout << "Invalid command!" << endl;
        return false;
    }

    return true;
}
bool check_valid_input_compare_function(string & input, int & input_number) {
    try {
        input_number = std::stoi(input);
    }catch (const invalid_argument & err) {
        cout << "Programme does not support working with name of number (e.g 'first', 'one')" << endl;
        return false;
    }

    if (input_number != 1 && input_number != 2) {
        cout << "Invalid command!" << endl;
        return false;
    }

    return true;
}

bool check_valid_input_time(string & input, int & input_number)
{
    try {
        input_number = std::stoi(input);
    }catch (const invalid_argument & err) {
        cout << "Programme does not support working with name of number (e.g 'first', 'one')" << endl;
        return false;
    }

    if (input_number < 0 || input_number > 4) {
        cout << "Invalid command!" << endl;
        return false;
    }

    return true;
}



void interaction_with_user( vector <shared_ptr<CBackup>> & all_backups,int time_mode)
{
    // initialization of input variables
    string input;
    int input_number = 5;

    // variable for helping to do good output;
    string text_out;

    // interaction with user
    cout << txt::greetings;
    try {
        while (true) {

            if (time_mode != 0)
                if (scheduler::timer()) {
                    // delete everything from file
                    fstream schedule_file;
                    schedule_file.open(fs::current_path()/=".backups/time_mode",std::ios::out);
                    schedule_file<<"";
                    schedule_file.close();
                    time_mode = 0;
                }


            text_out = "";
            if (!(getline(cin, input) && !cin.eof()))
                throw InputException("There is end of input!");

            if (!check_valid_input_main_functions(input, input_number))
                continue;

            switch (input_number) {
                case 0: {
                    string new_backup;
                    cout << "Please, write name of backup, how you want to name your backup (in one word): ";

                    if (!(cin >> new_backup)) { break; }
                    cin.clear();
                    fflush(stdin);
                    std::cin.ignore(1);
                    while (NameExistsInBackups(all_backups, new_backup)) {
                        cout << "Please, try another name. It's name is already used." << endl;
                        if (!(getline(cin, new_backup) && !cin.eof())) break;

                    }

                    try {
                        CreateBackup(all_backups, new_backup);
                        text_out += txt::created_backup;
                    }
                    catch (const std::exception &e) {
                        cout << e.what() << endl;
                    }

                    string name_of_backup = HashExistInBackup(all_backups, new_backup);
                    if (!name_of_backup.empty()) {
                        text_out = "\nMinimum one backup found with the same files and directories, it has name: "
                                   + name_of_backup + "\n";
                    }
                    ShowStartMenu(text_out);
                    break;
                }

                case 1: {
                    string old_backup;
                    cout << "Please, write name of backup, which you want to return: ";
                    if (!(cin >> old_backup)) { break; }
                    cin.clear();
                    fflush(stdin);
                    std::cin.ignore(1);
                    if (!NameExistsInBackups(all_backups, old_backup)) {
                        text_out += "Sorry, you don't have backup with this name.\n";
                        ShowStartMenu(text_out);
                        break;
                    }

                    ReturnBackup(all_backups, old_backup);
                    text_out += "Backup is successfully restored in your curren folder!\n";
                    ShowStartMenu(text_out);
                    break;
                }

                case 2: {
                    cout << "If yo want to compare:\n"
                            "backup and actual state of directory - enter '1' \n"
                            "backup and another backup - enter '2'" << endl;

                    if (!(cin >> input))
                        throw InputException("There is end of input!");
                    cin.clear();
                    fflush(stdin);
                    std::cin.ignore(1);
                    if (!check_valid_input_compare_function(input, input_number))
                        continue;

                    switch (input_number) {
                            case 1 :{
                                string name_of_directory_for_comparing;
                                string tmp;
                                cout << "Please, enter the name of backup with which\n"
                                        "you want to compare current state of directory\n";
                                if (!(cin >> name_of_directory_for_comparing)) {
                                    throw InputException("There is end of input!");
                                } else {
                                    // OK
                                    text_out += name_of_directory_for_comparing;
                                    cin.clear();
                                    fflush(stdin);
                                    std::cin.ignore(1);

                                    try {
                                        const CBackup tmp_backup = FindBackup(all_backups, name_of_directory_for_comparing);
                                        CompareBackupWithActualState(tmp_backup);
                                    }
                                    catch (const std::exception &e) {

                                        text_out += e.what();
                                        ShowStartMenu(text_out);
                                        continue;
                                    }
                                }
                                break;
                            }
                            case 2 : {
                                string old_backup, new_backup;
                                cout << "Please, enter the name of new backup (one word name)\n";

                                if (cin >> old_backup) {
                                    cin.clear();
                                    fflush(stdin);
                                    std::cin.ignore(1);
                                    cout << "Please, enter the name of old backup (one word name)\n";

                                    if (cin >> new_backup) {
                                        cin.clear();
                                        fflush(stdin);
                                        std::cin.ignore(1);
                                        try {
                                            const CBackup old_compare_backup = FindBackup(all_backups, old_backup);
                                            const CBackup new_compare_backup = FindBackup(all_backups, new_backup);

                                            CompareBackups(old_compare_backup, new_compare_backup);
                                        }
                                        catch (const std::exception &e) {

                                            text_out += e.what();
                                            ShowStartMenu(text_out);
                                            continue;
                                        }
                                    }
                                    else
                                        throw InputException("There is end of input!");
                                } else
                                    throw InputException("There is end of input!");
                                break;
                            }
                        }
                        break;
                    }
                    case 3: {
                        ShowStartMenu();
                        ShowBackups(all_backups);
                        break;
                    }
                    case 4:
                    {

                        cout << "Please, choose time, how long wait to do backup of current directory:\n"
                                "do not do backups depends on time - 0\n"
                                "1 day - 1\n"
                                "7 days - 2\n"
                                "30 days - 3\n"
                                "365 days - 4\n";

                        if (!(getline(cin, input) && !cin.eof()))
                            throw InputException("There is end of input!");

                        if (!check_valid_input_time(input, input_number))
                            continue;

                        auto now = std::chrono::system_clock::now();
                        switch (input_number) {
                            case 0: {
                                time_mode = 3;
                                scheduler::add(CreateBackupOnTime,now+std::chrono::seconds(10));
                                SaveInfoToScheduleMode(time_mode,now+std::chrono::seconds(10));
                                break;
                            }
                            case 1: {
                                time_mode = 1;
                                scheduler::add(CreateBackupOnTime,now+std::chrono::seconds(86400));
                                SaveInfoToScheduleMode(time_mode,now+std::chrono::seconds(86400));
                                break;
                            }
                            case 2: {
                                time_mode = 2;
                                scheduler::add(CreateBackupOnTime,now+std::chrono::seconds(604800));
                                SaveInfoToScheduleMode(time_mode,now+std::chrono::seconds(604800));
                                break;
                            }
                            case 3: {
                                time_mode = 3;
                                scheduler::add(CreateBackupOnTime,now+std::chrono::seconds(2592000));
                                SaveInfoToScheduleMode(time_mode,now+std::chrono::seconds(2592000));
                                break;
                            }
                            case 4: {
                                time_mode = 4;
                                scheduler::add(CreateBackupOnTime,now+std::chrono::seconds(31536000));
                                SaveInfoToScheduleMode(time_mode,now+std::chrono::seconds(31536000));
                                break;
                            }
                        }
                        break;
                    }

                    case 5:
                        return;
                }

        }
    }
    catch (const std::exception & e){
        cout << e.what() << endl;
    }
}

int main() {

    try{
        init_directory();

    }catch (...){
        cout << "Exception occurred" << endl;
        return 1;
    }

    vector<shared_ptr<CBackup>> all_backups;

    // read information about next date if it exists backup from file time_mode
    int time_mode_to_backup;
    long  time_to_next_backup;
    try {
        if (fs::exists(fs::current_path()/=".backups/time_mode")){
            fstream time_mode_file;
            time_mode_file.open(fs::current_path()/=".backups/time_mode",std::ios::in);
            string mode, sec_from_epoch;
            time_mode_file >> mode;
            time_mode_to_backup = stoi(mode);
            if (time_mode_to_backup != 0) {
                time_mode_file >> sec_from_epoch;
                cout << "pis" << endl;
                time_to_next_backup = stoi(sec_from_epoch);
                cout << "1pis" << endl;
                time_t epoch = time_to_next_backup;
                scheduler::add(CreateBackupOnTime,epoch);
            }
        }else
            time_mode_to_backup = init_time_mode();
    }
    catch(const std::invalid_argument & e){
    }
    catch(const std::exception & e)
    {
        cout << e.what()<< endl;
        return 1;
    }

    ReadPreviousBackups(all_backups);

    try {
        interaction_with_user(all_backups,time_mode_to_backup);
    }catch (const std::exception  & e)
    {
        cout << e.what();
        return 1;
    }


    return 0;
}


