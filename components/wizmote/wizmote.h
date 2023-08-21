#pragma once

#include "esphome/components/esp_now/esp_now_component.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace wizmote {

typedef struct WizMotePacket {
  uint8_t bssid[6];

  uint8_t seq[4];       // Incremetal sequence number 32 bit unsigned integer LSB first
  uint8_t button;       // Identifies which button is being pressed

  uint32_t sequence;

  static inline WizMotePacket build(esp_now::ESPNowPacket espnow_packet) {
    WizMotePacket packet;
    memcpy(packet.bssid, espnow_packet.get_bssid().data(), 6);
    memcpy(&(packet.seq), espnow_packet.get_data().data(), espnow_packet.get_data().size());
    packet.sequence = encode_uint32(packet.seq[3], packet.seq[2], packet.seq[1], packet.seq[0]);
    return packet;
  }
} WizMotePacket;

class WizMoteListener : public esp_now::ESPNowListener {
 public:
  void on_esp_now_message(esp_now::ESPNowPacket packet) override;

  Trigger<WizMotePacket> *get_on_button_trigger() { return this->on_button_; }

 protected:
  uint32_t last_sequence_ = 0;
  Trigger<WizMotePacket> *on_button_ = new Trigger<WizMotePacket>();
};

}  // namespace wizmote
}  // namespace esphome
