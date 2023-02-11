//
//  debugpse.cpp
//  pypse
//
//  Created by Florent Jacquemard on 25/11/2022.
//

#include <iostream>

#include "Speller.hpp"
#include "Weber.hpp"


/// dump the matric of Weber distance between tonalities
void WeberTable()
{
    pse::Weber godfried = pse::Weber();
    godfried.dump();
}

void Waldstein(pse::Speller& sp);
void LG470(pse::Speller& sp);
void LG128(pse::Speller& sp);


int main(int argc, const char* argv[])
{
    std::cout << "Debug PSE\n";
    
    spdlog_setVerbosity(5);
    spdlog_setPattern();

    // pse::Weber godfried = pse::Weber();
    // godfried.dump();
    // return 0;
    
    pse::Speller sp = pse::Speller();
       
    //LG128(sp);
    // LG470(sp);
    Waldstein(sp);
    
    std::cout << sp.size() << " notes" << std::endl;
    bool status = sp.respell();

    std::cout << sp.size() << " spelled notes" << std::endl;
    for (size_t i = 0; i < sp.size(); ++i)
    {
        std::cout << i << ' ';
        std::cout << sp.name(i);
        std::cout << sp.octave(i);
        std::cout << sp.accidental(i);
        std::cout << std::endl;
    }

    return 0;
}


