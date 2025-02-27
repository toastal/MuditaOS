﻿// Copyright (c) 2017-2022, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "Database.hpp"
#include "DatabaseInitializer.hpp"

#include <log/log.hpp>

#include <purefs/filesystem_paths.hpp>
#include <gsl/util>

#include <cassert>
#include <cstring>
#include <memory>

/* Declarations *********************/
extern sqlite3_vfs *sqlite3_ecophonevfs(void);

[[nodiscard]] static bool isNotPragmaRelated(const char *msg)
{
    return nullptr == strstr(msg, "PRAGMA");
}

extern "C"
{
    int sqlite3_os_init(void)
    {
        /*
         ** The following macro defines an initializer for an sqlite3_vfs object.
         ** The name of the VFS is NAME.  The pAppData is a pointer to a pointer
         ** to the "finder" function.  (pAppData is a pointer to a pointer because
         ** silly C90 rules prohibit a void* from being cast to a function pointer
         ** and so we have to go through the intermediate pointer to avoid problems
         ** when compiling with -pedantic-errors on GCC.)
         **
         ** The FINDER parameter to this macro is the name of the pointer to the
         ** finder-function.  The finder-function returns a pointer to the
         ** sqlite_io_methods object that implements the desired locking
         ** behaviors.  See the division above that contains the IOMETHODS
         ** macro for addition information on finder-functions.
         **
         ** Most finders simply return a pointer to a fixed sqlite3_io_methods
         ** object.  But the "autolockIoFinder" available on MacOSX does a little
         ** more than that; it looks at the filesystem type that hosts the
         ** database file and tries to choose an locking method appropriate for
         ** that filesystem time.
         */

        sqlite3_vfs_register(sqlite3_ecophonevfs(), 1);
        return SQLITE_OK;
    }

    /*
     ** Shutdown the operating system interface.
     **
     ** Some operating systems might need to do some cleanup in this routine,
     ** to release dynamically allocated objects.  But not on unix.
     ** This routine is a no-op for unix.
     */
    int sqlite3_os_end(void)
    {
        return SQLITE_OK;
    }

    /* Internal Defines ***********************/
    void errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
    {
        if (isNotPragmaRelated(zMsg)) {
            LOG_ERROR("(%d) %s\n", iErrCode, zMsg);
        }
    }
}

constexpr auto dbApplicationId = 0x65727550; // ASCII for "Pure"

Database::Database(const char *name, bool readOnly)
    : dbConnection(nullptr), dbName(name), queryStatementBuffer{nullptr}, isInitialized_(false),
      initializer(std::make_unique<DatabaseInitializer>(this))
{
    const int flags = (readOnly) ? (SQLITE_OPEN_READONLY) : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    LOG_INFO("Opening database: %s", dbName.c_str());
    if (const auto rc = sqlite3_open_v2(name, &dbConnection, flags, nullptr); rc != SQLITE_OK) {
        LOG_ERROR("SQLITE INITIALIZATION ERROR! rc=%d dbName=%s", rc, name);
        throw DatabaseInitialisationError{"Failed to initialize the sqlite db"};
    }

    initQueryStatementBuffer();
    pragmaQuery("PRAGMA integrity_check;");
    pragmaQuery("PRAGMA locking_mode=EXCLUSIVE");

    if (pragmaQueryForValue("PRAGMA application_id;", dbApplicationId)) {
        LOG_DEBUG("Database %s initialized", dbName.c_str());
        isInitialized_ = true;
        return;
    }

    /** Workaround for [MOS-351].
     * Updater app loads databases to wrong directory (1/current/user/db) rather than
     * (3/user/db). Therefore, we need to check both directories to support both flashing image and updating through
     * updater.
     * !!! This workaround should be removed with minor patch of updater, however it cannot be released in one package
     * with database changes.
     **/
    auto filePath = (purefs::dir::getCurrentOSPath() / "user/db");
    if (!std::filesystem::exists(filePath)) {
        filePath = (purefs::dir::getUserDiskPath() / "db");
    }

    LOG_INFO("Running scripts: %s", filePath.c_str());
    isInitialized_ = initializer->run(filePath.c_str(), InitScriptExtension);

    if (isInitialized_) {
        populateDbAppId();
    }
}

void Database::populateDbAppId()
{
    std::stringstream setAppIdPragma;
    setAppIdPragma << "PRAGMA application_id=" << dbApplicationId << ";";
    pragmaQuery(setAppIdPragma.str());
}

