#pragma once

#include <pic32mx.h>

/*=============================================================================
 * Inte Registers - Module 1
 */
#define MASTER_INTERRUPT_1_READ (IFS(0) >> 31) & 1
#define MASTER_INTERRUPT_1_CLR IFSCLR(0) = 0x80000000;

/*=============================================================================
 * Control Registers - Module 1
 */
#define ON_1_READ (I2C1CON >> 15) & 1
#define ON_1_SET I2C1CONSET = 0x8000
#define ON_1_CLR I2C1CONCLR = 0x8000

#define ACKDT_1_READ (I2C1CON >> 5) & 1
#define ACKDT_1_SET I2C1CONSET = 0x20
#define ACKDT_1_CLR I2C1CONCLR = 0x20

#define ACKEN_1_READ (I2C1CON >> 4) & 1
#define ACKEN_1_SET I2C1CONSET = 0x10
#define ACKEN_1_CLR I2C1CONCLR = 0x10

#define RCEN_1_READ (I2C1CON >> 3) & 1
#define RCEN_1_SET I2C1CONSET = 0x8
#define RCEN_1_CLR I2C1CONCLR = 0x8

#define PEN_1_READ (I2C1CON >> 2) & 1
#define PEN_1_SET I2C1CONSET = 0x4
#define PEN_1_CLR I2C1CONCLR = 0x4

#define RSEN_1_READ (I2C1CON >> 1) & 1
#define RSEN_1_SET I2C1CONSET = 0x2
#define RSEN_1_CLR I2C1CONCLR = 0x2

#define SEN_1_READ I2C1CON & 1
#define SEN_1_SET I2C1CONSET = 0x1
#define SEN_1_CLR I2C1CONCLR = 0x1

/*=============================================================================
 * Status Registers - Module 1
 */
#define ACKSTAT_1_READ (I2C1STAT >> 15) & 1
#define ACKSTAT_1_SET I2C1STATSET = 0x8000
#define ACKSTAT_1_CLR I2C1STATCLR = 0x8000

#define TRSTAT_1_READ (I2C1STAT >> 14) & 1
#define TRSTAT_1_SET I2C1STATSET = 0x4000
#define TRSTAT_1_CLR I2C1STATCLR = 0x4000

#define IWCOL_1_READ (I2C1STAT >> 7) & 1
#define IWCOL_1_SET I2C1STATSET = 0x80
#define IWCOL_1_CLR I2C1STATCLR = 0x80

#define P_1_READ (I2C1STAT >> 4) & 1
#define P_1_SET I2C1STATSET = 0x10
#define P_1_CLR I2C1STATCLR = 0x10

#define S_1_READ (I2C1STAT >> 3) & 1
#define S_1_SET I2C1STATSET = 0x8
#define S_1_CLR I2C1STATCLR = 0x8

#define RBF_1_READ (I2C1STAT >> 1) & 1
#define RBF_1_SET I2C1STATSET = 0x2
#define RBF_1_CLR I2C1STATCLR = 0x2

#define TBF_1_READ I2C1STAT & 1
#define TBF_1_SET I2C1STATSET = 0x1
#define TBF_1_CLR I2C1STATCLR = 0x1
