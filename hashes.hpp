#pragma once
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include <sstream>
#include <iomanip>

std::optional<std::string> CalcSha256ForFile(const std::string & filename);
std::optional<std::string> CalcSha256ForString(const std::string & str);