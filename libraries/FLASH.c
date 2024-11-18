#include "FLASH.h"

int8_t FLASH_Busqueda (void) {
	uint8_t n = 0, version;
    uint32_t *memoria;
    while (n < 16) {
    	memoria = (uint32_t *) (FLASH_ADDRESS_BASE + LONGITUD_DATO * n + 0x4 * 4);
    	version = *memoria;
    	if (version == 0xFF) break;
    	n ++;
    }
    return n - 1;
}

void FLASH_Lectura (uint8_t *contraste, uint8_t *volumen, uint8_t *version,
					  int16_t *angulo_alarma, int32_t *distancia_alarma) {
    uint32_t *memoria;
    if (FLASH_Busqueda() != -1) {
    	memoria = (uint32_t *) (FLASH_ADDRESS_BASE + LONGITUD_DATO * FLASH_Busqueda());
		for (uint8_t i = 0; i < 5; i ++) {
			switch (i) {
			case 0: *contraste = *memoria;
				break;
			case 1: *volumen = *memoria;
				break;
			case 2: *angulo_alarma = *memoria;
				break;
			case 3: *distancia_alarma = *memoria;
					break;
			case 4: *version = *memoria;
				break;
			}
			memoria ++;
		}
    }
    else {
    	FLASH_Guardado (*contraste, *volumen, &(*version), *angulo_alarma, *distancia_alarma);
    }
}

void FLASH_Guardado (uint8_t contraste, uint8_t volumen, uint8_t *version,
					int16_t angulo_alarma, int32_t distancia_alarma) {
    uint32_t datos[5];
    datos[0] = contraste;
    datos[1] = volumen;
    datos[2] = angulo_alarma;
    datos[3] = distancia_alarma;
    (*version) ++;
    if (*version == 16) {
    	*version = 0;
    	// Borrado flash
        if( IAP_PrepareSectorForWrite(SECTOR, SECTOR) != kStatus_IAP_Success){
        	// Error
        }
        if(IAP_EraseSector(SECTOR, SECTOR, 12000000)!=kStatus_IAP_Success){
        	// Error
        }
    }
    datos[4] = *version;
    // Escritura flash
    if(IAP_PrepareSectorForWrite(SECTOR, SECTOR)!=kStatus_IAP_Success){
    	// Errpr
    }
    status_t resp = IAP_CopyRamToFlash(FLASH_ADDRESS_BASE + LONGITUD_DATO * datos[4], &datos, 64, 12000000);
    if(resp!=kStatus_IAP_Success){
    	// Error
    }
}
