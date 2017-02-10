#pragma once

#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2minterface.h"

class frdm_client : public M2MInterfaceObserver
{
public:
    struct state
    {
        enum
        {
            initialized,
            boostrapped,
            registered,
            unregistered,
            error
        };
    };

public:
    static M2MDevice* make_device();

public:
    static const M2MInterface::NetworkStack stack_type = M2MInterface::LwIP_IPv4;
    static const M2MInterface::BindingMode socket_mode = M2MInterface::TCP;

public:
    frdm_client(const String& server_address, void* handler = NULL);
    ~frdm_client();

public:
    void connect(const M2MObjectList& objects);
    void disconnect();

    virtual void bootstrap_done(M2MSecurity* security) { if (security) m_state = state::boostrapped; }
    virtual void object_registered(M2MSecurity*, const M2MServer&) { m_state = state::registered; }
    virtual void object_unregistered(M2MSecurity*) { m_state = state::unregistered; }
    virtual void registration_updated(M2MSecurity*, const M2MServer&) {}
    virtual void value_updated(M2MBase*, M2MBase::BaseType) {}
    virtual void error(M2MInterface::Error error) { m_state = state::error; }

    int get_state() const { return m_state; }
    M2MSecurity* get_security() const { return m_security; }

private:
    String m_address;
    volatile int m_state;

    M2MInterface* m_interface;
    M2MSecurity* m_security;
};

