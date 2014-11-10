# 1 "sha1.c"
# 1 "<command-line>"
# 1 "sha1.c"
# 10 "sha1.c"
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 1 3
# 9 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\crtdefs.h" 1 3
# 10 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\crtdefs.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 1 3
# 12 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw_mac.h" 1 3
# 46 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw_mac.h" 3
             
# 55 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw_mac.h" 3
             
# 13 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 2 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw_secapi.h" 1 3
# 14 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 2 3
# 282 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\vadefs.h" 1 3
# 9 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\vadefs.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 1 3
# 686 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\sdks/_mingw_directx.h" 1 3
# 687 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 2 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\sdks/_mingw_ddk.h" 1 3
# 688 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 2 3
# 10 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\vadefs.h" 2 3


#pragma pack(push,_CRT_PACKING)
# 22 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\vadefs.h" 3
  typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 101 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\vadefs.h" 3
#pragma pack(pop)
# 283 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 2 3


#pragma pack(push,_CRT_PACKING)
# 377 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
__extension__ typedef unsigned long long size_t;
# 387 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
__extension__ typedef long long ssize_t;
# 399 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
__extension__ typedef long long intptr_t;
# 412 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
__extension__ typedef unsigned long long uintptr_t;
# 425 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
__extension__ typedef long long ptrdiff_t;
# 435 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;
# 463 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;







typedef __time64_t time_t;
# 656 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\_mingw.h" 3
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{
  __asm__ __volatile__("int {$}3":);
}




const char *__mingw_get_crt_info (void);






