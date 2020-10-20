/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes/osc.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: osc", "[probe][osc]" )
{
    SECTION( "simple test" )
    {
        // Example from the paper Adaptative Heapsort
        // by Levcopoulos and Petersson

        std::forward_list<int> li = { 6, 3, 9, 8, 4, 7, 1, 11 };
        CHECK( cppsort::probe::osc(li) == 17 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 17 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::osc(tricky, &internal_compare<int>::compare_to) == 17 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::osc(li) == 0 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // Example from the paper Adaptative Heapsort
        // by Levcopoulos and Petersson, the upper bound
        // should be (size * (size - 2) - 1) / 2

        std::forward_list<int> li = { 8, 5, 10, 3, 12, 1, 13, 2, 11, 4, 9, 6, 7 };
        CHECK( cppsort::probe::osc(li) == 71 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 71 );
    }
}
