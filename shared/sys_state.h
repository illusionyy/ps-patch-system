#include <stdbool.h>
#include <sys/cdefs.h>

__BEGIN_DECLS
bool isPatchAllowed_(const char* user);
__END_DECLS

#define isPatchAllowed() isPatchAllowed_(FILE_FUNC_LINE)
