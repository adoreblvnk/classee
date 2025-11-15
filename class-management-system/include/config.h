#ifndef CONFIG_H
#define CONFIG_H

#define SHUFFLE 1
#ifdef TESTING
// test file paths
#define DB_FILE "test_P3_7.txt"
#define LOG_FILE "test_cms.log"
#define TLOG_FILE "test_cms.tlog"
#define TLOG_TMP_FILE "test_cms.tlog.tmp"
#else
// default file paths
#define DB_FILE "data/P3_7-CMS.txt"
#define LOG_FILE "data/cms.log"
#define TLOG_FILE "data/cms.tlog"
#define TLOG_TMP_FILE "data/cms.tlog.tmp"
#endif // TESTING

#endif // CONFIG_H