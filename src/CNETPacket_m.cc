//
// Generated file, do not edit! Created by opp_msgtool 6.1 from src/CNETPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "CNETPacket_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace cnetfinal {

Register_Class(CNETPacket)

CNETPacket::CNETPacket(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

CNETPacket::CNETPacket(const CNETPacket& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

CNETPacket::~CNETPacket()
{
}

CNETPacket& CNETPacket::operator=(const CNETPacket& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void CNETPacket::copy(const CNETPacket& other)
{
    this->sourceIP = other.sourceIP;
    this->destinationIP = other.destinationIP;
    this->sourcePort = other.sourcePort;
    this->destinationPort = other.destinationPort;
    this->protocol = other.protocol;
}

void CNETPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sourceIP);
    doParsimPacking(b,this->destinationIP);
    doParsimPacking(b,this->sourcePort);
    doParsimPacking(b,this->destinationPort);
    doParsimPacking(b,this->protocol);
}

void CNETPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sourceIP);
    doParsimUnpacking(b,this->destinationIP);
    doParsimUnpacking(b,this->sourcePort);
    doParsimUnpacking(b,this->destinationPort);
    doParsimUnpacking(b,this->protocol);
}

const char * CNETPacket::getSourceIP() const
{
    return this->sourceIP.c_str();
}

void CNETPacket::setSourceIP(const char * sourceIP)
{
    this->sourceIP = sourceIP;
}

const char * CNETPacket::getDestinationIP() const
{
    return this->destinationIP.c_str();
}

void CNETPacket::setDestinationIP(const char * destinationIP)
{
    this->destinationIP = destinationIP;
}

int CNETPacket::getSourcePort() const
{
    return this->sourcePort;
}

void CNETPacket::setSourcePort(int sourcePort)
{
    this->sourcePort = sourcePort;
}

int CNETPacket::getDestinationPort() const
{
    return this->destinationPort;
}

void CNETPacket::setDestinationPort(int destinationPort)
{
    this->destinationPort = destinationPort;
}

const char * CNETPacket::getProtocol() const
{
    return this->protocol.c_str();
}

void CNETPacket::setProtocol(const char * protocol)
{
    this->protocol = protocol;
}

class CNETPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_sourceIP,
        FIELD_destinationIP,
        FIELD_sourcePort,
        FIELD_destinationPort,
        FIELD_protocol,
    };
  public:
    CNETPacketDescriptor();
    virtual ~CNETPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(CNETPacketDescriptor)

CNETPacketDescriptor::CNETPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(cnetfinal::CNETPacket)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

CNETPacketDescriptor::~CNETPacketDescriptor()
{
    delete[] propertyNames;
}

bool CNETPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CNETPacket *>(obj)!=nullptr;
}

const char **CNETPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *CNETPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int CNETPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int CNETPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_sourceIP
        FD_ISEDITABLE,    // FIELD_destinationIP
        FD_ISEDITABLE,    // FIELD_sourcePort
        FD_ISEDITABLE,    // FIELD_destinationPort
        FD_ISEDITABLE,    // FIELD_protocol
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *CNETPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sourceIP",
        "destinationIP",
        "sourcePort",
        "destinationPort",
        "protocol",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int CNETPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "sourceIP") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destinationIP") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sourcePort") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "destinationPort") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "protocol") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *CNETPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_sourceIP
        "string",    // FIELD_destinationIP
        "int",    // FIELD_sourcePort
        "int",    // FIELD_destinationPort
        "string",    // FIELD_protocol
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **CNETPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CNETPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CNETPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void CNETPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'CNETPacket'", field);
    }
}

const char *CNETPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CNETPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceIP: return oppstring2string(pp->getSourceIP());
        case FIELD_destinationIP: return oppstring2string(pp->getDestinationIP());
        case FIELD_sourcePort: return long2string(pp->getSourcePort());
        case FIELD_destinationPort: return long2string(pp->getDestinationPort());
        case FIELD_protocol: return oppstring2string(pp->getProtocol());
        default: return "";
    }
}

void CNETPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceIP: pp->setSourceIP((value)); break;
        case FIELD_destinationIP: pp->setDestinationIP((value)); break;
        case FIELD_sourcePort: pp->setSourcePort(string2long(value)); break;
        case FIELD_destinationPort: pp->setDestinationPort(string2long(value)); break;
        case FIELD_protocol: pp->setProtocol((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CNETPacket'", field);
    }
}

omnetpp::cValue CNETPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceIP: return pp->getSourceIP();
        case FIELD_destinationIP: return pp->getDestinationIP();
        case FIELD_sourcePort: return pp->getSourcePort();
        case FIELD_destinationPort: return pp->getDestinationPort();
        case FIELD_protocol: return pp->getProtocol();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'CNETPacket' as cValue -- field index out of range?", field);
    }
}

void CNETPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sourceIP: pp->setSourceIP(value.stringValue()); break;
        case FIELD_destinationIP: pp->setDestinationIP(value.stringValue()); break;
        case FIELD_sourcePort: pp->setSourcePort(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destinationPort: pp->setDestinationPort(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_protocol: pp->setProtocol(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CNETPacket'", field);
    }
}

const char *CNETPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr CNETPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void CNETPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    CNETPacket *pp = omnetpp::fromAnyPtr<CNETPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CNETPacket'", field);
    }
}

}  // namespace cnetfinal

namespace omnetpp {

}  // namespace omnetpp

