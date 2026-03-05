#include <gtest/gtest.h>

#include "AL/al.h"
#include "api/OpenAL/Helper.h"

TEST(AudioApiOpenALHelper, alGetErrorStringTest) {
    ASSERT_EQ("AL_INVALID_NAME: Invalid name parameter",
              OpenAL::alGetErrorString(AL_INVALID_NAME));
}
