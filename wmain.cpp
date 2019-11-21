
#include "dbj--pprintpp/pprintpp.hpp"

#include <cassert>
#include <cstring>
#include <typeinfo>
#include <cstdio>
#include <array>
#include <exception>

using namespace std;

extern "C" int test_1(int = 0, wchar_t* [] = nullptr);
extern "C" int test_2(int = 0, wchar_t* [] = nullptr);
extern "C" int test_3(int = 0, wchar_t* [] = nullptr);

array test_units = {
	test_1, test_2, test_3
};

int wmain(int argc, wchar_t * argv [])
{
	::printf("\n\n-------------------------------------------------------\n");
	::printf("\nCompiled with: ");
#ifdef __clang__
	//__clang__             // set to 1 if compiler is clang
	//	__clang_major__       // integer: major marketing version number of clang
	//	__clang_minor__       // integer: minor marketing version number of clang
	//	__clang_patchlevel__  // integer: marketing patch level of clang
	//	__clang_version__     // string: full version number
	printf( "CLANG: %s\n" , __clang_version__);
#else
	printf("_MSVC_LANG: %lu\n", _MSVC_LANG);
#endif

	pprintf("\n PPRINTPP_AVOID_STL is set to {}", PPRINTPP_AVOID_STL);
	pprintf("\n PPRINTPP_STANDARD_CHAR_PTR is set to {}", PPRINTPP_STANDARD_CHAR_PTR);
	::printf("\n-------------------------------------------------------\n");

	int cnt_ = 1;
	for (auto tu : test_units) {
		::printf("TEST %03d ]---------------------------------------------\n\n", cnt_++);
		tu( argc, argv );
		::printf("\n\n-------------------------------------------------------\n");
	}

	::printf("\nDone...\n");
 	::system("@pause");
	return EXIT_SUCCESS;
}