#ifndef ERRNO_H
#define ERRNO_H

// Error codes to be used within the application.
// They are represented by a signed char
#define ERR_NONE            ((signed char)0)    // 0x00: no error occured
#define ERR_UNKNOWN         ((signed char)-1)   // 0xFF: unkonwn error occured
#define ERR_NOMEM           ((signed char)-2)   // 0xFE: not enough memory to perform the requested operation
#define ERR_BUSY            ((signed char)-3)   // 0xFD: device or resource busy
#define ERR_IO              ((signed char)-4)   // 0xFC: generic IO error
#define ERR_TIMEOUT         ((signed char)-5)   // 0xFB: error due to timeout
#define ERR_REQUEST         ((signed char)-6)   // 0xFA: invalid request or requested function can't be executed at the moment
#define ERR_NOMSG           ((signed char)-7)   // 0xF9: No message of desired type
#define ERR_PARAM           ((signed char)-8)   // 0xF8: Parameter error
#define ERR_PROTO           ((signed char)-9)   // 0xF7: Protocol error
#define ERR_PROTO_CRC       ((signed char)-10)  // 0xF6: Protocol check error
#define ERR_UNLINKED        ((signed char)-11)  // 0xF5: Unlinked / Invalid link
#define ERR_HARDWARE        ((signed char)-12)  // 0xF4: Hardware error (Internal error code, need to reset hardware)

#define ERR_LAST_ERROR      ((signed char)-15)

#endif // ERRNO_H

