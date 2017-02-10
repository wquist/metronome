#pragma once

#include "mbed.h"
#include "mbedtls/entropy_poll.h"

namespace utils
{
	unsigned entropy_seed()
	{
		unsigned seed;
		size_t length;

		unsigned char* seed_bytes = reinterpret_cast<unsigned char*>(&seed);
		mbedtls_hardware_poll(NULL, seed_bytes, sizeof(unsigned), &length);

		return seed;
	}

	// Default time of 0.2 seconds is fine for this lab
	void pulse(DigitalOut& led, float t = 0.2f)
	{
		led = false;
		wait(t);
		led = true;
	}
}
