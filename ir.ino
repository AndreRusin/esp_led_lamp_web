#ifdef ESP_USE_IR

void irTick()
{
  if (irrecv.decode(&results)) {
    String irCode = resultToHexidecimal(&results);
    int8_t currentMode = db[kk::current_eff];

    // вкл
    if (irCode == "0xF7C03F") {
      if (!ONflag) {
        ONflag = true;
        loadingFlag = true;
        changePower();
      }
    }
    // выкл
    if (irCode == "0xF740BF") {
      if (ONflag) {
        ONflag = false;
        changePower();
      }
    }

    // след эффект
    if (ONflag && irCode == "0xF7708F") {
      if (++currentMode >= (int8_t)MODE_AMOUNT) currentMode = 0;
      db[kk::current_eff] = currentMode;
      FastLED.setBrightness(modes[currentMode].bright);
      loadingFlag = true;
      FastLED.clear();
      delay(1);
    }

    // пред эффект
    if (ONflag && irCode == "0xF730CF") {
      if (--currentMode < 0) currentMode = MODE_AMOUNT - 1;
      db[kk::current_eff] = currentMode;
      FastLED.setBrightness(modes[currentMode].bright);
      loadingFlag = true;
      FastLED.clear();
      delay(1);
    }

    // яркость +
    if (ONflag && irCode == "0xF700FF") {
      uint8_t delta = modes[currentMode].bright < 10U // определение шага изменения яркости: при яркости [1..10] шаг = 1, при [11..16] шаг = 3, при [17..255] шаг = 15
          ? 1U
          : 15U;
        modes[currentMode].bright =
          constrain(modes[currentMode].bright + delta, 1, 255);
        db[SH(getCurrentEffectKey())] = modes[currentMode];
        currentState = modes[currentMode];
        FastLED.setBrightness(modes[currentMode].bright);

        #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), modes[currentMode].bright);
        #endif
    }

    // яркость -
    if (ONflag && irCode == "0xF7807F") {
      uint8_t delta = modes[currentMode].bright < 10U // определение шага изменения яркости: при яркости [1..10] шаг = 1, при [11..16] шаг = 3, при [17..255] шаг = 15
          ? 1U
          : 15U;
        modes[currentMode].bright =
          constrain(modes[currentMode].bright - delta, 1, 255);
        db[SH(getCurrentEffectKey())] = modes[currentMode];
        currentState = modes[currentMode];
        FastLED.setBrightness(modes[currentMode].bright);

        #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), modes[currentMode].bright);
        #endif
    }

    // изменение скорости + зел(10)
    if (ONflag && irCode == "0xF7906F") {
      modes[currentMode].speed = constrain(
        modes[currentMode].speed + 1,
        effects[currentMode].min_speed,
        effects[currentMode].max_speed
        );
      db[SH(getCurrentEffectKey())] = modes[currentMode];
      currentState = modes[currentMode];

      #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), modes[currentMode].speed);
      #endif
    }

    // изменение скорости - голуб(18)
    if (ONflag && irCode == "0xF78877") {
      modes[currentMode].speed = constrain(
        modes[currentMode].speed - 1,
        effects[currentMode].min_speed,
        effects[currentMode].max_speed
        );
      db[SH(getCurrentEffectKey())] = modes[currentMode];
      currentState = modes[currentMode];

      #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), modes[currentMode].speed);
      #endif
    }

    // изменение масштаба + красн(9)
    if (ONflag && irCode == "0xF710EF") {
      modes[currentMode].scale = constrain(
        modes[currentMode].scale + 1,
        effects[currentMode].min_scale,
        effects[currentMode].max_scale
        );
      db[SH(getCurrentEffectKey())] = modes[currentMode];
      currentState = modes[currentMode];

      #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), modes[currentMode].scale);
      #endif
    }
    // изменение масштаба - синяя(11)
    if (ONflag && irCode == "0xF750AF") {
      modes[currentMode].scale = constrain(
        modes[currentMode].scale - 1,
        effects[currentMode].min_scale,
        effects[currentMode].max_scale
        );
      db[SH(getCurrentEffectKey())] = modes[currentMode];
      currentState = modes[currentMode];

      #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), modes[currentMode].scale);
      #endif
    }

    irrecv.resume();
  }
}
#endif
