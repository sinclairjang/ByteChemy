#include "prlpch.h"
#include "Option.h"

namespace Pearl
{
	Option::Option()
		: nBST_(), chBST_()
	{
	}

	Option::~Option()
	{
	}

	void Option::Set(Key key, Numeric value) const
	{
		nBST_[key] = value;
	}

	void Option::Set(Key key, Characters value) const
	{
		chBST_[key] = value;
	}

	void Option::QueryOptionValue(Key key, Numeric* value) const
	{
		auto q = nBST_.find(key);

		if (q == nBST_.end())
		{
			value = nullptr;
		}
		else
		{
			*value = q->second;
		}
	}

	void Option::QueryOptionValue(Key key, Characters* value) const
	{
		auto q = chBST_.find(key);

		if (q == chBST_.end())
		{
			value = nullptr;
		}
		else
		{
			*value = q->second;
		}
	}
}