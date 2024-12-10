//
// Created by wxk on 2024/12/9.
//

#include "../include/RandomRange.h"

RandomRange::RandomRange(){}


RandomRange::RandomRange(int low, int high)
        : generator(std::random_device{}()),
          int_distribution(std::uniform_int_distribution<int>(low, high)) {}


RandomRange::RandomRange(double low, double high)
        : generator(std::random_device{}()),
          real_distribution(std::uniform_real_distribution<double>(low, high)) {}


int RandomRange::getRandomInt() {
    return int_distribution(generator);
}


double RandomRange::getRandomReal() {
    return real_distribution(generator);
}