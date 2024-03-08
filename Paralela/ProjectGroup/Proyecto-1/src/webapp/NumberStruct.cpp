// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "NumberStruct.hpp"
// Override of the opertaror == for the num_t struct
bool operator==(num_t num1, num_t num2) {
    return num1.fact == num2.fact && num1.key == num2.key && num1.num
    == num2.num;
}
