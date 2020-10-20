The following utilities are available in the directory `cpp-sort/utility` and live in the namespace `cppsort::utility`. While not always directly related to sorting, they might be useful to solve common problems when writing and/or using sorters or adapters.

### `adapter_storage`

```cpp
#include <cpp-sort/utility/adapter_storage.h>
```

`adapter_storage` is a wrapper type meant to be used to store a *sorter* in the internals of *sorter adapter* which adapts a single *sorter*. One of its goal is to remain an empty type when possible in order to play nice with the parts of the libraries that allow to cast empty sorters to function pointers. It provides three different operations:
* *Construction:* it is constructed with a copy of the wrapped sorter which it stores in its internals. If `Sorter` is empty and default-constructible, then `adapter_storage<Sorter>` is also empty and default-constructible.
* *Sorter access:* a `get()` member function returns a reference to the wrapped sorter with `const` and reference qualifications matching those of the `adapter_storage` instance. If `Sorter` is empty and default-constructible, then `get()` returns a default-constructed instance of `Sorter` instead.
* *Invocation:* it has a `const`-qualified `operator()` which forwards its parameter to the corresponding operator in the wrapped `Sorter` (or in a default-constructed instance of `Sorter`) and returns its result.

```cpp
template<typename Sorter>
struct adapter_storage;
```

The usual way to use it when implementing a *sorter adapter* is to make said adapter inherit from `adapter_storage<Sorter>` and to feed it a copy of the *original sorter*. Then either `get()` or `operator()` can be used to correctly call the wrapped sorter.

*New in version 1.5.0*

### `as_comparison` and `as_projection`

```cpp
#include <cpp-sort/utility/functional.h>
```

When given a *Callable* that satisfies both the comparison and projection concepts, every sorter in **cpp-sort** considers that it is a comparison function (unless it is a projection-only sorter, in which case the function is considered to be a projection). To make such ambiguous callables usable as projections, the utility function `as_projection` is provided, which wraps a function and exposes only its unary overload. A similar `as_comparison` function is also provided in case one needs to explicitly expose the binary overload of a function.

The result of `as_projection` also inherits from `projection_base`, which makes it usable to [[compose projections|Chainable projections]] with `operator|`.

```cpp
template<typename Function>
auto as_projection(Function&& func)
    -> /* implementation-defined */;

template<typename Function>
auto as_comparison(Function&& func)
    -> /* implementation-defined */;
```

*Changed in version 1.7.0:* `as_comparison` and `as_projection` accept any *Callable*.

*Changed in version 1.7.0:* the object returned by `as_projection` inherits from `projection_base`.

### `as_function`

```cpp
#include <cpp-sort/utility/as_function.h>
```

`as_function` is an utility function borrowed from Eric Niebler's [Ranges v3](https://github.com/ericniebler/range-v3) library. This function takes a parameter and does what it can to return an object callable with the usual function call syntax. It is notably useful to transform a pointer to member data into a function taking an instance of the class and returning the member.

To be more specific, `as_function` returns the passed object as is if it is already callable with the usual function call syntax, and uses [`std::mem_fn`](http://en.cppreference.com/w/cpp/utility/functional/mem_fn) to wrap the passed object otherwise. It is worth noting that when the original object is returned, its potential to be called in a `constexpr` context remains the same, which makes `as_function` superior to `std::invoke` in this regard (prior to C++20).

```cpp
struct wrapper { int foo; };
// func is a function taking an instance of wrapper
// and returning the value of its member foo
auto&& func = utility::as_function(&wrapper::foo);
```

### Branchless traits

```cpp
#include <cpp-sort/utility/branchless_traits.h>
```

Some of the library's algorithms ([`pdq_sorter`](https://github.com/Morwenn/cpp-sort/wiki/Sorters#pdq_sorter) and everything that relies on it, which means many things since it's the most common fallback for other algorithms) may use a different logic depending on whether the use comparison and projection functions are branchful and branchless. Unfortunately, it isn't something that can be deterministically determined; in order to provide the information, the following traits are available (they always inherit from either `std::true_type` or `std::false_type`):

