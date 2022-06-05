#include <string>

class InputException : public std::exception
{
    std::string _msg;
public:
    explicit InputException(std::string  msg);
    const char* what() const noexcept override;
};

class FileException : public std::exception
{
    std::string _msg;
public:
    explicit FileException(std::string  msg);
    const char* what() const noexcept override;
};
