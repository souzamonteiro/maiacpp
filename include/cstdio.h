// ISO/IEC 14882:1998(E) - 27.8.2 C Library files

#ifndef _CSTDIO_
#define _CSTDIO_

namespace std {

typedef /* implementation-defined */ FILE;
typedef /* implementation-defined */ fpos_t;

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

} // namespace std

#define BUFSIZ      /* implementation-defined */
#define EOF         /* implementation-defined */
#define FILENAME_MAX /* implementation-defined */
#define FOPEN_MAX   /* implementation-defined */
#define L_tmpnam    /* implementation-defined */
#define NULL        /* implementation-defined */
#define SEEK_CUR    /* implementation-defined */
#define SEEK_END    /* implementation-defined */
#define SEEK_SET    /* implementation-defined */
#define TMP_MAX     /* implementation-defined */
#define _IOFBF      /* implementation-defined */
#define _IOLBF      /* implementation-defined */
#define _IONBF      /* implementation-defined */
#define stderr      /* implementation-defined */
#define stdin       /* implementation-defined */
#define stdout      /* implementation-defined */

#endif