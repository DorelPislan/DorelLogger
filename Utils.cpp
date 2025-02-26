
#include "pch.h"
#include "Utils.h"

/*static*/ bool Utils::IsFlagSet(UINT aValue, UINT aFlag)
{
  return (aValue & aFlag) == aFlag;
}
