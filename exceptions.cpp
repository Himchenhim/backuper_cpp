#include "exceptions.h"


InputException::InputException(std::string msg) : _msg(std::move(msg)){}

const char *InputException::what() const noexcept {
    return _msg.c_str();
}


FileException::FileException(std::string msg) : _msg(std::move(msg)) {}

const char *FileException::what() const noexcept {
    return _msg.c_str();
}
