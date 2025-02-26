#pragma once

#ifndef _SINK_BASE_H_
#define _SINK_BASE_H_

#include "../format/Format.h"
#include "ISink.h"

namespace DorelLogger
{

class FormatResolver;

class SinkBase : public ISink
{
public:
  // class -ctor
  SinkBase(std::wstring aName);

  // declaration of ISink interface members

  const std::wstring & GetName() override;

  void           SetMessageFormat(const std::wstring & aFormat) override;
  const Format & GetMessageFormat() const override;

  void        SetMinLogLevel(ISink::MessageType aNewMin) override;
  MessageType GetMinLogLevel() const override;
  bool        ShouldLog(MessageType aType) const override;

private:
  std::wstring mName;

  MessageType mMinLogLevel;

  Format mFormat;
};
};  // namespace DorelLogger

#endif  //_SINK_BASE_H_
