#ifndef NNGCPP_MESSAGE_PIPE_H
#define NNGCPP_MESSAGE_PIPE_H

#define NNG_ONLY
#include <nngcpp.h>

#include "../core/having_one.hpp"
#include "../core/can_close.hpp"

#include "../core/options.h"

#include <functional>
#include <algorithm>

/* Due to some namespace weirdness during the include graph forward declarations, this could
not live in another namespace for whatever reason. After careful consideration, decided that
Message Pipe is closer in stature to Dialer, Listener, and even Socket, even though it bridges
the API into Messaging. Therefore, decided that it should live in the nng::* namespace. */

namespace nng {

#ifndef NNGCPP_ADDRESS_H
    class address;
#endif //NNGCPP_ADDRESS_H

#ifndef NNGCPP_MESSAGE_BASE_H
    class message_base;
#endif // NNGCPP_MESSAGE_BASE_H

    // TODO: so, equal_to is available, but not_equal_to is not?
    class message_pipe : public having_one, public can_close, public std::equal_to<message_pipe> {
    public:

        typedef ::nng_pipe nng_type;

    protected:

        nng_type pid;

        msg_type* _msgp;

    private:

        options_reader _options;

        typedef std::function<::nng_pipe()> get_pipe_func;
        typedef std::function<void()> set_pipe_func;
        typedef std::function<int()> close_pipe_func;

        get_pipe_func __getter;
        set_pipe_func __setter;
        close_pipe_func __closer;

        //void configure(msg_type* msgp);
        void configure(nng_type pid, msg_type* msgp = nullptr);

        virtual void set(msg_type* const msgp);

    public:

        // TODO: TBD: is exposing the raw pointer ctor really necessary? may make it protected at least? if at all...
        message_pipe(msg_type* const msgp);

        message_pipe(message_base* const mbp);

        virtual ~message_pipe();

        virtual bool has_one() const override;

        virtual void close() override;

        virtual void reset();

        virtual void set(message_base* const mbp);

        virtual options_reader* const options();

        bool operator==(const message_pipe& rhs);
        // TODO: TBD: so, apparently there is no std::not_equal_to available from the Microsoft implementation, however, we should still be able to override the operator
        bool operator!=(const message_pipe& rhs);
    };
}

#endif // NNGCPP_MESSAGE_PIPE_H
