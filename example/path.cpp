/**
 * file path.cpp
 *
 * brief Implementation of path
 *
 * Path abstraction class
 *
 */

/***** WARNING **************************************************************
 *                                                                          *
 * THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose *
 * your changes at the next build cycle. This is great for temporary printf *
 * statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places *
 * for commits are:                                                         *
 *                                                                          *
 * - The XML model used for this code generation: path.xml, or              *
 * - The generation script that built this file: zxdb_cpp                   *
 *                                                                          *
 ***************************************************************************/

/*
  ==========================================================================
  ==========================================================================
*/

#include "path.hpp"
#include "file.hpp"

bool Path::table_exists = false;

extern std::string db_url;

//! Save object into database
void Path::save() {
    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Update stuff in databse
    smt = conn.prepareCached("UPDATE path SET "
                             "url = :url WHERE id = :id");
    smt
        .set("url", url)
        .execute();
}

//! Set Path::id of parent file
void Path::set_parent_file_id(uint64_t id) {
    file_id = id;
    dirty = true;
}

//! Get id of parent file
uint64_t Path::get_parent_file_id() const {
    return file_id;
}

//! Set parent file
void Path::set_parent_file(const File parent) {
}

//! Get parent file
File Path::get_parent_file() const {
}

//! Setter for url
std::string& Path::set_url(const std::string val) {
    url = val;
    dirty = true;
    return url;
}

//! Getter for url
std::string  Path::get_url() const {
    return url;
}

//! Equality comparison
bool Path::operator==(const Path& other) const {
    return
        url == (other.url);
}

//! Checks whether table exists and is correct
void Path::init() {
    if(table_exists)
        return;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Create database if does not exist
    smt = conn.prepare("CREATE TABLE IF NOT EXISTS path ( "
                       "id INTEGER PRIMARY KEY AUTOINCREMENT "
                       ", url TEXT "
                       ", file_id INTEGER DEFAULT 0"
                       " ) ");
    smt.execute();

    table_exists = true;
}

//! Assignment operator
bool Path::operator=(const Path& other) {
    url = other.url;
    dirty = true;
}

//! Exporting structure for future import
void Path::serialize(cxxtools::SerializationInfo& si) {
    si.addMember("url").setValue(get_url());
}

//! Importing previously exported structure
Path Path::deserialize(const cxxtools::SerializationInfo& si) {
    std::string in_url;
    si.getMember("url") >>= in_url;
    Path f(
        in_url
    );
    return f;
}

//! Default constructor
Path::Path(
            const std::string& in_url) {
    init();
    db_id = 0;
    dirty = false;
    url = in_url;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;
    tntdb::Row row;

    // Add into database if doesn't exists
    smt = conn.prepareCached("INSERT INTO path ( "
                             "url, "
                             "file_id ) "
                             "SELECT "
                             ":url, "
                             ":file_id"
                             "WHERE 1 NOT IN "
                             "(SELECT 1 FROM path WHERE "
                             "url = :url LIMIT 1)");
    smt
        .set("url", url)
        .set("file_id", file_id)
        .execute();

    // Get ID from the database
    smt = conn.prepareCached("SELECT "
                             "  id "
                             ", file_id "
                             " FROM path WHERE "
                             "url = :url LIMIT 1");
    row = smt
        .set("url", url)
        .selectRow();
    row[0].get(db_id);
    row[1].get(file_id);
}

//! Runs specified function on every matching element
void Path::for_each(std::function<void(Path)> what,
        std::string where,
        std::function<void(tntdb::Statement&)> set) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    init();

    // Query data
    std::string query = "SELECT id "
                             ", url "
                             ", file_id "
                             " FROM path ";
    if(!where.empty()) {
        query += " WHERE ";
        query += where;
    }
    smt = conn.prepareCached(query);
    set(smt);

    // Construct response
    for(auto row: smt) {
        Path it(
              row.getUnsigned64("id")
            , row.getString("url")
            , row.getUnsigned64("file_id")
        );
        what(it);
    }
}

//! Returns element specified by it's database id
Path Path::get_by_id(uint64_t id) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    init();

    // Query data
    std::string query = "SELECT id "
                             ", url "
                             ", file_id "
                             " FROM path "
                             " WHERE id = :id ";
    smt = conn.prepareCached(query);
    smt.set("id", id);

    // Construct response
    auto row = smt.selectRow();
    Path ret(
              row.getUnsigned64("id")
            , row.getString("url")
            , row.getUnsigned64("file_id")
        );
    return ret;
}


