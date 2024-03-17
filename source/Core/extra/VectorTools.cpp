#pragma once

#include <iostream>
#include <string>
#include <vector>

template <typename T>
void removeVectorVar(std::vector<T>& vec, std::size_t pos)
{
    std::vector<T>::iterator it = vec.begin();
    std::advance(it, pos - 1);
    vec.erase(it);
};