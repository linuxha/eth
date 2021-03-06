#include "utility/w5100.h"
#include "utility/socket.h"
extern "C" {
#include "string.h"
}

#include "eth.h"
#include "ethClient.h"
#include "ethServer.h"

ethServer::ethServer(uint16_t port)
{
  _port = port;
}

void ethServer::begin()
{
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    ethClient client(sock);
    if (client.status() == SnSR::CLOSED) {
      socket(sock, SnMR::TCP, _port, 0);
      listen(sock);
      ethClass::_server_port[sock] = _port;
      break;
    }
  }  
}

void ethServer::accept()
{
  int listening = 0;

  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    ethClient client(sock);

    if (ethClass::_server_port[sock] == _port) {
      if (client.status() == SnSR::LISTEN) {
        listening = 1;
      } 
      else if (client.status() == SnSR::CLOSE_WAIT && !client.available()) {
        client.stop();
      }
    } 
  }

  if (!listening) {
    begin();
  }
}

ethClient ethServer::available()
{
  accept();

  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    ethClient client(sock);
    if (ethClass::_server_port[sock] == _port &&
        (client.status() == SnSR::ESTABLISHED ||
         client.status() == SnSR::CLOSE_WAIT)) {
      if (client.available()) {
        // XXX: don't always pick the lowest numbered socket.
        return client;
      }
    }
  }

  return ethClient(MAX_SOCK_NUM);
}

#if defined(__PIC32MX__)
// @FIXME - grrr, should be size_t
void ethServer::write(uint8_t b) {
  write(&b, 1);
}

void ethServer::write(const uint8_t *buffer, size_t size) 
{
  size_t n = 0;
  
  accept();

  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    ethClient client(sock);

    if (ethClass::_server_port[sock] == _port &&
      client.status() == SnSR::ESTABLISHED) {
	//n += client.write(buffer, size);
      client.write(buffer, size);
    }
  }
  
  //return n;
}
#else
size_t ethServer::write(uint8_t b) 
{
  return write(&b, 1);
}

size_t ethServer::write(const uint8_t *buffer, size_t size) 
{
  size_t n = 0;
  
  accept();

  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    ethClient client(sock);

    if (ethClass::_server_port[sock] == _port &&
      client.status() == SnSR::ESTABLISHED) {
      n += client.write(buffer, size);
    }
  }
  
  return n;
}
#endif
