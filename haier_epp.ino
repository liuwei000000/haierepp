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

uint8_t gCheckFlag = 0;
uint64_t gAddr = 0;


/* UART_CMD_GET_VER(61) <===> UART_CMD_GET_VER_ACK(62) */
uint8_t gVerBuf[19]  = {0x45, 0x2B, 0x2B, 0x31, 0x2E, 0x31, 0x35, 
					    0x00, 0x31, 0x34, 0x30, 0x36, 0x32, 0x38, 
					    0x30, 0x31, 0x00, 0x00, 0x00};
/* UART_CMD_GETTYPEID(70) <===>  UART_CMD_GETTYPEIDACK(71) */
uint8_t gTypeBuf[32] = {0x11, 0x1C, 0x12, 0x00, 0x24, 0x00, 0x08, 
					    0x10, 0x06, 0x05, 0x00, 0x41, 0x80, 0x01, 
                        0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
					    0x00, 0x00, 0x00, 0x00};

/* UART_CMD_SETINTO_CONFIGMODE(F2) */
uint8_t gSoftBuf[2]   = {0x00, 0x02};
#if 0
/* UART_CMD_SET_INTERVAL(7C) <===> UART_CMD_SET_INTERVAL_ACK(7D) */
uint8_t gInterBuf1[2] = {0x00, 0x00};
uint8_t gInterBuf2[2] = {0x00, 0x0A};
/* UART_CMD_CTRL(01) <===> UART_CMD_STATUS(02) */
uint8_t gCtrlBuf[62] = {0x6D, 0x01, 0x00, 0x41, 0x00, 0x2A, 0x00,
                      0x0B, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
      				  0x00, 0x64, 0x50, 0x00, 0x00, 0x41, 0x41, 
      				  0x00, 0x00, 0x00, 0x4A, 0x0A, 0x30, 0x40, 
                      0x00, 0x4B, 0x40, 0x00, 0x00, 0x40, 0x00, 
                      0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 
                      0x18, 0x11, 0x80, 0x00, 0x00, 0x00, 0x00, 
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      				  0x80, 0x00, 0x06, 0x00, 0x00, 0x10};
/* UART_CMD_SETINTO_WORKMODE(F4) */
#endif

void debug(int num)
{
    Serial.print("Msg:");
    Serial.println(num);
}

void softap()
{
    volatile  uint8_t len = 0;
    volatile  uint8_t *buf = NULL;

    //Serial.println("softap 111");
    if (gCheckFlag == 0) {
        //Serial.println("softap 222");
        len = 2;
        buf = gSoftBuf;
        frameWrite(UART_CMD_SETINTO_CONFIGMODE, (uint8_t *)buf, (uint8_t)len, gAddr);
        gCheckFlag = 1;
    }
    //Serial.println("softap 333");
}

void setup() {
  //  Set cm baud rate:
  Serial.begin(9600);
  /* D2 change from low to high */
  attachInterrupt(0, softap, RISING);
}

void loop(){
  process();
  delay(2000);
}

int process(){
  frameHdr fhdr;
  uint8_t *pdata;
  int8_t ret = OK;  
  
  ret = frameRead(&fhdr, pdata);
  if (OK == ret) {
      praseFrame(&fhdr, pdata);
      frameFree(pdata);
  }  
}

int sendCommand(void)
{
  return 1;
}

int frameWrite(uint8_t type, uint8_t *data, uint8_t data_len, uint64_t addr)
{
  //type 1 byte sum 1 byte, so 1+1 = 2
  uint8_t len = data_len + ADDR_LEN + 2; 
  uint8_t sum = 0;
  //header
  Serial.write(0xff);
  Serial.write(0xff);
  //----- len -----------
  Serial.write(len);
  sum += len;
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
  //Serial.println(f->type);
  uint8_t len = 0;
  uint8_t *buf = NULL;
  
switch (f->type) {
  case 0:
    break;
  case UART_CMD_GET_VER:
    Serial.println("GET VER");
    len = 19;
	buf = gVerBuf;
	frameWrite(UART_CMD_GET_VER_ACK, buf, len, f->addr);
    break;
  case UART_CMD_SET_CODE:
    Serial.println("SET CODE");
    len = 0;
    buf = NULL;
	frameWrite(UART_CMD_ACK, buf, len, f->addr);
    break;
  case UART_CMD_GETTYPEID:
    Serial.println("GET TYPE");
    len = 32;
	buf = gTypeBuf;
	frameWrite(UART_CMD_GETTYPEIDACK, buf, len, f->addr);
#if 0
    if (gCheckFlag == 0) {
        Serial.println("Soft Ap");
        len = 2;
        buf = gSoftBuf;
        frameWrite(UART_CMD_SETINTO_CONFIGMODE, buf, len, f->addr);
        gCheckFlag = 1;
    }
#endif
    break;
  default :
    Serial.print("Wrong Type:");
    Serial.println(f->type,HEX);
    break;  
  }
}

int frameRead(struct frameHdr *d, uint8_t *data) {
  int i = 0;
  uint8_t sum = 0;
  uint8_t len = 0;
  uint8_t *buf = NULL;
  int val = 0;
  
  if (Serial.available() > 0) {
    //Serial.println("frameRead 111 ");
    /* check configure mode */
    val = Serial.read();
#if 0    
    if (val == 'a'){
        Serial.println("frameRead 222 ");
        if ((Serial.available() > 0)&&(Serial.read() == 'a')) {
            Serial.println("frameRead 333 ");
            if (gCheckFlag == 0) {
                Serial.println("Soft Ap");
                len = 2;
                buf = gSoftBuf;
                frameWrite(UART_CMD_SETINTO_CONFIGMODE, buf, len, gAddr);
                gCheckFlag = 1;
            }
        }
        Serial.println("frameRead 444 ");
        goto error;
    }
#endif    
    //check head
    //Serial.println("frameRead 555 ");
    if (!(Serial.available() > 0 && val == 0xff)) {
        //Serial.println("frameRead 666 ");
        goto error;
    }   
    if (!(Serial.available() > 0 && Serial.read() == 0xff)) 
        goto error;
    //read len
    if (!(Serial.available() > 0 )) goto error;
    d->len = Serial.read();
    if (d->len < ADDR_LEN + 2) goto error;
    d->data_len = d->len - (ADDR_LEN + 2);
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
      //((uint8_t *)(&gAddr))[i] = r;
      //Serial.println(data[i], HEX);
      //Serial.println(gAddr[i], HEX);
      sum += r;
      i++;
    }
    if (i != ADDR_LEN) goto error;
    
    //read type
    if (!(Serial.available() > 0 )) goto error;
    d->type = Serial.read();
    //Serial.print("Msg Type:");
    //Serial.println(d->type,HEX);

    sum += d->type;
    
    i = 0;
    while(Serial.available() > 0 && i < d->data_len ) {
      data[i] = Serial.read();
      //Serial.println(data[i], HEX);
      sum += data[i] ;
      i++;
    }
    //Serial.println(sum, HEX);
    //check sum
    if (!(Serial.available() > 0 && Serial.read() == sum)) goto error;
  }
  //clear serial
  while(Serial.available()> 0)  Serial.read();
  //Serial.println("OK");
  return OK;
error:
  //clear serial
  Serial.println("READ_ERROR");
  while(Serial.available()> 0)  Serial.read();
  return READ_ERROR;
}

void frameFree(uint8_t *d)
{
  if (!d) free(d);
}

