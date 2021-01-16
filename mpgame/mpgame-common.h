#ifndef MPGAMECOMMON_H
#define MPGAMECOMMON_H

#include "Utils/ISerializable.h"
#include "Utils/DebugTools.h"
#include "Physics/vec.h"

struct UpdatePacket {
    enum Type : uint8_t
    {
        Connect = 0x00,    // "i'm new"
        Update = 0x01,     // "i have new information"
        Disconnect = 0x02, // "im disconnecting gracefully"
        Ping = 0x03,       // "you still there?"
        Pong = 0x04,       // "i'm still here"
        Heartbeat = 0x05,  // "i'm alive"
    } type { Type::Update };

    UpdatePacket() { }
    UpdatePacket(const std::string& s, double x, double y, Type type = Type::Update)
        : name(s), x(x), y(y), type(type) { }


    std::string name;
    double x;
    double y;

    std::array<char, 128> to_binary() const {
        std::array<char, 128> array;
        std::fill(array.begin(), array.end(), 0);
        std::memcpy(&array[0], &x, 8);
        std::memcpy(&array[8], &y, 8);
        std::memcpy(&array[16], &type, 1);
        std::strcpy(&array[17], name.data());
        //report("made packet: _{}_ _{}_ _{}_ _{}_", x, y, type, name);
        //dump_hex(array.data(), 128);
        ASSERT(name[0] != 0);
        return array;
    }

    static UpdatePacket from_binary(const std::array<char, 128>& array) {
        UpdatePacket packet;
        packet.name.resize(128, ' ');
        std::memcpy(&packet.x, &array[0], 8);
        std::memcpy(&packet.y, &array[8], 8);
        std::memcpy(&packet.type, &array[16], 1);
        std::strcpy(&packet.name[0], &array[17]);
        packet.name = packet.name.substr(0, packet.name.find_first_of(' '));
        //report("got packet: _{}_ _{}_ _{}_ _{}_", packet.x, packet.y, int(packet.type), packet.name);
        //dump_hex(array.data(), 128);
        // dump_hex((char*)&packet.type, 1);
        ASSERT(packet.name[0] != 0);
        return packet;
    }
};

#endif // MPGAMECOMMON_H
