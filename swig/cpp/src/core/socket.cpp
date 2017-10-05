#include <vector>

#include "socket.h"
#include "dialer.h"
#include "listener.h"
#include "nng_exception.h"

namespace nng {

    socket::socket(const nng_ctor_func& nng_ctor)
        : sid(0)
            , sender()
            , receiver()
            , options() {

        auto ec = nng_ctor(&sid);
        // TODO: TBD: if ec is other than 0, throw nng_exception ...
    }

    socket::~socket() {
        close();
    }

    // TODO: TBD: ditto ec handling...
    void socket::listen(const std::string& addr, int flags) {
        auto ec = ::nng_listen(sid, addr.c_str(), nullptr, flags);
    }

    void socket::listen(const std::string& addr, listener* const lp, int flags) {
        auto ec = ::nng_listen(sid, addr.c_str(), lp ? &(lp->lid) : nullptr, flags);
    }

    void socket::dial(const std::string& addr, int flags) {
        auto ec = ::nng_dial(sid, addr.c_str(), nullptr, flags);
    }

    void socket::dial(const std::string& addr, dialer* const dp, int flags) {
        auto ec = ::nng_dial(sid, addr.c_str(), dp ? &(dp->did) : nullptr, flags);
    }

    void socket::close() {
        if (sid) {
            auto ec = ::nng_close(sid);
            sid = 0;
        }
    }

    template<class Buffer>
    int send(int id, Buffer const& buffer, std::size_t sz, int flags) {
        auto ec = ::nng_send(id, (void*)&buffer[0], sz, flags);
        return ec;
    }
    
    template<class Buffer>
    int try_receive(int id, Buffer& buffer, std::size_t& sz, int flags) {
        buffer.resize(sz);
        auto ec = ::nng_recv(id, &buffer[0], &sz, flags);
        return ec;
    }

    int socket::send(const std::string& str, int flags) {
        return nng::send(sid, str, str.length() + 1, flags);
    }

    int socket::send(const std::string& str, send_size_type sz, int flags) {
        return nng::send(sid, str, sz, flags);
    }

    int socket::send(const send_vector& buffer, int flags) {
        return nng::send(sid, buffer, buffer.size(), flags);
    }

    int socket::send(const send_vector& buffer, send_size_type sz, int flags) {
        return nng::send(sid, buffer, sz, flags);
    }

    int socket::try_receive(std::string& str, receive_size_type sz, int flags) {
        return nng::try_receive(sid, str, sz, flags);
    }

    std::string socket::receive_str(receive_size_type& sz, int flags) {
        std::string str;
        auto ec = nng::try_receive(sid, str, sz, flags);
        return str;
    }

    int socket::try_receive(receive_vector& buffer, receive_size_type& sz, int flags) {
        return nng::try_receive(sid, buffer, sz, flags);
    }

    socket::receive_vector socket::receive_buffer(receive_size_type& sz, int flags) {
        receive_vector buffer;
        auto ec = nng::try_receive(sid, buffer, sz, flags);
        return buffer;
    }

    // Convenience option wrappers.
    void socket::set_option_int(const std::string& name, int val) {
        auto ec = ::nng_setopt_int(sid, name.c_str(), val);
    }

    void socket::set_option_size(const std::string& name, option_size_type val) {
        auto ec = ::nng_setopt_size(sid, name.c_str(), val);
    }

    void socket::set_option_usec(const std::string& name, uint64_t val) {
        auto ec = ::nng_setopt_usec(sid, name.c_str(), val);
    }

    void socket::get_option_int(const std::string& name, int* valp) {
        auto ec = ::nng_getopt_int(sid, name.c_str(), valp);
    }

    void socket::get_option_size(const std::string& name, option_size_type* valp) {
        auto ec = ::nng_getopt_size(sid, name.c_str(), valp);
    }

    void socket::get_option_usec(const std::string& name, uint64_t* valp) {
        auto ec = ::nng_getopt_usec(sid, name.c_str(), valp);
    }

    protocol_type to_protocol_type(int value) {
        return static_cast<protocol_type>(value);
    }

    flag_type to_flag_type(int value) {
        return static_cast<flag_type>(value);
    }

    int to_int(const protocol_type value) {
        return static_cast<int>(value);
    }

    int to_int(const flag_type value) {
        return static_cast<int>(value);
    }

    protocol_type socket::get_protocol() const {
        return to_protocol_type(::nng_protocol(sid));
    }

    protocol_type socket::get_peer() const {
        return to_protocol_type(::nng_peer(sid));
    }

    bool socket::is_protocol_configured() const {
        return get_protocol() != protocol_none;
    }

    bool socket::is_peer_configured() const {
        return get_peer() != protocol_none;
    }
}
