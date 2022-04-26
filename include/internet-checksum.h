#ifndef _INTENET_CHECKSUM_H
#define _INTENET_CHECKSUM_H

#include <Arduino.h>

/**
 * @brief Compute Internet checksum of the given buffer for `len' bytes.
 * http://www.faqs.org/rfcs/rfc1071.html (4.1)
 */
uint16_t InternetChecksum(const uint8_t *data, uint16_t len);

#endif
