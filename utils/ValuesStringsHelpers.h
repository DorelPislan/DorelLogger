#ifndef _DOREL_LOGGER_VALUES_STRINGS_HELPERS_H_
#define _DOREL_LOGGER_VALUES_STRINGS_HELPERS_H_

#include "LoggingUtilities.h"

#ifndef VALUE_STRING_PAIR
// creates a structure with 2 members : first is the parameter and the second
//  is a string constant containing the parameter name as received by MACRO
// E.g. VAL_TEXT_PAIR(ERRROR_SUCCES) -> {ERRROR_SUCCES, L"ERRROR_SUCCES"}
#define VALUE_STRING_PAIR(aValue) { static_cast<uint32_t>(aValue), L#aValue }
#endif

#define VALUES_STRINGS_ARRAY_HEADER(aArrayName, aValuesCount) \
  std::array<DorelLogger::LoggingUtilities::ValueStringPair, aValuesCount> aArrayName = { {
#define VALUES_STRINGS_ARRAY_FOOTER \
  }                                 \
  }                                 \
  ;
#define VALUES_STRINGS_ARRAY_1_ELEMENTS(aValue) VALUE_STRING_PAIR(aValue),

#define VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue1, aValue2) \
  VALUE_STRING_PAIR(aValue1), VALUE_STRING_PAIR(aValue2),

#define VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3) \
  VALUE_STRING_PAIR(aValue1), VALUE_STRING_PAIR(aValue2), VALUE_STRING_PAIR(aValue3),

#define DEFINE_VALUES_STRINGS_ARRAY_1(aArrayName, aValue1) \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 1)               \
  VALUES_STRINGS_ARRAY_1_ELEMENTS(aValue1)                 \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_2(aArrayName, aValue1, aValue2) \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 2)                        \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue1, aValue2)                 \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_3(aArrayName, aValue1, aValue2, aValue3) \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 3)                                 \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                 \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_4(aArrayName, aValue1, aValue2, aValue3, aValue4) \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 4)                                          \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue1, aValue2)                                   \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue3, aValue4)                                   \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_5(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5) \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 5)                                                   \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue1, aValue2)                                            \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue3, aValue4, aValue5)                                   \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_6(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                      aValue6)                                                 \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 6)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                   \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_7(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                      aValue6, aValue7)                                        \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 7)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                   \
  VALUES_STRINGS_ARRAY_1_ELEMENTS(aValue7)                                                     \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_8(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                      aValue6, aValue7, aValue8)                               \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 8)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                   \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue7, aValue8)                                            \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_9(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                      aValue6, aValue7, aValue8, aValu9)                       \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 9)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                   \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_10(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValu9, aValue10)             \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 10)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_1_ELEMENTS(aValue10)                                                     \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_11(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValu9, aValue10, aValue11)   \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 11)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue10, aValue11)                                           \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_12(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValu9, aValue10, aValue11,   \
                                       aValue12)                                                \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 12)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue10, aValue11, aValue12)                                 \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_13(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValu9, aValue10, aValue11,   \
                                       aValue12, aValue13)                                      \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 13)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue10, aValue11, aValue12)                                 \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue13)                                                     \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_14(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValue9, aValue10, aValue11,  \
                                       aValue12, aValue13, aValue14)                            \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 14)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue10, aValue11, aValue12)                                 \
  VALUES_STRINGS_ARRAY_2_ELEMENTS(aValue13, aValue14)                                           \
  VALUES_STRINGS_ARRAY_FOOTER

#define DEFINE_VALUES_STRINGS_ARRAY_15(aArrayName, aValue1, aValue2, aValue3, aValue4, aValue5, \
                                       aValue6, aValue7, aValue8, aValue9, aValue10, aValue11,  \
                                       aValue12, aValue13, aValue14, aValue15)                  \
  VALUES_STRINGS_ARRAY_HEADER(aArrayName, 15)                                                   \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue1, aValue2, aValue3)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue4, aValue5, aValue6)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue7, aValue8, aValue9)                                    \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue10, aValue11, aValue12)                                 \
  VALUES_STRINGS_ARRAY_3_ELEMENTS(aValue13, aValue14, aValue15)                                 \
  VALUES_STRINGS_ARRAY_FOOTER

#endif  // _DOREL_LOGGER_VALUES_STRINGS_HELPERS_H_