void Database::initQueryStatementBuffer()
{
    queryStatementBuffer = static_cast<char *>(sqlite3_malloc(maxQueryLen));
    if (queryStatementBuffer == nullptr) {
        LOG_ERROR("Unable to allocate memory for query statement buffer.");
        throw DatabaseInitialisationError{"Failed to initialize the query statement buffer"};
    }
    clearQueryStatementBuffer();
}

void Database::clearQueryStatementBuffer()
{
    std::memset(queryStatementBuffer, 0, maxQueryLen);
}

Database::~Database()
{
    sqlite3_free(queryStatementBuffer);
    sqlite3_close(dbConnection);
}

bool Database::initialize()
{
    if (const auto code = sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, (void *)1); code != SQLITE_OK) {
        //(void*)1 is taken from official SQLITE examples and it appears that it ends variable args list
        return false;
    }
    return sqlite3_initialize() == SQLITE_OK;
}

bool Database::deinitialize()
{
    return sqlite3_shutdown() == SQLITE_OK;
}

bool Database::execute(const char *format, ...)
{
    if (format == nullptr) {
        return false;
    }

    auto cleanup = gsl::finally([this] { clearQueryStatementBuffer(); });

    va_list ap;
    va_start(ap, format);
    sqlite3_vsnprintf(maxQueryLen, queryStatementBuffer, format, ap);
    va_end(ap);

    if (const int result = sqlite3_exec(dbConnection, queryStatementBuffer, nullptr, nullptr, nullptr);
        result != SQLITE_OK) {
        LOG_ERROR("Execution of query failed with %d", result);
        return false;
    }
    return true;
}

std::unique_ptr<QueryResult> Database::query(const char *format, ...)
{
    if (format == nullptr) {
        return nullptr;
    }

    auto cleanup = gsl::finally([this] { clearQueryStatementBuffer(); });

    va_list ap;
    va_start(ap, format);
    sqlite3_vsnprintf(maxQueryLen, queryStatementBuffer, format, ap);
    va_end(ap);

    auto queryResult = std::make_unique<QueryResult>();
    if (const int result = sqlite3_exec(dbConnection, queryStatementBuffer, queryCallback, queryResult.get(), nullptr);
        result != SQLITE_OK) {
        if (isNotPragmaRelated(queryStatementBuffer)) {
            LOG_ERROR("SQL query failed selecting : %d", result);
        }
        return nullptr;
    }
    return queryResult;
}

int Database::queryCallback(void *usrPtr, int count, char **data, char **columns)
{
    QueryResult *db = reinterpret_cast<QueryResult *>(usrPtr);

    std::vector<Field> row;
    for (uint32_t i = 0; i < (uint32_t)count; i++) {
        try {
            row.push_back(Field{data[i]});
        }
        catch (...) {
            LOG_FATAL("Error on column: %" PRIu32, i);
        }
    }

    db->addRow(row);

    return 0;
}

uint32_t Database::getLastInsertRowId()
{
    return sqlite3_last_insert_rowid(dbConnection);
}

auto Database::pragmaQueryForValue(const std::string &pragmaStatement, const std::int32_t value) -> bool
{
    auto results = query(pragmaStatement.c_str());

    if (!results || results->getRowCount() == 0) {
        LOG_DEBUG("no results!");
        return false;
    }

    do {
        const auto fieldsCount = results->getFieldCount();
        for (uint32_t i = 0; i < fieldsCount; i++) {
            Field field{(*results)[i]};
            if (field.getInt32() == value) {
                LOG_DEBUG("Found the match: %" PRIx32, value);
                return true;
            }
        }
    } while (results->nextRow());

    return false;
}

void Database::pragmaQuery(const std::string &pragmaStatement)
{
    auto results = query(pragmaStatement.c_str());

    if (results && results->getRowCount()) {
        do {
            const auto fieldsCount = results->getFieldCount();
            for (uint32_t i = 0; i < fieldsCount; i++) {
                Field field = (*results)[i];
            }
        } while (results->nextRow());
    }
    else {
        LOG_DEBUG("no results!");
    }
}

bool Database::storeIntoFile(const std::filesystem::path &backupPath)
{
    LOG_INFO("Backup database: %s, into file: %s - STARTED", dbName.c_str(), backupPath.c_str());
    if (const auto rc = execute("VACUUM INTO '%q';", backupPath.c_str()); !rc) {
        LOG_ERROR("Backup database: %s, into file: %s - FAILED", dbName.c_str(), backupPath.c_str());
        return false;
    }
    LOG_INFO("Backup database: %s, into file: %s - SUCCEDED", dbName.c_str(), backupPath.c_str());
    return true;
}
