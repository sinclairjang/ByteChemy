#ifndef _PRL_INTERNAL_H
#define _PRL_INTERNAL_H

#include <exception>
#include <stdio.h>
#include <stdexcept>

// This is extracted from DirectXTK source.
namespace DirectX12
{
	// Helper class for COM excepttions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) noexcept : result(hr) {}

		const char* what() const noexcept override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

		HRESULT get_result() const noexcept { return result; }

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr) noexcept(false)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}

	template<UINT TNameLength>
	inline void SetDebugObjectName(_In_ ID3D12Resource* resource, _In_z_ const char(&name)[TNameLength])
	{
		#if defined(_DEBUG) || defined(PROFILE)
			HRESULT nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
			if (FAILED(nameSet)) { throw std::runtime_error("Failed to set debug name"); }
		#endif
	}
}
#endif // _PRL_INTERNAL_H