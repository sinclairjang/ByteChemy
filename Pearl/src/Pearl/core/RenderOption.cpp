#include "prlpch.h"
#include "RenderOption.h"

namespace Pearl
{
	void RenderOption::Set(Key key, Numeric value) const
	{
		nBST_[key] = value;
	}

	void RenderOption::Set(Key key, Characters value) const
	{
		chBST_[key] = value;
	}

	void RenderOption::QueryOptionValue(Key key, Numeric* value) const
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

	void RenderOption::QueryOptionValue(Key key, Characters* value) const
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