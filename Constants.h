#pragma once

#define AP_NAME               ("LedLamp")                   // имя WiFi точки доступа, используется как при запросе SSID и пароля WiFi сети роутера, так и при работе в режиме ESP_MODE = 0
#define AP_PASS               ("12345678")  

#define ESP_USE_IR

#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе ESP может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#if defined (ESP_USE_BUTTON)
#define BUTTON_STEP_TIMEOUT   (100U)                        // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#endif

//#define GENERAL_DEBUG                                       // если строка не закомментирована, будут выводиться отладочные сообщения
#define LED_PIN               (2U)                          // пин ленты                (D4)
#define BTN_PIN               (13U)                          // пин кнопки               (D7)
#define BTN_IR               (12U)                           // пин ir приемника         (D6)

// --- МАТРИЦА -------------------------
#define BRIGHTNESS            (40U)                         // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT         (2000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH                 (8U)                         // ширина матрицы
#define HEIGHT                (18U)                         // высота матрицы

#define COLOR_ORDER           (RGB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE           (1U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (3U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (1U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/


#define EFF_SPARKLES          (0U)                          // Конфетти
#define EFF_FIRE              (1U)                          // Огонь
#define EFF_WHITTE_FIRE       (2U)                          // Белый огонь
#define EFF_RAINBOW_VER       (3U)                          // Радуга вертикальная
#define EFF_RAINBOW_HOR       (4U)                          // Радуга горизонтальная
#define EFF_RAINBOW_DIAG      (5U)                          // Радуга диагональная
#define EFF_COLORS            (6U)                          // Смена цвета
#define EFF_MADNESS           (7U)                          // Безумие 3D
#define EFF_CLOUDS            (8U)                          // Облака 3D
#define EFF_LAVA              (9U)                          // Лава 3D
#define EFF_PLASMA            (10U)                         // Плазма 3D
#define EFF_RAINBOW           (11U)                         // Радуга 3D
#define EFF_RAINBOW_STRIPE    (12U)                         // Павлин 3D
#define EFF_ZEBRA             (13U)                         // Зебра 3D
#define EFF_FOREST            (14U)                         // Лес 3D
#define EFF_OCEAN             (15U)                         // Океан 3D
#define EFF_COLOR             (16U)                         // Цвет
#define EFF_SNOW              (17U)                         // Снегопад
#define EFF_SNOWSTORM         (18U)                         // Метель
#define EFF_STARFALL          (19U)                         // Звездопад
#define EFF_MATRIX            (20U)                         // Матрица
#define EFF_LIGHTERS          (21U)                         // Светлячки
#define EFF_LIGHTER_TRACES    (22U)                         // Светлячки со шлейфом
#define EFF_PAINTBALL         (23U)                         // Пейнтбол
#define EFF_CUBE              (24U)                         // Блуждающий кубик
#define EFF_WHITE_COLOR       (25U)                         // Белый свет
#define MODE_AMOUNT           (26U)                         // количество режимов


// ================ РЕЕСТР ДОСТУПНЫХ ЭФФЕКТОВ ===================
// ==== ДЛЯ ПЕРЕДАЧИ В ПРИЛОЖЕНИЯ С ПОДДЕРЖКОЙ ЭТОЙ ФУНКЦИИ =====
// формат записи:
//"Название эффекта,min_скорость,max_скорость,min_масштаб,max_масштаб,выбор_ли_цвета_это(0-нет,1-да, 2-для кометы);"
constexpr EffectInfo effects[] = {
    {"Конфетти", 1,255, 1,100,false},
    {"Огонь", 50,255, 1,100,true},
    {"Водопад", 50,255, 1,100,true},
    {"Радуга горизонтальная", 50,255, 1,50,false},
    {"Радуга вертикальная", 50,255, 1,50,false},
    {"Радуга диагональная", 50,255, 1,100,false},
    {"Пульс + Смена цвета", 50,255, 1,100,false},
    {"Безумие", 1,150, 1,100,false},
    {"Облака", 1,15, 1,50,false},
    {"Лава", 5,60, 1,100,false},
    {"Плазма", 1,30, 1,100,false},
    {"Радуга 3D", 1,70, 1,100,false},
    {"Павлин", 1,15, 1,30,false},
    {"Зебра", 1,30, 7,40,false},
    {"Лес", 2,30, 70,100,false},
    {"Океан", 2,15, 4,30,false},
    {"Бассейн + Цвет", 100,255, 1,100,true},
    {"Снегопад", 50,255, 1,100,false},
    {"Метель", 50,255, 1,100,false},
    {"Звездопад", 50,255, 1,100,false},
    {"Матрица", 100,240, 1,100,false},
    {"Светлячки", 50,240, 1,100,false},
    {"Светлячки со шлейфом", 60,255, 1,100,false},
    {"Пейнтбол", 215,255, 1,100,false},
    {"Блуждающий кубик", 70,235, 1,100,false},
    {"Белый свет", 1,255, 1,100,false}
};

constexpr uint8_t EFFECT_COUNT = sizeof(effects)/sizeof(effects[0]);
String getEffectsNames()
{
    String result;
    for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
        result += effects[i].name;
        if (i < EFFECT_COUNT - 1) result += ";";
    }

    return result;
}

// === ОСТАЛЬНОЕ ДЛЯ РАЗРАБОТЧИКОВ =====

#define LOG Serial

// --- БИБЛИОТЕКИ ----------------------
#ifdef FASTLED_INTERRUPT_RETRY_COUNT
#undef FASTLED_INTERRUPT_RETRY_COUNT
#endif
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
//#define FASTLED_ALLOW_INTERRUPTS      (1)                 // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER

#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
