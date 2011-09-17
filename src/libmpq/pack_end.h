#ifdef _PACK_BEGIN
#undef _PACK_BEGIN
#else
#error "pack_begin.h must be includede before pack_end.h"
#endif

#ifdef _MSC_VER
  #pragma pack(pop)
#endif

#undef PACK_STRUCT