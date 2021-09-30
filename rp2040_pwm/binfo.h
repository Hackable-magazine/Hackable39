#ifdef VERSION
#define BI_VERSION VERSION "\n" GIT_SHA1
#else
#define BI_VERSION GIT_SHA1
#endif

#ifdef DESCRIPTION
#define BI_DESCRIPTION DESCRIPTION "\n" MY_SOURCE_DIR
#else
#define BI_DESCRIPTION MY_SOURCE_DIR
#endif

#ifdef URL
#define BI_URL URL "\nhttp://github.com/Hackable/" GIT_REPO
#else
#define BI_URL "http://github.com/Hackable/" GIT_REPO
#endif