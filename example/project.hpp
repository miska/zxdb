/**
 * file project.hpp
 *
 * brief Header for project
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

#ifndef __PROJECT_H_INCLUDED__
#define __PROJECT_H_INCLUDED__

#include <vector>
#include <string>
#include <functional>
#include <tntdb.h>
#include <cxxtools/serializationinfo.h>


class File;

class Project {
private:
    //! Database id
    uint64_t db_id;
    //! Indicates changes not yet saved in database
    bool dirty;
    //! Indicates whether existence of the table was already checked
    static bool table_exists;
    //! Name of the project
    std::string name;
    //! Description of the project
    std::string description;
    //! Private constructor purely for deserializer
    Project():
              db_id(0)
            , name("")
            , description("")
            {}
    //! Private constructor directly from database
    Project(
              const uint64_t& in_db_id
            , const std::string& in_name
            , const std::string& in_description
            ) :
              db_id(in_db_id)
            , dirty(false)
            , name(in_name)
            , description(in_description)
            {}
    //! Returns element specified by it's database id
    static Project get_by_id(uint64_t id);

public:
    //! Destructor
    virtual ~Project() {
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
    //! Setter for name
    virtual std::string& set_name(const std::string);
    //! Getter for name
    virtual std::string  get_name() const;
    //! Setter for description
    virtual std::string& set_description(const std::string);
    //! Getter for description
    virtual std::string  get_description() const;
    //! Get all files belonging to this project
    virtual std::vector<File> get_files() const;
    //! Add new file to this project
    virtual void add_file(File& file);
    //! Default constructor
    Project(
            const std::string& in_name,
            const std::string& in_description
        );
    //! Copy constructor
    Project(const Project& other) :
              db_id(other.db_id)
            , dirty(other.dirty)
            , name(other.name)
            , description(other.description)
            {}
    //! Move constructor
    Project(const Project&& other) :
            db_id(other.db_id),
            dirty(other.dirty),
            name(move(other.name)),
            description(move(other.description)) {}
    //! Equality comparison
    virtual bool operator==(const Project& other) const;
    //! Assignment operator
    virtual bool operator=(const Project& other);
    //! Inequality comparison
    bool operator!=(const Project& other) const {
        return !operator==(other);
    }
    //! Runs specified function on every matching element
    static void for_each(std::function<void(Project)> what,
        std::string where = "",
        std::function<void(tntdb::Statement&)> set = [](tntdb::Statement&) {});
    //! Return matching instances
    static std::vector<Project> search(std::string where = "",
        std::function<void(tntdb::Statement&)> set = [](tntdb::Statement&) {}) {
        std::vector<Project> ret;
        Project::for_each([&ret](Project project) {
                ret.push_back(project);
            }, where, set);
        return ret;
    }
    //! Exporting structure for future import
    void serialize(cxxtools::SerializationInfo& si);
    //! Importing previously exported structure
    static Project deserialize(const cxxtools::SerializationInfo& si);
};

#endif
