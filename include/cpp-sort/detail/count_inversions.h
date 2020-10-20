/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_COUNT_INVERSIONS_H_
#define CPPSORT_DETAIL_COUNT_INVERSIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "move.h"

namespace cppsort::detail
{
    template<
        typename ResultType,
        typename RandomAccessIterator1,
        typename RandomAccessIterator2,
        typename Compare
    >
    auto count_inversions_merge(RandomAccessIterator1 first, RandomAccessIterator1 middle,
                                RandomAccessIterator1 last, RandomAccessIterator2 cache,
                                Compare compare)
        -> ResultType
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);

        ResultType inversions = 0;

        // Shrink the problem size on the left side
        while (comp(*first, *middle)) {
            ++first;
        }

        auto first2 = middle;
        auto result = cache;
        for (auto first1 = first ; first1 != middle ; ++result)
        {
            if (first2 == last) {
                detail::move(first1, middle, result);
                break;
            }
            if (comp(*first2, *first1)) {
                *result = iter_move(first2);
                ++first2;
                inversions += middle - first1;
            } else {
                *result = iter_move(first1);
                ++first1;
            }
        }

        // Move everything back to the original array, note that
        // everything after first2 hasn't been moved to the cache
        // since it's already in its final place
        detail::move(cache, cache + (first2 - first), first);

        return inversions;
    }

    template<
        typename ResultType,
        typename RandomAccessIterator1,
        typename RandomAccessIterator2,
        typename Compare
    >
    auto count_inversions(RandomAccessIterator1 first, RandomAccessIterator1 last,
                          RandomAccessIterator2 cache, Compare compare)
        -> ResultType
    {
        auto size = last - first;
        if (size < 2) {
            return 0;
        }

        ResultType inversions = 0;
        auto middle = first + size / 2;

        inversions += count_inversions<ResultType>(first, middle, cache, compare);
        inversions += count_inversions<ResultType>(middle, last, cache, compare);
        inversions += count_inversions_merge<ResultType>(std::move(first), std::move(middle),
                                                         std::move(last), std::move(cache),
                                                         std::move(compare));
        return inversions;
    }
}

#endif // CPPSORT_DETAIL_COUNT_INVERSIONS_H_
