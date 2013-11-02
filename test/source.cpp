#include "tests.h"
#include <tsl.h>
#include <string>

static std::string readString(TSLSource *src)
{
	std::string str;
	char c;
	while (c = tslSource_get(src))
		str += c;
	return str;
}

bool test_source_fail()
{
	TSLSource source;
	TEST_ASSERT(tslSource_fileOpen(&source, "nofile.txt") != 0, "File open failed");
	tslSource_free(&source);
	return true;
}

bool test_source_file()
{
	FILE *file = fopen("test.txt", "w");
	TEST_ASSERT(file, "Opened test file");
	fprintf(file, "teststring");
	fclose(file);

	TSLSource source;
	TEST_ASSERT(tslSource_fileOpen(&source, "test.txt") == 0, "File open success");
	TEST_ASSERT(readString(&source) == "teststring", "Read expected string");

	tslSource_free(&source);

	return true;
}

bool test_source_cstringref()
{
	TSLSource source;
	TEST_ASSERT(tslSource_cStringRef(&source, 0) != 0, "Source create fail");
	TEST_ASSERT(tslSource_cStringRef(&source, "cstringreftest") == 0, "Source create success");

	TEST_ASSERT(readString(&source) == "cstringreftest", "Read expected string");

	tslSource_free(&source);

	return true;
}

static bool utf8test(const char *str, int len)
{
	TSLSource source;
	TEST_ASSERT(tslSource_cStringRef(&source, str) == 0, "Source create success");

	char buf[4];
	TEST_ASSERT(tslSource_getUtf8Char(&source, buf) - buf == len, "Read correct count of UTF-8 bytes");

	return true;
}

bool test_source_utf8()
{
	TEST_DEFER(utf8test("", 1));
	TEST_DEFER(utf8test("A", 1));
	TEST_DEFER(utf8test("\x24", 1));
	TEST_DEFER(utf8test("\xC2\xA2", 2));
	TEST_DEFER(utf8test("\xE2\x82\xAC", 3));
	TEST_DEFER(utf8test("\xF0\xA4\xAD\xA2", 4));
	TEST_DEFER(utf8test("\xF0\xA4\xAD\xA2\xA2", 0));
	TEST_DEFER(utf8test("\xF0\x24", 0));
	TEST_DEFER(utf8test("\xA2", 0));

	return true;
}