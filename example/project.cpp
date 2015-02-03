/**
 * file project.cpp
 *
 * brief Implementation of project
 *
 * Project abstraction class
 *
 */

/***** WARNING **************************************************************
 *                                                                          *
 * THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose *
 * your changes at the next build cycle. This is great for temporary printf *
 * statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places *
 * for commits are:                                                         *
 *                                                                          *
 * - The XML model used for this code generation: project.xml, or           *
 * - The generation script that built this file: zxdb_cpp                   *
 *                                                                          *
 ***************************************************************************/

/*
  ==========================================================================
  ==========================================================================
*/

#include "project.hpp"
#include "file.hpp"

bool Project::table_exists = false;

extern std::string db_url;

//! Save object into database
void Project::save() {
    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Update stuff in databse
    smt = conn.prepareCached("UPDATE projects SET "
                             "name = :name "
                             ","
                             "description = :description "
                             " WHERE id = :id");
    smt
        .set("name", name)
        .set("description", description)
        .set("id", db_id)
        .execute();
}

//! Get all files belonging to this project
std::vector<File> Project::get_files() const {
    auto id = db_id;
    File::db_init();
    auto ret = File::search("project_id = :project_id",
        [id](tntdb::Statement& st) { st.set("project_id", id); } );
    return ret;
}

//! Add new file to this project
void Project::add_file(File& file) {
    file.set_parent_project_id(db_id);
}

//! Setter for name
std::string& Project::set_name(const std::string val) {
    name = val;
    dirty = true;
    return name;
}

//! Getter for name
std::string  Project::get_name() const {
    return name;
}

//! Setter for description
std::string& Project::set_description(const std::string val) {
    description = val;
    dirty = true;
    return description;
}

//! Getter for description
std::string  Project::get_description() const {
    return description;
}

//! Equality comparison
bool Project::operator==(const Project& other) const {
    return
        name == (other.name) &&
        description == (other.description);
}

//! Checks whether table exists and is correct
void Project::db_init() {
    if(table_exists)
        return;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    // Create database if does not exist
    smt = conn.prepare("CREATE TABLE IF NOT EXISTS projects ( "
                       "id INTEGER PRIMARY KEY AUTOINCREMENT "
                       ", name TEXT "
                       ", description TEXT "
                       " ) ");
    smt.execute();

    table_exists = true;
}

//! Assignment operator
Project& Project::operator=(const Project& other) {
    name = other.name;
    description = other.description;
    dirty = true;
    return (*this);
}

//! Exporting structure for future import
void Project::serialize(cxxtools::SerializationInfo& si) {
    si.addMember("name").setValue(get_name());
    si.addMember("description").setValue(get_description());
    cxxtools::SerializationInfo& nsi = si.addMember("file");
    {
        auto list = get_files();
        for(auto i: list) {
            i.serialize(nsi.addMember(""));
        }
    }
    nsi.setTypeName("set");
    nsi.setCategory(cxxtools::SerializationInfo::Array);
}

//! Importing previously exported structure
Project Project::deserialize(const cxxtools::SerializationInfo& si) {
    std::string in_name;
    si.getMember("name") >>= in_name;
    std::string in_description;
    si.getMember("description") >>= in_description;
    Project f(
        in_name,
        in_description
    );
    {
        auto msi = si.getMember("file");
        for(auto it : msi) {
            File tmp = File::deserialize(it);
            tmp.set_parent_project(f);
        }
    }
    return f;
}

//! Default constructor
Project::Project(
            const std::string& in_name,
            const std::string& in_description) {
    db_init();
    db_id = 0;
    dirty = false;
    name = in_name;
    description = in_description;

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;
    tntdb::Row row;

    // Add into database if doesn't exists
    smt = conn.prepareCached("INSERT INTO projects ( "
                             "name, "
                             "description ) "
                             "SELECT "
                             ":name, "
                             ":description "
                             "WHERE 1 NOT IN "
                             "(SELECT 1 FROM projects WHERE "
                             "name = :name AND "
                             "description = :description LIMIT 1)");
    smt
        .set("name", name)
        .set("description", description)
        .execute();

    // Get ID from the database
    smt = conn.prepareCached("SELECT "
                             "  id "
                             " FROM projects WHERE "
                             "name = :name AND "
                             "description = :description LIMIT 1");
    row = smt
        .set("name", name)
        .set("description", description)
        .selectRow();
    row[0].get(db_id);
}

//! Runs specified function on every matching element
void Project::for_each(std::function<void(Project)> what,
        std::string where,
        std::function<void(tntdb::Statement&)> set) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    db_init();

    // Query data
    std::string query = "SELECT id "
                             ", name "
                             ", description "
                             " FROM projects ";
    if(!where.empty()) {
        query += " WHERE ";
        query += where;
    }
    smt = conn.prepareCached(query);
    set(smt);

    // Construct response
    for(auto row: smt) {
        Project it(
              row.getUnsigned64("id")
            , row.getString("name")
            , row.getString("description")
        );
        what(it);
    }
}

//! Returns element specified by it's database id
Project Project::get_by_id(uint64_t id) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    db_init();

    // Query data
    std::string query = "SELECT id "
                             ", name "
                             ", description "
                             " FROM projects "
                             " WHERE id = :id ";
    smt = conn.prepareCached(query);
    smt.set("id", id);

    // Construct response
    auto row = smt.selectRow();
    Project ret(
              row.getUnsigned64("id")
            , row.isNull("name") ? "" : row.getString("name")
            , row.isNull("description") ? "" : row.getString("description")
        );
    return ret;
}

//! Deletes specified elements
void Project::remove(std::string where,
    std::function<void(tntdb::Statement&)> set) {

    tntdb::Connection conn = tntdb::connectCached(db_url);
    tntdb::Statement smt;

    db_init();

    // Query data
    std::string query = "DELETE FROM projects";
    if(!where.empty()) {
        query += " WHERE ";
        query += where;
    }
    smt = conn.prepareCached(query);
    set(smt);
    smt.execute();
}

