#ifdef ESP_USE_BUTTON

bool brightDirection;
static bool startButtonHolding = false;                     // флаг: кнопка удерживается для изменения яркости/скорости/масштаба лампы кнопкой


void buttonTick()
{
  touch.tick();
  uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;
  int8_t currentMode = db[kk::current_eff];


  // однократное нажатие
  if (clickCount == 1U)
  {
    ONflag = !ONflag;
    changePower();
    loadingFlag = true;
  }


  // двухкратное нажатие
  if (ONflag && clickCount == 2U)
  {
    if (++currentMode >= (int8_t)MODE_AMOUNT) currentMode = 0;
    db[kk::current_eff] = currentMode;
    FastLED.setBrightness(modes[currentMode].bright);
    loadingFlag = true;
    FastLED.clear();
    delay(1);
  }


  // трёхкратное нажатие
  if (ONflag && clickCount == 3U)
  {
    if (--currentMode < 0) currentMode = MODE_AMOUNT - 1;
    db[kk::current_eff] = currentMode;
    FastLED.setBrightness(modes[currentMode].bright);
    loadingFlag = true;
    FastLED.clear();
    delay(1);
  }


  // кнопка только начала удерживаться
  if (ONflag && touch.isHolded())
  {
    brightDirection = !brightDirection;
    startButtonHolding = true;
  }


  // кнопка нажата и удерживается
  if (ONflag && touch.isStep())
  {
    switch (touch.getHoldClicks())
    {
      case 0U:                                              // просто удержание (до удержания кнопки кликов не было) - изменение яркости
      {
        uint8_t delta = modes[currentMode].bright < 10U // определение шага изменения яркости: при яркости [1..10] шаг = 1, при [11..16] шаг = 3, при [17..255] шаг = 15
          ? 1U
          : 5U;
        modes[currentMode].bright =
          constrain(brightDirection
            ? modes[currentMode].bright + delta
            : modes[currentMode].bright - delta,
          1, 255);
        db[SH(getCurrentEffectKey())] = modes[currentMode];
        currentState = modes[currentMode];

        FastLED.setBrightness(modes[currentMode].bright);

        #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение яркости: %d\n"), modes[currentMode].bright);
        #endif

        break;
      }

      case 1U:                                              // удержание после одного клика - изменение скорости
      {
        modes[currentMode].speed = constrain(brightDirection ? modes[currentMode].speed + 1 : modes[currentMode].speed - 1, effects[currentMode].min_speed,
        effects[currentMode].max_speed);
        db[SH(getCurrentEffectKey())] = modes[currentMode];
        currentState = modes[currentMode];

        #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение скорости: %d\n"), modes[currentMode].speed);
        #endif

        break;
      }

      case 2U:                                              // удержание после двух кликов - изменение масштаба
      {
        modes[currentMode].scale = constrain(brightDirection ? modes[currentMode].scale + 1 : modes[currentMode].scale - 1, effects[currentMode].min_scale,
        effects[currentMode].max_scale);
        db[SH(getCurrentEffectKey())] = modes[currentMode];
        currentState = modes[currentMode];

        #ifdef GENERAL_DEBUG
        LOG.printf_P(PSTR("Новое значение масштаба: %d\n"), modes[currentMode].scale);
        #endif

        break;
      }

      default:
        break;
    }
  }


  // кнопка отпущена после удерживания
  if (ONflag && !touch.isHold() && startButtonHolding)      // кнопка отпущена после удерживания, нужно отправить MQTT сообщение об изменении яркости лампы
  {
    startButtonHolding = false;
    loadingFlag = true;
  }
}
#endif
