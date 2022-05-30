#pragma once
#include "CDataUnit.hpp"


class CBackup{
private:
    // тут будет храниться дерево, с полной иерархией папок, файлов и ссылок
    string name_of_backup;
    string hash_of_backup;

    // проверять на то, что IsDirectory
    shared_ptr<CDataUnit> root_directory;
public:
    CBackup(string name);
    // если бэкапа с заданным именем не существует, то возвращаем false, или кидаем ошибку
    // меняет все файлы в директории на те, которые были подняты с бэкапа
    bool ReadBackup(const string & name_of_backup_to_read);

    bool SaveBackup(const string & name_of_backup_to_save);
    ostream & ShowFileHierarchy(ostream & os);
};

