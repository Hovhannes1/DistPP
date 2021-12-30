#include "message.h"

Message *Message::clone() const {
    Message *ptr = new Message(*this);
    return ptr;
}