#pragma pack(pop)
# 11 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\crtdefs.h" 2 3
# 26 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\crtdefs.h" 3
typedef size_t rsize_t;
# 153 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\crtdefs.h" 3
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {
  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;
} threadlocinfo;
# 10 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 2 3
# 36 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 3
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _memccpy(void *_Dst,const void *_Src,int _Val,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memchr(const void *_Buf ,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _memicmp_l(const void *_Buf1,const void *_Buf2,size_t _Size,_locale_t _Locale);
  int __attribute__((__cdecl__)) memcmp(const void *_Buf1,const void *_Buf2,size_t _Size);
  void * __attribute__((__cdecl__)) memcpy(void * __restrict__ _Dst,const void * __restrict__ _Src,size_t _Size) ;
  void * __attribute__((__cdecl__)) mempcpy (void *_Dst, const void *_Src, size_t _Size);
  void * __attribute__((__cdecl__)) memset(void *_Dst,int _Val,size_t _Size);

  void * __attribute__((__cdecl__)) memccpy(void *_Dst,const void *_Src,int _Val,size_t _Size) ;
  int __attribute__((__cdecl__)) memicmp(const void *_Buf1,const void *_Buf2,size_t _Size) ;


  char * __attribute__((__cdecl__)) _strset(char *_Str,int _Val) ;
  char * __attribute__((__cdecl__)) _strset_l(char *_Str,int _Val,_locale_t _Locale) ;
  char * __attribute__((__cdecl__)) strcpy(char * __restrict__ _Dest,const char * __restrict__ _Source);
  char * __attribute__((__cdecl__)) strcat(char * __restrict__ _Dest,const char * __restrict__ _Source);
  int __attribute__((__cdecl__)) strcmp(const char *_Str1,const char *_Str2);
  size_t __attribute__((__cdecl__)) strlen(const char *_Str);
  size_t __attribute__((__cdecl__)) strnlen(const char *_Str,size_t _MaxCount);
  void *__attribute__((__cdecl__)) memmove(void *_Dst,const void *_Src,size_t _Size) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strdup(const char *_Src);
  char *__attribute__((__cdecl__)) strchr(const char *_Str,int _Val);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcmpi(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricmp_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  int __attribute__((__cdecl__)) strcoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strcoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll(const char *_Str1,const char *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _stricoll_l(const char *_Str1,const char *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strncoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll (const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicoll_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  size_t __attribute__((__cdecl__)) strcspn(const char *_Str,const char *_Control);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strerror(const char *_ErrMsg) ;
  char *__attribute__((__cdecl__)) strerror(int) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strlwr(char *_String) ;
  char *strlwr_l(char *_String,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strncat(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) strncmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp(const char *_Str1,const char *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _strnicmp_l(const char *_Str1,const char *_Str2,size_t _MaxCount,_locale_t _Locale);
  char *strncpy(char * __restrict__ _Dest,const char * __restrict__ _Source,size_t _Count) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset(char *_Str,int _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strnset_l(char *str,int c,size_t count,_locale_t _Locale) ;
  char *__attribute__((__cdecl__)) strpbrk(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strrchr(const char *_Str,int _Ch);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strrev(char *_Str);
  size_t __attribute__((__cdecl__)) strspn(const char *_Str,const char *_Control);
  char *__attribute__((__cdecl__)) strstr(const char *_Str,const char *_SubStr);
  char *__attribute__((__cdecl__)) strtok(char * __restrict__ _Str,const char * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strupr(char *_String) ;
  __attribute__ ((__dllimport__)) char *_strupr_l(char *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) strxfrm(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _strxfrm_l(char * __restrict__ _Dst,const char * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);


  char *__attribute__((__cdecl__)) strdup(const char *_Src) ;
  int __attribute__((__cdecl__)) strcmpi(const char *_Str1,const char *_Str2) ;
  int __attribute__((__cdecl__)) stricmp(const char *_Str1,const char *_Str2) ;
  char *__attribute__((__cdecl__)) strlwr(char *_Str) ;
  int __attribute__((__cdecl__)) strnicmp(const char *_Str1,const char *_Str,size_t _MaxCount) ;
  int __attribute__((__cdecl__)) strncasecmp (const char *, const char *, size_t);
  int __attribute__((__cdecl__)) strcasecmp (const char *, const char *);







  char *__attribute__((__cdecl__)) strnset(char *_Str,int _Val,size_t _MaxCount) ;
  char *__attribute__((__cdecl__)) strrev(char *_Str) ;
  char *__attribute__((__cdecl__)) strset(char *_Str,int _Val) ;
  char *__attribute__((__cdecl__)) strupr(char *_Str) ;





  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsdup(const wchar_t *_Str);
  wchar_t *__attribute__((__cdecl__)) wcscat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  wchar_t *__attribute__((__cdecl__)) wcschr(const wchar_t *_Str,wchar_t _Ch);
  int __attribute__((__cdecl__)) wcscmp(const wchar_t *_Str1,const wchar_t *_Str2);
  wchar_t *__attribute__((__cdecl__)) wcscpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source) ;
  size_t __attribute__((__cdecl__)) wcscspn(const wchar_t *_Str,const wchar_t *_Control);
  size_t __attribute__((__cdecl__)) wcslen(const wchar_t *_Str);
  size_t __attribute__((__cdecl__)) wcsnlen(const wchar_t *_Src,size_t _MaxCount);
  wchar_t *wcsncat(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  int __attribute__((__cdecl__)) wcsncmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  wchar_t *wcsncpy(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count) ;
  wchar_t *__attribute__((__cdecl__)) _wcsncpy_l(wchar_t * __restrict__ _Dest,const wchar_t * __restrict__ _Source,size_t _Count,_locale_t _Locale) ;
  wchar_t *__attribute__((__cdecl__)) wcspbrk(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsrchr(const wchar_t *_Str,wchar_t _Ch);
  size_t __attribute__((__cdecl__)) wcsspn(const wchar_t *_Str,const wchar_t *_Control);
  wchar_t *__attribute__((__cdecl__)) wcsstr(const wchar_t *_Str,const wchar_t *_SubStr);
  wchar_t *__attribute__((__cdecl__)) wcstok(wchar_t * __restrict__ _Str,const wchar_t * __restrict__ _Delim) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcserror(int _ErrNum) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) __wcserror(const wchar_t *_Str) ;
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicmp_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsrev(wchar_t *_Str);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsset(wchar_t *_Str,wchar_t _Val) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcslwr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcslwr_l(wchar_t *_String,_locale_t _Locale) ;
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcsupr(wchar_t *_String) ;
  __attribute__ ((__dllimport__)) wchar_t *_wcsupr_l(wchar_t *_String,_locale_t _Locale) ;
  size_t __attribute__((__cdecl__)) wcsxfrm(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _wcsxfrm_l(wchar_t * __restrict__ _Dst,const wchar_t * __restrict__ _Src,size_t _MaxCount,_locale_t _Locale);
  int __attribute__((__cdecl__)) wcscoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcscoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsncoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _wcsnicoll_l(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount,_locale_t _Locale);


  wchar_t *__attribute__((__cdecl__)) wcsdup(const wchar_t *_Str) ;

  int __attribute__((__cdecl__)) wcsicmp(const wchar_t *_Str1,const wchar_t *_Str2) ;
  int __attribute__((__cdecl__)) wcsnicmp(const wchar_t *_Str1,const wchar_t *_Str2,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsnset(wchar_t *_Str,wchar_t _Val,size_t _MaxCount) ;
  wchar_t *__attribute__((__cdecl__)) wcsrev(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsset(wchar_t *_Str,wchar_t _Val) ;
  wchar_t *__attribute__((__cdecl__)) wcslwr(wchar_t *_Str) ;
  wchar_t *__attribute__((__cdecl__)) wcsupr(wchar_t *_Str) ;
  int __attribute__((__cdecl__)) wcsicoll(const wchar_t *_Str1,const wchar_t *_Str2) ;







# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\sec_api\\string_s.h" 1 3
# 9 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\sec_api\\string_s.h" 3
# 1 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 1 3
# 10 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\sec_api\\string_s.h" 2 3







  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strset_s(char *_Dst,size_t _DstSize,int _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strerror_s(char *_Buf,size_t _SizeInBytes,const char *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strerror_s(char *_Buf,size_t _SizeInBytes,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strlwr_s_l(char *_Str,size_t _Size,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strnset_s(char *_Str,size_t _Size,int _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s(char *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strupr_s_l(char *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncat_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncat_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcpy_s(char * _Dst, size_t _SizeInBytes, const char * _Src);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strncpy_s(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _strncpy_s_l(char *_Dst,size_t _DstSizeInChars,const char *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) strtok_s(char *_Str,const char *_Delim,char **_Context);
  __attribute__ ((__dllimport__)) char *__attribute__((__cdecl__)) _strtok_s_l(char *_Str,const char *_Delim,char **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) strcat_s(char * strDestination, size_t numberOfElements, const char * strSource);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) memmove_s(void *_dest,size_t _numberOfElements,const void *_src,size_t _count);


  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) wcstok_s(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcserror_s(wchar_t *_Buf,size_t _SizeInWords,int _ErrNum);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) __wcserror_s(wchar_t *_Buffer,size_t _SizeInWords,const wchar_t *_ErrMsg);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s(wchar_t *_Dst,size_t _DstSizeInWords,wchar_t _Val,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s(wchar_t *_Str,size_t _SizeInWords,wchar_t _Val);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s(wchar_t *_Str,size_t _SizeInWords);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcslwr_s_l(wchar_t *_Str,size_t _SizeInWords,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s(wchar_t *_Str,size_t _Size);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsupr_s_l(wchar_t *_Str,size_t _Size,_locale_t _Locale);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscpy_s(wchar_t * _Dst, size_t _nElem, const wchar_t * _Src);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcscat_s(wchar_t * _Dst, size_t _nElem, const wchar_t * _Src);

  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncat_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncat_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) wcsncpy_s(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsncpy_s_l(wchar_t *_Dst,size_t _DstSizeInChars,const wchar_t *_Src,size_t _MaxCount,_locale_t _Locale);
  __attribute__ ((__dllimport__)) wchar_t *__attribute__((__cdecl__)) _wcstok_s_l(wchar_t *_Str,const wchar_t *_Delim,wchar_t **_Context,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val,_locale_t _Locale);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _wcsnset_s_l(wchar_t *_Str,size_t _SizeInChars,unsigned int _Val, size_t _Count,_locale_t _Locale);





  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) memcpy_s (void *_dest,size_t _numberOfElements,const void *_src,size_t _count);
# 177 "e:\\seth-ib\\mingw\\mingw64\\x86_64-w64-mingw32\\include\\string.h" 2 3
# 11 "sha1.c" 2


# 1 "sha1.h" 1
# 9 "sha1.h"
typedef struct {
 unsigned long long size;
 unsigned int H[5];
 unsigned int W[16];
} blk_SHA_CTX;

void blk_SHA1_Init(blk_SHA_CTX *ctx);
void blk_SHA1_Update(blk_SHA_CTX *ctx, const void *dataIn, unsigned long len);
void blk_SHA1_Final(unsigned char hashout[20], blk_SHA_CTX *ctx);
# 14 "sha1.c" 2
# 119 "sha1.c"
static void blk_SHA1_Block(blk_SHA_CTX *ctx, const unsigned int *data)
{
 unsigned int A,B,C,D,E;
 unsigned int array[16];

 A = ctx->H[0];
 B = ctx->H[1];
 C = ctx->H[2];
 D = ctx->H[3];
 E = ctx->H[4];


 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 0)); (*(volatile unsigned int *)&(array[(0)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((((C^D)&B)^D)) + (0x5a827999); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 1)); (*(volatile unsigned int *)&(array[(1)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((((B^C)&A)^C)) + (0x5a827999); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 2)); (*(volatile unsigned int *)&(array[(2)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((((A^B)&E)^B)) + (0x5a827999); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 3)); (*(volatile unsigned int *)&(array[(3)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((((E^A)&D)^A)) + (0x5a827999); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 4)); (*(volatile unsigned int *)&(array[(4)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((((D^E)&C)^E)) + (0x5a827999); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 5)); (*(volatile unsigned int *)&(array[(5)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((((C^D)&B)^D)) + (0x5a827999); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 6)); (*(volatile unsigned int *)&(array[(6)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((((B^C)&A)^C)) + (0x5a827999); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 7)); (*(volatile unsigned int *)&(array[(7)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((((A^B)&E)^B)) + (0x5a827999); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 8)); (*(volatile unsigned int *)&(array[(8)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((((E^A)&D)^A)) + (0x5a827999); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 9)); (*(volatile unsigned int *)&(array[(9)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((((D^E)&C)^E)) + (0x5a827999); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 10)); (*(volatile unsigned int *)&(array[(10)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((((C^D)&B)^D)) + (0x5a827999); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 11)); (*(volatile unsigned int *)&(array[(11)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((((B^C)&A)^C)) + (0x5a827999); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 12)); (*(volatile unsigned int *)&(array[(12)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((((A^B)&E)^B)) + (0x5a827999); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 13)); (*(volatile unsigned int *)&(array[(13)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((((E^A)&D)^A)) + (0x5a827999); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 14)); (*(volatile unsigned int *)&(array[(14)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((((D^E)&C)^E)) + (0x5a827999); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ntohl(*(unsigned int *)(data + 15)); (*(volatile unsigned int *)&(array[(15)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((((C^D)&B)^D)) + (0x5a827999); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);


 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(16 +13)&15]) ^ (array[(16 +8)&15]) ^ (array[(16 +2)&15]) ^ (array[(16)&15]))); __res; }); (*(volatile unsigned int *)&(array[(16)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((((B^C)&A)^C)) + (0x5a827999); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(17 +13)&15]) ^ (array[(17 +8)&15]) ^ (array[(17 +2)&15]) ^ (array[(17)&15]))); __res; }); (*(volatile unsigned int *)&(array[(17)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((((A^B)&E)^B)) + (0x5a827999); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(18 +13)&15]) ^ (array[(18 +8)&15]) ^ (array[(18 +2)&15]) ^ (array[(18)&15]))); __res; }); (*(volatile unsigned int *)&(array[(18)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((((E^A)&D)^A)) + (0x5a827999); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(19 +13)&15]) ^ (array[(19 +8)&15]) ^ (array[(19 +2)&15]) ^ (array[(19)&15]))); __res; }); (*(volatile unsigned int *)&(array[(19)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((((D^E)&C)^E)) + (0x5a827999); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);


 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(20 +13)&15]) ^ (array[(20 +8)&15]) ^ (array[(20 +2)&15]) ^ (array[(20)&15]))); __res; }); (*(volatile unsigned int *)&(array[(20)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0x6ed9eba1); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(21 +13)&15]) ^ (array[(21 +8)&15]) ^ (array[(21 +2)&15]) ^ (array[(21)&15]))); __res; }); (*(volatile unsigned int *)&(array[(21)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0x6ed9eba1); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(22 +13)&15]) ^ (array[(22 +8)&15]) ^ (array[(22 +2)&15]) ^ (array[(22)&15]))); __res; }); (*(volatile unsigned int *)&(array[(22)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0x6ed9eba1); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(23 +13)&15]) ^ (array[(23 +8)&15]) ^ (array[(23 +2)&15]) ^ (array[(23)&15]))); __res; }); (*(volatile unsigned int *)&(array[(23)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0x6ed9eba1); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(24 +13)&15]) ^ (array[(24 +8)&15]) ^ (array[(24 +2)&15]) ^ (array[(24)&15]))); __res; }); (*(volatile unsigned int *)&(array[(24)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0x6ed9eba1); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(25 +13)&15]) ^ (array[(25 +8)&15]) ^ (array[(25 +2)&15]) ^ (array[(25)&15]))); __res; }); (*(volatile unsigned int *)&(array[(25)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0x6ed9eba1); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(26 +13)&15]) ^ (array[(26 +8)&15]) ^ (array[(26 +2)&15]) ^ (array[(26)&15]))); __res; }); (*(volatile unsigned int *)&(array[(26)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0x6ed9eba1); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(27 +13)&15]) ^ (array[(27 +8)&15]) ^ (array[(27 +2)&15]) ^ (array[(27)&15]))); __res; }); (*(volatile unsigned int *)&(array[(27)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0x6ed9eba1); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(28 +13)&15]) ^ (array[(28 +8)&15]) ^ (array[(28 +2)&15]) ^ (array[(28)&15]))); __res; }); (*(volatile unsigned int *)&(array[(28)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0x6ed9eba1); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(29 +13)&15]) ^ (array[(29 +8)&15]) ^ (array[(29 +2)&15]) ^ (array[(29)&15]))); __res; }); (*(volatile unsigned int *)&(array[(29)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0x6ed9eba1); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(30 +13)&15]) ^ (array[(30 +8)&15]) ^ (array[(30 +2)&15]) ^ (array[(30)&15]))); __res; }); (*(volatile unsigned int *)&(array[(30)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0x6ed9eba1); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(31 +13)&15]) ^ (array[(31 +8)&15]) ^ (array[(31 +2)&15]) ^ (array[(31)&15]))); __res; }); (*(volatile unsigned int *)&(array[(31)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0x6ed9eba1); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(32 +13)&15]) ^ (array[(32 +8)&15]) ^ (array[(32 +2)&15]) ^ (array[(32)&15]))); __res; }); (*(volatile unsigned int *)&(array[(32)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0x6ed9eba1); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(33 +13)&15]) ^ (array[(33 +8)&15]) ^ (array[(33 +2)&15]) ^ (array[(33)&15]))); __res; }); (*(volatile unsigned int *)&(array[(33)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0x6ed9eba1); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(34 +13)&15]) ^ (array[(34 +8)&15]) ^ (array[(34 +2)&15]) ^ (array[(34)&15]))); __res; }); (*(volatile unsigned int *)&(array[(34)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0x6ed9eba1); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(35 +13)&15]) ^ (array[(35 +8)&15]) ^ (array[(35 +2)&15]) ^ (array[(35)&15]))); __res; }); (*(volatile unsigned int *)&(array[(35)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0x6ed9eba1); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(36 +13)&15]) ^ (array[(36 +8)&15]) ^ (array[(36 +2)&15]) ^ (array[(36)&15]))); __res; }); (*(volatile unsigned int *)&(array[(36)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0x6ed9eba1); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(37 +13)&15]) ^ (array[(37 +8)&15]) ^ (array[(37 +2)&15]) ^ (array[(37)&15]))); __res; }); (*(volatile unsigned int *)&(array[(37)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0x6ed9eba1); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(38 +13)&15]) ^ (array[(38 +8)&15]) ^ (array[(38 +2)&15]) ^ (array[(38)&15]))); __res; }); (*(volatile unsigned int *)&(array[(38)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0x6ed9eba1); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(39 +13)&15]) ^ (array[(39 +8)&15]) ^ (array[(39 +2)&15]) ^ (array[(39)&15]))); __res; }); (*(volatile unsigned int *)&(array[(39)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0x6ed9eba1); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);


 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(40 +13)&15]) ^ (array[(40 +8)&15]) ^ (array[(40 +2)&15]) ^ (array[(40)&15]))); __res; }); (*(volatile unsigned int *)&(array[(40)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(41 +13)&15]) ^ (array[(41 +8)&15]) ^ (array[(41 +2)&15]) ^ (array[(41)&15]))); __res; }); (*(volatile unsigned int *)&(array[(41)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(42 +13)&15]) ^ (array[(42 +8)&15]) ^ (array[(42 +2)&15]) ^ (array[(42)&15]))); __res; }); (*(volatile unsigned int *)&(array[(42)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(43 +13)&15]) ^ (array[(43 +8)&15]) ^ (array[(43 +2)&15]) ^ (array[(43)&15]))); __res; }); (*(volatile unsigned int *)&(array[(43)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(44 +13)&15]) ^ (array[(44 +8)&15]) ^ (array[(44 +2)&15]) ^ (array[(44)&15]))); __res; }); (*(volatile unsigned int *)&(array[(44)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(45 +13)&15]) ^ (array[(45 +8)&15]) ^ (array[(45 +2)&15]) ^ (array[(45)&15]))); __res; }); (*(volatile unsigned int *)&(array[(45)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(46 +13)&15]) ^ (array[(46 +8)&15]) ^ (array[(46 +2)&15]) ^ (array[(46)&15]))); __res; }); (*(volatile unsigned int *)&(array[(46)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(47 +13)&15]) ^ (array[(47 +8)&15]) ^ (array[(47 +2)&15]) ^ (array[(47)&15]))); __res; }); (*(volatile unsigned int *)&(array[(47)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(48 +13)&15]) ^ (array[(48 +8)&15]) ^ (array[(48 +2)&15]) ^ (array[(48)&15]))); __res; }); (*(volatile unsigned int *)&(array[(48)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(49 +13)&15]) ^ (array[(49 +8)&15]) ^ (array[(49 +2)&15]) ^ (array[(49)&15]))); __res; }); (*(volatile unsigned int *)&(array[(49)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(50 +13)&15]) ^ (array[(50 +8)&15]) ^ (array[(50 +2)&15]) ^ (array[(50)&15]))); __res; }); (*(volatile unsigned int *)&(array[(50)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(51 +13)&15]) ^ (array[(51 +8)&15]) ^ (array[(51 +2)&15]) ^ (array[(51)&15]))); __res; }); (*(volatile unsigned int *)&(array[(51)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(52 +13)&15]) ^ (array[(52 +8)&15]) ^ (array[(52 +2)&15]) ^ (array[(52)&15]))); __res; }); (*(volatile unsigned int *)&(array[(52)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(53 +13)&15]) ^ (array[(53 +8)&15]) ^ (array[(53 +2)&15]) ^ (array[(53)&15]))); __res; }); (*(volatile unsigned int *)&(array[(53)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(54 +13)&15]) ^ (array[(54 +8)&15]) ^ (array[(54 +2)&15]) ^ (array[(54)&15]))); __res; }); (*(volatile unsigned int *)&(array[(54)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(55 +13)&15]) ^ (array[(55 +8)&15]) ^ (array[(55 +2)&15]) ^ (array[(55)&15]))); __res; }); (*(volatile unsigned int *)&(array[(55)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(56 +13)&15]) ^ (array[(56 +8)&15]) ^ (array[(56 +2)&15]) ^ (array[(56)&15]))); __res; }); (*(volatile unsigned int *)&(array[(56)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(57 +13)&15]) ^ (array[(57 +8)&15]) ^ (array[(57 +2)&15]) ^ (array[(57)&15]))); __res; }); (*(volatile unsigned int *)&(array[(57)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(58 +13)&15]) ^ (array[(58 +8)&15]) ^ (array[(58 +2)&15]) ^ (array[(58)&15]))); __res; }); (*(volatile unsigned int *)&(array[(58)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(59 +13)&15]) ^ (array[(59 +8)&15]) ^ (array[(59 +2)&15]) ^ (array[(59)&15]))); __res; }); (*(volatile unsigned int *)&(array[(59)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);


 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(60 +13)&15]) ^ (array[(60 +8)&15]) ^ (array[(60 +2)&15]) ^ (array[(60)&15]))); __res; }); (*(volatile unsigned int *)&(array[(60)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0xca62c1d6); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(61 +13)&15]) ^ (array[(61 +8)&15]) ^ (array[(61 +2)&15]) ^ (array[(61)&15]))); __res; }); (*(volatile unsigned int *)&(array[(61)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0xca62c1d6); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(62 +13)&15]) ^ (array[(62 +8)&15]) ^ (array[(62 +2)&15]) ^ (array[(62)&15]))); __res; }); (*(volatile unsigned int *)&(array[(62)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0xca62c1d6); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(63 +13)&15]) ^ (array[(63 +8)&15]) ^ (array[(63 +2)&15]) ^ (array[(63)&15]))); __res; }); (*(volatile unsigned int *)&(array[(63)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0xca62c1d6); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(64 +13)&15]) ^ (array[(64 +8)&15]) ^ (array[(64 +2)&15]) ^ (array[(64)&15]))); __res; }); (*(volatile unsigned int *)&(array[(64)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0xca62c1d6); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(65 +13)&15]) ^ (array[(65 +8)&15]) ^ (array[(65 +2)&15]) ^ (array[(65)&15]))); __res; }); (*(volatile unsigned int *)&(array[(65)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0xca62c1d6); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(66 +13)&15]) ^ (array[(66 +8)&15]) ^ (array[(66 +2)&15]) ^ (array[(66)&15]))); __res; }); (*(volatile unsigned int *)&(array[(66)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0xca62c1d6); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(67 +13)&15]) ^ (array[(67 +8)&15]) ^ (array[(67 +2)&15]) ^ (array[(67)&15]))); __res; }); (*(volatile unsigned int *)&(array[(67)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0xca62c1d6); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(68 +13)&15]) ^ (array[(68 +8)&15]) ^ (array[(68 +2)&15]) ^ (array[(68)&15]))); __res; }); (*(volatile unsigned int *)&(array[(68)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0xca62c1d6); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(69 +13)&15]) ^ (array[(69 +8)&15]) ^ (array[(69 +2)&15]) ^ (array[(69)&15]))); __res; }); (*(volatile unsigned int *)&(array[(69)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0xca62c1d6); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(70 +13)&15]) ^ (array[(70 +8)&15]) ^ (array[(70 +2)&15]) ^ (array[(70)&15]))); __res; }); (*(volatile unsigned int *)&(array[(70)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0xca62c1d6); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(71 +13)&15]) ^ (array[(71 +8)&15]) ^ (array[(71 +2)&15]) ^ (array[(71)&15]))); __res; }); (*(volatile unsigned int *)&(array[(71)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0xca62c1d6); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(72 +13)&15]) ^ (array[(72 +8)&15]) ^ (array[(72 +2)&15]) ^ (array[(72)&15]))); __res; }); (*(volatile unsigned int *)&(array[(72)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0xca62c1d6); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(73 +13)&15]) ^ (array[(73 +8)&15]) ^ (array[(73 +2)&15]) ^ (array[(73)&15]))); __res; }); (*(volatile unsigned int *)&(array[(73)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0xca62c1d6); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(74 +13)&15]) ^ (array[(74 +8)&15]) ^ (array[(74 +2)&15]) ^ (array[(74)&15]))); __res; }); (*(volatile unsigned int *)&(array[(74)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0xca62c1d6); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(75 +13)&15]) ^ (array[(75 +8)&15]) ^ (array[(75 +2)&15]) ^ (array[(75)&15]))); __res; }); (*(volatile unsigned int *)&(array[(75)&15]) = (TEMP)); E += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (A)); __res; }) + ((B^C^D)) + (0xca62c1d6); B = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (B)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(76 +13)&15]) ^ (array[(76 +8)&15]) ^ (array[(76 +2)&15]) ^ (array[(76)&15]))); __res; }); (*(volatile unsigned int *)&(array[(76)&15]) = (TEMP)); D += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (E)); __res; }) + ((A^B^C)) + (0xca62c1d6); A = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (A)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(77 +13)&15]) ^ (array[(77 +8)&15]) ^ (array[(77 +2)&15]) ^ (array[(77)&15]))); __res; }); (*(volatile unsigned int *)&(array[(77)&15]) = (TEMP)); C += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (D)); __res; }) + ((E^A^B)) + (0xca62c1d6); E = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (E)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(78 +13)&15]) ^ (array[(78 +8)&15]) ^ (array[(78 +2)&15]) ^ (array[(78)&15]))); __res; }); (*(volatile unsigned int *)&(array[(78)&15]) = (TEMP)); B += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (C)); __res; }) + ((D^E^A)) + (0xca62c1d6); D = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (D)); __res; }); } while (0);
 do { unsigned int TEMP = ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (1), "0" ((array[(79 +13)&15]) ^ (array[(79 +8)&15]) ^ (array[(79 +2)&15]) ^ (array[(79)&15]))); __res; }); (*(volatile unsigned int *)&(array[(79)&15]) = (TEMP)); A += TEMP + ({ unsigned int __res; __asm__("rol" " %1,%0":"=r" (__res):"i" (5), "0" (B)); __res; }) + ((C^D^E)) + (0xca62c1d6); C = ({ unsigned int __res; __asm__("ror" " %1,%0":"=r" (__res):"i" (2), "0" (C)); __res; }); } while (0);

 ctx->H[0] += A;
 ctx->H[1] += B;
 ctx->H[2] += C;
 ctx->H[3] += D;
 ctx->H[4] += E;
}

