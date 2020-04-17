#pragma once

#include <vector>

#include "util/singleton.h"

namespace qbot 
{

class BlockList
{
public:
	bool add(int64_t v);
	bool del(int64_t v);

	const std::vector<int64_t>& get() const;

	bool operator==(int64_t rhs) const;
	bool operator!=(int64_t rhs) const;
};

bool operator!=(const int64_t al, const BlockList& v);
bool operator==(const int64_t al, const BlockList& v);

typedef qff233::Singleton<BlockList> BlockListMgr;

}