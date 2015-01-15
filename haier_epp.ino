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

typedef struct f{
  uint16_t    head;
  uint8_t     len;
  uint8_t     addr[6];
  uint8_t     type;
}
frameInfo, *pframeInfo;

void setup() {
  //  Set cm baud rate:
  Serial.begin(9600);
}

void loop(){
  frameInfo a;
  pframeInfo b;
}

int sendCommand(void)
{
  return 1;
}

int praseFrame(uint8_t *data)
{
  if (!checkFrame(data))
    return CHECK_ERROR;
}

int checkFrame(uint8_t *data)
{
  pframeInfo fi = (pframeInfo)data;
  //check head
  if (fi->head != HEAD_MG)
    return CHECK_ERROR;
  //check sum;
  uint8_t sum = 0;
  for (int i = 0; i< fi->len;) {
    sum += *(&(fi->len) + i);
  }
  if (*(&(fi->len) + fi->len-1) != sum) {
    return CHECK_ERROR;
  }
  return 0;
}

