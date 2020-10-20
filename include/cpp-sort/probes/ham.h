/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_HAM_H_
#define CPPSORT_PROBES_HAM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/indirect_compare.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto ham_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (size < 2) {
                return 0;
            }

            ////////////////////////////////////////////////////////////
            // Indirectly sort the iterators

            // Copy the iterators in a vector
            std::vector<ForwardIterator> iterators;
            iterators.reserve(size);
            for (ForwardIterator it = first ; it != last ; ++it) {
                iterators.push_back(it);
            }

            // Sort the iterators on pointed values
            cppsort::detail::pdqsort(
                iterators.begin(), iterators.end(),
                cppsort::detail::indirect_compare(compare, projection),
                utility::identity{}
            );

            ////////////////////////////////////////////////////////////
            // Count the number of values not in place

            difference_type count = 0;
            for (auto&& it: iterators) {
                if (comp(proj(*first), proj(*it)) ||
                    comp(proj(*it), proj(*first))) {
                    ++count;
                }
                ++first;
            }
            return count;
        }

        struct ham_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return ham_probe_algo(std::begin(iterable), std::end(iterable),
                                      utility::size(iterable),
                                      std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return ham_probe_algo(first, last, std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }
        };
    }

    inline constexpr sorter_facade<detail::ham_impl> ham{};
}

#endif // CPPSORT_PROBES_HAM_H_
