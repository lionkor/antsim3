// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "doctest.h"
#include "Physics/vec.h"

TEST_CASE("vec ctor defaults") {
    vecd v_d;
    CHECK(v_d.x == 0.0);
    CHECK(v_d.y == 0.0);

    vecd v_d_copy(v_d);
    CHECK(v_d.x == v_d_copy.x);
    CHECK(v_d.y == v_d_copy.y);

    vecd v_d_moved(std::move(v_d));
    CHECK(v_d.x == 0);
    CHECK(v_d.y == 0);
    CHECK(v_d_moved.x == 0);
    CHECK(v_d_moved.y == 0);
}

TEST_CASE("vecd ctors") {
    for (double i = -100; i < 100; i += 10) {
        vecd v_a(i, i * 2);
        CHECK(v_a.x == i);
        CHECK(v_a.y == i * 2);
    }
}

TEST_CASE("vecd operator []") {
    vecd v(120.0, 350.0);
    CHECK(v[0] == 120.0);
    CHECK(v[1] == 350.0);
    
    const vecd v_const(120.0, 350.0);
    CHECK(v_const[0] == 120.0);
    CHECK(v_const[1] == 350.0);
}

// TODO add more
