/*
 * Copyright (c) 2015-2016 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT0_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT0_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

namespace cppsort
{
namespace detail
{
    template<>
    struct low_comparisons_sorter_impl<0u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator,
                        Compare={}, Projection={}) const
            -> void
        {}
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT0_H_
