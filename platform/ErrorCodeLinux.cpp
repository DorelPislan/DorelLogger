#include "ErrorCode.h"

#include <system_error>

namespace DorelLogger
{
/*static*/ std::wstring ErrorCode::GetTextForErrorCode(uint32_t aCode)
{
  std::string text = std::generic_category().message(static_cast<int>(aCode));

  if (text.empty())
    return L"<UNKNOWN>";

  return std::wstring(text.begin(), text.end());
}
};  // namespace DorelLogger
