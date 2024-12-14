#include "definitions.h"

volatile uint8_t flag_tick = 0;
volatile uint16_t flag_tick_barra = 0;
uint8_t teclas[] = { // Valores de las teclas
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'*', '0', '#', 'D',
};
uint8_t volumen = 10, volumen_config = 0, contraste = 80, version = 15, cambio_de_config = 0;
int32_t distancia_alarma = 0;
uint8_t unidad_distancia_alarma = 1;
int16_t angulo_alarma = 0;
int8_t signo_angulo_alarma = 1;

int main(void) {
	gravedad_t gravedad;
	int16_t angulo = 0, angulo_resultante = 0, angulo_cero = 0;
	int32_t distancia = 0;
	opcion_t opcion = ninguno;
	menu_t menu = inicio;
	uint8_t boton = '\0', pulsado = 0;
	uint8_t numeros_configuracion = 0;
	uint8_t alarma = 0;
	modo_t modo;
	uint8_t actualizar_menu = 0;

	uint8_t positivo = 0, negativo = 0; // Flags para marcar el signo en menu de mostrar
	uint8_t metros = 0, centimetros = 0;

	FLASH_Lectura (&contraste, &volumen, &version, &angulo_alarma, &distancia_alarma);

	if (angulo_alarma < 0) signo_angulo_alarma = -1;
	if (distancia_alarma > 9999) unidad_distancia_alarma = 100;

    	GPIO_Inicializacion ();
    	ENCODER_Inicializacion ();
    	SCTIMER_Inicializacion ();
    	DAC_Inicializacion ();
    	SysTick_Inicializacion ();
    	I2C_Inicializacion ();
    	ADXL345_Configuracion ();
    	ADXL345_Apagar ();
    	OLED_Inicio ();
	OLED_Contraste (contraste * 255 / 99);
    	OLED_CopiarImagen (inicio_1, sizeof (inicio_1));
    	OLED_Refresco ();

    	while(1) {
    		if (numeros_configuracion == 0) {
    			boton = TECLADO_Lectura (0, 3, 3, 3);
    			// Lectura de la tecla #
    			GPIO_PinWrite (GPIO, PUERTO_0, COLUMNA_3, 0);
	    		if (GPIO_PinRead (GPIO, PUERTO_0, FILA_4) == 0) {
	    			Delay_ms (50);
	    			if (GPIO_PinRead (GPIO, PUERTO_0, FILA_4) == 0) {
	    				boton = teclas[4 * 3 + 2];
	    			}
	    		}
			GPIO_PinWrite (GPIO, PUERTO_0, COLUMNA_3, 1);
    		}
    		else {
    			boton = TECLADO_Lectura (0, 0, 3, 2);
    		}
    		if (boton == '\0') pulsado = 0;
    		if (boton != '\0' && pulsado == 0) {
    			pulsado = 1;
    			INDICADOR_Enciendo (TIEMPO_INDICACION, 99);
    			switch (boton) {
    			case 'A':
    				actualizar_menu = tecla_a (&opcion, &menu, &alarma, &modo, &numeros_configuracion);
    				break;
    			case 'B':
    				actualizar_menu = tecla_b (&opcion, &menu, &alarma, &modo, &numeros_configuracion,
    					        	   &angulo_cero, angulo, &distancia, &gravedad);
    				break;
    			case 'C':
    				actualizar_menu = tecla_c (&opcion, &menu, alarma, &angulo_cero, &distancia);
    				break;
    			case 'D':
    				actualizar_menu = tecla_d (&opcion, &menu, &gravedad);
    				break;
    			case '0': case '1': case '2': case '3':	case '4':
    			case '5': case '6': case '7': case '8': case '9':
    				tecla_num (&opcion, &menu, &numeros_configuracion, &boton);
    				break;
    			case '*':
    				break;
    			case '#':
    				tecla_numeral (&opcion, menu);
    				break;
    			}
    			if (actualizar_menu) {
    				mostrar_menu (menu, &positivo, &negativo, &centimetros, &metros);
    				opcion = ninguno;
    				actualizar_menu = 0;
    			}
    			OLED_Refresco ();
    		}
    		menu_independiente (menu, opcion, numeros_configuracion, alarma, &gravedad, &angulo,
    				    &angulo_resultante, angulo_cero, &distancia, &positivo, &negativo,
				    &metros, &centimetros);
    	}
    	return 0 ;
}

void SysTick_Handler (void) {
	flag_tick ++;
	flag_tick_barra ++;
	if (antirrebote_encoder != 0) antirrebote_encoder --;
	if (flag_alarma != 0) flag_alarma --;
	if (flag_buzzer != 0) flag_buzzer --;
	if (flag_buzzer == 0 && DAC_on != 2) {
		DAC_SetBufferValue (DAC1, 0);
	    	SCTIMER_StopTimer (SCT0, kSCTIMER_Counter_U);
	    	SCT0->OUTPUT |= (1 << 4);
		DAC_on = 2;
	}
}

void SCT0_IRQHandler(void) {
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
