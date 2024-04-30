#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>


typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here

        // 1. Translate k to int[]
        int len = k.length();
        int arr[28] = {0};
        for (int i = 0; i < len; i++) {
          //std::cout << "k[" << i << "]: " << k[i] << std::endl;
          if (k[len - 1 - i] >= 'a' && k[len - 1 - i] <= 'z') {
            arr[27 - i] = k[len - 1 - i] - 97;
          }
          else if (k[len - 1 - i] >= 'A' && k[len - 1 - i] <= 'Z') {
            arr[27 - i] = k[len - 1 - i] - 65;
          }
          else if (k[len - 1 - i] >= '0' && k[len - 1 - i] <= '9') {
            arr[27 - i] = k[len - 1 - i] - 48 + 26;
          }
          //std::cout << "arr[" << i << "]: " << arr[i] << std::endl;
        }
        // we should now have an arr[] of base-36 values of length len
        // leading values should all be 0s

        // 2. Convert this int[len] to w[5]
        unsigned long long w[5]= {0};
        for (int i = 4; i >= 0; i--) {
            if (i > 0) {
                w[i] = pow(36, 5) * arr[i*6 - 2] + pow(36, 4) * arr[i*6-1] + pow(36, 3) * arr[i*6] + pow(36, 2) * arr[i*6+1] + pow(36, 1) * arr[i*6+2] + pow(36, 0) * arr[i*6+3];
            }
            else {
                w[i] = pow(36, 3) * arr[i*6] + pow(36, 2) * arr[i*6+1] + pow(36, 1) * arr[i*6+2] + pow(36, 0) * arr[i*6+3];
            }
            //std::cout << "w[" << i << "]: " << w[i] << std::endl;
        }

        // 3. Hash with h(k) function
        HASH_INDEX_T h = (rValues[0]*w[0] + rValues[1]*w[1] + rValues[2]*w[2] + rValues[3]*w[3] + rValues[4]*w[4]);

        return h;        

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    /*HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it

    }
    */
    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
