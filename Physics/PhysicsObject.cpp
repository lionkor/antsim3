// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const vecd& pos, const vecd& size, World& world)
    : PhysicalObject(pos, size, world) {
}


std::stringstream PhysicsObject::to_stream() const {
    auto ss = PhysicalObject::to_stream();
    ss << "vel=" << m_vel << ";"
       << "collider=" << m_collider << ";";
    return ss;
}

void PhysicsObject::update() {
}
