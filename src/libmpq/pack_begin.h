#ifndef _PACK_BEGIN
#define _PACK_BEGIN
#else
#error "pack_begin.h may not be included twice!"
#endif

#ifdef _MSC_VER
  #pragma pack(push,1)
  #define PACK_STRUCT
#else
  /* we assume GNU here */
  #define PACK_STRUCT __attribute__((packed))
#endif