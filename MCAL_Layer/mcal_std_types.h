/* 
 * File:   mcal_std_types.h
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 2:11 PM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/* Section: Includes */
#include "std_libraries.h"
#include "compiler.h"

/* Section: Data Type Declarations */
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long   uint32_t;

typedef signed char  sint8_t;
typedef signed short sint16_t;
typedef signed long   sint32_t;

typedef uint8_t std_ReturnType;

/* Section: Macro Declarations */
#define STD_HIGH        0x01
#define STD_LOW         0x00

#define STD_ON          0x01
#define STD_OFF         0x00

#define STD_ACTIVE      0x01  
#define STD_IDLE        0x00

#define E_OK            (std_ReturnType)0x01
#define E_NOT_OK        (std_ReturnType)0x00

/* Section: Macro Functions Declarations */

/* Section: Function Declaration*/

#endif	/* MCAL_STD_TYPES_H */

