#pragma once
#include <string>
using std::string;

namespace text{
    const string greetings = "Hello, user!\n"
                             "It is simple programme for creating incremental backups\n"
                             "0 - create backup\n"
                             "1 - get back to previous backup\n"
                             "2 - compare backups\n"
                             "3 - show all backups\n"
                             "4 - close programme\n\n";
    const string reminder = "Reminder!\n"
                            "It is simple programme for creating incremental backups\n"
                            "0 - create backup\n"
                            "1 - get back to previous backup\n"
                            "2 - compare backups\n"
                            "3 - show all backups\n"
                            "4 - close programme\n\n";
    const string created_backup = "Backup is successfully created!";
}

size_t number_of_repeating(const string & big_string, const string & small_string);


