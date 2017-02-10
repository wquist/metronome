#pragma once

#include "mbed.h"

class metronome
{
public:
    enum { beat_samples = 3 };

public:
    metronome()
    : m_timing(false), m_beat_count(0) {}
    ~metronome() {}

public:
	// Call when entering "learn" mode
    void start_timing();
	// Call when leaving "learn" mode
    void stop_timing();

	// Should only record the current time when timing
	// Insert the time at the next free position of m_beats
    void tap();

    bool is_timing() const { return m_timing; }
	// Calculate the BPM from the deltas between m_beats
	// Return 0 if there are not enough samples
    size_t get_bpm() const;

private:
    bool m_timing;
    Timer m_timer;

	// Insert new samples at the end of the array, removing the oldest
    size_t m_beats[beat_samples];
    size_t m_beat_count;
};
