#include <Arduino.h>
#include <MTSimpleBroadcast.hpp>

// Pi Pico SPI1 pins
#define PIN_SCK 10
#define PIN_MOSI 11
#define PIN_MISO 12
#define PIN_CS 4

// LoRa
#define PIN_IRQ 20 // DIO1
#define PIN_BUSY 2
#define PIN_RST 3

// MediumFast Meshtastic Config
LoraConfig lora_config_mt = {
    /*.frequency = */ 913.125,  // config
    /*.bandwidth = */ 250,      // config
    /*.spreading_factor = */ 9, // config
    /*.coding_rate = */ 5,      // config
    /*.sync_word = */ 0x2B,
    /*.preamble_length = */ 16,
    /*.output_power = */ 22, // config
    /*.tcxo_voltage = */ 1.8,
    /*.use_regulator_ldo = */ false,
}; //

MTSimpleBroadcast mtsb;

void setup() {
    // Initialize SPI1 for LoRa radio
    SPI1.setSCK(PIN_SCK);
    SPI1.setTX(PIN_MOSI);
    SPI1.setRX(PIN_MISO);
    SPI1.begin();

    // Initialize Random with a floating pin
    randomSeed(analogRead(A0));

    // Initialize Radio
    mtsb.RadioInit(SPI1, PIN_CS, PIN_IRQ, PIN_RST, PIN_BUSY, lora_config_mt);
    mtsb.setSendHopLimit(2);

    // Configure Node Info
    uint32_t node_id = 0xABBABAAB;
    std::string short_name = "rppt";
    std::string long_name = "pi_pico_test";
    uint8_t hardware_model = 44;
    MTHelpers::NodeInfoBuilder(mtsb.getMyNodeInfo(), node_id, short_name, long_name, hardware_model);

    // Define Channel
    MTSB_ChannelEntry channel("MediumFast", "AQ==");

    // Broadcast Node Info
    mtsb.broadcastMyNodeInfo(channel);
    delay(500);

    // Broadcast Text Message
    std::string test_msg = "Hello World!";
    mtsb.broadcastTextMessage(test_msg, channel);
}

void loop() {}
