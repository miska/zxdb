/**
 * file file.cpp
 *
 * brief Implementation of file
 *
 * File abstraction class
 *
 */

/***** WARNING **************************************************************
 *                                                                          *
 * THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose *
 * your changes at the next build cycle. This is great for temporary printf *
 * statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places *
 * for commits are:                                                         *
 *                                                                          *
 * - The XML model used for this code generation: file.xml, or              *
 * - The generation script that built this file: zxdb_cpp                   *
 *                                                                          *
 ***************************************************************************/

/*
  ==========================================================================
  ==========================================================================
*/

#include "file.hpp"
#include "path.hpp"
#include "project.hpp"

bool File::table_exists = false;

extern std::string db_url;

//! Save object into database
void File::save() {
    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Update stuff in databse
    smt = conn.prepareCached("UPDATE file SET "
                             "size = :size, "
                             "hash = :hash WHERE id = :id");
    smt
        .set("size", size)
        .set("hash", hash)
        .execute();
}

//! Get all paths belonging to this file
std::vector<Path> File::get_paths() const {
    auto id = db_id;
    Path::init();
    auto ret = Path::search("file_id = :file_id",
        [id](tntdb::Statement& st) { st.set("file_id", id); } );
    return ret;
}

//! Add new path to this file
void File::add_path(Path& path) {
    path.set_parent_file_id(db_id);
}

//! Set File::id of parent project
void File::set_parent_project_id(uint64_t id) {
    project_id = id;
    dirty = true;
}

//! Get id of parent project
uint64_t File::get_parent_project_id() const {
    return project_id;
}

//! Set parent project
void File::set_parent_project(const Project parent) {
}

//! Get parent project
Project File::get_parent_project() const {
}

//! Setter for size
int64_t& File::set_size(const int64_t val) {
    size = val;
    dirty = true;
    return size;
}

//! Getter for size
int64_t   File::get_size() const {
    return size;
}

//! Setter for hash
std::string& File::set_hash(const std::string val) {
    hash = val;
    dirty = true;
    return hash;
}

//! Getter for hash
std::string  File::get_hash() const {
    return hash;
}

//! Equality comparison
bool File::operator==(const File& other) const {
    return
        size == (other.size) &&
        hash == (other.hash);
}

//! Checks whether table exists and is correct
void File::init() {
    if(table_exists)
        return;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Create database if does not exist
    smt = conn.prepare("CREATE TABLE IF NOT EXISTS file ( "
                       "id INTEGER PRIMARY KEY AUTOINCREMENT "
                       ", size BIGINT "
                       ", hash TEXT "
                       ", project_id INTEGER DEFAULT 0"
                       " ) ");
    smt.execute();

    table_exists = true;
}

//! Assignment operator
bool File::operator=(const File& other) {
    size = other.size;
    hash = other.hash;
    dirty = true;
}

//! Exporting structure for future import
void File::serialize(cxxtools::SerializationInfo& si) {
    si.addMember("size").setValue(get_size());
    si.addMember("hash").setValue(get_hash());
    auto nsi = si.addMember("path");
    nsi.setTypeName("set");
    nsi.setCategory(cxxtools::SerializationInfo::Array);
    {
        auto list = get_paths();
        for(auto i: list) {
            i.serialize(nsi.addMember(""));
        }
    }
}

//! Importing previously exported structure
File File::deserialize(const cxxtools::SerializationInfo& si) {
    int64_t in_size;
    si.getMember("size") >>= in_size;
    std::string in_hash;
    si.getMember("hash") >>= in_hash;
    File f(
        in_size,
        in_hash
    );
    {
        auto msi = si.getMember("path");
        for(auto it : msi) {
            Path tmp = Path::deserialize(it);
            tmp.set_parent_file(f);
        }
    }
    return f;
}

//! Default constructor
File::File(
            const int64_t& in_size,
            const std::string& in_hash) {
    init();
    db_id = 0;
    dirty = false;
    size = in_size;
    hash = in_hash;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;
    tntdb::Row row;

    // Add into database if doesn't exists
    smt = conn.prepareCached("INSERT INTO file ( "
                             "size, "
                             "hash, "
                             "project_id ) "
                             "SELECT "
                             ":size, "
                             ":hash, "
                             ":project_id "
                             "WHERE 1 NOT IN "
                             "(SELECT 1 FROM file WHERE "
                             "size = :size AND "
                             "hash = :hash LIMIT 1)");
    smt
        .set("size", size)
        .set("hash", hash)
        .set("project_id", project_id)
        .execute();

    // Get ID from the database
    smt = conn.prepareCached("SELECT "
                             "  id "
                             ", project_id "

                             " FROM file WHERE "
                             "size = :size AND "
                             "hash = :hash LIMIT 1");
    row = smt
        .set("size", size)
        .set("hash", hash)
        .selectRow();
    row[0].get(db_id);
    row[1].get(project_id);
}

//! Runs specified function on every matching element
void File::for_each(std::function<void(File)> what,
        std::string where,
        std::function<void(tntdb::Statement&)> set) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    init();

    // Query data
    std::string query = "SELECT id "
                             ", size "
                             ", hash "
                             ", project_id "
                             " FROM file ";
    if(!where.empty()) {
        query += " WHERE ";
        query += where;
    }
    smt = conn.prepareCached(query);
    set(smt);

    // Construct response
    for(auto row: smt) {
        File it(
              row.getUnsigned64("id")
            , row.getInt64("size")
            , row.getString("hash")
            , row.getUnsigned64("project_id")
        );
        what(it);
    }
}

//! Returns element specified by it's database id
File File::get_by_id(uint64_t id) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    init();

    // Query data
    std::string query = "SELECT id "
                             ", size "
                             ", hash "
                             ", project_id "
                             " FROM file "
                             " WHERE id = :id ";
    smt = conn.prepareCached(query);
    smt.set("id", id);

    // Construct response
    auto row = smt.selectRow();
    File ret(
              row.getUnsigned64("id")
            , row.getInt64("size")
            , row.getString("hash")
            , row.getUnsigned64("project_id")
        );
    return ret;
}


