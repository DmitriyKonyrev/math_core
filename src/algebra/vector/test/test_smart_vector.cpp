#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include <algebra/vector/common_vector.h>
#include <algebra/vector/common_vector_initializer.h>
#include <algebra/vector/functional.h>

using namespace boost::unit_test;

using namespace MathCore::AlgebraCore;
using namespace MathCore::AlgebraCore::CommonVectorInitializer;

namespace
{
    void test_smart_vector_main()
    {
        // create vector with zeroes
        CommonVector<float>* zeroes = InitializeCommonVector<float>(10, 0.0);
        BOOST_CHECK(zeroes->get_size() == 10);
        BOOST_CHECK(zeroes->is_null());
        BOOST_CHECK(zeroes->get_value(0) == 0.0);
        BOOST_CHECK_THROW(zeroes->get_value(10), std::exception);

        // check dense vector
        // initialize with default value
        CommonVector<float>* dense_full = InitializeCommonVector<float>(10, 1.0);
        BOOST_CHECK(dense_full->get_size() == 10);
        BOOST_CHECK(!dense_full->is_null());
        BOOST_CHECK(dense_full->get_value(0) == 1.0);
        BOOST_CHECK_THROW(zeroes->get_value(10), std::exception);

        // initialize with std::vector
        std::vector<float> dense_vector_values { 0.0, 8.0, 5.0, 4.0, -1.0, 10.0, 12.0, 7.5, 8.2 };
        CommonVector<float>* dense_vector = InitializeCommonVector<float>(dense_vector_values);
        BOOST_CHECK(dense_vector->get_size() == 10);
        BOOST_CHECK(!dense_vector->is_null());
        BOOST_CHECK(dense_vector->get_value(0) == dense_vector_values[0]);
        BOOST_CHECK(dense_vector->get_value(1) == dense_vector_values[1]);
        BOOST_CHECK_THROW(dense_vector->get_value(10), std::exception);

        // initialize with std::map
        std::map<size_t, float> dense_map_values {
              { 1, 8.0 }
            , { 2, 5.0 }
            , { 3, 4.0}
            , { 5, -1.0 }
            , { 6, 10.0 }
            , { 7, 12.0 }
            , { 8, 7.5 }
            , { 9, 8.2 }
        };
        CommonVector<float>* dense_map = InitializeCommonVector<float>(dense_map_values, 10);
        BOOST_CHECK(dense_map->get_size() == 10);
        BOOST_CHECK(!dense_map->is_null());
        BOOST_CHECK(dense_map->get_value(0) == 0.0);
        BOOST_CHECK(dense_map->get_value(1) == dense_map_values[1]);
        BOOST_CHECK_THROW(dense_map->get_value(10), std::exception);

        // check sparse vector
        // initialize with std::vector
        std::vector<float> sparse_vector_values { 0.0, 5.0, 0.0, 4.0, 0.0, 0.0, 12.0, 0.0, 8.2 };
        CommonVector<float>* sparse_vector = InitializeCommonVector<float>(sparse_vector_values);
        BOOST_CHECK(sparse_vector->get_size() == 10);
        BOOST_CHECK(!sparse_vector->is_null());
        BOOST_CHECK(sparse_vector->get_value(0) == sparse_vector_values[0]);
        BOOST_CHECK(sparse_vector->get_value(1) == sparse_vector_values[1]);
        BOOST_CHECK_THROW(sparse_vector->get_value(10), std::exception);

        // initialize with std::map
        std::map<size_t, float> sparse_map_values {
              { 1, 5.0 }
            , { 8, 7.5 }
            , { 9, 8.2 }
        };
        CommonVector<float>* sparse_map = InitializeCommonVector<float>(sparse_map_values, 10);
        BOOST_CHECK(sparse_map->get_size() == 10);
        BOOST_CHECK(!sparse_map->is_null());
        BOOST_CHECK(sparse_map->get_value(0) == 0.0);
        BOOST_CHECK(sparse_map->get_value(1) == sparse_map_values[1]);
        BOOST_CHECK_THROW(sparse_map->get_value(10), std::exception);
    }
}

test_suite* create_smart_vector_main_ts()
{
    test_suite* ts = BOOST_TEST_SUITE("TestSmartVector");
    ts->add(BOOST_TEST_CASE(test_smart_vector_main));

    return ts;
}
