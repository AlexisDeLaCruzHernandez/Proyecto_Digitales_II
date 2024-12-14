#ifndef FLASH_H_
#define FLASH_H_

#include "fsl_iap.h"

#define SECTOR 63
#define FLASH_ADDRESS_BASE 0x0000FC00
#define LONGITUD_DATO 64

int8_t FLASH_Busqueda (void);
void FLASH_Lectura (uint8_t *contraste, uint8_t *volumen, uint8_t *version,
		    int16_t *angulo_alarma, int32_t *distancia_alarma);
void FLASH_Guardado (uint8_t contraste, uint8_t volumen, uint8_t *version,
		     int16_t angulo_alarma, int32_t distancia_alarma);

#endif /* FLASH_H_ */
