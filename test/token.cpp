#include "tests.h"
#include <tsl.h>
#include <vector>

static bool testTokens(const char* src, const std::vector<TSLToken>& target)
{
	char errbuf[64];

	TSLLexer lex;

	lex.errbuf = errbuf;
	lex.errbuf_sz = sizeof(errbuf);

	tslSource_cStringRef(&lex.source, src);

	tslLexer_init(&lex);

	std::vector<TSLToken> got;
	TSLToken tok;

	do
	{
		TEST_ASSERT_R(tslLexer_scan(&lex, &tok) == 0, "Got token", lex.errbuf);
		got.push_back(tok);
	} while (tok.type != TSLTOK_END);

	TEST_ASSERT(got.size() == target.size(), "Scanned correct amount of tokens");

	for (unsigned i = 0; i < got.size(); i++)
	{
		TEST_ASSERT(got[i].type == target[i].type, "Scanned correct tokens");
	}

	tslLexer_free(&lex);

	return true;
}

static TSLToken mtok(TSLTokenType type)
{
	TSLToken t;
	t.type = type;
	return t;
}

bool test_token_basic()
{
	bool ret = true;

	TEST_DEFER(testTokens("", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("  \t\t\r\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("  \t\t\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("#linecomment", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("#comment\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));

	return ret;
}

bool test_token_failpos()
{
	char *src =
		"#testcomment\n"
		"  `\n";

	TSLLexer lex;
	lex.errbuf = 0;
	tslSource_cStringRef(&lex.source, src);
	tslLexer_init(&lex);

	TSLToken tok;

	while (tslLexer_scan(&lex, &tok) == 0)
		;

	TEST_ASSERT(lex.line == 2, "Correct line");
	TEST_ASSERT(lex.col == 3, "Correct column");

	tslLexer_free(&lex);

	return true;
}
