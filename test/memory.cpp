#include "tests.h"
#include <tsl.h>

static int dummy[2] = { 0 };

static void* testAlloc(size_t s)
{
	return &dummy[0];
}

static void* testRealloc(void* p, size_t s)
{
	return (int*)p + s;
}

static void testFree(void* p)
{
	*(int*)p = 123;
}

bool test_memory_hooks()
{
	tsl_SetMemoryHooks(testAlloc, testRealloc, testFree);

	void *ptr = testAlloc(1);
	TEST_ASSERT(ptr == &dummy[0], "Malloc hook works");
	ptr = testRealloc(ptr, 1);
	TEST_ASSERT(ptr == &dummy[1], "Realloc hook works");
	testFree(ptr);
	TEST_ASSERT(dummy[1] == 123, "Free hook works");

	return true;
}
