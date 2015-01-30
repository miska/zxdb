/**
 * file file_test.cpp
 *
 * brief Tests for file
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

#include <string>
#include <stdio.h>
#include <unistd.h>
#include "catch.hpp"
#include "file.hpp"
#include "path.hpp"

extern std::string db_url;

TEST_CASE( "File works", "[file]" ) {
    db_url  = "sqlite:";
    db_url += tmpnam(NULL);
    unlink(db_url.c_str());
    File::db_reinit();
    Path::db_reinit();

    SECTION("Check that getters works") {
        File test_a(
            1,
            "a"
        );
        REQUIRE(test_a.get_size() == 1);
        REQUIRE(test_a.get_hash() == "a");
    }
    SECTION("Check that setters works") {
        File test_a(
            1,
            "a"
        );
        test_a.set_size(2);
        test_a.set_hash("b");
        REQUIRE(test_a.get_size() == 2);
        REQUIRE(test_a.get_hash() == "b");
    }

    SECTION("Check that comparison works") {
        File test_a(
            1,
            "a"
        );
        File test_b(
            2,
            "b"
        );
        REQUIRE(test_a != test_b);
        test_a.set_size(2);
        test_a.set_hash("b");
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that assignment works") {
        File test_a(
            1,
            "a"
        );
        File test_b(
            2,
            "b"
        );
        REQUIRE(test_a != test_b);
        test_a = test_b;
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that copy constructor works") {
        File test_a(
            1,
            "a"
        );
        File test_b(test_a);
        REQUIRE(test_a == test_b);
    }

    SECTION("Check that search works") {
        File test_a(
            1,
            "a"
        );
        File test_b(
            2,
            "b"
        );
        std::vector<File> res;
        res = File::search();
        REQUIRE(res.size() == 2);
        res = File::search(
            "size = 2 AND "
            "hash = 'b'"
        );
        REQUIRE(res.size() == 1);
        REQUIRE(res[0] == test_b);
        res = File::search(
            "size = :num AND "
            "hash = :str"
            ,
            [](tntdb::Statement& st) {
                st.set("str", "b").set("num", 2);
            }
        );
        REQUIRE(res.size() == 1);
        REQUIRE(res[0] == test_b);
    }

    SECTION("Check that remove works") {
        File test_a(
            1,
            "a"
        );
        File test_b(
            2,
            "b"
        );
        std::vector<File> res;
        res = File::search();
        REQUIRE(res.size() == 2);
		File::remove(
            "size = 2 AND "
            "hash = 'b'"
        );
        res = File::search();
        REQUIRE(res.size() == 1);
		File::remove();
        res = File::search();
        REQUIRE(res.size() == 0);
    }

    SECTION("Getting members works") {
        File test_a(
            1,
            "a"
        );
        REQUIRE(test_a.get_paths().size() == 0);
    }

    SECTION("Serialization/deserialization works") {
        File test_a(
            1,
            "a"
        );
        cxxtools::SerializationInfo si;
        test_a.serialize(si);
        File test_b = File::deserialize(si);
        REQUIRE(test_a == test_b);
    }

    unlink(db_url.c_str());
}

