/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/dis.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: dis", "[probe][dis]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 47, 53, 46, 41, 59, 81, 74, 97, 100, 45 };
        CHECK( cppsort::probe::dis(li) == 9 );
        CHECK( cppsort::probe::dis(std::begin(li), std::end(li)) == 9 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::dis(tricky, &internal_compare<int>::compare_to) == 9 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::dis(li) == 0 );
        CHECK( cppsort::probe::dis(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::forward_list<int> li = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::dis(li) == 10 );
        CHECK( cppsort::probe::dis(std::begin(li), std::end(li)) == 10 );
    }
}
