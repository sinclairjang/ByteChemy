#pragma once

namespace Pearl
{
	typedef int MouseCode;

	// From imgui.h
	enum MouseCode_
	{
		Left = 0,
		Right = 1,
		Middle = 2,

		// End of list
		Count = 5
	};

	typedef int MouseCursor;

	// From imgui.h
	enum MouseCursor_
	{
		None = -1,
		Arror = 0,
		TextInput,
		ResizeAll,
		ResizeNS,
		ResizeEW,
		ResizeNESW,
		ResizeNWSE,
		Hand,
		NotAllowed,

		// End of list
		Count
	};
}