//
//  debugpse.cpp
//  pypse
//
//  Created by Florent Jacquemard on 25/11/2022.
//

#include <iostream>

#include "PSE.hpp"
#include "PS13.hpp"
#include "PS14.hpp"
#include "Weber.hpp"
#include "samples.hpp"


/// dump the matric of Weber distance between tonalities
void WeberTable()
{
    pse::Weber godfried = pse::Weber();
    godfried.dump();
}


int main(int argc, const char* argv[])
{
    std::cout << "Debug PS" << std::endl;
    
    spdlog_setVerbosity(5);
    spdlog_setPattern();

    // pse::Weber godfried = pse::Weber();
    // godfried.dump();
    // return 0;

    pse::PSE sp(135, true);
    //pse::PS13 sp;

    //issue7(sp);
    //BWV_858prelude(sp);
    //BWV_857fugue(sp);
    //BWV_864fugue(sp);
    //LG101(sp);
    //LG461(sp);
    // Waldstein(sp);
    Airegin(sp);

    std::cout << "spelling " << sp.size() << " notes" << std::endl;
    bool status = sp.spell();

    if (!status)
    {
        ERROR("spell failure");
        return 1;
    }

    std::cout << "renaming " << sp.size() << " notes" << std::endl;
    status = status && sp.rename();
    
    if (status)
    {
        std::cout << sp.size() << " spelled notes" << std::endl;
        for (size_t i = 0; i < sp.size(); ++i)
        {
            std::cout << i << ' ';
            std::cout << sp.name(i);
            std::cout << sp.octave(i);
            std::cout << sp.accidental(i);
            std::cout << std::endl;
        }
    }
    else 
    {
        ERROR("renaming failure");
        return 2;
    }

    std::cout << "rewriting" << sp.size() << " notes" << std::endl;
    size_t rew = sp.rewritePassing();
    std::cout << "(" << rew << " notes rewritten)" << std::endl;
    
    return 0;
}
