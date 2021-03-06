#ifndef TSLTEST_TEST_H
#define TSLTEST_TEST_H

#include <cstdio>

extern FILE* logfile;

#define TSTSSTR2(x) #x
#define TSTSSTR(x) TSTSSTR2(x)

#define TEST_ASSERT(x, m) do { if (!(x)) { printf("> Assert failed: '%s'\n>  (" __FILE__ ":" TSTSSTR(__LINE__) ")\n", m); return false; } } while (0)
#define TEST_ASSERT_R(x, m, r) do { if (!(x)) { printf("> Assert failed: '%s'\n> %s\n>  (" __FILE__ ":" TSTSSTR(__LINE__) ")\n", m, r); return false; } } while (0)
#define TEST_DEFER(x) do { fprintf(logfile, " - %s\n", #x); if (!(x)) { printf("> " #x " failed\n>  (" __FILE__ ":" TSTSSTR(__LINE__) ")\n"); ret = false; } } while (0)

#define DO_TEST(x) do { fprintf(logfile, "-- %s --\n", #x); bool res = (x)(); printf("%-30s ", #x); if (res) { printf("PASS\n"); } else { printf("FAIL\n"); }  } while (0)

bool test_memory_hooks();

bool test_unicode();

bool test_source_fail();
bool test_source_file();
bool test_source_cstringref();
bool test_source_utf8();

bool test_token_empty();
bool test_token_strings();
bool test_token_failpos();
bool test_token_errors();

#endif
