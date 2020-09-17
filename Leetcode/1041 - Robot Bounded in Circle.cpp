enum class direction {
    north = 0,
    east,
    south,
    west
};

struct position {
    int x = 0;
    int y = 0;

    direction orientation = direction::north;

    bool operator==(const position & p_other) {
        return x == p_other.x && y == p_other.y && orientation == p_other.orientation;
    }
};


class Solution {
public:
    bool isRobotBounded(const std::string & instructions) {
        position current;

        for (const auto & command : instructions) {
            switch (command) {
            case 'G':
                go_forward(current);
                break;
            case 'L':
                turn_left(current);
                break;
            case 'R':
                turn_right(current);
                break;
            }
        }

        if (current.x == 0 && current.y == 0) {
            return true;
        }

        if (current.orientation != direction::north) {
            return true;
        }

        return false;
    }

private:
    void go_forward(position & pos) {
        switch (pos.orientation) {
        case direction::north:
            pos.y++;
            break;
        case direction::east:
            pos.x++;
            break;
        case direction::south:
            pos.y--;
            break;
        case direction::west:
            pos.x--;
            break;
        }
    }

    void turn_right(position & pos) {
        update_orientation(static_cast<int>(pos.orientation) + 1, pos);
    }

    void turn_left(position & pos) {
        update_orientation(static_cast<int>(pos.orientation) - 1, pos);
    }

    void update_orientation(const int value, position & pos) {
        if (value > static_cast<int>(direction::west)) {
            pos.orientation = direction::north;
        }
        else if (value < static_cast<int>(direction::north)) {
            pos.orientation = direction::west;
        }
        else {
            pos.orientation = static_cast<direction>(value);
        }
    }
};