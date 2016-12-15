# include <unistd.h>
# include <apr_errno.h>
# include <apr_file_io.h>

# include "db.h"
# include "bstrlib.h"
# include "e.h"


static FILE *DB_open(const char *path, const char *mode)
{
	return fopen(path, mode);
}

static void DB_close(FILE *db)
{
	fclose(db);
}

static bstring DB_load()
{
	FILE *db = NULL;
	bstring data = NULL;

	db = DB_open(DB_FILE, "r");
	check(db, "Failed to open database: %s", DB_FILE);

	data = bread((bNread) fread, db);
	check(data, "Failed to read from db file: %s", DB_FILE);

	DB_close(db);
	return data;

end:
	if (db) DB_close(db);
	if (data) bdestroy(data);
	return NULL;
}

int DB_update(const char *url)
{
	if (DB_find(url)) {
		log_info("Already installed: %s", url);
	}

	FILE *db = DB_open(DB_FILE, "a+");
	check(db, "Failed to open DB file: %s", DB_FILE);

	bstring line = bfromcstr(url);
	bconchar(line, '\n');
	int rc = fwrite(line->data, blength(line), 1, db);
	check(rc == 1, "Failed to append to the db");

	return 0;

end:
	if (db) DB_close(db);
	return -1;
}

int DB_find(const char *url)
{
	bstring data = NULL;
	bstring line = bfromcstr(url);

	int rc = -1;
	data = DB_load();
	check(data, "Failed to load db: %s", DB_FILE);

	if (binstr(data, 0, line) == BSTR_ERR) {
		rc = 0;
	} else {
		rc = 1;
	}

end: // fallthrough
	if (data) bdestroy(data);
	if (line) bdestroy(line);
	return rc;
}

int DB_init()
{
	int rc = -1;
	apr_pool_t *p = NULL;
	apr_pool_initialize();
	apr_pool_create(&p, NULL);

	if (access(DB_DIR, W_OK | X_OK) == -1) {
		apr_status_t rc = apr_dir_make_recursive(DB_DIR, APR_UREAD
			| APR_UWRITE | APR_UEXECUTE | APR_GREAD | APR_GWRITE
			| APR_GEXECUTE, p);
		check(rc == APR_SUCCESS, "Failed to make database dir: %s", DB_DIR);
	}

	if (access(DB_FILE, W_OK) == -1) {
		FILE *db = DB_open(DB_FILE, "w");
		check(db, "Cannot open database: %s", DB_FILE);
		DB_close(db);
	}

	rc = 0;

end: // fallthrough
	apr_pool_destroy(p);
	return rc;
}

int DB_list()
{
	bstring data = DB_load();
	check(data, "Failed to load database: %s", DB_FILE);
	printf("%s", bdata(data));
	bdestroy(data);
	return 0;

end:
	return -1;
}