#include "block_list.h"

#include "util/common.h"
#include "file_store.h"

namespace qbot 
{


bool
BlockList::add(int64_t v)
{
	auto it = qff233::FileStore::GetFile<qff233::FileStoreInt64>("BlockList");
	if (!it)
	{
		return false;
	}
	it->addLine(v);
	return true;
}
	
bool 
BlockList::del(int64_t v)
{
	auto it = qff233::FileStore::GetFile<qff233::FileStoreInt64>("BlockList");
	if (!it)
	{
		return false;
	}
	it->delLine(v);
	return true;
}

const std::vector<int64_t>& 
BlockList::get() const
{
	auto fsi = qff233::FileStore::GetFile<qff233::FileStoreInt64>("BlockList");
	if (fsi) {

		return fsi->getContent();
	}
	return { };
}

bool 
BlockList::operator==(int64_t rhs) const
{
	for (const auto& i : this->get()) {
		if (rhs == i) {
			return true;
		}
	}
	return false;
}
	
bool
BlockList::operator!=(int64_t rhs) const
{
	return !(*this == rhs);
}

bool 
operator!=(const int64_t al, const BlockList& v)
{
	return (v != al);
}

bool
operator==(const int64_t al, const BlockList& v)
{
	return (v == al);
}


}