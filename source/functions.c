#include "definitions.h"

void Delay_ms (uint8_t mseg) {
	flag_tick = 0;
	while (flag_tick <= mseg);
}

void GPIO_Inicializacion (void) {
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	gpio_pin_config_t in_config = {kGPIO_DigitalInput};
	GPIO_PortInit (GPIO, PUERTO_0);
	GPIO_PinInit (GPIO, PUERTO_0, FILA_1, &in_config);
    GPIO_PinInit (GPIO, PUERTO_0, FILA_2, &in_config);
    GPIO_PinInit (GPIO, PUERTO_0, FILA_3, &in_config);
    GPIO_PinInit (GPIO, PUERTO_0, FILA_4, &in_config);
    GPIO_PinInit (GPIO, PUERTO_0, COLUMNA_1, &out_config);
    GPIO_PinInit (GPIO, PUERTO_0, COLUMNA_2, &out_config);
    GPIO_PinInit (GPIO, PUERTO_0, COLUMNA_3, &out_config);
    GPIO_PinInit (GPIO, PUERTO_0, COLUMNA_4, &out_config);
}

void SysTick_Inicializacion (void) {
	(void) SysTick_Config (SystemCoreClock / 1000);
}

void I2C_Inicializacion (void) {
	uint32_t baudRate = 400000;
	uint32_t frecuencia = 12000000;
	i2c_master_config_t  i2config;
	CLOCK_Select (kI2C1_Clk_From_MainClk);
	CLOCK_EnableClock (kCLOCK_Swm);
	SWM_SetMovablePinSelect (SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_27);
	SWM_SetMovablePinSelect (SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_28);
	CLOCK_DisableClock (kCLOCK_Swm);
	I2C_MasterGetDefaultConfig (&i2config);
	i2config.baudRate_Bps = baudRate;
	I2C_MasterInit (I2C1, &i2config, frecuencia);
}

uint8_t TECLADO_Lectura (uint8_t Fila_Ini, uint8_t Columna_Ini, uint8_t Fila_Fin, uint8_t Columna_Fin) {
	for (uint8_t columna = Columna_Ini; columna <= Columna_Fin; columna ++) {
	    GPIO_PinWrite (GPIO, PUERTO_0, COLUMNA_1 + columna, 0);
	    for (uint8_t fila = Fila_Ini; fila <= Fila_Fin; fila ++) {
	    	if (GPIO_PinRead (GPIO, PUERTO_0, FILA_1 + fila) == 0) {
	    		Delay_ms (50);
	    		if (GPIO_PinRead (GPIO, PUERTO_0, FILA_1 + fila) == 0) {
	    			GPIO_PinWrite (GPIO, PUERTO_0, COLUMNA_1 + columna, 1);
	    			return teclas[4 * fila + columna];
	    		}
	    	}
	    }
	    GPIO_PinWrite (GPIO, PUERTO_0, COLUMNA_1 + columna, 1);;
	}
	return '\0';
}

void OLED_BarraEscritura (uint8_t X, uint8_t Y) {
	if (flag_tick_barra >= 500) {
		OLED_ToggleRectangulo (X, Y, X + 1, Y + 11);
		flag_tick_barra = 0;
		OLED_Refresco ();
	}
}

void toggle_op_a (menu_t menu) {
	switch (menu) {
	case inicio:
	case alarma:
		OLED_ToggleRectangulo (1, 1, 47, 47);
		break;
	case config_general:
		OLED_ToggleRectangulo (1, 1, 13, 14);
		break;
	case config_grados:
		OLED_ToggleRectangulo (19, 1, 63, 14);
		break;
	case mostrar_grados:
		// Nada
		break;
	case config_distancia:
		OLED_ToggleRectangulo (26, 1, 63, 14);
		break;
	case mostrar_distancia:
		// Nada
		break;
	}
}

