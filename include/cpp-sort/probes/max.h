/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_MAX_H_
#define CPPSORT_PROBES_MAX_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
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
#include "../detail/equal_range.h"
#include "../detail/indirect_compare.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto max_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;
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
            // Maximum distance an element has to travel in order to
            // reach its sorted position

            difference_type max_dist = 0;
            difference_type it_pos = 0;
            for (auto it = first ; it != last ; ++it) {
                // Find the range where *first belongs once sorted
                auto rng = cppsort::detail::equal_range(
                    iterators.begin(), iterators.end(), proj(*it), compare,
                    [&proj](const auto& iterator) {
                        return proj(*iterator);
                    });
                auto pos_min = std::distance(iterators.begin(), rng.first);
                auto pos_max = std::distance(iterators.begin(), rng.second);

                // If *first isn't into one of its sorted positions, computed the closest
                if (it_pos < pos_min) {
                    max_dist = std::max(pos_min - it_pos, max_dist);
                } else if (it_pos >= pos_max) {
                    max_dist = std::max(it_pos - pos_max + 1, max_dist);
                }

                ++it_pos;
            }
            return max_dist;
        }

        struct max_impl
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
                return max_probe_algo(std::begin(iterable), std::end(iterable),
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
                return max_probe_algo(first, last, std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }
        };
    }

    inline constexpr sorter_facade<detail::max_impl> max{};
}

#endif // CPPSORT_PROBES_MAX_H_
