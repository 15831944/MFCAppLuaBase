#ifndef __TH_BLOCKALLOCATOR_H__
#define __TH_BLOCKALLOCATOR_H__

const int g_chunkSize = 16 * 1024;
const int g_maxBlockSize = 640;
const int g_blockSizes = 14;
const int g_chunkArrayIncrement = 128;

struct Block;
struct Chunk;

// 简单内存池
class thAllocator
{
public:
	static thAllocator* getInstance(void);

public:
	static void clear(void);

	static void* allocate(int size);
	static void free(void *p, int size);

	// class
	template<class T>
	static T* allocateType()
	{
		 return new (allocate(sizeof(T))) T();
	}
	template<class T, class Arg>
	static T* allocateType(Arg arg)
	{
		return new(allocate(sizeof(T))) T(arg);
	}
	template<class T>
	static void freeType(T* p)
	{
		free(p, sizeof(T));
	}

protected:
	thAllocator(void);
	~thAllocator(void);

	void* _allocate(int size);
	void _free(void *p, int size);
	void _clear(void);

private:
	static thAllocator* self;
	Chunk*			m_chunks;
	int				m_chunkCount;
	int				m_chunkSpace;
	Block*			m_freeLists[g_blockSizes];
	static int		s_blockSizes[g_blockSizes];
	static char		s_blockSizeLookup[g_maxBlockSize + 1];
	static bool		s_blockSizeLookupInitialized;
};

#endif