#include "definitions.h"

uint8_t tecla_b (opcion_t *op, menu_t *menu, uint8_t *con_alarma, modo_t *modo, uint8_t *numeros_configuracion,
				 int16_t *angulo_cero, int16_t angulo, int32_t *distancia, gravedad_t *gravedad) {
	switch (*op) {
	case ninguno: break;
	case op_a:
		if (*menu == inicio || *menu == alarma) toggle_op_a (*menu);
		else return 0;
		break;
	case op_b:
		if (*menu == inicio) {
			*menu = alarma;
			*modo = grados;
			return 1;
		}
		else if (*menu == alarma) {
			if (*modo == distancias) {
				*menu = mostrar_distancia;
				ENCODER_Activo ();
			}
			else {
				*menu = mostrar_grados;
				ADXL345_Encender (&(*gravedad));
			}
			*con_alarma = 0;
			return 1;
		}
		else return 0;
		break;
	case op_c:
		if (*menu != inicio && *menu != config_grados && *menu != config_distancia) toggle_op_c (*menu);
		break;
	case op_d:
		if (*menu == inicio) toggle_op_d (*menu);
		if (*menu == mostrar_distancia || *menu == mostrar_grados) return 0;
		break;
	case op_numeral:
		OLED_ToggleRectangulo (101, 19, 126, 47);
		break;
	}
	toggle_op_b (*menu);
	if (*menu == inicio || *menu == alarma || *menu == config_general) {
		*op = op_b;
	}
	switch (*menu) {
	case inicio: case alarma: break;
	case config_general:
		OLED_RectanguloRelleno (61, 28, 14, 11, Apagar_Pixel);
		(*numeros_configuracion) ++;
		volumen_config = 0;
		flag_tick_barra = 0;
		break;
	case config_grados:
		signo_angulo_alarma = signo_angulo_alarma * (-1);
    	angulo_alarma = angulo_alarma * signo_angulo_alarma;
    	cambio_de_config = 1;
    	break;
	case mostrar_grados:
		*angulo_cero = angulo;
	case mostrar_distancia:
		*distancia = 0;
		OLED_Refresco ();
		Delay_ms (100);
		toggle_op_b (*menu);
		*op = ninguno;
		return 1;
	case config_distancia:
		if (unidad_distancia_alarma == 1) {
			unidad_distancia_alarma = 100;
			distancia_alarma = distancia_alarma * 100;
		}
		else {
			unidad_distancia_alarma = 1;
			distancia_alarma = distancia_alarma / 100;
		}
    	cambio_de_config = 1;
		break;
	}
	return 0;
}
