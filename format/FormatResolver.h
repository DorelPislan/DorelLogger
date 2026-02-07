#ifndef _DOREL_LOGGER_FORMAT_RESOLVER_H_
#define _DOREL_LOGGER_FORMAT_RESOLVER_H_

#include "../logger/ThreadsNames.h"
#include "../sinks/ISink.h"
#include "FormatTraits.h"

namespace DorelLogger
{

class GlobalVariables;

/**
 * Class responsible for combining format of a sink withe the message to log
 * into string that will be sent to sink
 */
class FormatResolver
{
public:
  // class c-tor
  FormatResolver(const GlobalVariables & aGlobalVars,
                 ISink::MessageType      aMessageType,
                 const char *            aSourceFile,
                 const char *            aSourceFunction,
                 size_t                  aSourceLine,
                 std::wstring_view       aMessage);

  ISink::MessageType GetMessageType() const;

  const char * GetSourceFile() const;
  const char * GetSourceFunction() const;
  size_t       GetSourceLine() const;

  const std::wstring_view & GetMessageBody() const;

  // useful for logging statistic data
  void SetMessage(std::wstring_view aMessage);

  void           SetFormat(const Format * aFormat);
  const Format * GetFormat() const;

  std::wstring Resolve(const Format & aFormat);

private:
  const GlobalVariables & mGlobalVars;

  mutable std::optional<std::chrono::time_point<
    std::chrono::local_t,
    std::chrono::duration<std::chrono::system_clock::rep, std::chrono::system_clock::period>>>
    mCachedTime;

  const Format * mFormat{ nullptr };  // when this is set the Sink will use this instead of its own

  ISink::MessageType mMessageType;
  const char *       mSourceFile;
  const char *       mSourceFunction;
  size_t             mSourceLine;
  std::wstring_view  mMessage;

  // cached values

  std::wstring ResolveVar(FormatTraits::VariableId aVarId);

  std::pair<int, int> ComputePadding(int                         aValueLength,
                                     int                         aReqWidth,
                                     FormatTraits::AlignmentType aAlgn);

  const wchar_t * GetMessageTypeString();

  auto GetCurrentTime() const
  {
    if (!mCachedTime)
    {
      mCachedTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
    }

    return *mCachedTime;
  }
};

}  // namespace DorelLogger
#endif  //_DOREL_LOGGER_FORMAT_RESOLVER_H_
