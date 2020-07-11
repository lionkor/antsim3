// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef COMMON_H
#define COMMON_H

#include <cstdint>
#include <functional>

using std::size_t;

template<typename T>
using Ref = std::reference_wrapper<T>;

#define make_ref std::ref

#endif // COMMON_H