void toggle_op_b (menu_t menu) {
	switch (menu) {
	case inicio:
	case alarma:
		OLED_ToggleRectangulo (51, 1, 97, 47);
		break;
	case config_general:
		OLED_ToggleRectangulo (51, 1, 62, 15);
		break;
	case config_grados:
		OLED_ToggleRectangulo (1, 17, 15, 31);
		OLED_ToggleRectangulo (1, 33, 15, 47);
		break;
	case mostrar_grados:
		OLED_ToggleRectangulo (1, 1, 12, 23);
		break;
	case config_distancia:
		OLED_ToggleRectangulo (1, 17, 22, 31);
		OLED_ToggleRectangulo (1, 33, 22, 47);
		break;
	case mostrar_distancia:
		OLED_ToggleRectangulo (1, 1, 12, 23);
		break;
	}
}

void toggle_op_c (menu_t menu) {
	if (menu != inicio) OLED_ToggleRectangulo (101, 1, 126, 15);
}

void toggle_op_d (menu_t menu) {
	switch (menu) {
	case inicio:
		OLED_ToggleRectangulo (101, 1, 126, 15);
		break;
	case alarma:
	case config_general:
		// Nada
		break;
	case config_grados:
	case config_distancia:
		OLED_ToggleRectangulo (67, 1, 97, 47);
		break;
	case mostrar_grados:
	case mostrar_distancia:
		OLED_ToggleRectangulo (1, 25, 12, 47);
		break;
	}
}

void mostrar_menu (menu_t menu, uint8_t *positivo, uint8_t *negativo, uint8_t *centimetros, uint8_t *metros) {
	switch (menu) {
	case inicio:
	    OLED_CopiarImagen (inicio_1, sizeof (inicio_1));
		break;
	case alarma:
	    OLED_CopiarImagen (alarma_2, sizeof (alarma_2));
		break;
	case config_general:
	    OLED_CopiarImagen (config_general_3, sizeof (config_general_3));
		OLED_EscribirNumero (11, 28, contraste / 10, Encender_Pixel);
		OLED_EscribirNumero (19, 28, contraste % 10, Encender_Pixel);
		OLED_EscribirNumero (61, 28, volumen / 10, Encender_Pixel);
		OLED_EscribirNumero (69, 28, volumen % 10, Encender_Pixel);
		break;
	case config_grados:
	    OLED_CopiarImagen (config_grados_4, sizeof (config_grados_4));
		OLED_EscribirNumero (21, 26, abs (angulo_alarma) / 1000, Encender_Pixel);
		OLED_EscribirNumero (29, 26, abs (angulo_alarma) / 100 - (abs (angulo_alarma) / 1000) * 10, Encender_Pixel);
		OLED_EscribirNumero (37, 26, abs (angulo_alarma) / 10 - (abs (angulo_alarma) / 100) * 10, Encender_Pixel);
		OLED_EscribirNumero (48, 26, abs (angulo_alarma) - (abs (angulo_alarma) / 10) * 10, Encender_Pixel);
		if (signo_angulo_alarma == 1) OLED_ToggleRectangulo (1, 17, 15, 31);
		else OLED_ToggleRectangulo (1, 33, 15, 47);
		break;
	case mostrar_grados:
	    OLED_CopiarImagen (mostrar_grados_5, sizeof (mostrar_grados_5));
	    for (uint8_t i = 0; i < 3; i ++) OLED_EscribirNumero (54 + 8 * i, 30, 0, Encender_Pixel);
		OLED_EscribirNumero (81, 30, 0, Encender_Pixel);
		OLED_ToggleRectangulo (53, 1, 73, 21);
		*positivo = 1;
		*negativo = 0;
		break;
	case config_distancia:
	    OLED_CopiarImagen (config_distancia_6, sizeof (config_distancia_6));
		// Si unidad_alarma_distancia es 1 ya esta guardado en cm y se muestra en cm
		// Si unidad_alarma_distancia es 100 esta guardado en cm pero se muestra en m, se divide por 100
		distancia_alarma = distancia_alarma / unidad_distancia_alarma; // Si es cm se deja igual y si es m sacamos los 0
		OLED_EscribirNumero (28, 26, distancia_alarma / 1000, Encender_Pixel);
		OLED_EscribirNumero (36, 26, distancia_alarma / 100 - (distancia_alarma / 1000) * 10, Encender_Pixel);
		OLED_EscribirNumero (44, 26, distancia_alarma / 10 - (distancia_alarma / 100) * 10, Encender_Pixel);
		OLED_EscribirNumero (55, 26, distancia_alarma - (distancia_alarma / 10) * 10, Encender_Pixel);
		distancia_alarma = distancia_alarma * unidad_distancia_alarma; // Devolvemos los 0 si estaba en m
		if (unidad_distancia_alarma == 1) OLED_ToggleRectangulo (1, 17, 22, 31);
		else OLED_ToggleRectangulo (1, 33, 22, 47);
		break;
	case mostrar_distancia:
	    OLED_CopiarImagen (mostrar_distancia_7, sizeof (mostrar_distancia_7));
	    for (uint8_t i = 0; i < 3; i ++) OLED_EscribirNumero (63 + 8 * i, 32, 0, Encender_Pixel);
		OLED_EscribirNumero (90, 32, 0, Encender_Pixel);
		OLED_ToggleRectangulo (51, 27, 60, 36);
		*positivo = 1;
		*negativo = 0;
		OLED_ToggleRectangulo (51, 1, 74, 23);
		*centimetros = 1;
		*metros = 0;
		break;
	}
}


