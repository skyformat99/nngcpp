#include "binary_message.h"

#include "../core/nng_exception.h"

#include <functional>

namespace nng {

    namespace messaging {

        binary_message::binary_message()
            : message_base()
            , _header()
            , _body() {
        }

        binary_message::binary_message(::nng_msg* msgp)
            : message_base(msgp)
            , _header(msgp)
            , _body(msgp) {
        }

        binary_message::~binary_message() {
            free();
        }

        binary_message::operator std::string() {
            std::string s;
            const auto buf = body()->get();
            for (auto it = buf.begin(); it != buf.end(); it++) {
                s.push_back(((std::string::value_type*)*it)[0]);
            }
            return s;
        }

        binary_message_header* const binary_message::header() {
            return &_header;
        }

        binary_message_body* const binary_message::body() {
            return &_body;
        }

        message_base::size_type binary_message::get_size() const {
            // Should be both (Header.Size + Body.Size).
            return _header.get_size() + _body.get_size();
        }

        void binary_message::clear() {
            _header.clear();
            _body.clear();
        }

        void binary_message::set_msgp(::nng_msg* msgp) {
            // Frees any previously existing internal message beforehand.
            message_base::set_msgp(msgp);
            _header.set_msgp(msgp);
            _body.set_msgp(msgp);
        }

        void binary_message::allocate(size_type sz) {
            if (_msgp != nullptr) { return; }
            const auto errnum = ::nng_msg_alloc(&_msgp, sz);
            THROW_NNG_EXCEPTION_EC(errnum);
            // This may be a little redundant at that point, but it will work.
            binary_message::set_msgp(_msgp);
        }

        void binary_message::free() {
            if (_msgp == nullptr) { return; }
            ::nng_msg_free(_msgp);
            _msgp = nullptr;
            _header.set_msgp(_msgp);
            _body.set_msgp(_msgp);
        }
    }
}