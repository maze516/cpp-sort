/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_SORTERS_MERGE_SORTER_H_
#define CPPSORT_SORTERS_MERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/identity.h>
#include <cpp-sort/utility/size.h>
#include "../detail/merge_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    struct merge_sorter:
        sorter_facade<merge_sorter>
    {
        template<
            typename ForwardIterable,
            typename Compare = std::less<>
        >
        auto operator()(ForwardIterable& iterable, Compare compare={}) const
            -> void
        {
            detail::merge_sort(
                std::begin(iterable),
                std::end(iterable),
                compare,
                utility::identity{},
                utility::size(iterable)
            );
        }

        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity
        >
        auto operator()(ForwardIterator first, ForwardIterator last,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            detail::merge_sort(
                first, last,
                compare, projection,
                std::distance(first, last)
            );
        }
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<>
    struct sorter_traits<merge_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = true;
    };
}

#endif // CPPSORT_SORTERS_MERGE_SORTER_H_
