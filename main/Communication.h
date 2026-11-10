#pragma once 
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Broadcast address: every ESP-NOW device on the channel hears it,
// so you don't need to know the hub's MAC for a first test.
uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct __attribute__((packed)) ControlMsg {
  uint8_t  type;
  uint8_t  control;
  uint16_t value;
};

struct Event {
  uint8_t    mac[6];   // sender's MAC = which module it came from
  ControlMsg msg;
};

QueueHandle_t events;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
  if (len != sizeof(ControlMsg)) return;
  Event event;
  memcpy(event.mac, info->src_addr, 6);
  memcpy(&event.msg, data, sizeof(ControlMsg));
  xQueueSend(events, &event, 0);
}

void sendControl(uint8_t type, uint8_t control, uint16_t value)
{
  ControlMsg msg = {type, control, value};
  esp_now_send(broadcastAddr, (uint8_t*)&msg, sizeof(msg));
}

void hub_setup()
{
  events = xQueueCreate(32, sizeof(Event));

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onReceive);
  Serial.println("Hub ready");
}

void hub_loop()
{
  Event e;
  while (xQueueReceive(events, &e, 0)) 
  {
    // This is where the hub will map events to MIDI later
    Serial.printf("module ..%02X:%02X  type %u  ctl %u  value %u\n",
    e.mac[4], e.mac[5], e.msg.type, e.msg.control, e.msg.value);
  }
}

void module_setup()
{
  WiFi.mode(WIFI_STA);              // radio on, not joined to any network
  if (esp_now_init() != ESP_OK) 
  {
      Serial.println("ESP-NOW init failed");
      return;
  }

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, broadcastAddr, 6);
  peer.channel = 0;                 // 0 = current channel
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void module_loop()
{
  sendControl(4, 5, 6); // Example: send a control message with type=0, control=0, value=0
}