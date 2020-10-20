/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <utility>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/stable_sort.h>
#include <cpp-sort/utility/functional.h>

namespace
{
    struct tricky_function
    {
        template<typename T, typename U>
        auto operator()(T&& lhs, U&& rhs)
            noexcept(noexcept(std::forward<T>(lhs) > std::forward<U>(rhs)))
            -> decltype(std::forward<T>(lhs) > std::forward<U>(rhs))
        {
            // Compare values in reverse order
            return std::forward<T>(lhs) > std::forward<U>(rhs);
        }

        template<typename T>
        auto operator()(T&& value) const noexcept
            -> decltype(std::forward<T>(value))
        {
            return std::forward<T>(value);
        }
    };
}

TEST_CASE( "try mixed comparison/projection function object",
           "[utility][as_projection]" )
{
    std::vector<int> collection(100);
    std::iota(std::begin(collection), std::end(collection), 0);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    tricky_function func;

    SECTION( "without an untransformed function" )
    {
        auto vec = collection;
        cppsort::sort(vec, func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(std::begin(vec), std::end(vec), func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec, func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, vec, func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(vec, func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(std::begin(vec), std::end(vec), func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, vec, func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), func);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "with a function wrapped in as_projection" )
    {
        auto vec = collection;
        cppsort::sort(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::stable_sort(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::stable_sort(std::begin(vec), std::end(vec), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "with a function wrapped in as_comparison" )
    {
        auto vec = collection;
        cppsort::sort(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(std::begin(vec), std::end(vec), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(std::begin(vec), std::end(vec), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        cppsort::stable_sort(cppsort::pdq_sort, std::begin(vec), std::end(vec), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }
}
