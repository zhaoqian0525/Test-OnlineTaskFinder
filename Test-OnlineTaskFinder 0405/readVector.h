//
// Created by 10157 on 2022/11/26.
//

#ifndef LEARN8_READVECTOR_H
#define LEARN8_READVECTOR_H

#endif //LEARN8_READVECTOR_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;


template<class T>
std::string printVectorToString(const std::vector<T> &x, const std::string &separator = " -> ") {
    std::stringstream s;
    for (int i = 0; i < x.size(); i++) {
        if (i > 0) {
            s << separator;
        }
        s << x[i];
    }
    return s.str();
}

template<class T>
std::string printVectorToStringWithNoArrows(const std::vector<T> &x, const std::string &separator = " , ") {
    std::stringstream s;
    for (int i = 0; i < x.size(); i++) {
        if (i > 0) {
            s << separator;
        }
        s << x[i];
    }
    return s.str();
}


template<class T>
void printVector(const std::vector<T> &x, const std::function<std::string(T const &)> &stringConversion,
                 const std::string &separator = ", ") {
    for (int i = 0; i < x.size(); i++) {
        if (i > 0) {
            std::cout << separator;
        }
        std::cout << stringConversion(x[i]);
    }
    std::cout << std::endl;
}
