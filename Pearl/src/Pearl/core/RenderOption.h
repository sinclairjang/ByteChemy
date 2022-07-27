#pragma once

namespace Pearl 
{
	using Key = std::string;
	using Numeric = float;
	using Characters = std::string;

	class RenderOption
	{
	public:
		void Set(Key key, Numeric value) const;
		void Set(Key key, Characters value) const;

		void QueryOptionValue(Key key, Numeric* value) const;
		void QueryOptionValue(Key key, Characters* value) const;

	private:
		static std::map<Key, Numeric> nBST_;
		static std::map<Key, Characters> chBST_;
	};
}
