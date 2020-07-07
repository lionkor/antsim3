// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along 
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef MATH_H
#define MATH_H

template<class FloatT>
decltype(auto) lerp(FloatT a, FloatT b, FloatT f) {
    return a + f * (b - a);
}

#endif // MATH_H
