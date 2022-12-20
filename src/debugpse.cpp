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


int main(int argc, const char * argv[])
{
    std::cout << "Debug PSE\n";
    
    spdlog_setVerbosity(5);
    spdlog_setPattern();

    // pse::Weber godfried = pse::Weber();
    // godfried.dump();
    // return 0;
    
    pse::Speller sp = pse::Speller();
       
    // content of LG 470 (loop)
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


// content of LG 128
//sp.add(67, 0);
//sp.add(67, 0);
//sp.add(67, 0);
//sp.add(65, 0);
//sp.add(65, 0);
//sp.add(63, 1);
//sp.add(63, 1);
//sp.add(63, 1);
//sp.add(62, 1);
//sp.add(60, 1);
//sp.add(69, 1);
//sp.add(69, 1);
//sp.add(72, 2);
//sp.add(72, 2);
//sp.add(72, 2);
//sp.add(72, 2);
//sp.add(62, 2);
//sp.add(72, 2);
//sp.add(72, 3);
//sp.add(72, 3);
//sp.add(72, 3);
//sp.add(72, 3);
//sp.add(72, 3);
//sp.add(62, 3);
//sp.add(70, 4);
//sp.add(69, 4);
//sp.add(68, 4);
//sp.add(67, 4);
//sp.add(70, 4);
//sp.add(76, 4);
//sp.add(76, 4);
//sp.add(76, 5);
//sp.add(74, 5);
//sp.add(70, 5);
//sp.add(69, 5);
//sp.add(68, 5);
//sp.add(72, 6);
//sp.add(69, 6);
//sp.add(65, 6);
//sp.add(66, 6);
//sp.add(73, 6);
//sp.add(72, 6);
//sp.add(71, 6);
//sp.add(72, 7);
//sp.add(77, 7);
//sp.add(72, 7);
//sp.add(72, 7);
//sp.add(65, 7);
//sp.add(68, 8);
//sp.add(68, 8);
//sp.add(67, 8);
//sp.add(60, 8);
//sp.add(60, 8);
//sp.add(60, 8);
//sp.add(60, 8);
//sp.add(60, 8);
//sp.add(61, 9);
//sp.add(70, 9);
//sp.add(70, 9);
//sp.add(75, 10);
//sp.add(67, 10);
//sp.add(66, 10);
//sp.add(64, 10);
//sp.add(64, 10);
//sp.add(73, 10);
//sp.add(74, 10);
//sp.add(75, 11);
//sp.add(74, 11);
//sp.add(74, 11);
//sp.add(67, 11);
//sp.add(72, 12);
//sp.add(72, 12);
//sp.add(72, 12);
//sp.add(71, 12);
//sp.add(72, 12);
//sp.add(74, 12);
//sp.add(74, 13);
//sp.add(67, 13);
//sp.add(69, 13);
//sp.add(71, 13);
//sp.add(72, 13);
//sp.add(74, 13);
//sp.add(75, 13);
//sp.add(74, 14);
//sp.add(72, 14);
//sp.add(72, 14);
//sp.add(80, 14);
//sp.add(75, 14);
//sp.add(72, 14);
//sp.add(68, 14);
//sp.add(66, 15);
//sp.add(75, 15);
//sp.add(74, 15);
//sp.add(72, 15);
