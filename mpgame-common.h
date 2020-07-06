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
    UpdatePacket() { }
    UpdatePacket(const std::string& s, float x, float y)
        : name(s), x(x), y(y) { }

    enum Type
    {
        Update,     // "i have new information"
        Disconnect, // "im disconnecting gracefully"
    } type;

    std::string name;
    double      x;
    double      y;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar& type;
        ar& name;
        ar& x;
        ar& y;
    }
};

#endif // MPGAMECOMMON_H