void menu_independiente (menu_t menu, opcion_t op, uint8_t numeros_configuracion, uint8_t con_alarma,
						 gravedad_t *gravedad, int16_t *angulo, int16_t *angulo_resultante,
						 int16_t angulo_cero, int32_t *distancia, uint8_t *positivo, uint8_t *negativo,
						 uint8_t *metros, uint8_t *centimetros) {
	switch (menu) {
	case inicio: case alarma: break;
	case config_general:
		if (op == op_a) {
			if (numeros_configuracion == 1) OLED_BarraEscritura (11, 28);
			else OLED_BarraEscritura (19,28);
		}
		if (op == op_b) {
			if (numeros_configuracion == 1) OLED_BarraEscritura (61, 28);
			else OLED_BarraEscritura (69,28);
		}
		break;
	case config_grados:
		if (op == op_a) {
			if (numeros_configuracion == 1) OLED_BarraEscritura (21, 26);
			else if (numeros_configuracion == 2) OLED_BarraEscritura (29, 26);
			else if (numeros_configuracion == 3) OLED_BarraEscritura (37, 26);
			else OLED_BarraEscritura (48, 26);
		}
		break;
	case mostrar_grados:
		if (con_alarma == 1 && op != op_d) {
			if (abs (*angulo_resultante - angulo_alarma) <= 10) {
				INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 11, 99);
			}
			else if (abs (*angulo_resultante - angulo_alarma) <= 50) {
				INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 21, 70);
			}
			else if (abs (*angulo_resultante - angulo_alarma) <= 100) {
				INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 41, 40);
			}
			else {
				INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 61, 10);
			}
		}
		if (GPIO_PinRead (GPIO, PUERTO_0, INT_ADXL345) == 1) {
			*angulo = ADXL345_Angulo (&(*gravedad));
			if (gravedad->indice == 0 && op != op_d) {
				*angulo_resultante = *angulo - angulo_cero;
				if (*angulo_resultante >= 0 && *positivo == 0) {
    				OLED_ToggleRectangulo (53, 1, 73, 21);
    				OLED_ToggleRectangulo (75, 1, 95, 21);
    				*positivo = 1;
    				*negativo = 0;
				}
    			if (*angulo_resultante < 0 && *negativo == 0) {
    				OLED_ToggleRectangulo (53, 1, 73, 21);
    				OLED_ToggleRectangulo (75, 1, 95, 21);
    				*positivo = 0;
    				*negativo = 1;
    			}
    			OLED_RectanguloRelleno (54, 30, 22, 11, Apagar_Pixel);
    			OLED_RectanguloRelleno (81, 30, 6, 11, Apagar_Pixel);
    			*angulo_resultante = abs (*angulo_resultante);
    			OLED_EscribirNumero (54, 30, *angulo_resultante / 1000, Encender_Pixel);
    			OLED_EscribirNumero (62, 30, *angulo_resultante / 100 - (*angulo_resultante / 1000) * 10, Encender_Pixel);
    			OLED_EscribirNumero (70, 30, *angulo_resultante / 10 - (*angulo_resultante / 100) * 10, Encender_Pixel);
    			OLED_EscribirNumero (81, 30, *angulo_resultante - (*angulo_resultante / 10) * 10, Encender_Pixel);
	    		OLED_Refresco ();
			}
		}
	case config_distancia:
		if (op == op_a) {
			if (numeros_configuracion == 1) OLED_BarraEscritura (28, 26);
			else if (numeros_configuracion == 2) OLED_BarraEscritura (36, 26);
			else if (numeros_configuracion == 3) OLED_BarraEscritura (44, 26);
			else OLED_BarraEscritura (55, 26);
		}
		break;
	case mostrar_distancia:
		if (op != op_d) {
			if (antirrebote_encoder == 1) {
				*distancia += ENCODER_Lectura ();
				int32_t distancia_resultante = *distancia;
				if (distancia_resultante >= 0 && *positivo == 0) {
					OLED_ToggleRectangulo (51, 27, 60, 36);
					OLED_ToggleRectangulo (51, 38, 60, 47);
					*positivo = 1;
					*negativo = 0;
				}
				if (distancia_resultante < 0 && *negativo == 0) {
					OLED_ToggleRectangulo (51, 27, 60, 36);
					OLED_ToggleRectangulo (51, 38, 60, 47);
					*positivo = 0;
					*negativo = 1;
				}
				OLED_RectanguloRelleno (63, 32, 22, 11, Apagar_Pixel);
				OLED_RectanguloRelleno (90, 32, 6, 11, Apagar_Pixel);
				distancia_resultante = abs (distancia_resultante);
				if (distancia_resultante <= 9999 && *centimetros == 0) { // En cm mostramos
					OLED_ToggleRectangulo (51, 1, 74, 23);
					OLED_ToggleRectangulo (76, 1, 97, 23);
					*centimetros = 1;
					*metros = 0;
				}
				if (distancia_resultante > 9999 && *metros == 0) { // En m mostramos
					OLED_ToggleRectangulo (51, 1, 74, 23);
					OLED_ToggleRectangulo (76, 1, 97, 23);
					*centimetros = 0;
					*metros = 1;
				}
				if (distancia_resultante > 9999) distancia_resultante = distancia_resultante / 100;
				OLED_EscribirNumero (63, 32, distancia_resultante / 1000, Encender_Pixel);
				OLED_EscribirNumero (71, 32, distancia_resultante / 100 - (distancia_resultante / 1000) * 10, Encender_Pixel);
				OLED_EscribirNumero (79, 32, distancia_resultante / 10 - (distancia_resultante / 100) * 10, Encender_Pixel);
				OLED_EscribirNumero (90, 32, distancia_resultante - (distancia_resultante / 10) * 10, Encender_Pixel);
    			OLED_Refresco ();
			}
			if (con_alarma == 1) {
				if (abs (*distancia - distancia_alarma) <= 10 * unidad_distancia_alarma) {
					INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 11, 99);
				}
				else if (abs (*distancia - distancia_alarma) <= 20 * unidad_distancia_alarma) {
					INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 21, 70);
				}
				else if (abs (*distancia - distancia_alarma) <= 50 * unidad_distancia_alarma) {
					INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 41, 40);
				}
				else {
					INDICADOR_Alarma (TIEMPO_INDICACION, TIEMPO_INDICACION * 61, 10);
				}
			}
		}
	}
}
