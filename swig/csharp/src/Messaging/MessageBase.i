/*
  Copyright (c) 2017 Michael W. Powell <mwpowellhtx@gmail.com> All rights reserved.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.
*/

// Include the file we want to wrap a first time.
%{
#include "cpp/src/core/types.h"
#include "cpp/src/messaging/message_base.h"
%}

%module(directors="1") Messaging;

%include "std_vector.i"

namespace nng {

%ignore IClearable;
%ignore supports_getting_msg;
%ignore get_msgp;

%typemap(cstype) std::vector<uint8_t> "ByteVector"

%typemap(csimports) _MessageBase %{
using System;
using System.Runtime.InteropServices;
%}

%rename("MessageBase") _MessageBase;

%typemap(csclassmodifiers) _MessageBase "public abstract partial class"
%typemap(csinterfaces) _MessageBase "IHaveOne, IClearable"

// Ignore mappings for internally facing API.
%ignore _MessageBase::get_message() const;
%ignore _MessageBase::cede_message();
%ignore _MessageBase::retain(msg_type*);

}

%include "cpp/src/core/types.h"
%include "cpp/src/messaging/message_base.h"

SWIG_STD_VECTOR_ENHANCED(nng::uint8_t)
%template(ByteVector) std::vector<nng::uint8_t>;