// content of 2 first bars, left hand of Beethoven Sonata 21, mvt 1
// to debug pruning
void Waldstein(pse::Speller& sp)
{
// measure 1 (39)
    sp.add(36, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
    sp.add(48, 0, true);
    sp.add(52, 0, true);
    sp.add(36, 0, true);
    sp.add(43, 0, false);
// measure 2 (42)
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(48, 1, true);
    sp.add(52, 1, true);
    sp.add(36, 1, true);
    sp.add(43, 1, false);
    sp.add(50, 1, true);
    sp.add(54, 1, true);
    sp.add(36, 1, true);
    sp.add(45, 1, false);
    sp.add(50, 1, true);
    sp.add(54, 1, true);
    sp.add(36, 1, true);
    sp.add(45, 1, false);
// measure 3
    sp.add(50, 2, true);
    sp.add(55, 2, true);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(59, 2, true);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(57, 2, false);
    sp.add(55, 2, true);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 2, true);
    sp.add(43, 2, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(35, 3, true);
    sp.add(43, 3, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 4, true);
    sp.add(50, 4, true);
    sp.add(34, 4, true);
    sp.add(41, 4, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(46, 5, true);
    sp.add(50, 5, true);
    sp.add(34, 5, true);
    sp.add(41, 5, false);
    sp.add(48, 5, true);
    sp.add(52, 5, true);
    sp.add(34, 5, true);
    sp.add(43, 5, false);
    sp.add(48, 5, true);
    sp.add(52, 5, true);
    sp.add(34, 5, true);
    sp.add(43, 5, false);
    sp.add(48, 6, true);
    sp.add(53, 6, true);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(57, 6, true);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(55, 6, false);
    sp.add(53, 6, true);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(33, 6, true);
    sp.add(41, 6, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(32, 7, true);
    sp.add(41, 7, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 8, true);
    sp.add(41, 8, false);
    sp.add(32, 8, true);
    sp.add(41, 8, false);
    sp.add(32, 8, true);
    sp.add(41, 8, false);
    sp.add(31, 9, true);
    sp.add(41, 9, false);
    sp.add(31, 9, true);
    sp.add(41, 9, false);
    sp.add(32, 9, true);
    sp.add(41, 9, false);
    sp.add(32, 9, true);
    sp.add(41, 9, false);
    sp.add(31, 9, true);
    sp.add(41, 9, false);
    sp.add(31, 9, true);
    sp.add(41, 9, false);
    sp.add(32, 9, true);
    sp.add(41, 9, false);
    sp.add(32, 9, true);
    sp.add(41, 9, false);
    sp.add(31, 10, true);
    sp.add(41, 10, false);
    sp.add(43, 10, true);
    sp.add(55, 10, false);
    sp.add(43, 10, true);
    sp.add(55, 10, false);
    sp.add(57, 10, false);
    sp.add(59, 10, false);
    sp.add(60, 11, false);
    sp.add(43, 11, true);
    sp.add(55, 11, false);
    sp.add(39, 11, true);
    sp.add(51, 11, false);
    sp.add(36, 11, true);
    sp.add(48, 11, false);
    sp.add(31, 12, true);
    sp.add(43, 12, false);
    sp.add(36, 13, true);
    sp.add(48, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 13, false);
    sp.add(55, 13, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(55, 14, false);
    sp.add(48, 14, false);
    sp.add(57, 14, false);
    sp.add(48, 14, false);
    sp.add(57, 14, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 15, true);
    sp.add(50, 15, false);
    sp.add(55, 15, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(47, 16, true);
    sp.add(50, 16, false);
    sp.add(55, 16, false);
    sp.add(38, 17, true);
    sp.add(50, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 17, false);
    sp.add(57, 17, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(57, 18, false);
    sp.add(50, 18, false);
    sp.add(59, 18, false);
    sp.add(50, 18, false);
    sp.add(59, 18, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 19, true);
    sp.add(52, 19, false);
    sp.add(57, 19, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 20, true);
    sp.add(52, 20, false);
    sp.add(57, 20, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(48, 21, true);
    sp.add(52, 21, false);
    sp.add(58, 21, false);
    sp.add(47, 22, false);
    sp.add(51, 22, false);
    sp.add(54, 22, false);
    sp.add(59, 22, false);
    sp.add(47, 22, false);
    sp.add(51, 22, false);
    sp.add(54, 22, false);
    sp.add(59, 22, false);
    sp.add(47, 22, false);
    sp.add(52, 22, false);
    sp.add(55, 22, false);
    sp.add(59, 22, false);
    sp.add(47, 22, false);
    sp.add(52, 22, false);
    sp.add(55, 22, false);
    sp.add(59, 22, false);
    sp.add(47, 23, false);
    sp.add(54, 23, false);
    sp.add(57, 23, false);
    sp.add(59, 23, false);
    sp.add(47, 23, false);
    sp.add(54, 23, false);
    sp.add(57, 23, false);
    sp.add(59, 23, false);
    sp.add(47, 23, false);
    sp.add(52, 23, false);
    sp.add(55, 23, false);
    sp.add(59, 23, false);
    sp.add(47, 23, false);
    sp.add(52, 23, false);
    sp.add(55, 23, false);
    sp.add(59, 23, false);
    sp.add(47, 24, false);
    sp.add(51, 24, false);
    sp.add(54, 24, false);
    sp.add(59, 24, false);
    sp.add(47, 24, false);
    sp.add(51, 24, false);
    sp.add(54, 24, false);
    sp.add(59, 24, false);
    sp.add(47, 24, false);
    sp.add(52, 24, false);
    sp.add(55, 24, false);
    sp.add(59, 24, false);
    sp.add(47, 24, false);
    sp.add(52, 24, false);
    sp.add(55, 24, false);
    sp.add(59, 24, false);
    sp.add(47, 25, false);
    sp.add(54, 25, false);
    sp.add(57, 25, false);
    sp.add(59, 25, false);
    sp.add(47, 25, false);
    sp.add(54, 25, false);
    sp.add(57, 25, false);
    sp.add(59, 25, false);
    sp.add(47, 25, false);
    sp.add(52, 25, false);
    sp.add(55, 25, false);
    sp.add(59, 25, false);
    sp.add(47, 25, false);
    sp.add(52, 25, false);
    sp.add(55, 25, false);
    sp.add(59, 25, false);
    sp.add(47, 26, false);
    sp.add(51, 26, false);
    sp.add(54, 26, false);
    sp.add(59, 26, false);
    sp.add(58, 26, false);
    sp.add(59, 26, false);
    sp.add(55, 26, false);
    sp.add(52, 26, false);
    sp.add(47, 26, false);
    sp.add(51, 26, false);
    sp.add(54, 26, false);
    sp.add(59, 26, false);
    sp.add(58, 26, false);
    sp.add(59, 26, false);
    sp.add(55, 26, false);
    sp.add(52, 26, false);
    sp.add(47, 27, false);
    sp.add(51, 27, false);
    sp.add(54, 27, false);
    sp.add(59, 27, false);
    sp.add(47, 27, false);
    sp.add(52, 27, false);
    sp.add(55, 27, false);
    sp.add(59, 27, false);
    sp.add(47, 27, false);
    sp.add(51, 27, false);
    sp.add(54, 27, false);
    sp.add(59, 27, false);
    sp.add(47, 27, false);
    sp.add(52, 27, false);
    sp.add(55, 27, false);
    sp.add(59, 27, false);
    sp.add(47, 28, true);
    sp.add(51, 28, true);
    sp.add(54, 28, true);
    sp.add(59, 28, false);
    sp.add(54, 29, false);
    sp.add(54, 29, false);
    sp.add(51, 29, false);
    sp.add(59, 29, false);
    sp.add(54, 29, false);
    sp.add(51, 29, false);
    sp.add(47, 29, false);
    sp.add(35, 29, false);
    sp.add(39, 29, false);
    sp.add(42, 29, false);
    sp.add(47, 29, false);
    sp.add(35, 30, false);
    sp.add(47, 30, false);
    sp.add(37, 30, false);
    sp.add(49, 30, false);
    sp.add(39, 30, false);
    sp.add(51, 30, false);
    sp.add(39, 31, false);
    sp.add(51, 31, false);
    sp.add(40, 31, false);
    sp.add(52, 31, false);
    sp.add(42, 31, false);
    sp.add(54, 31, false);
    sp.add(42, 32, false);
    sp.add(54, 32, false);
    sp.add(44, 32, false);
    sp.add(56, 32, false);
    sp.add(45, 33, false);
    sp.add(57, 33, false);
    sp.add(47, 33, false);
    sp.add(59, 33, false);
    sp.add(49, 33, false);
    sp.add(61, 33, false);
    sp.add(51, 33, false);
    sp.add(63, 33, false);
    sp.add(64, 34, true);
    sp.add(68, 34, false);
    sp.add(59, 34, true);
    sp.add(63, 34, true);
    sp.add(66, 34, false);
    sp.add(61, 34, true);
    sp.add(64, 34, false);
    sp.add(56, 35, true);
    sp.add(60, 35, true);
    sp.add(63, 35, false);
    sp.add(57, 35, true);
    sp.add(61, 35, false);
    sp.add(54, 36, true);
    sp.add(57, 36, true);
    sp.add(59, 36, true);
    sp.add(63, 36, false);
    sp.add(56, 36, true);
    sp.add(59, 36, true);
    sp.add(64, 36, false);
    sp.add(57, 36, true);
    sp.add(61, 36, true);
    sp.add(66, 36, false);
    sp.add(64, 37, true);
    sp.add(68, 37, true);
    sp.add(59, 37, false);
    sp.add(63, 37, true);
    sp.add(66, 37, false);
    sp.add(52, 38, true);
    sp.add(56, 38, false);
    sp.add(47, 38, true);
    sp.add(51, 38, true);
    sp.add(54, 38, false);
    sp.add(49, 38, true);
    sp.add(52, 38, false);
    sp.add(44, 39, true);
    sp.add(48, 39, true);
    sp.add(51, 39, false);
    sp.add(45, 39, true);
    sp.add(49, 39, false);
    
}


// content of LG 470 (to debug a loop)
void LG470(pse::Speller& sp)
{
    sp.add(60, 0);
    sp.add(60, 0);
    sp.add(60, 0);
    sp.add(62, 0);
    sp.add(63, 0);
    sp.add(63, 0);
    sp.add(62, 0);
    sp.add(65, 1);
    sp.add(65, 1);
    sp.add(65, 1);
    sp.add(67, 1);
    sp.add(68, 1);
    sp.add(68, 1);
    sp.add(67, 1);
    sp.add(72, 2);
    sp.add(72, 2);
    sp.add(72, 2);
    sp.add(74, 2);
    sp.add(75, 2);
    sp.add(75, 2);
    sp.add(74, 2);
    sp.add(72, 2);
    sp.add(74, 2);
    sp.add(75, 2);
    sp.add(75, 3);
    sp.add(74, 3);
    sp.add(76, 3);
    sp.add(77, 3);
    sp.add(80, 3);
    sp.add(80, 3);
    sp.add(79, 3);
    sp.add(80, 3);
    sp.add(82, 3);
    sp.add(80, 3);
    sp.add(79, 3);
    sp.add(77, 3);
    sp.add(75, 3);
    sp.add(74, 3);
    sp.add(72, 3);
    sp.add(70, 3);
    sp.add(68, 3);
    sp.add(67, 3);
    sp.add(65, 3);
    sp.add(63, 3);
    sp.add(62, 3);
    sp.add(65, 3);
    sp.add(68, 3);
    sp.add(65, 3);
    sp.add(62, 3);
    sp.add(63, 4);
    sp.add(75, 4);
    sp.add(75, 4);
    sp.add(75, 4);
    sp.add(77, 4);
    sp.add(79, 4);
    sp.add(79, 4);
    sp.add(77, 4);
    sp.add(65, 4);
    sp.add(65, 4);
    sp.add(65, 4);
    sp.add(65, 4);
    sp.add(65, 4);
    sp.add(65, 5);
    sp.add(77, 5);
    sp.add(77, 5);
    sp.add(77, 5);
    sp.add(79, 5);
    sp.add(80, 5);
    sp.add(80, 5);
    sp.add(79, 5);
    sp.add(67, 5);
    sp.add(69, 5);
    sp.add(69, 5);
    sp.add(70, 5);
    sp.add(70, 5);
    sp.add(69, 6);
    sp.add(81, 6);
    sp.add(81, 6);
    sp.add(81, 6);
    sp.add(83, 6);
    sp.add(84, 6);
    sp.add(84, 6);
    sp.add(83, 6);
    sp.add(83, 6);
    sp.add(84, 6);
    sp.add(85, 7);
    sp.add(86, 7);
    sp.add(86, 7);
    sp.add(87, 7);
    sp.add(88, 7);
    sp.add(89, 7);
    sp.add(89, 7);
    sp.add(89, 8);
    sp.add(89, 8);
    sp.add(87, 8);
    sp.add(86, 8);
    sp.add(89, 8);
    sp.add(87, 8);
    sp.add(86, 8);
    sp.add(84, 8);
    sp.add(83, 8);
    sp.add(83, 8);
    sp.add(83, 8);
    sp.add(83, 8);
    sp.add(84, 8);
    // cas extreme de mesure :
    // - 30 notes
    // - beaucoup de chromatisme (presque une alteration par note)
    // (extraction min cost de file d'attente)
    // -> exploration quasi en largeur d'abord sur cet exemple
    // -> explosion exponentielle
    // [ ] essayer de laisser tourner?
    // [ ] option sans les ## et bb ?
    sp.add(84, 9);
    sp.add(84, 9);
    sp.add(83, 9);
    sp.add(84, 9);
    sp.add(86, 9);
    sp.add(84, 9);
    sp.add(82, 9);
    sp.add(80, 9);
    sp.add(79, 9);
    sp.add(82, 9);
    sp.add(80, 9);
    sp.add(80, 9);
    sp.add(80, 9);
    sp.add(80, 9);
    sp.add(79, 9);
    sp.add(87, 9);
    sp.add(87, 9);
    sp.add(86, 9);
    sp.add(85, 9);
    sp.add(84, 9);
    sp.add(83, 9);
    sp.add(82, 9);
    sp.add(81, 9);
    sp.add(80, 9);
    sp.add(79, 9);
    sp.add(78, 9);
    sp.add(77, 9);
    sp.add(76, 9);
    sp.add(75, 9);
    sp.add(74, 10);
    sp.add(73, 10);
    sp.add(72, 10);
    sp.add(71, 10);
    sp.add(70, 10);
    sp.add(69, 10);
    sp.add(68, 10);
    sp.add(67, 10);
    sp.add(66, 10);
    sp.add(65, 10);
    sp.add(64, 10);
    sp.add(63, 10);
    sp.add(62, 10);
    sp.add(61, 10);
    sp.add(60, 10);
    sp.add(59, 10);
    sp.add(68, 10);
    sp.add(59, 10);
    sp.add(60, 11);
}


// content of LG 128
void LG128(pse::Speller& sp)
{
    sp.add(67, 0);
    sp.add(67, 0);
    sp.add(67, 0);
    sp.add(65, 0);
    sp.add(65, 0);
    sp.add(63, 1);
    sp.add(63, 1);
    sp.add(63, 1);
    sp.add(62, 1);
    sp.add(60, 1);
    sp.add(69, 1);
    sp.add(69, 1);
    sp.add(72, 2);
    sp.add(72, 2);
    sp.add(72, 2);
    sp.add(72, 2);
    sp.add(62, 2);
    sp.add(72, 2);
    sp.add(72, 3);
    sp.add(72, 3);
    sp.add(72, 3);
    sp.add(72, 3);
    sp.add(72, 3);
    sp.add(62, 3);
    sp.add(70, 4);
    sp.add(69, 4);
    sp.add(68, 4);
    sp.add(67, 4);
    sp.add(70, 4);
    sp.add(76, 4);
    sp.add(76, 4);
    sp.add(76, 5);
    sp.add(74, 5);
    sp.add(70, 5);
    sp.add(69, 5);
    sp.add(68, 5);
    sp.add(72, 6);
    sp.add(69, 6);
    sp.add(65, 6);
    sp.add(66, 6);
    sp.add(73, 6);
    sp.add(72, 6);
    sp.add(71, 6);
    sp.add(72, 7);
    sp.add(77, 7);
    sp.add(72, 7);
    sp.add(72, 7);
    sp.add(65, 7);
    sp.add(68, 8);
    sp.add(68, 8);
    sp.add(67, 8);
    sp.add(60, 8);
    sp.add(60, 8);
    sp.add(60, 8);
    sp.add(60, 8);
    sp.add(60, 8);
    sp.add(61, 9);
    sp.add(70, 9);
    sp.add(70, 9);
    sp.add(75, 10);
    sp.add(67, 10);
    sp.add(66, 10);
    sp.add(64, 10);
    sp.add(64, 10);
    sp.add(73, 10);
    sp.add(74, 10);
    sp.add(75, 11);
    sp.add(74, 11);
    sp.add(74, 11);
    sp.add(67, 11);
    sp.add(72, 12);
    sp.add(72, 12);
    sp.add(72, 12);
    sp.add(71, 12);
    sp.add(72, 12);
    sp.add(74, 12);
    sp.add(74, 13);
    sp.add(67, 13);
    sp.add(69, 13);
    sp.add(71, 13);
    sp.add(72, 13);
    sp.add(74, 13);
    sp.add(75, 13);
    sp.add(74, 14);
    sp.add(72, 14);
    sp.add(72, 14);
    sp.add(80, 14);
    sp.add(75, 14);
    sp.add(72, 14);
    sp.add(68, 14);
    sp.add(66, 15);
    sp.add(75, 15);
    sp.add(74, 15);
    sp.add(72, 15);
}
