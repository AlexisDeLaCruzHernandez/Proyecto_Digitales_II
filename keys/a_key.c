#include "definitions.h"

uint8_t tecla_a (opcion_t *op, menu_t *menu, uint8_t *con_alarma, modo_t *modo, uint8_t *numeros_configuracion) {
	if (*menu != mostrar_grados && *menu != mostrar_distancia) {
		switch (*op) {
		case ninguno: break;
		case op_a:
			if (*menu == inicio) {
				*menu = alarma;
				*modo = distancias;
				return 1;
			}
			else if (*menu == alarma) {
				if (*modo == distancias) *menu = config_distancia;
				else *menu = config_grados;
				*con_alarma = 1;
				return 1;
			}
			else return 0;
		case op_b:
			if (*menu == inicio || *menu == alarma) toggle_op_b (*menu);
			else return 0;
			break;
		case op_c:
			if (*menu != inicio) toggle_op_c (*menu);
			else return 0;
			break;
		case op_d:
			if (*menu != alarma && *menu != config_general) toggle_op_d (*menu);
			else return 0;
			break;
		case op_numeral:
			OLED_ToggleRectangulo (101, 19, 126, 47);
			break;
		}
		toggle_op_a (*menu);
		*op = op_a;
		if (*menu == config_general) {
			OLED_RectanguloRelleno (11, 28, 14, 11, Apagar_Pixel);
			(*numeros_configuracion) ++;
			contraste = 0;
			flag_tick_barra = 0;
		}
		else if (*menu == config_grados) {
			angulo_alarma = 0;
			OLED_RectanguloRelleno (21, 26, 23, 11, Apagar_Pixel);
			OLED_RectanguloRelleno (48, 26, 6, 11, Apagar_Pixel);
			(*numeros_configuracion) ++;
			flag_tick_barra = 0;
		}
		else if (*menu == config_distancia) {
			distancia_alarma = 0;
			OLED_RectanguloRelleno (28, 26, 23, 11, Apagar_Pixel);
			OLED_RectanguloRelleno (55, 26, 6, 11, Apagar_Pixel);
			(*numeros_configuracion) ++;
			flag_tick_barra = 0;
		}
	}
	return 0;
}
