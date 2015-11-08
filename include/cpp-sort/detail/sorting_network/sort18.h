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
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT18_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT18_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/utility/identity.h>
#include "../swap_if.h"

namespace cppsort
{
    template<>
    struct sorting_network_sorter<18u>:
        sorter_facade<sorting_network_sorter<18u>>
    {
        using sorter_facade<sorting_network_sorter<18u>>::operator();

        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            detail::swap_if(first[4u], first[12u], compare, projection);
            detail::swap_if(first[5u], first[13u], compare, projection);
            detail::swap_if(first[0u], first[7u], compare, projection);
            detail::swap_if(first[10u], first[17u], compare, projection);
            detail::swap_if(first[2u], first[3u], compare, projection);
            detail::swap_if(first[14u], first[15u], compare, projection);
            detail::swap_if(first[6u], first[8u], compare, projection);
            detail::swap_if(first[9u], first[11u], compare, projection);
            detail::swap_if(first[1u], first[16u], compare, projection);
            detail::swap_if(first[2u], first[6u], compare, projection);
            detail::swap_if(first[11u], first[15u], compare, projection);
            detail::swap_if(first[1u], first[9u], compare, projection);
            detail::swap_if(first[8u], first[16u], compare, projection);
            detail::swap_if(first[4u], first[10u], compare, projection);
            detail::swap_if(first[7u], first[13u], compare, projection);
            detail::swap_if(first[3u], first[12u], compare, projection);
            detail::swap_if(first[5u], first[14u], compare, projection);
            detail::swap_if(first[0u], first[2u], compare, projection);
            detail::swap_if(first[15u], first[17u], compare, projection);
            detail::swap_if(first[1u], first[4u], compare, projection);
            detail::swap_if(first[13u], first[16u], compare, projection);
            detail::swap_if(first[0u], first[5u], compare, projection);
            detail::swap_if(first[12u], first[17u], compare, projection);
            detail::swap_if(first[0u], first[1u], compare, projection);
            detail::swap_if(first[16u], first[17u], compare, projection);
            detail::swap_if(first[3u], first[7u], compare, projection);
            detail::swap_if(first[10u], first[14u], compare, projection);
            detail::swap_if(first[6u], first[9u], compare, projection);
            detail::swap_if(first[8u], first[11u], compare, projection);
            detail::swap_if(first[2u], first[15u], compare, projection);
            detail::swap_if(first[3u], first[8u], compare, projection);
            detail::swap_if(first[9u], first[14u], compare, projection);
            detail::swap_if(first[4u], first[5u], compare, projection);
            detail::swap_if(first[12u], first[13u], compare, projection);
            detail::swap_if(first[6u], first[10u], compare, projection);
            detail::swap_if(first[2u], first[6u], compare, projection);
            detail::swap_if(first[7u], first[11u], compare, projection);
            detail::swap_if(first[1u], first[4u], compare, projection);
            detail::swap_if(first[13u], first[16u], compare, projection);
            detail::swap_if(first[14u], first[15u], compare, projection);
            detail::swap_if(first[2u], first[3u], compare, projection);
            detail::swap_if(first[11u], first[15u], compare, projection);
            detail::swap_if(first[15u], first[16u], compare, projection);
            detail::swap_if(first[1u], first[2u], compare, projection);
            detail::swap_if(first[11u], first[14u], compare, projection);
            detail::swap_if(first[3u], first[6u], compare, projection);
            detail::swap_if(first[13u], first[14u], compare, projection);
            detail::swap_if(first[3u], first[4u], compare, projection);
            detail::swap_if(first[14u], first[15u], compare, projection);
            detail::swap_if(first[2u], first[3u], compare, projection);
            detail::swap_if(first[5u], first[6u], compare, projection);
            detail::swap_if(first[11u], first[12u], compare, projection);
            detail::swap_if(first[7u], first[9u], compare, projection);
            detail::swap_if(first[8u], first[10u], compare, projection);
            detail::swap_if(first[9u], first[10u], compare, projection);
            detail::swap_if(first[7u], first[8u], compare, projection);
            detail::swap_if(first[5u], first[11u], compare, projection);
            detail::swap_if(first[6u], first[12u], compare, projection);
            detail::swap_if(first[10u], first[12u], compare, projection);
            detail::swap_if(first[5u], first[7u], compare, projection);
            detail::swap_if(first[12u], first[14u], compare, projection);
            detail::swap_if(first[3u], first[5u], compare, projection);
            detail::swap_if(first[10u], first[13u], compare, projection);
            detail::swap_if(first[4u], first[7u], compare, projection);
            detail::swap_if(first[12u], first[13u], compare, projection);
            detail::swap_if(first[4u], first[5u], compare, projection);
            detail::swap_if(first[8u], first[9u], compare, projection);
            detail::swap_if(first[6u], first[9u], compare, projection);
            detail::swap_if(first[8u], first[11u], compare, projection);
            detail::swap_if(first[9u], first[12u], compare, projection);
            detail::swap_if(first[5u], first[8u], compare, projection);
            detail::swap_if(first[6u], first[7u], compare, projection);
            detail::swap_if(first[10u], first[11u], compare, projection);
            detail::swap_if(first[6u], first[8u], compare, projection);
            detail::swap_if(first[9u], first[11u], compare, projection);
            detail::swap_if(first[7u], first[10u], compare, projection);
            detail::swap_if(first[9u], first[10u], compare, projection);
            detail::swap_if(first[7u], first[8u], compare, projection);
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT18_H_
