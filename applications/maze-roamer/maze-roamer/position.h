#pragma once


struct position {
    int x = 0;
    int y = 0;

    bool operator==(const position& p_other) const {
        return x == p_other.x && y == p_other.y;
    }

    bool operator!=(const position& p_other) const {
        return !(*this == p_other);
    }
};
