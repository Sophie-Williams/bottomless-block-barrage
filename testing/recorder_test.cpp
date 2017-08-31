#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <fstream>
#include <recorder.hpp>

// Dummy implementation
unsigned long osGetTime()
{
    return 1504164600;
}

std::vector<int> kInitial =
{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 0,
};

std::vector<int> kNext1 =
{
    1, 2, 3, 4, 5, 6
};

std::vector<int> kNext2 =
{
    6, 5, 4, 3, 2, 1
};

std::vector<int> kNext3 =
{
    3, 5, 3, 3, 1, 1
};

const char kExpected[] =
"BBB\0" // magic
"\0\2" // version
"\xB\x6\x1\x0\x63"  // header info
"\0\0\0\0\0\0"      // row 1 initial
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\0\0\0\0\0\0"
"\1\0\0\0\0\0"
"\1\2\0\0\0\0"

"\x12\0\0\0"        // number of next entries
"\1\2\3\4\5\6"
"\6\5\4\3\2\1"
"\3\5\3\3\1\1"

"\5\0\0\0"          // number of input entires
"\0\0\0\0" "\0\0\0\0" "\3\0\0\0" // trigger held frames
"\1\0\0\0" "\0\0\0\0" "\1\0\0\0"
"\0\0\0\0" "\0\0\0\0" "\2\0\0\0"
"\0\0\0\0" "\1\0\0\0" "\1\0\0\0"
"\1\0\0\0" "\0\0\0\0" "\1\0\0\0";

BOOST_AUTO_TEST_CASE(TestRecorder)
{
    Recorder recorder;
    recorder.settings(11, 6, PanelTable::ENDLESS, 0, 99);
    std::vector<Panel::Type> initial(kInitial.begin(), kInitial.end());

    recorder.set_initial(initial);
    std::vector<Panel::Type> next1(kNext1.begin(), kNext1.end());
    std::vector<Panel::Type> next2(kNext2.begin(), kNext2.end());
    std::vector<Panel::Type> next3(kNext3.begin(), kNext3.end());

    recorder.add_next(next1);
    recorder.add_next(next2);
    recorder.add_next(next3);

    recorder.add_input(0, 0);
    recorder.add_input(0, 0);
    recorder.add_input(0, 0);
    recorder.add_input(1, 0);
    recorder.add_input(0, 0);
    recorder.add_input(0, 0);
    recorder.add_input(0, 1);
    recorder.add_input(1, 0);

    std::stringstream file(std::stringstream::out | std::stringstream::binary);
    recorder.save(file);

    std::string binary_data = file.str();
    // NUL terminator
    std::string expected(kExpected, sizeof(kExpected) - 1);
    BOOST_REQUIRE_EQUAL(binary_data.size(), expected.size());
    // Ehh to easily print out the value of whats failing...
    for (unsigned int i = 0; i < binary_data.size(); i++)
        BOOST_CHECK_EQUAL(binary_data[i], expected[i]);
    BOOST_CHECK_EQUAL_COLLECTIONS(binary_data.begin(), binary_data.end(), expected.begin(), expected.end());


}
