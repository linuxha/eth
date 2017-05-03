#ifndef ethernetclient_h
#define ethernetclient_h

#if defined(__AVR__)
#include "Arduino.h"	
//#else
#endif

#include "Print.h"
// missing
#include "Client.h"
#include "IPAddress.h"

class ethClient : public Client {

public:
  ethClient();
  ethClient(uint8_t sock);

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
#if defined(__PIC32MX__)
  virtual void write(uint8_t);
  virtual void write(const uint8_t *buffer, size_t size);
#else
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buffer, size_t size);
#endif
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const ethClient&);
  virtual bool operator!=(const ethClient& rhs) { return !this->operator==(rhs); };

  friend class ethServer;
  
  using Print::write;

private:
  static uint16_t _srcport;
  uint8_t _sock;
};

#endif
