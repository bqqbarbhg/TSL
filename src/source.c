#include "source.h"
#include "memory.h"

#define TSL_FILE_CHUNK_SIZE 256

static void filePtrFree(TSLSource *src)
{
	free(src->buf);
	if (src->state)
		fclose((FILE*)src->state);
}

static char* filePtrGet(void *state, char *buf)
{
	FILE* file = (FILE*)state;
	size_t reads = fread(buf, 1, TSL_FILE_CHUNK_SIZE, file);

	if (reads < TSL_FILE_CHUNK_SIZE)
		buf[reads] = 0;

	return buf + reads;
}


int tslSource_fileOpen(TSLSource *src, const char* filename)
{
	FILE *file = fopen(filename, "rb");
	return tslSource_filePtr(src, file);
}

int tslSource_filePtr(TSLSource *src, FILE* file)
{
	src->buf = 0;
	src->state = 0;
	src->free = filePtrFree;

	// Fail if file is NULL
	if (!file)
		return 1;

	src->buf = (char*)tsl_malloc(TSL_FILE_CHUNK_SIZE);
	src->get = filePtrGet;
	src->state = file;

	src->end = filePtrGet(file, src->buf);
	src->ptr = src->buf;

	return 0;
}

void tslSource_free(TSLSource *src)
{
	src->free(src);
}