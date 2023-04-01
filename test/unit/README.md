
googletest README:
https://github.com/google/googletest/blob/master/googletest/README.md

googletest primer:
https://github.com/google/googletest/blob/master/docs/primer.md


* assertions:  
  - `ASSERT_TRUE` / `EXPECT_TRUE`
  - `ASSERT_EQ` / `EXPECT_EQ`
  - `ASSERT_NE`

* definition TEST macros:
```
TEST(TestSuiteName, TestName) { ... test body … }
```

* Test Fixtures: 
    * class definition for reuse
    with a `SetUp()`
    * destructor with override for cleanup if needed
    * definition TEST_F

* invocation: 
```
#include "gtest/gtest.h"
```
and `RUN_ALL_TESTS()` in main

environment var :
```
export DYLD_FRAMEWORK_PATH=$DYLD_FRAMEWORK_PATH:$HOME/Code/googletest//
```


Xcode:
Xcode test targets for Swift and ObjectiveC only

see also
(better integration?)
https://github.com/mattstevens/xcode-googletest

Google testing blog: https://testing.googleblog.com

