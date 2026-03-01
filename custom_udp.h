#pragma once

#include <esp_log.h>
#include <lwip/sockets.h>

static void send_dummy_udp(const char *ip) {
  int sock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sock < 0) {
    ESP_LOGE("custom_udp", "Unable to create socket: errno %d", errno);
    return;
  }

  struct sockaddr_in dest_addr;
  dest_addr.sin_addr.s_addr = inet_addr(ip);
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(9); // Standard Wake-on-LAN discard port

  const char *payload = "WAKEUP!";
  int err = sendto(sock, payload, strlen(payload), 0,
                   (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  if (err < 0) {
    ESP_LOGE("custom_udp", "Error occurred during sending: errno %d", errno);
  } else {
    ESP_LOGI("custom_udp", "Wakeup ping sent to %s", ip);
  }

  close(sock);
}
