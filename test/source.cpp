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
}