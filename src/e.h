/*
 * check
 * sentinel
 *
 * log_err
 * log_warn
 * log_info
 * log_debug (disabled by setting NDEBUG)
 *
 */


# ifndef __eh__
#  define __eh__

#  include <stdio.h>
#  include <errno.h>
#  include <string.h>

#  define err_string() (errno == 0 ? "None": strerror(errno))

#  define log_err(M, ...) fprintf(stderr, \
	" [x] File \"%s\", line %d, in %s - (end: %s)\n   - " M "\n\n", \
	__FILE__, __LINE__, __FUNCTION__, err_string(), ##__VA_ARGS__)

#  define log_warn(M, ...) fprintf(stderr, \
	" [!] " M " (end: %s)\n", ##__VA_ARGS__, err_string())

#  define log_info(M, ...) fprintf(stdout, \
	" [+] " M "\n", ##__VA_ARGS__)

#  define check(A, M, ...) if(!(A)) {\
	log_err(M, ##__VA_ARGS__); errno = 0; goto end; }

#  define sentinel(M, ...) {\
	log_err(M, ##__VA_ARGS__); errno = 0; goto end; }

#  define check_mem(A) if(!(A)) {\
	log_err("Memory error"), errno = 0; goto end; }


#  ifdef NDEBUG
#   define log_debug(M, ...)
#  else
#   define log_debug(M, ...) fprintf(stderr, \
	" [DEBUG] File \"%s\", line %d, in %s - (end: %s)\n   - " M "\n\n", \
	__FILE__, __LINE__, __FUNCTION__, err_string(), ##__VA_ARGS__)
#  endif


#  define check_debug(A, M, ...) if(!(A)) {\
	log_debug(M, ##__VA_ARGS__); errno = 0; goto end; }

# endif