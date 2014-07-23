
#ifndef My_stdint_H
#define My_stdint_H

//#include "my_stdint.h"

#ifndef __STM32F10x_H


#define     __I     volatile const          /*!< defines 'read only' permissions      */
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */


typedef signed long long my_int64_t;
typedef signed long my_int32_t;
typedef signed short my_int16_t;
typedef char my_int8_t;

typedef unsigned long long my_uint64_t;
typedef unsigned long my_uint32_t;
typedef unsigned short my_uint16_t;
typedef unsigned char my_uint8_t;


typedef my_int64_t  s64;
typedef my_int32_t  s32;
typedef my_int16_t s16;
typedef my_int8_t  s8;

typedef const my_int64_t sc64;  /*!< Read Only */
typedef const my_int32_t sc32;  /*!< Read Only */
typedef const my_int16_t sc16;  /*!< Read Only */
typedef const my_int8_t sc8;   /*!< Read Only */

typedef __IO my_int64_t  vs64;
typedef __IO my_int32_t  vs32;
typedef __IO my_int16_t  vs16;
typedef __IO my_int8_t   vs8;

typedef __I my_int64_t vsc64;  /*!< Read Only */
typedef __I my_int32_t vsc32;  /*!< Read Only */
typedef __I my_int16_t vsc16;  /*!< Read Only */
typedef __I my_int8_t vsc8;   /*!< Read Only */



typedef my_uint64_t  u64;
typedef my_uint32_t  u32;
typedef my_uint16_t u16;
typedef my_uint8_t  u8;

typedef const my_uint64_t uc64;  /*!< Read Only */
typedef const my_uint32_t uc32;  /*!< Read Only */
typedef const my_uint16_t uc16;  /*!< Read Only */
typedef const my_uint8_t uc8;   /*!< Read Only */

typedef __IO my_uint64_t  vu64;
typedef __IO my_uint32_t  vu32;
typedef __IO my_uint16_t vu16;
typedef __IO my_uint8_t  vu8;

typedef __I my_uint64_t vuc64;  /*!< Read Only */
typedef __I my_uint32_t vuc32;  /*!< Read Only */
typedef __I my_uint16_t vuc16;  /*!< Read Only */
typedef __I my_uint8_t vuc8;   /*!< Read Only */


#endif


#endif

