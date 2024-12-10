#ifndef ADXL345_H_
#define ADXL345_H_

#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include <math.h>

#define PI 3.14159265359

#define ADDRESS_ADXL345 0x1D // El pin SDO está en alto

#define INT_ENABLE 0x2E
#define BW_RATE 0x2C
#define DATA_FORMAT 0x31
#define OFSZ 0x20
#define POWER_CTL 0x2D

#define ADXL345_ON 0x08
#define ADXL345_OFF 0x00

#define PUERTO_0 0 // Puerto donde esta el pin de interrupción
#define INT_ADXL345 28 // Pin INT0 del ADXL345

typedef struct { // Estructura que almacena los datos del acelerometro
	float gravedades_x[10];
	float gravedades_y[10];
	float gravedades_z[10];
	float suma_gx, suma_gy , suma_gz;
	uint8_t indice;
} gravedad_t;

void ADXL345_Comando (uint8_t direccionRegistro, uint8_t valor);
void ADXL345_Configuracion (void);
void ADXL345_Encender (gravedad_t *gravedad);
void ADXL345_Apagar (void);
int16_t ADXL345_Angulo (gravedad_t *gravedad);

#endif /* ADXL345_H_ */
