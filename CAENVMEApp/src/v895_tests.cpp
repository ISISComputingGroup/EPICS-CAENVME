#include <gtest/gtest.h>
#include <epicsTypes.h>
#include "CAENVMEWrapper.h"

namespace {

    TEST(V895Tests, test_GIVEN_wrapper_WHEN_created_THEN_looks_ok) {
        std::unique_ptr<CAENVMEWrapper> vme(new CAENVMEWrapper(true, cvV1718, 0, 0));
        ASSERT_STREQ(vme->boardFWRelease().c_str(), "SIM");
    }
    
    TEST(V895Tests, test_GIVEN_wrapper_WHEN_write_value_THEN_readback_ok) {
        std::unique_ptr<CAENVMEWrapper> vme(new CAENVMEWrapper(true, cvV1718, 0, 0));
        epicsUInt16 val = 14567, testval;
        vme->writeCycle(23, &val, cvA32_U_DATA, cvD16);
        vme->readCycle(23, &testval, cvA32_U_DATA, cvD16);
        ASSERT_EQ(val, testval);
    }

} // namespace
