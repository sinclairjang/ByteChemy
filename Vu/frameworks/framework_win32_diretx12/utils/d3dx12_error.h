#pragma once

// Wrapper for COM excepttions
class com_exception;

// Helper utility converts D3D API failures into exception
void ThrowIfFailed(HRESULT hr) noexcept(false);

template<UINT TNameLength>
void SetDebugObjectName(ID3D12Resource* resource, const char(&name)[TNameLength])
{
#if defined(_DEBUG) || defined(PROFILE)
    HRESULT nameSet = resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
    if (FAILED(nameSet)) { throw std::runtime_error("Failed to set debug name"); }
#endif
}
