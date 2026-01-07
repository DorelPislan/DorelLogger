#ifndef _DOREL_LOGGER_MESSAGE_BUILDER_H_
#define _DOREL_LOGGER_MESSAGE_BUILDER_H_

namespace DorelLogger
{

/**
 * Class used to compose message to be logged from multiple
 * values using << operator
 */
class MessageBuilder : public std::wstring
{
public:
  // class c-tor
  MessageBuilder(size_t aEstimatedMessageLength = 0)
  {
    if (aEstimatedMessageLength > 0)
      this->reserve(aEstimatedMessageLength);
  }

  const wchar_t * data() const { return this->c_str(); }

  MessageBuilder & operator<<(char * aStr)
  {
    return this->operator<<(static_cast<const char *>(aStr));
  }

  MessageBuilder & operator<<(const char * aStr)
  {
    if (aStr)
    {
      std::string_view strView(aStr);

      (*this) << strView;
    }
    else
    {
      (*this).append(L"<nullStr>");
    }
    return (*this);
  }

  /**
   * We also need this overload because std::format() does not want
   * to mix with ascii string
   */
  MessageBuilder & operator<<(const std::string_view & aStrView)
  {
    (*this).append(aStrView.cbegin(), aStrView.cend());
    return (*this);
  }

  MessageBuilder & operator<<(wchar_t * aStr)
  {
    return this->operator<<(static_cast<const wchar_t *>(aStr));
  }

  MessageBuilder & operator<<(const wchar_t * aStr)
  {
    if (aStr)
    {
      (*this).append(aStr);
    }
    else
    {
      (*this).append(L"<nullStr>");
    }
    return (*this);
  }

  /**
   *  This is required because we format in a wstring and std::format does not want
   * to put std::string in a std::wstring
   */
  MessageBuilder & operator<<(const std::string & aStr)
  {
    (*this).append(aStr.begin(), aStr.end());

    return (*this);
  }

  // since we use std::format for conversion the code is the same for every type
  // so its perfect for a template function
  // we can extend the possibilities of this class by providing a custom
  // formatter for any custom type
  template <typename ArgumentType>
  MessageBuilder & operator<<(const ArgumentType & aVal)
  {
    auto res = std::format(L"{}", aVal);
    this->append(res);

    return *this;
  }
};

};  // namespace DorelLogger

#endif  //_DOREL_LOGGER_MESSAGE_BUILDER_H_
