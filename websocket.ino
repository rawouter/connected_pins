//////////////
// WEBSOCKET
//////////////
#ifdef USE_WEBSOCKET

#include <WebSockets.h>
#include <WebSocketsClient.h>

#define SSOCKETURL "shrouded-bayou-62366.herokuapp.com"

WebSocketsClient webSocket;

// I don't know why we need this !
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);

void setup_websocket() {
  webSocket.beginSSL(SSOCKETURL, 443);
  webSocket.onEvent(webSocketEvent);
}

void check_websocket() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  digitalWrite(2, HIGH * 0.8);
  switch (type) {
    case WStype_DISCONNECTED:
      serprintf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        serprintf("[WSc] Connected to url: %s\n",  payload);
        webSocket.sendTXT("Connected"); // Say hello to server when connected
      }
      break;
    case WStype_TEXT:
      set_color_from_json((char*) payload);
      break;
    case WStype_BIN:
#ifdef DEBUG
      serprintf("[WSc] get binary length: %u\n", length);
#endif
      hexdump(payload, length);
      break;
  }
  digitalWrite(2, HIGH);
}

#endif
