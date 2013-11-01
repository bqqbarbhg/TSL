#include "tests.h"
#include <tsl.h>
#include <string>

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

	char filebuf[32], *fileptr = filebuf;

	while (*fileptr++ = tslSource_get(&source))
		;

	TEST_ASSERT(std::string(filebuf) == "teststring", "Read expected string");

	tslSource_free(&source);

	return true;
}