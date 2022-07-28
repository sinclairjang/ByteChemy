#pragma once

namespace Pearl 
{
	using Key = std::string;
	using Numeric = float;
	using Characters = std::string;

	class Option
	{
	public:
		Option();
		~Option();

		void Set(Key key, Numeric value) const;
		void Set(Key key, Characters value) const;

		void QueryOptionValue(Key key, Numeric* value) const;
		void QueryOptionValue(Key key, Characters* value) const;

	private:
		mutable std::map<Key, Numeric> nBST_;
		mutable std::map<Key, Characters> chBST_;
	};
}
