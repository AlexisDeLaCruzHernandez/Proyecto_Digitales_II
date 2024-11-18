#include "ENCODER.h"

volatile uint8_t antirrebote_encoder = 0, B = 0;

void ENCODER_Inicializacion (void) {
	gpio_pin_config_t in_config = {kGPIO_DigitalInput};

	//GPIO_PortInit (GPIO, PUERTO_0); // Ya inicializado en otro lado
	GPIO_PinInit (GPIO, PUERTO_0, ENCODER_A, &in_config);
	GPIO_PinInit (GPIO, PUERTO_0, ENCODER_B, &in_config);

	SYSCON_AttachSignal (SYSCON, kPINT_PinInt0, kSYSCON_GpioPort0Pin9ToPintsel);
	PINT_Init (PINT);
	PINT_PinInterruptConfig (PINT, kPINT_PinInt0, kPINT_PinIntEnableBothEdges, ENCODER_Interrupcion);
	PINT_DisableCallback (PINT); // Desactiva interrupciones para encoder
}

void ENCODER_Activo (void) {
	PINT_EnableCallback (PINT);
}
void ENCODER_Desactivo (void) {
	PINT_DisableCallback (PINT);
}

void ENCODER_Interrupcion (pint_pin_int_t pintr, uint32_t pmatch_status) {
	if (antirrebote_encoder == 0) {
		antirrebote_encoder = 6;
		B = GPIO_PinRead (GPIO, PUERTO_0, ENCODER_B);
	}
}

int8_t ENCODER_Lectura (void) {
	uint8_t A;
	A = GPIO_PinRead (GPIO, PUERTO_0, ENCODER_A);
	antirrebote_encoder = 0;
	if (A == B) return -UNIDAD_DISTANCIA;
	else return UNIDAD_DISTANCIA;
}
