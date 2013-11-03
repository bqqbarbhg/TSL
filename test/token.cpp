#include "tests.h"
#include <tsl.h>
#include <vector>

static void freetok(TSLToken &tok)
{
	if (tok.type == TSLTOK_IDENTIFIER || tok.type == TSLTOK_STRING)
		tsl_free((void*)tok.val.string);
}

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
		if (got[i].type == TSLTOK_IDENTIFIER || got[i].type == TSLTOK_STRING)
		{
			TEST_ASSERT(!strcmp(got[i].val.string, target[i].val.string), "Scanned strings are equal");
		}
	}

	for (auto& tok : got)
		freetok(tok);

	tslLexer_free(&lex);

	return true;
}

static TSLToken mtok(TSLTokenType type, const char* str=nullptr)
{
	TSLToken t;
	t.type = type;
	t.val.string = str;
	return t;
}

bool test_token_empty()
{
	bool ret = true;

	TEST_DEFER(testTokens("", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("  \t\t\r\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("  \t\t\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("#linecomment", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("#comment\n", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));

	return ret;
}

bool test_token_strings()
{
	bool ret = true;

	TEST_DEFER(testTokens("\"asd\"", (std::vector<TSLToken>({ mtok(TSLTOK_STRING, "asd"), mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("\"uni\\u0024\"", (std::vector<TSLToken>({ mtok(TSLTOK_STRING, "uni\x24"), mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("\"uni\\u00a2\"", (std::vector<TSLToken>({ mtok(TSLTOK_STRING, "uni\xC2\xA2"), mtok(TSLTOK_END) }))));
	TEST_DEFER(testTokens("\"uni\\u20Ac\"", (std::vector<TSLToken>({ mtok(TSLTOK_STRING, "uni\xE2\x82\xAC"), mtok(TSLTOK_END) }))));

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

static bool testLexError(const char *src, TSLTokenErrorType errType, int col)
{
	TSLLexer lex;

	lex.errbuf = 0;
	tslSource_cStringRef(&lex.source, src);
	tslLexer_init(&lex);

	TSLToken tok;

	while (tslLexer_scan(&lex, &tok) == 0)
		TEST_ASSERT(tok.type != TSLTOK_END, "Found error");

	TEST_ASSERT(lex.errType == errType, "Correct error type");
	TEST_ASSERT(lex.col == col, "Correct error position");

	tslLexer_free(&lex);

	return true;
}

bool test_token_errors()
{
	bool ret = true;
	TEST_DEFER(testLexError("\"\\u0FXA\"", TSLLEXERR_INVALID_HEX_DIGIT, 6));
	TEST_DEFER(testLexError("\"\\!\"", TSLLEXERR_UNKNOWN_ESCAPE_SEQUENCE, 3));
	TEST_DEFER(testLexError("`", TSLLEXERR_UNEXPECTED_CHARACTER, 1));

	return ret;
}