#include "texts.hpp"
#include <iostream>

size_t number_of_repeating(const string & big_string, const string & small_string){
    int count = 0;
    for (int i = big_string.size() - 1; i >= 0; --i) {
        if (big_string.substr(i, small_string.size()) == small_string) {
            count++;
            i-=30;
        }
    }
    return count;
}