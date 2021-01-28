#include "thscript_allocator.h"
#include "thscript_dec.h"

#include <limits.h>
#include <memory.h>
#include <stddef.h>
//#include <malloc.h>
#include <stdlib.h>

thAllocator* thAllocator::self = nullptr;

int thAllocator::s_blockSizes[g_blockSizes] =
{
	16,		// 0
	32,		// 1
	64,		// 2
	96,		// 3
	128,	// 4
	160,	// 5
	192,	// 6
	224,	// 7
	256,	// 8
	320,	// 9
	384,	// 10
	448,	// 11
	512,	// 12
	640,	// 13
};
char thAllocator::s_blockSizeLookup[g_maxBlockSize + 1];
bool thAllocator::s_blockSizeLookupInitialized;

struct Chunk
{
	int blockSize;
	Block *blocks;
};

struct Block
{
	Block *next;
};

thAllocator* thAllocator::getInstance(void)
{
	if (!self) self = new thAllocator();
	return self;
}

void thAllocator::clear(void)
{
	getInstance();
	if (self)
	{
		self->_clear();
	}
}

void* thAllocator::allocate(int size)
{
	getInstance();
	if (self)
	{
		return self->_allocate(size);
	}
	return nullptr;
}

void thAllocator::free(void *p, int size)
{
	getInstance();
	if (self)
	{
		return self->_free(p,size);
	}
}

thAllocator::thAllocator()
{
	__thDF_assert(g_blockSizes < UCHAR_MAX, "thAllocator");

	m_chunkSpace = g_chunkArrayIncrement;
	m_chunkCount = 0;
	m_chunks = (Chunk *)malloc(m_chunkSpace * sizeof(Chunk));

	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));
	memset(m_freeLists, 0, sizeof(m_freeLists));

	if (s_blockSizeLookupInitialized == false)
	{
		int j = 0;
		for (int i = 1; i <= g_maxBlockSize; ++i)
		{
			__thDF_assert(j < g_blockSizes, "thAllocator");
			if (i <= s_blockSizes[j])
			{
				s_blockSizeLookup[i] = (char)j;
			}
			else
			{
				++j;
				s_blockSizeLookup[i] = (char)j;
			}
		}

		s_blockSizeLookupInitialized = true;
	}
}

thAllocator::~thAllocator()
{
	for (int i = 0; i < m_chunkCount; ++i)
	{
		::free(m_chunks[i].blocks);
	}

	::free(m_chunks);
}

void* thAllocator::_allocate(int size)
{
	if (size == 0)
		return nullptr;

	__thDF_assert(0 < size, "thAllocator::_allocate");

	if (size > g_maxBlockSize)
	{
		return malloc(size);
	}

	int index = s_blockSizeLookup[size];
	__thDF_assert(0 <= index && index < g_blockSizes, "thAllocator::_allocate");

	if (m_freeLists[index])
	{
		Block *block = m_freeLists[index];
		m_freeLists[index] = block->next;
		return block;
	}
	else
	{
		if (m_chunkCount == m_chunkSpace)
		{
			Chunk *oldChunks = m_chunks;
			m_chunkSpace += g_chunkArrayIncrement;
			m_chunks = (Chunk *)malloc(m_chunkSpace * sizeof(Chunk));
			memcpy(m_chunks, oldChunks, m_chunkCount * sizeof(Chunk));
			memset(m_chunks + m_chunkCount, 0, g_chunkArrayIncrement * sizeof(Chunk));
			::free(oldChunks);
		}

		Chunk *chunk = m_chunks + m_chunkCount;
		chunk->blocks = (Block *)malloc(g_chunkSize);

		int blockSize = s_blockSizes[index];
		chunk->blockSize = blockSize;
		int blockCount = g_chunkSize / blockSize;
		__thDF_assert(blockCount * blockSize <= g_chunkSize, "thAllocator::_allocate");
		for (int i = 0; i < blockCount - 1; ++i)
		{
			Block *block = (Block *)((char *)chunk->blocks + blockSize * i);
			Block *next = (Block *)((char *)chunk->blocks + blockSize * (i + 1));
			block->next = next;
		}
		Block *last = (Block *)((char *)chunk->blocks + blockSize * (blockCount - 1));
		last->next = nullptr;

		m_freeLists[index] = chunk->blocks->next;
		++m_chunkCount;

		return chunk->blocks;
	}
}

void thAllocator::_free(void *p, int size)
{
	if (size == 0)
	{
		return;
	}

	__thDF_assert(0 < size, "thAllocator::_free");

	if (size > g_maxBlockSize)
	{
		::free(p);
		return;
	}

	int index = s_blockSizeLookup[size];
	__thDF_assert(0 <= index && index < g_blockSizes, "thAllocator::_free");

	Block *block = (Block *)p;
	block->next = m_freeLists[index];
	m_freeLists[index] = block;
}

void thAllocator::_clear(void)
{
	for (int i = 0; i < m_chunkCount; ++i)
	{
		::free(m_chunks[i].blocks);
	}

	m_chunkCount = 0;
	memset(m_chunks, 0, m_chunkSpace * sizeof(Chunk));

	memset(m_freeLists, 0, sizeof(m_freeLists));
}