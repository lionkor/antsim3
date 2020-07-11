// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#include "UDPClient.h"


UDPClient::UDPClient(boost::asio::io_service& io_service, const std::string& host, uint16_t port)
    : m_io_service(io_service), m_socket(m_io_service, boost_ip::udp::endpoint(boost_ip::udp::v4(), 0)) {
    m_endpoint = resolve_endpoint(m_io_service, host, port);
}

UDPClient::~UDPClient() {
    m_socket.close();
}

std::vector<std::string> UDPClient::receive() {
    std::vector<std::string> data;
    while (m_socket.available() > 0) {
        std::string str;
        str.resize(PACKET_SIZE, ' ');
        m_socket.receive_from(boost::asio::buffer(str.data(), PACKET_SIZE), m_endpoint);
        //report("read {} bytes: _{}_", str.size(), str);
        data.push_back(str);
    }

    return data;
}
