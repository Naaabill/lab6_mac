#ifndef __DUMMY_SERIAL_H__
#define __DUMMY_SERIAL_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** serial line init */
void serialInit();

/** send one ASCII char on the serial line. */
void serialPutChar(char c);

/** send a NULL terminate ASCII string on the serial line. */
void serialPutString(char *c);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
