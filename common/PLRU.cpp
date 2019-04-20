//
// Created by zeal4u on 2019/3/4.
//

#include <stdexcept>
#include "PLRU.h"

void PLRU::update_plru(uint32_t way) {
    if (way >= way_num_) {
        char output[256];
        sprintf(output, "Way %d out of way number %d !", way, way_num_);
        throw std::out_of_range(output);
    }
    uint32_t pos = 0;
    uint32_t mid = way_num_ / 2;
    for (int i = 0; i < level_; ++i) {
        if (way >= mid) {
            way -= mid;
            plru_value_ &= ~(1 << pos);
            pos = pos * 2 + 2;
        } else {
            plru_value_ |= 1 << pos;
            pos = pos * 2 + 1;
        }
        mid /= 2;
    }
}

void PLRU::test_plru() {
    PLRU plru(4);
    for (int i = 0; i < 8; i++) {
        plru.update_plru(i);
        std::cout << plru.get_plru_value() << " ";
    }
}
