
# Testing [dbj--pprintpp](https://github.com/dbj-systems/dbj--pprintpp)

> Notes 

- Built using Visual Studio 2019, with both CL and CLANG compiler as coming with VS.
- Visual Studio 2019 is being updated very often.
   - We do always update and check, regularly , if CL capabilities are 
improved so it can compile the code that CLANG could, but CL could not.
   - See the `test_1()`.
   - "test" are just functions in `wmain.cpp`


####  Treatment of: char *

dbj--printpp uses the `::printf()` standard behaviour. That is, i treats `char *` as a string.

`#define PPRINTPP_STANDARD_CHAR_PTR 1` is the default 

`pprintf` provides additional service to its users. It can treat `char *` as a pointer, not as a string.  This is useful when users, do not or can not, rely on strings being zero terminated.

To achieve that please set

`#define PPRINTPP_STANDARD_CHAR_PTR 0` 

before including, `pprintpp.hpp`.


```cpp
#define PPRINTPP_STANDARD_CHAR_PTR 0
#include <pprintpp.h>

char const * message_ = "The Message";

#if PPRINTPP_STANDARD_CHAR_PTR 
    // standard
    // output the string
    pprintf("{}", message_) ;
    // output the pointer
    pprintf("{p}", message_) ;
#else
    // safe
    // output the string
    pprintf("{s}", message_) ;
    // output the pointer
    pprintf("{}", message_) ;
#endif
```
#### The roadmap

1. testing [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer)
1. support for `wchar_t`
   - Basically implementing `wpprintf` that will target [`::wprintf`](https://en.cppreference.com/w/c/io/fwprintf)
   - All Windows API functions that accept LP[C]WSTR (that is: are wchar_t based), expect UTF-16 encoding.
   - The size of wchar_t is implementation-defined, and in fact most Linux compilers (such as GCC) define it to be 32-bit large, designed to hold text in UTF-32 encoding.
   - With MSVC compiler, `wchar_t` is 16-bit large, with the intention that it be used to represent Unicode strings in UTF-16 encoding.
   - Windows native `char` is `wchar_t`
      - All Windows 'char' stdio is transformed to `wchar_t` before final output
      - Windows 'char' stdio is performance overhead

```cpp
wpprintf(L"{s}{d}{c}", L"The answer is: ", 42, L'!' );
```

- Why using wprintf? 
- there are standard formatters (aka formating characters) in `printf` to output narrow strings and chars, and vice versa. Thus one can output wide strings and chars using `printf` only.
   - that incurs performance overhead on windows, as all the char API's, translate to `wchar_t` and call the `wchar_t` counterparts.


#### In case you wish to contribute please fork and send pull requests. 

---
Original work in this repository is &copy; dbj@dbj.org (dusanjovanovic.org)

Licence for this repository is **[CC BY SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)**