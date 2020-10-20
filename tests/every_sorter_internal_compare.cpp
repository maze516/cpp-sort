/*
 * Copyright (c) 2017-2019 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEMPLATE_TEST_CASE( "test every sorter with a pointer to member function comparison",
                    "[sorters][as_function]",
                    cppsort::block_sorter<>,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter<>,
                    cppsort::heap_sorter<5>,
                    cppsort::insertion_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<internal_compare<int>> collection;
    collection.reserve(35);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 35, -47);

    using sorter = TestType;
    sorter{}(collection, &internal_compare<int>::compare_to);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}
