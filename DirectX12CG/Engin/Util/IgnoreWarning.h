#pragma once

#define PragmaFunc(x) __pragma(x)

#define PragmaPush PragmaFunc(warning(push))

#define PragmaPop PragmaFunc(warning(pop))

#define PragmaWarningNum(x) PragmaFunc(warning(disable:x))

#define IgnoreWarnings \
PragmaWarningNum(4061) \
PragmaWarningNum(4062) \
PragmaWarningNum(4127) \
PragmaWarningNum(4244) \
PragmaWarningNum(4263) \
PragmaWarningNum(4264) \
PragmaWarningNum(4265) \
PragmaWarningNum(4266) \
PragmaWarningNum(4355) \
PragmaWarningNum(4365) \
PragmaWarningNum(4514) \
PragmaWarningNum(4619) \
PragmaWarningNum(4623) \
PragmaWarningNum(4625) \
PragmaWarningNum(4626) \
PragmaWarningNum(4668) \
PragmaWarningNum(4702) \
PragmaWarningNum(4710) \
PragmaWarningNum(4711) \
PragmaWarningNum(4777) \
PragmaWarningNum(4820) \
PragmaWarningNum(5026) \
PragmaWarningNum(5027) \
PragmaWarningNum(5039) \
PragmaWarningNum(5204) \
PragmaWarningNum(5219) \
PragmaWarningNum(5220) \
PragmaWarningNum(5246) \
PragmaWarningNum(5266) \
PragmaWarningNum(5267) 

#define WarningIgnoreBegin \
PragmaPush \
IgnoreWarnings

#define WarningIgnoreEnd \
PragmaPop