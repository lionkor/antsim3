#ifndef MPGAMECOMMON_H
#define MPGAMECOMMON_H

#include <string>
#include <sstream>
#include "fmt/format.h"
#include <arpa/inet.h>
#include <cstdint>

static_assert(sizeof(float) == sizeof(std::uint32_t));

static inline std::string pad(size_t size, char pad_char) {
    return std::string(size, pad_char);
}

struct UpdatePacket {
    char  name[16];
    float x;
    float y;

    std::string to_network_data() {
        // name x y
        name[sizeof(name) - 1] = '\0';
        auto result            = fmt::format("{}{}{} {}", name, pad(sizeof(name) - std::strlen(name) - 1, ' '), htonl(*reinterpret_cast<std::uint32_t*>(&x)), htonl(*reinterpret_cast<std::uint32_t*>(&y)));
        return fmt::format("{}{}", result, pad(UpdatePacket::network_size() - result.size(), ' '));
    }

    static size_t network_size() { return 16 + 11 + 11 + 1; }

    static UpdatePacket from_network_data(const std::string& str) {
        std::stringstream ss(str);
        UpdatePacket      packet;
        ss.read(packet.name, sizeof(packet.name) - 1);
        auto space_iter = std::find(packet.name, packet.name + 16, ' ');
        if (space_iter != packet.name + 16) {
            packet.name[space_iter - packet.name] = 0;
        }
        std::uint32_t temp;
        ss >> temp;
        temp     = ntohl(temp);
        packet.x = *reinterpret_cast<float*>(&temp);
        ss >> temp;
        temp     = ntohl(temp);
        packet.y = *reinterpret_cast<float*>(&temp);
        return packet;
    }

    std::string clean_name() const {
    }
};

#endif // MPGAMECOMMON_H
