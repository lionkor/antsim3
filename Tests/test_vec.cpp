#include "doctest.h"
#include "Physics/vec.h"

TEST_CASE("vec ctor defaults") {
    vec<double> v_d;
    CHECK(v_d.x == 0.0);
    CHECK(v_d.y == 0.0);

    vec<double> v_d_copy(v_d);
    CHECK(v_d.x == v_d_copy.x);
    CHECK(v_d.y == v_d_copy.y);

    vec<double> v_d_moved(std::move(v_d));
    CHECK(v_d.x == 0);
    CHECK(v_d.y == 0);
    CHECK(v_d_moved.x == 0);
    CHECK(v_d_moved.y == 0);
}

TEST_CASE("vec<double> ctors") {
    for (double i = -100; i < 100; i += 10) {
        vec<double> v_a(i, i * 2);
        CHECK(v_a.x == i);
        CHECK(v_a.y == i * 2);
    }
}

TEST_CASE("vec<double> operator []") {
    vec<double> v(120.0, 350.0);
    CHECK(v[0] == 120.0);
    CHECK(v[1] == 350.0);
    
    const vec<double> v_const(120.0, 350.0);
    CHECK(v_const[0] == 120.0);
    CHECK(v_const[1] == 350.0);
}

// TODO add more
