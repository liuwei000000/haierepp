#define UART_CMD_CTRL                          0x01
#define UART_CMD_STATUS                        0x02
#define UART_CMD_INVALIDCMD                    0x03
#define UART_CMD_ALARM                         0x04
#define UART_CMD_ACK                           0x05
#define UART_CMD_REPORT                        0x06
#define UART_CMD_STOPALARM                     0x09
#define UART_CMD_GET_VER                       0x61
#define UART_CMD_GET_VER_ACK                   0x62
#define UART_CMD_GET_CODE                      0x63
#define UART_CMD_SET_CODE                      0x64
#define UART_CMD_GETTYPEID                     0x70
#define UART_CMD_GETTYPEIDACK                  0x71
#define UART_CMD_GET_ALARM_ACK                 0x74
#define UART_CMD_SET_INTERVAL                  0x7C
#define UART_CMD_SET_INTERVAL_ACK              0x7D
#define UART_CMD_INQ_NET_STATUS                0xF0
#define UART_CMD_INQ_NET_STATUS_ACK            0xF1
#define UART_CMD_SETINTO_CONFIGMODE            0xF2
#define UART_CMD_SETINTO_CONFIGMODE_ACK        0xF3
#define UART_CMD_SETINTO_WORKMODE              0xF4
#define UART_CMD_SETINTO_WORKMODE_ACK          0xF5
#define UART_CMD_UWT_TRANSMIT                  0xF6
#define UART_CMD_RPT_NET_STATUS                0xF7
#define UART_CMD_CHECK_DEV                     0x88
#define UART_CMD_CHECK_DEV_ACK                 0x89
#define UART_CMD_TRANS_DOWN                    0x8a
#define UART_CMD_TRANS_UP                      0x8b
#define UART_CMD_SET_BIGDATA_INTV              0xFA
#define UART_CMD_SET_BIGDATA_INTV_ACK          0xFB
//===============================================
#define HEAD_MG                                0xffff
#define CHECK_ERROR                            -1
#define READ_ERROR                             -1
#define OK                                     0
//===============================================
#define ADDR_LEN                               6

typedef struct frameHdr{
  uint8_t     len;
  uint8_t     data_len;
  uint64_t    addr;
  uint8_t     type;
}
frameHdr;

void setup() {
  //  Set cm baud rate:
  Serial.begin(9600);
}

void loop(){
  process();
  delay(2000);
}

int process(){
  frameHdr fhdr;
  uint8_t *pdata;
  frameRead(&fhdr, pdata);
  //praseFrame(data);
  frameFree(pdata);
}

int sendCommand(void)
{
  return 1;
}

int frameWrite(uint8_t type, uint8_t *data, uint8_t data_len, uint64_t addr)
{
  uint8_t len = data_len + ADDR_LEN + 1; 
  uint8_t sum = 0;
  //header
  Serial.write(0xff);
  Serial.write(0xff);
  //----- len -----------
  Serial.write(len);
  sum += data_len;
  //----- addr -----------
  for (int i = 0 ; i < ADDR_LEN; i++)
  {
    sum += ((uint8_t *)(&addr))[i];
    Serial.write(((uint8_t *)(&addr))[i]);
  }
  //----- type ----------
  Serial.write(type);
  sum += type;
  //------ data ------   
  for (int i = 0 ; i < data_len; i++)
  {
    sum += data[i];
    Serial.write(data[i]);
  }
  //------  check sum --------
  Serial.write(sum);  
}

int praseFrame(struct frameHdr *f, uint8_t *data)
{
  switch (f->type) {
  case UART_CMD_GET_VER:
    break;
  case UART_CMD_GETTYPEID:
    break;
  }
}

int frameRead(struct frameHdr *d, uint8_t *data) {
  int i = 0;
  uint8_t sum = 0;
  if (Serial.available() > 0) {
    //check head
    if (!(Serial.available() > 0 && Serial.read() == 0xff)) goto error;
    if (!(Serial.available() > 0 && Serial.read() == 0xff)) goto error;
    if (!(Serial.available() > 0 )) goto error;

    d->len = Serial.read();
    if (d->len < ADDR_LEN + 1) goto error;
    d->data_len = d->len - (ADDR_LEN + 1);
    //Serial.println(d->len, HEX);

    if (d->data_len > 0 ) {
      data = (uint8_t *) malloc(d->data_len);
      if (data == NULL) goto error;
    } 
    else {
      data = NULL;
    }

    i = 0;
    sum = 0;
    sum += d->len;
    while(Serial.available() > 0 && i < ADDR_LEN) {
      uint8_t r = Serial.read();
      ((uint8_t *)(&d->addr))[i] = r;
      //Serial.println(data[i], HEX);
      sum += r;
      i++;
    }
    if (i != ADDR_LEN) goto error;
    i = 0;

    while(Serial.available() > 0 && i < d->data_len ) {
      data[i] = Serial.read();
      sum += data[i] ;
      i++;
    }
    //Serial.println(sum, HEX);
    //check sum
    if (!(Serial.available() > 0 && Serial.read() == sum)) goto error;
  }
  while(Serial.available()> 0)  Serial.read();
  return OK;
error:
  //read all
  while(Serial.available()> 0)  Serial.read();
  return READ_ERROR;
}

void frameFree(uint8_t *d)
{
  if (!d) free(d);
}


















