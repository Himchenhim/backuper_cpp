#pragma once
#include "CDataUnit.hpp"

// BLOB
class CFile : public CDataUnit
{
public:

    // в каждом конструкторе так же нужно вызывать функцию, для подсчёта хеша
    // при конструировании сразу же сохраняем наш объект в папку objects
    CFile (string name_of_file, const vector<uint8_t> & file_data);
    CFile (string name_of_file, vector<uint8_t> && file_data);
    CFile (const CFile & src);
    CFile (CFile && src);

    ~CFile();
};
