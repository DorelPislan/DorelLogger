#ifndef _FORMAT_RESOLVER_H_
#define _FORMAT_RESOLVER_H_

#include "FormatTraits.h"

namespace DorelLogger
{

class ThreadNames;

/**
 * Class responsible for combining format of a sink withe the message to log
 * into string that will be sent to sink
 */
class FormatResolver
{
public:
  // class c-tor
  FormatResolver(DWORD                aCrtProcessId,
                 const std::wstring & aCrtProcessName,
                 const ThreadsNames & aThreadsNames,
                 ISink::MessageType   aMessageType,
                 const char *         aSourceFile,
                 const char *         aSourceFunction,
                 size_t               aSourceLine,
                 std::wstring_view    aMessage);

  ISink::MessageType        GetMessageType() const;
  const std::wstring_view & GetMessageBody() const;

  std::wstring Resolve(const Format & aFormat);

private:
  DWORD                mCrtProcessId;
  const std::wstring & mCrtProcessName;
  const ThreadsNames & mThreadsNames;

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
};

}  // namespace DorelLogger
#endif  //_LOGGER_H_
