#include "CFile.hpp"
#include "hashes.hpp"


// writing to file
CFile::CFile(string path_of_file,string name_of_file) {
    path_of_data_unit = move(path_of_file);
    name_of_data_unit = move(name_of_file);


    // write to file
    ifstream ifs(path_of_data_unit, std::ios::in | std::ios::binary);

    vector <char> data_from_file;
    char x;

    while ( ifs.get(x) )
    {
        data_from_file.push_back(x);
    }

    // create hash
    hash_of_data_unit = (CalcSha256ForFile(path_of_data_unit)).value();
    string directory = hash_of_data_unit.substr(0,2);
    string file = hash_of_data_unit.substr(2);


    // create directory
    string name_of_new_directory = ( (fs::current_path() /= ".backups/obj" )/= directory);
    if ( !fs::exists(name_of_new_directory))
        fs::create_directory(name_of_new_directory);

    // create file
    string filename (name_of_new_directory + '/' + file);
    std::fstream output_fstream;
    output_fstream.open(filename,std::ios_base::out);

    // write data to file
    for (char i : data_from_file)
        output_fstream << i;

}


bool CFile::IsFile() const { return true;}
bool CFile::IsDirectory() const {return false;}
bool CFile::IsLink() const {return false;}

// reading from file
CFile::CFile(string path, string name,string hash) {
    this->path_of_data_unit = move(path);
    this->name_of_data_unit = move(name);
    this->hash_of_data_unit = move(hash);
}

void CFile::Restore()const {
    // from where programme copy file
    string dir = hash_of_data_unit.substr(0,2);
    string file = hash_of_data_unit.substr(2);

    char x;

    std::fstream input_file;
    input_file.open(fs::current_path().string()+"/.backups/obj/" + dir + "/" + file,
                    std::ios::binary | std::ios::in);

    vector <char> data_from_file;

    while(input_file.get(x))
        data_from_file.push_back(x);

    std::fstream output_file;
    output_file.open(path_of_data_unit,std::ios::binary | std::ios::out);
    for (char i : data_from_file)
        output_file << i;


    input_file.close();
    output_file.close();
}

void CFile::Print(size_t level) const {
    auto lead = string(level*3,'-');
    std::cout << lead << name_of_data_unit << std::endl;
}
