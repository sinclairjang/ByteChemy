#include "fm_pch.h"
#include "DX12App_ErrorHandler.h"

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

void ThrowIfFailed(HRESULT hr) noexcept(false)
{
    if (FAILED(hr))
    {
        throw com_exception(hr);
    }
}

