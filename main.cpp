#include "mbed.h"
#include "utils.hpp"

#include "EthernetInterface.h"
#include "frdm_client.hpp"

#include "metronome.hpp"

#define IOT_ENABLED

namespace active_low
{
	const bool on = false;
	const bool off = true;
}

DigitalOut g_led_red(LED1);
DigitalOut g_led_green(LED2);
DigitalOut g_led_blue(LED3);

InterruptIn g_button_mode(SW3);
InterruptIn g_button_tap(SW2);

void on_mode()
{
    // Change modes
}

void on_tap()
{
    // Receive a tempo tap
}

int main()
{
	// Seed the RNG for networking purposes
    unsigned seed = utils::entropy_seed();
    srand(seed);

	// LEDs are active LOW - true/1 means off, false/0 means on
	// Use the constants for easier reading
    g_led_red = active_low::on;
    g_led_green = active_low::on;
    g_led_blue = active_low::on;

	// Button falling edge is on push (rising is on release)
    g_button_mode.fall(&on_mode);
    g_button_tap.fall(&on_tap);

#ifdef IOT_ENABLED
	// Turn on the blue LED until connected to the network
    g_led_blue = active_low::off;

	// Need to be connected with Ethernet cable for success
    EthernetInterface ethernet;
    if (ethernet.connect() != 0)
        return 1;

	// Pair with the device connector
    frdm_client client("coap://api.connector.mbed.com:5684", &ethernet);
    if (client.get_state() == frdm_client::state::error)
        return 1;

	// The REST endpoints for this device
	// Add your own M2MObjects to this list with push_back before client.connect()
    M2MObjectList objects;

    M2MDevice* device = frdm_client::make_device();
    objects.push_back(device);

	// Publish the RESTful endpoints
    client.connect(objects);

	// Connect complete; turn off blue LED forever
    g_led_blue = active_low::on;
#endif

    while (true)
    {
#ifdef IOT_ENABLED
        if (client.get_state() == frdm_client::state::error)
            break;
#endif

        // Insert any code that must be run continuously here
    }

#ifdef IOT_ENABLED
    client.disconnect();
#endif

    return 1;
}
