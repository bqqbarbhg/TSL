#include "tests.h"
#include <tsl.h>
#include <vector>

static bool testTokens(const char* src, const std::vector<TSLToken>& target)
{
	char errbuf[64];

	TSLTokenStream stream;

	stream.errbuf = errbuf;
	stream.errbuf_sz = sizeof(errbuf);

	tslSource_cStringRef(&stream.source, src);

	std::vector<TSLToken> got;
	TSLToken tok;

	do
	{
		TEST_ASSERT(tslToken_get(&stream, &tok) == 0, "Got token");
		got.push_back(tok);
	} while (tok.type != TSLTOK_END);

	TEST_ASSERT(got.size() == target.size(), "Scanned correct amount of tokens");

	for (unsigned i = 0; i < got.size(); i++)
	{
		TEST_ASSERT(got[i].type == target[i].type, "Scanned correct tokens");
	}
}

static TSLToken mtok(TSLTokenType type)
{
	TSLToken t;
	t.type = type;
	return t;
}

bool test_token_basic()
{
	TEST_DEFER(testTokens("", (std::vector<TSLToken>({ mtok(TSLTOK_END) }))));

	return true;
}