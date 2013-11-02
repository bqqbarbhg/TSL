#include <tsl.h>
#include "tests.h"

int main(int argc, char** argv)
{
	DO_TEST(test_source_fail);
	DO_TEST(test_source_file);
	DO_TEST(test_source_cstringref);
	DO_TEST(test_source_utf8);

	DO_TEST(test_token_basic);

	getchar();
}