/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <list>
#include <numeric>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/counting_adapter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/sorters/std_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/span.h>

TEST_CASE( "basic counting_adapter tests",
           "[counting_adapter][selection_sorter]" )
{
    // Selection sort always makes the same number of comparisons
    // for a given size of arrays, allowing to deterministically
    // check that number of comparisons
    constexpr auto sorter = cppsort::counting_adapter(cppsort::selection_sort);

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        std::size_t res = sorter(collection);
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "with projections" )
    {
        struct wrapper { int value; };

        // Pseudo-random number engine
        std::mt19937_64 engine(Catch::rngSeed());

        // Fill the collection
        std::vector<wrapper> tmp(80);
        helpers::iota(std::begin(tmp), std::end(tmp), 0, &wrapper::value);
        std::shuffle(std::begin(tmp), std::end(tmp), engine);
        std::list<wrapper> collection(std::begin(tmp), std::end(tmp));

        // Sort and check it's sorted
        std::size_t res = sorter(collection, &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "counting_adapter tests with std_sorter",
           "[counting_adapter][std_sorter]" )
{
    constexpr auto sorter = cppsort::counting_adapter(cppsort::std_sort);

    SECTION( "without projections" )
    {
        // Fill the collection
        std::vector<int> collection; collection.reserve(65);
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}

TEST_CASE( "counting_adapter with span",
           "[counting_adapter][span][selection_sorter]" )
{
    constexpr auto sorter = cppsort::counting_adapter(cppsort::selection_sort);

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        std::size_t res = sorter(make_span(collection));
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "with projections" )
    {
        struct wrapper { int value; };

        // Pseudo-random number engine
        std::mt19937_64 engine(Catch::rngSeed());

        // Fill the collection
        std::vector<wrapper> tmp(80);
        helpers::iota(std::begin(tmp), std::end(tmp), 0, &wrapper::value);
        std::shuffle(std::begin(tmp), std::end(tmp), engine);
        std::list<wrapper> collection(std::begin(tmp), std::end(tmp));

        // Sort and check it's sorted
        std::size_t res = sorter(make_span(collection), &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}
