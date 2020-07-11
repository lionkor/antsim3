// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "DebugTools.h"

class ISerializable
{
private:
public:
    friend class boost::serialization::access;

    /* IMPLEMENT THIS IN ALL INHERITING CLASSES
    template<class Archive>
    void serialize(Archive & ar, const unsigned int) {
    }
    */
};


template<class T>
[[nodiscard]] static inline std::string serialize_into_string(const T& object) {
    std::ostringstream oss;
    boost::archive::text_oarchive archive(oss);
    archive << object;
    return oss.str();
}

template<class T, size_t size>
[[nodiscard]] static inline std::array<std::uint8_t, size> serialize_into_array(const T& object) {
    std::ostringstream oss;
    boost::archive::text_oarchive archive(oss);
    archive << object;
    auto str = oss.str();
    ASSERT(str.size() <= size); // invalid size if this fails
    std::array<std::uint8_t, size> result;
    result.fill(std::uint8_t(' ')); // padding with zeroes
    std::copy(str.begin(), str.end(), result.data());
    return result;
}

template<class T>
[[nodiscard]] static inline T deserialize_from_string(const std::string& str) {
    std::istringstream iss(str);
    boost::archive::text_iarchive archive(iss);
    T object;
    archive >> object;
    return object;
}

#endif // ISERIALIZABLE_H
