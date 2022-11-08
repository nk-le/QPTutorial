#pragma once
#include <Arduino.h>
#include <qpcpp.hpp>

namespace QPConfig {

    // Sys-tick rate used by QP as timer reference.
    static constexpr auto TICKS_PER_SECOND = 100;

     // The largest signal-id that QP expects.
    static constexpr auto MAX_SIG = 600;

    // Width of the QP message queue. No QP-message can be larger than this!
    static constexpr auto MAX_MESSAGE_LEN = 512;

    /// <summary>
    /// Dummy for a type with 512 byte for sizing the maximum message size.
    /// </summary>
    struct LargeType
    {
        uint8_t Dummy[512] = { 0U };
    };

    void init();
};