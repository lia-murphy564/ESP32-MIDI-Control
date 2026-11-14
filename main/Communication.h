#pragma once 
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // placeholder

struct __attribute__((packed)) ControlMsg {
  uint8_t  type;
  uint8_t  control;
  uint16_t value;
};

struct Event {
  uint8_t    mac[6]; // sender
  ControlMsg msg;
};

enum class ComRole { Hub, Module };

class ComLink 
{
public:
  virtual ~ComLink() = default;

  virtual void begin() = 0;
  virtual void update() = 0;
  virtual void send(uint8_t type, uint8_t control, uint16_t value) = 0;
  // virtual void receive() = 0;

  // void send(uint8_t type, uint8_t control, uint16_t value) {
  //   return send(ControlMsg{type, control, value});
  // }
};

class ComLink_ESPNow : public ComLink
{
public:
  explicit ComLink_ESPNow(ComRole role) : role(role) {}

  void begin() override {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) 
    {
      Serial.println("ESP-NOW init failed");
      return;
    }

    if (role == ComRole::Hub) 
    {
      queue = xQueueCreate(32, sizeof(Event));
      esp_now_register_recv_cb(onReceive);
      // Serial.println("Hub ready");
    }

    if (role == ComRole::Module) 
    {
      esp_now_peer_info_t peer = {};
      memcpy(peer.peer_addr, broadcastAddr, 6);
      peer.channel = 0;
      peer.encrypt = false;
      esp_now_add_peer(&peer);
    }
  }

  void update() override
  {
    if (role == ComRole::Hub && queue)
    {
      Event e;
      while (xQueueReceive(queue, &e, 0)) 
      {
        Serial.printf("module ..%02X:%02X  type %u  ctl %u  value %u\n",
        e.mac[4], e.mac[5], e.msg.type, e.msg.control, e.msg.value);
      }
    }

    if (role == ComRole::Module) 
    {
      send(10, 11, 12); // example
      delay(100);
    }
  }

  void send(uint8_t type, uint8_t control, uint16_t value) override
  {
    if (role != ComRole::Module) return;
    ControlMsg msg = {type, control, value};
    esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
  }

private:
  static void onReceive(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
    if (len != sizeof(ControlMsg)) return;
    Event e;
    memcpy(e.mac, info->src_addr, 6);
    memcpy(&e.msg, data, sizeof(ControlMsg));
    xQueueSend(queue, &e, 0);
  }

  ComRole role;
  inline static QueueHandle_t queue = nullptr;
};