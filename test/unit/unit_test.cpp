//
//  main.cpp
//  GTests
//
//  Created by Florent Jacquemard on 11/02/2019.
//  Copyright © 2019 Florent Jacquemard. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"

#include "config.h" // check compile flags
//#include "pstrace.hpp"


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
