#include "CFile.hpp"
#include "hashes.hpp"


CFile::CFile(string name_of_file) {
    name_of_data_unit = move(name_of_file);

    // записать в файл
    ifstream ifs(name_of_data_unit, std::ios::in |std::ios::binary);

    vector <char> data_from_file;
    char x;

    while ( ifs.get(x) )
    {
        data_from_file.push_back(x);
    }
    // берём хеш
    hash_of_data_unit = (CalcSha256ForFile(name_of_data_unit)).value();
    std::cout << hash_of_data_unit << std::endl;
    string first_directory = hash_of_data_unit.substr(0,2);
    string second_file = hash_of_data_unit.substr(2);


    // создаём директорию
    string name_of_new_directory = ( (fs::current_path() /= ".backups/obj" )/= first_directory);
    if ( !fs::exists(name_of_new_directory))
        fs::create_directory(name_of_new_directory);

    // создаём файл, куда будем записывать данные
    string filename (name_of_new_directory + '/' + second_file);
    std::fstream output_fstream;
    output_fstream.open(filename,std::ios_base::out);

    // записываем данные в файл
    for (char i : data_from_file)
        output_fstream << i;

}


bool CFile::IsFile() const { return true;}
bool CFile::IsDirectory() const {return false;}
bool CFile::IsLink() const {return false;}

CFile::CFile(string name_of_file, string hash) {
    this->name_of_data_unit = move(name_of_file);
    this->hash_of_data_unit = move(hash);
}