```cpp
template<typename Compare, typename T>
struct is_probably_branchless_comparison;

template<typename Compare, typename T>
constexpr bool is_probably_branchless_comparison_v
    = is_probably_branchless_comparison<Compare, T>::value;
```

This trait tells whether the comparison function `Compare` is likely to generate branchless code when comparing two instances of `T`. By default it considers that the following comparison functions are likely to be branchless:
* `std::less<>` and `std::less<T>` for any `T` which satisfies [`std::is_arithmetic`](http://en.cppreference.com/w/cpp/types/is_arithmetic)
* `std::greater<>` and `std::greater<T>` for any `T` which satisfies [`std::is_arithmetic`](http://en.cppreference.com/w/cpp/types/is_arithmetic)

```cpp
template<typename Projection, typename T>
struct is_probably_branchless_projection;

template<typename Projection, typename T>
constexpr bool is_probably_branchless_projection_v
    = is_probably_branchless_projection<Projection, T>::value;
```

This trait tells whether the projection function `Projection` is likely to generate branchless code when called with an instance of `T`. By default it considers that the following projection functions are likely to be branchless:
* `cppsort::utility::identity` for any type
* Any type that satisfies [`std::is_member_function_pointer`](http://en.cppreference.com/w/cpp/types/is_member_function_pointer) provided it is called with an instance of the appropriate class

These traits can be specialized for user-defined types. If one of the traits is specialized to consider that a user-defined type is likely to be branchless with a comparison/projection function, cv-qualified and reference-qualified versions of the same user-defined type will also be considered to produce branchless code when compared/projected with the same function.

### Buffer providers

```cpp
#include <cpp-sort/utility/buffer.h>
```

Buffer providers are classes designed to be passed to dedicated sorters; they describe how a the sorter should allocate a temporary buffer. Every buffer provider shall have a `buffer` inner class constructible with an `std::size_t`, and providing the methods `begin`, `end`, `cbegin`, `cend` and `size`.

```cpp
template<std::size_t N>
struct fixed_buffer;
```

This buffer provider allocates `N` elements on the stack. It uses [`std::array`](http://en.cppreference.com/w/cpp/container/array) behind the scenes, so it also allows buffers of size 0. The runtime size passed to the buffer at construction is discarded.

```cpp
template<typename SizePolicy>
struct dynamic_buffer;
```

This buffer provider allocates on the heap a number of elements depending on a given *size policy* (a class whose `operator()` takes the size of the collection and returns another size). You can use the function objects from `utility/functional.h` as basic size policies. The buffer construction may throw an instance of `std::bad_alloc` if it fails to allocate the required memory.

### Miscellaneous function objects

```cpp
#include <cpp-sort/utility/functional.h>
```

This header provides the class `projection_base` and the mechanism used to compose projections with `operator|`. See [[Chainable projections]] for more information.

Also available in this header, the struct `identity` is a function object that can type any value of any movable type and return it as is. It is used as a default for every projection parameter in the library so that sorters view the values as they are by default, without a modification.

```cpp
struct identity:
    projection_base
{
    template<typename T>
    constexpr auto operator()(T&& t) const noexcept
        -> T&&;

    using is_transparent = /* implementation-defined */;
};
```

It is equivalent to the proposed `std::identity` from the [Ranges TS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4560.pdf) and will probably be replaced by the standard function object the day it makes its way into the standard.

This header also provides additional function objects implementing basic unary operations. These functions objects are designed to be used as *size policies* with `dynamic_buffer` and similar classes. The following function objects are available:
* `half`: returns the passed value divided by 2.
* `log`: returns the base 10 logarithm of the passed value.
* `sqrt`: returns the square root of the passed value.

Since C++17, the following utility is also available when some level of micro-optimization is needed:

```cpp
template<auto Function>
struct function_constant
{
    using value_type = decltype(Function);

    static constexpr value_type value = Function;

    template<typename... Args>
    constexpr auto operator()(Args&&... args) const
        noexcept(noexcept(as_function(Function)(std::forward<Args>(args)...)))
        -> decltype(as_function(Function)(std::forward<Args>(args)...));

    constexpr operator value_type() const noexcept;
};
```

This utility is modeled after [`std::integral_constant`](http://en.cppreference.com/w/cpp/types/integral_constant), but is different in that it takes its parameter as `template<auto>`, and `operator()` calls the wrapped value instead of returning it. The goal is to store function pointers and pointer to members "for free": they are only "stored" as a template parameter, which allows `function_constant` to be an empty class. This has two main advantages: `function_constant` can benefit from *Empty Base Class Optimization* since it weights virtually nothing, and it won't need to be pushed on the stack when passed to a function, while the wrapped pointer would have been if passed unwrapped. Unless you are micro-optimizing some specific piece of code, you shouldn't need this class.

`is_probably_branchless_comparison` and `is_probably_branchless_projection` will correspond to `std::true_type` if the wrapped `Function` also gives `std::true_type`. Moreover, you can even specialize these traits for specific `function_constant` instanciations if you need even more performance.

*Warning: `function_constant` is only available since C++17.*

*New in version 1.7.0:* `projection_base` and chainable projections.

### `iter_move` and `iter_swap`

```cpp
#include <cpp-sort/utility/iter_move.h>
```

The functions `iter_move` and `iter_swap` are equivalent to the same functions as proposed by [P0022](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0022r1.html): utility functions intended to be used with ADL to handle proxy iterators among other things. An algorithm can use them instead of `std::move` and possibly ADL-found `swap` to handle tricky classes such as `std::vector<bool>`.

The default implementation of `iter_move` simply move-returns the dereferenced iterator. `iter_swap` is a bit more tricky: if the iterators to be swapped have a custom `iter_move`, then `iter_swap` will use it, otherwise it will call an ADL-found `swap` or `std::swap` on the dereferenced iterators.

```cpp
template<typename Iterator>
auto iter_move(Iterator it)
    -> decltype(std::move(*it));

template<typename Iterator>
auto iter_swap(Iterator lhs, Iterator rhs)
    -> void;
```

### `make_integer_range`

```cpp
#include <cpp-sort/utility/make_integer_range.h>
```

***WARNING:** `make_integer_range` and `make_index_range` are deprecated in version 1.8.0 and removed in version 2.0.0.*

The class template `make_integer_range` can be used wherever an [`std::integer_sequence`](http://en.cppreference.com/w/cpp/utility/integer_sequence) can be used. An `integer_range` takes a type template parameter that shall be an integer type, then three integer template parameters which correspond to the beginning of the range, the end of the range and the « step ».

```cpp
template<
    typename Integer,
    Integer Begin,
    Integer End,
    Integer Step = 1
>
using make_integer_range = /* implementation-defined */;
```

`make_index_range` is a specialization of `integer_range` for `std::size_t`.

```cpp
template<
    std::size_t Begin,
    std::size_t End,
    std::size_t Step = 1u
>
using make_index_range = make_integer_range<std::size_t, Begin, End, Step>;
```

### `size`

```cpp
#include <cpp-sort/utility/size.h>
```

`size` is a function that can be used to get the size of an iterable. It is equivalent to the C++17 function [`std::size`](http://en.cppreference.com/w/cpp/iterator/size) but has an additional tweak so that, if the iterable is not a fixed-size C array and doesn't have a `size` method, it calls `std::distance(std::begin(iter), std::end(iter))` on the iterable. Therefore, this function can also be used for `std::forward_list` as well as some implementations of ranges.

### `static_const`

```cpp
#include <cpp-sort/utility/static_const.h>
```

`static_const` is a tiny utility used to instantiate function objects (for example sorters) and expose a single global instance to users of the library while avoiding ODR problems. It is taken straight from [Ranges v3](https://github.com/ericniebler/range-v3). The general pattern to instantiate function objects is as follows:

```cpp
namespace
{
    constexpr auto&& awesome_sort
        = cppsort::utility::static_const<awesome_sorter>::value;
}
```

You can read more about this instantiation pattern in [an article](http://ericniebler.com/2014/10/21/customization-point-design-in-c11-and-beyond/) by Eric Niebler.

*Warning: this header does not exist anymore in the C++17 branch; use [`inline` variables](http://en.cppreference.com/w/cpp/language/inline) instead.*
