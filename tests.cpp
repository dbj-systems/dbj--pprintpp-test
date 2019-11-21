
#include "dbj--pprintpp/pprintpp.hpp"


#include <cassert>
#include <cstring>
#include <typeinfo>
#include <cstdio>
#include <exception>

/*
---------------------------------------------------------------------------------------
Common test data

2019 Nov, c++20 both  msvc and clang 8, have the ability to printf 
utf-8 string literals, that is: u8 aka 'char8_t *'
char16_t and char32_t are treated as pointers by printf() 
*/

static char8_t  hiragana8[] = u8"ひらがな";
static char16_t hiragana16[] = u"ひらがな";
static char32_t hiragana32[] = U"ひらがな";

/*
---------------------------------------------------------------------------------------
Tests begin here
*/

extern "C" int test_1(int, wchar_t* [])
{
#if __clang__
	// this is apparently too complex for MSVC: 
	// Microsoft (R) C/C++ Optimizing Compiler Version 19.23.28106.4 for x86
	// pprintpp.hpp(156, 1) : fatal error C1202 : recursive type or function dependency context too complex
	// 
	pprintf("{35s} | Format str -> Result\n"
		"{35s} | ---------------------\n"
		"{35s} | \\{s}     -> \"{s}\"\n"
		"{35s} | \\{10s}   -> \"{10s}\"\n"
		"{35s} | \\{}      -> \"{}\"\n"
		"{35s} | \\{x}     -> \"{x}\"\n"
		"{35s} | \\{10}    -> \"{10}\"\n"
		"{35s} | \\{10x}   -> \"{10x}\"\n"
		"{35s} | \\{#10x}  -> \"{#10x}\"\n"
		"{35s} | \\{#010x} -> \"{#010x}\"\n"
		"{35s} | \\{}      -> \"{}\"\n"
		"{35s} | \\{10}    -> \"{10}\"\n"
		"{35s} | \\{5.2}   -> \"{5.2}\"\n",
		"Meaning", "---------------------",
		"String \"abc\"", "abc",
		"String \"abc\" + min width", "abc",
		"value 0x123, default", 0x123,
		"value 0x123, hex", 0x123,
		"minimum width", 0x123,
		"hex + min width", 0x123,
		"hex + min width + hex prefix", 0x123,
		"hex + min w. + hex prefix + 0-pad", 0x123,
		"FP", 12.345,
		"FP + min width", 12.34567890123456789,
		"FP + width + max precision", 12.34567890123456789
	);
#endif // __clang__
	return EXIT_SUCCESS;
}
/*
Example:
 auto compile_time_format_ =  AUTOFORMAT("{} {}", 123, 1.23f)
// must return: "%d %f"
 */
#define PPRINTPP_TEST(printf_format_correct_str, ...) \
    assert(!strcmp(AUTOFORMAT(__VA_ARGS__), printf_format_correct_str))

extern "C" int test_2(int, wchar_t* [])
{
	PPRINTPP_TEST("", "");
	// since dbj pprintpp 0.2.0 users can use utf8 string
	// and print them too
	PPRINTPP_TEST("%s", "{s}", hiragana8); 
	// just pointers
	PPRINTPP_TEST("%p", "{s}", hiragana16);
	PPRINTPP_TEST("%p", "{s}", hiragana32);

	PPRINTPP_TEST("%%", "%%");
	PPRINTPP_TEST("%d %f", "{} %f", 123, 1.23f);
	PPRINTPP_TEST("%f %d", "%f {}", 1.23f, 123);

	PPRINTPP_TEST(" { ", " \\{ ");
	PPRINTPP_TEST("{}", "\\{}");
	PPRINTPP_TEST(" { %d } ", " \\{ {} } ", 123);

	PPRINTPP_TEST("%p", "{}", nullptr);
	PPRINTPP_TEST("%p", "{}", reinterpret_cast<void*>(0));


#if (PPRINTPP_STANDARD_CHAR_PTR == 0)
	// display 'char *' with '%p'
	PPRINTPP_TEST("%p", "{}", "str");
#else
	// by default char * , get's `s%` pair
	PPRINTPP_TEST("%s", "{}", "str");
#endif

	 // `{s}` always get's '%s'
	PPRINTPP_TEST("%s", "{s}", "str");

	PPRINTPP_TEST("%c", "{}", static_cast<char>(123));

	PPRINTPP_TEST("%d", "{}", static_cast<short>(123));
	PPRINTPP_TEST("%d", "{}", 123);
	PPRINTPP_TEST("%ld", "{}", 123l);
	PPRINTPP_TEST("%lld", "{}", 123ll);

	PPRINTPP_TEST("%u", "{}", 123u);
	PPRINTPP_TEST("%lu", "{}", 123ul);
	PPRINTPP_TEST("%llu", "{}", 123ull);

	PPRINTPP_TEST("%x", "{x}", 123u);
	PPRINTPP_TEST("%lx", "{x}", 123ul);
	PPRINTPP_TEST("%llx", "{x}", 123ull);

	PPRINTPP_TEST("%d", "{}", true);

	PPRINTPP_TEST("%f", "{}", 1.0f);
	PPRINTPP_TEST("%lf", "{}", 1.0);

	PPRINTPP_TEST("%10d", "{10}", 123);
	PPRINTPP_TEST("%10x", "{10x}", 123u);
	PPRINTPP_TEST("%#10x", "{#10x}", 123u);

	// Give the user hex if asked for explicitly.
	PPRINTPP_TEST("%x", "{x}", 123);
	PPRINTPP_TEST("%lx", "{x}", 123l);
	PPRINTPP_TEST("%llx", "{x}", 123ll);

	pprintf( "\n\n[" __TIMESTAMP__ "] {s}",  "All basic tests have passed.");

	return 0;
}
#undef PPRINTPP_TEST

extern "C" int test_3(int, wchar_t* [])
{
	::printf("\n\n on windows10 make sure you set the console font that can display your glyphs");
	::printf("\n 'MS Gothic' seems ok for these utf-8 glyphs for Windows 10 Console, circa 2019 Nov");
	pprintf("\n\n u8 string literal -- {s}", hiragana8);
	pprintf("\n\n char16  string literal is printf'd as a pointer -- {s}", hiragana16);
	pprintf("\n\n char32  string literal is printf'd as a pointer -- {s}", hiragana32);

	pprintf("\n\nThis is \\{ how you mask } {s}.", "curly braces");

	pprintf("More formatting flags:\n"
		"{x}, {10x}, {#10x}, and {#010x} use the normal printf capabilities.\n",
		0x123, 0x123, 0x123, 0x123);

	pprintf("{20}\n", 1.23456);

	std::exception x1("runtime      error");
	std::exception x2("cosmological error");

	pprintf("\nException 1: '{ }' \nException 2: '{ }'\n", x1.what(), x2.what() );

	return EXIT_SUCCESS;
}
