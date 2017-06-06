#include <gmock/gmock.h>
#include "sqlite_modern_cpp.h"

#include <arrayfire.h>
#include <cstdio>
#include <vector>
#include <algorithm>

// bash: for i in /opt/arrayfire-3/lib/*; do base="`basename $i`"; echo $base; sudo ln -s /opt/arrayfire-3/lib/"$base" /usr/local/lib/"$base"; done
// https://github.com/arrayfire/arrayfire/wiki/Build-Instructions-for-Linux

using namespace af;
std::vector<float> input(100);
// Generate a random number between 0 and 1
// return a uniform number in [0,1].
double unifRand()
{
    return rand() / double(RAND_MAX);
}
void testBackend()
{
    af::info();
    af::dim4 dims(10, 10, 1, 1);
    af::array A(dims, &input.front());
    af_print(A);
    af::array B = af::constant(0.5, dims, f32);
    af_print(B);
}


class DataTest : public ::testing::Test
{

public:
    DataTest()
    {
    }
protected:
    // Per-test-case set-up.
    // Called before the first test in this test case.
    // Can be omitted if not needed.
    static void SetUpTestCase()
    {
        //shared_resource_ = new ...;
    }

    // Per-test-case tear-down.
    // Called after the last test in this test case.
    // Can be omitted if not needed.
    static void TearDownTestCase()
    {
        //delete shared_resource_;
        //shared_resource_ = NULL;
    }

    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp() {  }
    virtual void TearDown() {  }
};

TEST_F(DataTest, Test1)
{
    std::generate(input.begin(), input.end(), unifRand);
    try
    {
        printf("Trying CPU Backend\n");
        af::setBackend(AF_BACKEND_CPU);
        testBackend();
    }
    catch (af::exception &e)
    {
        printf("Caught exception when trying CPU backend\n");
        fprintf(stderr, "%s\n", e.what());
    }
    try
    {
        printf("Trying CUDA Backend\n");
        af::setBackend(AF_BACKEND_CUDA);
        testBackend();
    }
    catch (af::exception &e)
    {
        printf("Caught exception when trying CUDA backend\n");
        fprintf(stderr, "%s\n", e.what());
    }
    try
    {
        printf("Trying OpenCL Backend\n");
        af::setBackend(AF_BACKEND_OPENCL);
        testBackend();
    }
    catch (af::exception &e)
    {
        printf("Caught exception when trying OpenCL backend\n");
        fprintf(stderr, "%s\n", e.what());
    }
    EXPECT_TRUE(true);
}

TEST_F(DataTest, Test2)
{

}
