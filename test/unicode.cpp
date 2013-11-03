#include "tests.h"
#include <tsl.h>
#include <cstring>

static bool testUtf8Encoding(unsigned int point, const char *enc)
{
	char utfbuf[5];
	*tsl_pointToUtf8(utfbuf, point) = 0;
	TEST_ASSERT(strcmp(utfbuf, enc) == 0, "Correct encoding");
	return true;
}

bool test_unicode()
{
	bool ret = true;

	TEST_DEFER(testUtf8Encoding(0x0024, "\x24"));
	TEST_DEFER(testUtf8Encoding(0x00A2, "\xC2\xA2"));
	TEST_DEFER(testUtf8Encoding(0x20AC, "\xE2\x82\xAC"));
	TEST_DEFER(testUtf8Encoding(0x24B62, "\xF0\xA4\xAD\xA2"));

	TEST_DEFER(testUtf8Encoding(0x0, ""));
	TEST_DEFER(testUtf8Encoding(0xFFFFFF, ""));

	return ret;
}
