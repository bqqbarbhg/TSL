#include <tsl.h>
#include "tests.h"

int main(int argc, char** argv)
{
	DO_TEST(test_source_fail);
	DO_TEST(test_source_file);

	getchar();
}