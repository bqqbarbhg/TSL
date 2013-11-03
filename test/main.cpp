#include <tsl.h>
#include "tests.h"

int main(int argc, char** argv)
{
	DO_TEST(test_memory_hooks);
	tsl_SetMemoryHooks(0, 0, 0);

	DO_TEST(test_unicode);

	DO_TEST(test_source_fail);
	DO_TEST(test_source_file);
	DO_TEST(test_source_cstringref);
	DO_TEST(test_source_utf8);

	DO_TEST(test_token_empty);
	DO_TEST(test_token_strings);
	DO_TEST(test_token_failpos);
	DO_TEST(test_token_errors);

	getchar();
}