void blk_SHA1_Init(blk_SHA_CTX *ctx)
{
 ctx->size = 0;


 ctx->H[0] = 0x67452301;
 ctx->H[1] = 0xefcdab89;
 ctx->H[2] = 0x98badcfe;
 ctx->H[3] = 0x10325476;
 ctx->H[4] = 0xc3d2e1f0;
}

void blk_SHA1_Update(blk_SHA_CTX *ctx, const void *data, unsigned long len)
{
 unsigned int lenW = ctx->size & 63;

 ctx->size += len;


 if (lenW) {
  unsigned int left = 64 - lenW;
  if (len < left)
   left = len;
  memcpy(lenW + (char *)ctx->W, data, left);
  lenW = (lenW + left) & 63;
  len -= left;
  data = ((const char *)data + left);
  if (lenW)
   return;
  blk_SHA1_Block(ctx, ctx->W);
 }
 while (len >= 64) {
  blk_SHA1_Block(ctx, data);
  data = ((const char *)data + 64);
  len -= 64;
 }
 if (len)
  memcpy(ctx->W, data, len);
}

void blk_SHA1_Final(unsigned char hashout[20], blk_SHA_CTX *ctx)
{
 static const unsigned char pad[64] = { 0x80 };
 unsigned int padlen[2];
 int i;


 padlen[0] = htonl((uint32_t)(ctx->size >> 29));
 padlen[1] = htonl((uint32_t)(ctx->size << 3));

 i = ctx->size & 63;
 blk_SHA1_Update(ctx, pad, 1+ (63 & (55 - i)));
 blk_SHA1_Update(ctx, padlen, 8);


 for (i = 0; i < 5; i++)
  do { *(unsigned int *)(hashout + i*4) = htonl(ctx->H[i]); } while (0);
}
