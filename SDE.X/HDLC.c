#include <xc.h>
#include "HDLC.h"

void HDLCInit(void){

}

void sendBuf(unsigned char *buf, unsigned int count, enum TlmType commandByte){
    unsigned int i = 0;
    
    //Write start byte
    RS422_TxByte(HDLC_BOUNDARY);
    
    //Write command byte
    if(commandByte == HDLC_BOUNDARY || commandByte == HDLC_ESCAPE){
        RS422_TxByte(HDLC_ESCAPE);
        RS422_TxByte((unsigned char)(commandByte ^ 0b00100000));
    }
    else{
        RS422_TxByte(commandByte);
    }
    RS422_StartTx();
    
    //Perform byte-stuffing & write message
    for(i=0; i<count; i++){
        if(buf[i] == HDLC_BOUNDARY || buf[i] == HDLC_ESCAPE){
            RS422_TxByte(HDLC_ESCAPE);
            RS422_TxByte((unsigned char)(buf[i] ^ 0b00100000));
        }
        else{
            RS422_TxByte(buf[i]);
        }
    }

    RS422_TxByte(HDLC_BOUNDARY);
    RS422_StartTx();
}

void implementRx(void){
    //Check if there are unprocessed bytes in rx queue
    unsigned char toRead = (unsigned char)(255 - RXBUF_FREE);
    unsigned char isEscape = FALSE;
    unsigned char isInMessage = FALSE;
    __delay_ms(5);
    for(; toRead > 0; toRead--){
        unsigned char newByte = rxbuf[rxbufread++];
        
        if(newByte == HDLC_ESCAPE){
            isEscape = TRUE;
            continue;
        }

        //Not a control byte
        if(isEscape){
            if((newByte & 0b00100000) > 0){
                //We got an escape byte and then a byte without bit 5 clear
                //Abort reception
                //TODO: error handling
            }
            else{
                frameBuf[framePtr++] = (unsigned char)(newByte ^ 0b00100000);
                thisFrameSize++;
            }
        }
        else{
            frameBuf[framePtr++] = newByte;
            thisFrameSize++;
        }
        
        if(newByte == HDLC_BOUNDARY && !isEscape){
            if(isInMessage){
                //End of message.
                isInMessage = FALSE;
                validateFrame(thisFrameSize);
                framePtr = 0;
                thisFrameSize = 0;
                continue;
            }
            else{
                isInMessage = TRUE;
            }
        }
        
        if(framePtr == FRAMEBUF_SIZE){
            //Buffer overrun
            //TODO: error handling
            framePtr = 0;
            thisFrameSize = 0;
        }
    }
}

void validateFrame(unsigned char len){
    //A frame must have a start byte, a message type, and an end byte
    if(len < 3){
        return;
    }
    
    if(frameBuf[0] != HDLC_BOUNDARY){
        return;
    }
    
    if(frameBuf[(unsigned char)(len-1)] != HDLC_BOUNDARY){
        return;
    }
    
    //Copy everything but the boundary bytes
    memcpy(&messageBuf[msgRxPtr], frameBuf+1, (unsigned char)(len-2));
    messageBuf[msgRxPtr++].payloadLen = (unsigned char)(len-3);
    if(msgRxPtr == RX_MSG_QUEUE){
        msgRxPtr = 0;
    }
}