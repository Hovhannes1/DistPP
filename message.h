#ifndef MESSAGE_H
#define MESSAGE_H
#include <QSharedPointer>

class Message;
typedef QSharedPointer<Message> MessagePtr;

class Message {
public:
    Message(unsigned short id):messageId(id) {}
    virtual ~Message() {}
    unsigned short getId() { return messageId; }
    virtual Message *clone() const;
protected:
    unsigned short messageId;
};

template <class T>
class MessageOf:public Message {
public:
    MessageOf(unsigned short id,T ptr):Message(id),ptrData(ptr) {};
    T getData() { return ptrData; }
    virtual Message *clone() const override {
        MessageOf<T> *ptr = new MessageOf<T>(messageId,ptrData);
        return ptr;
    };
private:
    T ptrData;
};

#endif // MESSAGE_H
