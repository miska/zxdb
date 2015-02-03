/**
 * file project_test.cpp
 *
 * brief Tests for project
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

#include <string>
#include <stdio.h>
#include <unistd.h>
#include "catch.hpp"
#include "project.hpp"
#include "file.hpp"

extern std::string db_url;

TEST_CASE( "Project works", "[project]" ) {
    db_url  = "sqlite:";
    db_url += tmpnam(NULL);
    unlink(db_url.c_str());
    Project::db_reinit();
    File::db_reinit();

    SECTION("Check that getters works") {
        Project test_a(
            "a",
            "a"
        );
        REQUIRE(test_a.get_db_id() != 0);
    }

    SECTION("Check that getters works") {
        Project test_a(
            "a",
            "a"
        );
        REQUIRE(test_a.get_name() == "a");
        REQUIRE(test_a.get_description() == "a");
    }
    SECTION("Check that setters works") {
        Project test_a(
            "a",
            "a"
        );
        test_a.set_name("b");
        test_a.set_description("b");
        REQUIRE(test_a.get_name() == "b");
        REQUIRE(test_a.get_description() == "b");
    }

    SECTION("Check that comparison works") {
        Project test_a(
            "a",
            "a"
        );
        Project test_b(
            "b",
            "b"
        );
        REQUIRE(test_a != test_b);
        test_a.set_name("b");
        test_a.set_description("b");
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that assignment works") {
        Project test_a(
            "a",
            "a"
        );
        Project test_b(
            "b",
            "b"
        );
        REQUIRE(test_a != test_b);
        test_a = test_b;
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that copy constructor works") {
        Project test_a(
            "a",
            "a"
        );
        Project test_b(test_a);
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that search works") {
        Project test_a(
            "a",
            "a"
        );
        Project test_b(
            "b",
            "b"
        );
        std::vector<Project> res;
        res = Project::search();
        REQUIRE(res.size() == 2);
        res = Project::search(
            "name = 'b' AND "
            "description = 'b'"
        );
        REQUIRE(res.size() == 1);
        REQUIRE(res[0] == test_b);
        res = Project::search(
            "name = :str AND "
            "description = :str"
            ,
            [](tntdb::Statement& st) {
                st.set("str", "b").set("num", 2);
            }
        );
        REQUIRE(res.size() == 1);
        REQUIRE(res[0] == test_b);
    }

    SECTION("Check that setters works reliably") {
		{
        Project test_a(
            "a",
            "a"
        );
        test_a.set_name("b");
        test_a.set_description("b");
		}
        auto res = Project::search(
            "name = :str AND "
            "description = :str"
            ,
            [](tntdb::Statement& st) {
                st.set("str", "b").set("num", 2);
            }
        );
        REQUIRE(res.size() == 1);
    }

    SECTION("Check that remove works") {
        Project test_a(
            "a",
            "a"
        );
        Project test_b(
            "b",
            "b"
        );
        std::vector<Project> res;
        res = Project::search();
        REQUIRE(res.size() == 2);
        Project::remove(
            "name = 'b' AND "
            "description = 'b'"
        );
        res = Project::search();
        REQUIRE(res.size() == 1);
        Project::remove();
        res = Project::search();
        REQUIRE(res.size() == 0);
    }

    SECTION("Getting members works") {
        Project test_a(
            "a",
            "a"
        );
        REQUIRE(test_a.get_files().size() == 0);
    }

    SECTION("Serialization/deserialization works") {
        Project test_a(
            "a",
            "a"
        );
        cxxtools::SerializationInfo si;
        test_a.serialize(si);
        Project test_b = Project::deserialize(si);
        REQUIRE(test_a == test_b);
    }

    SECTION("Get by id works with empty strings") {
        Project test_a(
            "",
            ""
        );
        Project test_b = Project::get_by_id(test_a.get_db_id());
        REQUIRE(test_a == test_b);
    }

    unlink(db_url.c_str());
}

