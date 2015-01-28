/**
 * file file.hpp
 *
 * brief Header for file
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

#ifndef __FILE_H_INCLUDED__
#define __FILE_H_INCLUDED__

#include <vector>
#include <string>
#include <functional>
#include <tntdb.h>
#include <cxxtools/serializationinfo.h>


class Path;
class Project;

class File {
private:
    //! Database id
    uint64_t db_id;
    //! Indicates changes not yet saved in database
    bool dirty;
    //! Indicates whether existence of the table was already checked
    static bool table_exists;
    //! Size of the file
    int64_t size;
    //! Hash of the file
    std::string hash;
    //! Project this file belongs to
    uint64_t project_id;
    //! Private constructor purely for deserializer
    File():
              db_id(0)
            , size(0)
            , hash("")
            , project_id(0)
            {}
    //! Private constructor directly from database
    File(
              const uint64_t& in_db_id
            , const int64_t& in_size
            , const std::string& in_hash
            , uint64_t in_project_id
            ) :
              db_id(in_db_id)
            , dirty(false)
            , size(in_size)
            , hash(in_hash)
            , project_id(in_project_id)
            {}
    //! Returns element specified by it's database id
    static File get_by_id(uint64_t id);

public:
    //! Destructor
    virtual ~File() {
        if(dirty)
            save();
    }
    //! Makes sure that table exists (called from constructor)
    static void init();
    //! Tells class to reinitialize database (if you do something really nasty)
    static void reinit() {
        table_exists = false;
        init();
    }
    //! Save object into database
    virtual void save();
    //! Returns unique database id
    virtual uint64_t get_db_id() { return db_id; }
    //! Setter for size
    virtual int64_t& set_size(const int64_t);
    //! Getter for size
    virtual int64_t   get_size() const;
    //! Setter for hash
    virtual std::string& set_hash(const std::string);
    //! Getter for hash
    virtual std::string  get_hash() const;
    //! Get all paths belonging to this file
    virtual std::vector<Path> get_paths() const;
    //! Add new path to this file
    virtual void add_path(Path& path);
    //! Set id of parent project
    virtual void set_parent_project_id(uint64_t id);
    //! Get id of parent project
    virtual uint64_t get_parent_project_id() const;
    //! Set parent project
    virtual void set_parent_project(const Project parent);
    //! Get parent project
    virtual Project get_parent_project() const;
    //! Default constructor
    File(
            const int64_t& in_size,
            const std::string& in_hash
        );
    //! Copy constructor
    File(const File& other) :
              db_id(other.db_id)
            , dirty(other.dirty)
            , size(other.size)
            , hash(other.hash)
            , project_id(other.project_id)
            {}
    //! Move constructor
    File(const File&& other) :
            db_id(other.db_id),
            dirty(other.dirty),
            size(other.size),
            hash(move(other.hash)) {}
    //! Equality comparison
    virtual bool operator==(const File& other) const;
    //! Assignment operator
    virtual bool operator=(const File& other);
    //! Inequality comparison
    bool operator!=(const File& other) const {
        return !operator==(other);
    }
    //! Runs specified function on every matching element
    static void for_each(std::function<void(File)> what,
        std::string where = "",
        std::function<void(tntdb::Statement&)> set = [](tntdb::Statement&) {});
    //! Return matching instances
    static std::vector<File> search(std::string where = "",
        std::function<void(tntdb::Statement&)> set = [](tntdb::Statement&) {}) {
        std::vector<File> ret;
        File::for_each([&ret](File file) {
                ret.push_back(file);
            }, where, set);
        return ret;
    }
    //! Exporting structure for future import
    void serialize(cxxtools::SerializationInfo& si);
    //! Importing previously exported structure
    static File deserialize(const cxxtools::SerializationInfo& si);
};

#endif
