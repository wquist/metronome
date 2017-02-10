#include "frdm_client.hpp"

#include "mbed-client/m2mdevice.h"
#include "security.h"

M2MDevice* frdm_client::make_device()
{
    M2MDevice* device = M2MInterfaceFactory::create_device();
    if (!device)
        return NULL;

    device->create_resource(M2MDevice::Manufacturer, "Freescale");
    device->create_resource(M2MDevice::DeviceType, "FRDM");
    device->create_resource(M2MDevice::ModelNumber, "K64F");

    return device;
}

frdm_client::frdm_client(const String& server_address, void* handler)
: m_address(server_address), m_state(state::initialized)
{
    String endpoint_type = "";
    String context_addr = "";

    int32_t lifetime = 100;
    uint16_t port = 0;

    m_interface = M2MInterfaceFactory::create_interface
    (
        *this,
        MBED_ENDPOINT_NAME, endpoint_type,
        lifetime,
        port, MBED_DOMAIN, socket_mode, stack_type, context_addr
    );

    if (!m_interface)
    {
        m_state = state::error;
        return;
    }

    m_security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
    if (!m_security)
    {
        m_state = state::error;
        return;
    }

    m_security->set_resource_value(M2MSecurity::M2MServerUri, m_address);
    m_security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
    m_security->set_resource_value(M2MSecurity::ServerPublicKey, SERVER_CERT, sizeof(SERVER_CERT)-1);
    m_security->set_resource_value(M2MSecurity::PublicKey, CERT, sizeof(CERT)-1);
    m_security->set_resource_value(M2MSecurity::Secretkey, KEY, sizeof(KEY)-1);

    m_interface->set_platform_network_handler(handler);
}

frdm_client::~frdm_client()
{
    delete m_interface;
    delete m_security;
}

void frdm_client::connect(const M2MObjectList& objects)
{
    m_interface->register_object(m_security, objects);
}

void frdm_client::disconnect()
{
    m_interface->unregister_object(NULL);
}

