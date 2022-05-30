#include "CDataUnit.hpp"
#include "CBackup.hpp"
#include "CDirectory.hpp"
#include "CFile.hpp"
#include "CLink.hpp"
#include "CBackup.hpp"
#include "texts.hpp"
#include "filesystem_work.hpp"
#include <filesystem>

using std::cout;
using std::endl;
using std::cin;
using std::isdigit;
using std::stoi;
using std::invalid_argument;

namespace fs = std::filesystem;
namespace txt =  text;


// work with files
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
bool check_valid_input(string & input, int & input_number)
{
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
void interaction_with_user( set <shared_ptr<CBackup>> & all_backups)
{
    string input;
    int input_number = 5;
    cout << txt::greetings;
    while (true)
    {
        cin >> input;

        // TODO: проверять на ctrl+d
        if (!check_valid_input(input,input_number))
            continue;

        switch (input_number) {
            case 0:
                string name;
                cin >> name;
                try {
                    CreateBackup(all_backups, name );
                    cout << "Backup is successfully created!" << endl;
                }
                catch ( const std::exception & e){
                    cout << e.what() << endl;
                }
                break;
//            case 1:
//                // TODO как пользователь хочет к этому возвращаться
////                GetBackToBackup();
//                break;
//            case 2:
//                break;
//            case 3:
//                break;
//            case 4:
//                break;
//            case 5:
//                return;
//            default:
//                cout << "Smth unexpected happened" << endl;
//                break;
        }
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
    set <shared_ptr<CBackup>> all_backups;

    interaction_with_user(all_backups);



    return 0;
}
