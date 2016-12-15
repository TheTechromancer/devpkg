# ifndef _db_h
#  define _db_h

#  define INSTALL_DIR "/usr/local"
#  define DB_DIR INSTALL_DIR "/.devpkg"
#  define DB_FILE DB_DIR "/db"


int DB_init();
int DB_list();
int DB_update(const char *url);
int DB_find(const char *url);

# endif