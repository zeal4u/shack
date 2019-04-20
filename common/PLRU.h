//
// Created by zeal4u on 2019/3/4.
//

#ifndef LEETCODE_PLRU_H
#define LEETCODE_PLRU_H

#include <stdint-gcc.h>
#include <iostream>

class PLRU {
public:
    PLRU(uint32_t way_num):plru_value_(0), way_num_(way_num), level_(0)
    {
        for (uint32_t i = way_num_; i > 0; i >>= 1)
            level_++;
    }

    uint32_t get_plru_value() const { return plru_value_; }

    void update_plru(uint32_t way);

    static void test_plru();
private:
    PLRU(const PLRU&) = delete;
    PLRU& operator=(const PLRU&) = delete;


    uint32_t plru_value_;
    uint32_t way_num_;
    uint32_t level_;
};
#endif //LEETCODE_PLRU_H
