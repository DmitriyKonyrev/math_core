#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

test_suite* create_smart_vector_main_ts();

test_suite* init_unit_test_suite(int /*  argc*/, char** /*  argv*/)
{
    test_suite* master = &framework::master_test_suite();
    master->add(create_smart_vector_main_ts());

    return 0;
}
