#pragma once
#include <string>
using std::string;

namespace text{
    const string greetings = "Guten Tag, user!\n"
                             "It is simple programme for creating incremental backups\n"
                             "0 - create backup\n"
                             "1 - get back to previous backup\n"
                             "2 - compare backups\n"
                             "3 - set auto backup at certain time\n"
                             "4 - show all backups\n"
                             "5 - close programme\n\n";
    const string reminder = "Reminder!\n"
                            "It is simple programme for creating incremental backups\n"
                            "0 - create backup\n"
                            "1 - get back to previous backup\n"
                            "2 - compare backups\n"
                            "3 - set auto backup at certain time\n"
                            "4 - show all backups\n"
                            "5 - close programme\n\n";
    const string created_backup = "Backup is successfully created!";
}

size_t number_of_repeating(const string & big_string, const string & small_string);


