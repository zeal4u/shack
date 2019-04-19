//
// Created by zeal4u on 2019/4/3.
//
#define BOOST_TEST_MODULE mess_test_module
#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

#include "red_black_tree.h"
#include "problems.cpp"
#include "lru_hash_map.h"
#include "unistd.h"

#include <stdio.h>


BOOST_AUTO_TEST_CASE(test_insert_rb_tree) {
  RedBlackTree<int, int> rbt;
  rbt.Insert({1, 1});
  rbt.Insert({2, 1});
  rbt.Insert({3, 1});
  rbt.Insert({4, 1});
  rbt.Insert({5, 1});
  rbt.Insert({6, 1});
  rbt.Insert({7, 1});
  rbt.Insert({8, 1});
  rbt.Insert({9, 1});
  rbt.Insert({-1, 1});
  rbt.Insert({-2, 1});
  rbt.Insert({-3, 1});
  rbt.Insert({-4, 1});
  rbt.Insert({-5, 1});
  rbt.Insert({-6, 1});
  rbt.Insert({-7, 1});
  rbt.Insert({-8, 1});
  rbt.Insert({-9, 1});
  rbt.View(1);
  int value = 0;
  BOOST_CHECK(rbt.Get(1, value) == true);
  //BOOST_CHECK(rbt.Get(8, value) == true);
  BOOST_CHECK(rbt.Get(-1, value) == true);

  rbt.Delete(1);
  rbt.Delete(4);
  rbt.Delete(9);
  rbt.Delete(6);
  rbt.Delete(-6);
  rbt.Delete(-1);
  BOOST_CHECK(rbt.Get(1, value) == false);
  BOOST_CHECK(rbt.Get(4, value) == false);
  BOOST_CHECK(rbt.Get(9, value) == false);
  BOOST_CHECK(rbt.Get(6, value) == false);
  BOOST_CHECK(rbt.Get(-6, value) == false);
  BOOST_CHECK(rbt.Get(-1, value) == false);
  BOOST_CHECK(rbt.Min().first == -9);
  BOOST_CHECK(rbt.Max().first == 8);
  auto it = rbt.GetIterator();
  while (it->Next()) {
    std::cout << "{"<< it->Value().first << ", " << it->Value().second << "} ";
  }
  std::cout << std::endl;
  rbt.View(1);
  BOOST_CHECK(rbt.Valid());
}

/*
BOOST_AUTO_TEST_CASE(test_split) {
  std::vector<std::string> output = split("ab \'cde\"fg\'\" h");  
  BOOST_CHECK(output[0] == "ab");
  BOOST_CHECK(output[1] == "\'cde\"fg\'\"");
  BOOST_CHECK(output[2] == "h");

  output = split("ab \'cd e\"f \"g\'\" h");  
  BOOST_CHECK(output[0] == "ab");
  BOOST_CHECK(output[1] == "\'cd");
  BOOST_CHECK(output[2] == "e\"f \"g\'\"");
  BOOST_CHECK(output[3] == "h");
}
*/


//BOOST_AUTO_TEST_CASE(test_lru_map) {
//  LRUHashMap<int, int> map;
//  unsigned long long now = std::chrono::system_clock::now().time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
//  printf("%lld\n", now);
//  map.Set(1, 1);  
//  map.Set(2, 2);  
//  map.Set(3, 3);  
//  map.Set(4, 4);  
//  BOOST_CHECK(map.Get(1) == 1);
//  BOOST_CHECK(map.Get(2) == 2);
//  BOOST_CHECK(map.Get(3) == 3);
//  BOOST_CHECK(map.Get(4) == 4);
//
//  map.RetireNum(1);
//  BOOST_CHECK(map.Contains(1) == false);
//  sleep(1);
//  BOOST_CHECK(map.Contains(2) == true);
//  now = std::chrono::system_clock::now().time_since_epoch().count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
//  printf("%lld\n", now);
//  map.RetireByTime(now - 10 * 1000);
//  BOOST_CHECK(map.Contains(2) == true);
//}
