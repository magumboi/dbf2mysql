/*
 * common.h - Common definitions shared between dbf2mysql and mysql2dbf
 *
 * This header provides shared types, constants, and function declarations
 * used by both conversion utilities.
 */

#ifndef _COMMON_H
#define _COMMON_H

#include <mysql.h>
#include "dbf.h"

/* Version string */
#ifndef VERSION
#define VERSION "1.14"
#endif

/* Default values */
#define DEFAULT_DATABASE "test"
#define DEFAULT_TABLE    "test"
#define DEFAULT_PORT     0       /* Use MySQL default when 0 */

/* Configuration structure for database connection */
typedef struct {
    char *host;         /* MySQL server hostname */
    char *user;         /* MySQL username */
    char *pass;         /* MySQL password */
    char *dbase;        /* Database name */
    char *table;        /* Table name */
    char *charset;      /* Character set */
    unsigned int port;  /* MySQL server port (0 for default) */
    int verbose;        /* Verbosity level (0-3) */
} db_config;

/* Initialize a db_config structure with defaults */
void db_config_init(db_config *cfg);

/* Free memory allocated for db_config fields */
void db_config_free(db_config *cfg);

/*
 * Connect to MySQL server
 * Returns: MYSQL pointer on success, NULL on failure
 */
MYSQL *db_connect(MYSQL *mysql, const db_config *cfg);

/*
 * Print MySQL error message and optional context
 */
void print_mysql_error(MYSQL *mysql, const char *context);

/*
 * Safe string duplication - exits on memory allocation failure
 */
char *safe_strdup(const char *s);

/*
 * Print verbose message if verbosity level is met
 */
void verbose_print(int current_level, int required_level, const char *format, ...);

#endif /* _COMMON_H */
