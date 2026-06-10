#include <sys/cdefs.h>

#define SUCCESS 0
#define FAILED -1

__BEGIN_DECLS
int file_exists(const char* path);
int mkdirs(const char* dir);
void touch_file(const char* path);
int write_file(const char* file_path, const void* data, const size_t size);
int read_file(const char* file_path, void* data, const size_t size);
__END_DECLS
