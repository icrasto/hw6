#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#

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

        // keeping track of char count so can see when to move to next group of 6
        int charcount = 0;
        std::string currchunk;
        unsigned long long w[5] = {0, 0, 0, 0, 0};
        int windex = 4;
        // need to go through the string backwards
        for (int i = k.size()-1 ; i >=0; i-- ) {
          charcount++;
          currchunk += k[i];
          if (charcount == 6) {
            // complete base conversion and populate w[] array

            // go through curr chunk for base conversion - backwards bc first letter
            // of og string needs to be processed first which is the last now
            unsigned long long conversion = 0;
            for (int j = currchunk.size() -1; j >= 0; j--) {
              conversion = conversion * 36 + letterDigitToNumber(currchunk[j]);
            }
            w[windex] = conversion;
            // reset char count and go to next index of w[] to fill
            charcount = 0;
            windex--;
            currchunk = "";
          }
        }
        // check if there are chars left placed as 6 group chunk
        if (charcount > 0) {
          unsigned long long conversion = 0;
            for (int j = currchunk.size() -1; j >= 0; j--) {
              conversion = conversion * 36 + letterDigitToNumber(currchunk[j]);
            }
            w[windex] = conversion;
            windex--;
        }
        // indices of w that are left will remain 0 bc defaulted to that

        unsigned long long finalhash;
        finalhash = ((unsigned long long)rValues[0]*w[0] + (unsigned long long)rValues[1]*w[1] + 
        (unsigned long long)rValues[2]*w[2] + (unsigned long long)rValues[3]*w[3] + (unsigned long long)rValues[4]*w[4]);

        return finalhash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it

        // without using tolower() -> lowercase letters r exactly 32 more than
        // their uppercase letteres

        // convert to lowercase
        if (letter <= 'Z' && letter >= 'A') {
          letter += 32;
        }
        if (letter <= 'z' && letter >= 'a') {
          // a through z maps to 0 to 25
          // 'a' = 97 and 'z' = 122 so subtract by 97
          return (letter - 97);
        } else {
          // ascii value of 0 = 48, and 9 = 57 -> subtract by 48 and ADD 26
          return ((letter - 48) + 26);
        }
    }

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
