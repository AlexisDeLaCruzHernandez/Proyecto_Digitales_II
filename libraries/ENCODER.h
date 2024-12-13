#ifndef ENCODER_H_
#define ENCODER_H_

#include "fsl_gpio.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"

#define PUERTO_0 0 // Puerto donde estan los pines del encoder
#define ENCODER_A 9 // Pin encoder A, en la placa es el clk
#define ENCODER_B 8 // Pin encoder B, en la placa es el dt

#define UNIDAD_DISTANCIA 5 // Cuanta distancia equivale un pulso

void ENCODER_Inicializacion (void); // Inicializa los pines y configura las interrupciones
void ENCODER_Activo (void); // Habilito la interrupción
void ENCODER_Desactivo (void); // Desactivo la interrupción
void ENCODER_Interrupcion (pint_pin_int_t pintr, uint32_t pmatch_status); // Interrupción del pin A
int8_t ENCODER_Lectura (void); // Devuelve el numero que suma o resta a la distancia

extern volatile uint8_t antirrebote_encoder; // Flag para el antirrebote
extern volatile uint8_t B; // Valor del pin B

/*
Codigo en el main para que funcione:

	...
	if (antirrebote_encoder == 1) {
		distancia += ENCODER_Lectura;
	}
	...

void SysTick_Handler (void) { // Interrupción del SysTick
	if (antirrebote_encoder != 0) antirrebote_encoder --;
}
 */

#endif /* ENCODER_H_ */
