// ISO/IEC 14882:1998(E) - 27.8.2 C Library files

#ifndef _CSTDIO_
#define _CSTDIO_

#include <cstddef>
#include <cstdarg>

namespace std {

struct FILE;
typedef long fpos_t;

// File operations
FILE* fopen(const char* filename, const char* mode);
int fclose(FILE* stream);
int fflush(FILE* stream);
void setbuf(FILE* stream, char* buffer);
int setvbuf(FILE* stream, char* buffer, int mode, size_t size);

// Character input/output
int fgetc(FILE* stream);
int getc(FILE* stream);
int getchar(void);
int fputc(int c, FILE* stream);
int putc(int c, FILE* stream);
int putchar(int c);
int ungetc(int c, FILE* stream);

// String input/output
char* fgets(char* s, int n, FILE* stream);
int fputs(const char* s, FILE* stream);

// Formatted input/output
int fprintf(FILE* stream, const char* format, ...);
int fscanf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);
int scanf(const char* format, ...);
int sprintf(char* s, const char* format, ...);
int sscanf(const char* s, const char* format, ...);
int vfprintf(FILE* stream, const char* format, va_list arg);
int vprintf(const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);

// Direct input/output
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

// File positioning
int fseek(FILE* stream, long offset, int whence);
long ftell(FILE* stream);
void rewind(FILE* stream);
int fgetpos(FILE* stream, fpos_t* pos);
int fsetpos(FILE* stream, const fpos_t* pos);

// Error handling
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);

// Other
int remove(const char* filename);
int rename(const char* old, const char* new);
FILE* tmpfile(void);
char* tmpnam(char* s);

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

} // namespace std

#define BUFSIZ       1024          /* implementation-defined (WASM/MaiaC profile) */
#define EOF          (-1)          /* implementation-defined (WASM/MaiaC profile) */
#define FILENAME_MAX 255           /* implementation-defined (WASM/MaiaC profile) */
#define FOPEN_MAX    20            /* implementation-defined (WASM/MaiaC profile) */
#define L_tmpnam     255           /* implementation-defined (WASM/MaiaC profile) */
#define NULL         0             /* implementation-defined (WASM/MaiaC profile) */
#define SEEK_CUR     1             /* implementation-defined (WASM/MaiaC profile) */
#define SEEK_END     2             /* implementation-defined (WASM/MaiaC profile) */
#define SEEK_SET     0             /* implementation-defined (WASM/MaiaC profile) */
#define TMP_MAX      25            /* implementation-defined (WASM/MaiaC profile) */
#define _IOFBF       0             /* implementation-defined (WASM/MaiaC profile) */
#define _IOLBF       1             /* implementation-defined (WASM/MaiaC profile) */
#define _IONBF       2             /* implementation-defined (WASM/MaiaC profile) */
#define stderr       (::std::stderr) /* implementation-defined (WASM/MaiaC profile) */
#define stdin        (::std::stdin)  /* implementation-defined (WASM/MaiaC profile) */
#define stdout       (::std::stdout) /* implementation-defined (WASM/MaiaC profile) */

#endif