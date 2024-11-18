#ifndef ALARMA_H_
#define ALARMA_H_

#include "fsl_sctimer.h"
#include "fsl_dac.h"
#include "fsl_power.h"
#include "fsl_swm.h"

#define TIEMPO_INDICACION 50 // Tiempo en ms de la indicación

void SCTIMER_Inicializacion (void); // Inicializa el SCTimer
void DAC_Inicializacion (void); // Inicializa el DAC
void INDICADOR_Enciendo (uint8_t mseg, uint8_t brillo); // Enciende la indicación mseg ms
void INDICADOR_Alarma (uint8_t mseg_on, uint16_t mseg_off, uint8_t brillo); // Configuración de la alarma

extern uint32_t evento_pwm, evento_periodo; // Guardan el numero de los eventos
extern volatile uint16_t flag_alarma; // Flag para el tiempo de la alarma
extern volatile uint8_t DAC_on, flag_buzzer; // Guarda el estado del DAC, 0: apagado, 1: encendido, 2: desactivado

/*
Codigo en el main para que funcione:

	...
	INDICADOR_Enciendo (TIEMPO_INDICACION, 99); // Para las teclas
	...
	INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 11, 99) // Para alarma muy cerca
	...

void SCT0_IRQHandler(void) { // Interrupción del SCTimer
	if (SCTIMER_GetStatusFlags (SCT0) & (1 << evento_periodo)) {
		SCTIMER_ClearStatusFlags (SCT0, 1 << evento_periodo);
		if (DAC_on) {
			DAC_SetBufferValue (DAC1, 0);
			DAC_on = 0;
		}
		else {
			DAC_SetBufferValue (DAC1, (volumen == 0) ? 0 : (410 + volumen * 31 / 14));
			DAC_on = 1;
		}
	}
}
 */

#endif /* ALARMA_H_ */
