#include "CDataUnit.hpp"
#include "CBackup.hpp"
#include "texts.hpp"
#include "filesystem_work.hpp"
#include <filesystem>
#include <map>
#include <utility>
#include <vector>

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

class InputException : public std::exception
{
    std::string _msg;
public:
    explicit InputException(std::string  msg) : _msg(std::move(msg)){}

    const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

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
                if (x->GetHashData() == (*name_it)->GetHashData() && ! (x->GetName() == name))
                    return x->GetName();

    return "";
}

bool check_valid_input(string & input, int & input_number){
    try {
        input_number = std::stoi(input);
    }catch (const invalid_argument & err) {
        cout << "Programme does not work with text" << endl;
        return false;
    }

    if (input_number < 0 || input_number > 6) {
        cout << "Invalid command!" << endl;
        return false;
    }

    return true;
}

void interaction_with_user( vector <shared_ptr<CBackup>> & all_backups)
{
    // initialization of input variables
    string input;
    int input_number = 5;

    // variable for helping to do good output;
    string text_out;

    // interaction with user
    cout << txt::greetings;
    try {
        while (true)
        {
            text_out = "";
            if ( !(getline(cin, input) && !cin.eof()) )
                throw InputException("There is end of input!");

            if (!check_valid_input(input,input_number))
                continue;

            switch (input_number)
            {
                case 0: {
                    string new_backup;
                    cout << "Please, write name of backup, how you want to name your backup: ";

                    if (!(getline(cin, new_backup) && !cin.eof()) ) {break;}

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

                    string name_of_backup = HashExistInBackup(all_backups,new_backup);
                    if ( !name_of_backup.empty() ){
                        text_out  = "\nMinimum one backup found with the same files and directories, it has name: "
                            + name_of_backup + "\n";
                    }
                    ShowStartMenu(text_out);
                    break;
                }

                case 1: {
                    string old_backup;
                    cout << "Please, write name of backup, which you want to return: ";
                    if (!(getline(cin, old_backup) && !cin.eof())){break;}
                    if (!NameExistsInBackups(all_backups,old_backup)){
                        text_out +=  "Sorry, you don't have backup with this name.\n";
                        ShowStartMenu(text_out);
                        break;
                    }

                    ReturnBackup(all_backups,old_backup);
                    text_out += "Backup is successfully restored in your curren folder!\n";
                    ShowStartMenu(text_out);
                    break;
                }
                    case 2: {
                        break;
                    }
                    case 3: {
                        break;
                    }
                    case 4: {
                        ShowStartMenu();
                        ShowBackups(all_backups);
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

    // init
    try{
        init_directory();
    }catch (...){
        cout << "Exception occurred" << endl;
    }
    // инициализировать set<CBackup> backups; сразу же, после запуска. Проверять на наличие такого файла

    vector<shared_ptr<CBackup>> all_backups;

    ReadPreviousBackups(all_backups);

    try {
        interaction_with_user(all_backups);
    }catch (const std::exception  & e)
    {
        cout << e.what();
        cout << "hi!" << endl;
    }


    return 0;
}


// TODO tasks
// 1) поменять тип сохраняемых данных с <set<shared_ptr<CBackup>> на что-то другое
// 2) так же сделать мапу для CDirectoryб где ключ будет ПУТЬ к файлу и к его данным, а данные будут CDataUnit
// 3) сделать возвращение на CBackup (удалять всё, что было сейчас в директории, и вставлять предыдущий CBackup)
// 4) сделать так, что бы функция CreateBackup запускалась по заданному времени пользователя


// TODO errors
