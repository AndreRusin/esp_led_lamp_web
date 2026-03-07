#pragma once

struct EffectState {
    uint8_t bright;
    uint8_t speed;
    uint8_t scale;
};

struct EffectInfo {
    const char* name;
    uint16_t min_speed;
    uint16_t max_speed;
    uint16_t min_scale;
    uint16_t max_scale;
    bool hasColor;
};
