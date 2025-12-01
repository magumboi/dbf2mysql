/*
 * common.c - Common functions shared between dbf2mysql and mysql2dbf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "common.h"

void db_config_init(db_config *cfg) {
    if (cfg == NULL) return;
    
    cfg->host = NULL;
    cfg->user = NULL;
    cfg->pass = NULL;
    cfg->dbase = NULL;
    cfg->table = NULL;
    cfg->charset = NULL;
    cfg->verbose = 0;
}

void db_config_free(db_config *cfg) {
    if (cfg == NULL) return;
    
    free(cfg->host);
    free(cfg->user);
    free(cfg->pass);
    free(cfg->dbase);
    free(cfg->table);
    free(cfg->charset);
    
    cfg->host = NULL;
    cfg->user = NULL;
    cfg->pass = NULL;
    cfg->dbase = NULL;
    cfg->table = NULL;
    cfg->charset = NULL;
}

MYSQL *db_connect(MYSQL *mysql, const db_config *cfg) {
    MYSQL *sock;
    
    if (mysql == NULL || cfg == NULL) {
        return NULL;
    }
    
    if (!mysql_init(mysql)) {
        fprintf(stderr, "Can't initialize MySQL. Insufficient memory.\n");
        return NULL;
    }
    
    sock = mysql_real_connect(mysql, cfg->host, cfg->user, cfg->pass, 
                              NULL, 0, NULL, 0);
    if (!sock) {
        fprintf(stderr, "Couldn't connect to MySQL server!\n");
        fprintf(stderr, "Detailed report: %s\n", mysql_error(mysql));
        return NULL;
    }
    
    if (cfg->dbase != NULL) {
        if (mysql_select_db(sock, cfg->dbase) == -1) {
            fprintf(stderr, "Couldn't select database %s.\n", cfg->dbase);
            fprintf(stderr, "Detailed report: %s\n", mysql_error(sock));
            mysql_close(sock);
            return NULL;
        }
    }
    
    if (cfg->charset != NULL) {
        if (!mysql_set_character_set(mysql, cfg->charset)) {
            if (cfg->verbose > 0) {
                printf("Client character set: %s\n", 
                       mysql_character_set_name(mysql));
            }
        }
    }
    
    return sock;
}

void print_mysql_error(MYSQL *mysql, const char *context) {
    if (context != NULL) {
        fprintf(stderr, "%s\n", context);
    }
    fprintf(stderr, "MySQL error: %s\n", mysql_error(mysql));
}

char *safe_strdup(const char *s) {
    char *dup;
    
    if (s == NULL) {
        return NULL;
    }
    
    dup = strdup(s);
    if (dup == NULL) {
        fprintf(stderr, "Memory allocation error in safe_strdup\n");
        exit(1);
    }
    
    return dup;
}

void verbose_print(int current_level, int required_level, const char *format, ...) {
    va_list args;
    
    if (current_level >= required_level) {
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
