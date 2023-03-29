#pragma once

#include <stdexcept>
#include <string>

inline void Error(const std::string& message) {
    throw std::logic_error(message);
}