#pragma once

#include "structs.h"

tagBufferData ConvertToNetworkBuffer(tagData& stData);
tagData ConvertToDataStruct(tagBufferData& stData);
