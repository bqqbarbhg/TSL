#ifndef TSLTEST_TEST_H
#define TSLTEST_TEST_H

#include <cstdio>

#define TSTSSTR2(x) #x
#define TSTSSTR(x) TSTSSTR2(x)

#define TEST_ASSERT(x, m) do { if (!(x)) { printf("> Assert failed: '%s' (" __FILE__ ":" TSTSSTR(__LINE__) ") \n", m); return false; } } while (0)

#define DO_TEST(x) do { bool res = (x)(); printf("%-20s ", #x); if (res) { printf("PASS\n"); } else { printf("FAIL\n"); }  } while (0)

bool test_source_fail();
bool test_source_file();

#endif