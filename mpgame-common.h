#ifndef MPGAMECOMMON_H
#define MPGAMECOMMON_H

#include <string>
#include <sstream>
#include "fmt/format.h"
#include <arpa/inet.h>
#include <cstdint>

#include "Utils/ISerializable.h"

static constexpr size_t PACKET_SIZE = 128;

struct UpdatePacket : public ISerializable {
    UpdatePacket() {}
    UpdatePacket(const std::string& s, float x, float y)
        : name(s), x(x), y(y) { }

    std::string name;
    float       x;
    float       y;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar& name;
        ar& x;
        ar& y;
    }
};

#endif // MPGAMECOMMON_H
