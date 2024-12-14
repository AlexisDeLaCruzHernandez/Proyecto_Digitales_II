#include "definitions.h"

uint8_t tecla_c (opcion_t *op, menu_t *menu, uint8_t con_alarma, int16_t *angulo_cero, int32_t *distancia) {
	if (*menu != inicio) {
		switch (*op) {
		case ninguno: break;
		case op_a:
			if (*menu != config_general && *menu != config_grados && *menu != config_distancia) toggle_op_a (*menu);
			break;
		case op_b:
			if (*menu != config_general && *menu != config_grados && *menu != config_distancia) toggle_op_b (*menu);
			break;
		case op_c:
			switch (*menu) {
			case inicio: break;
			case alarma: case config_general:
				*menu = inicio;
				break;
			case config_grados: case config_distancia:
				*menu = alarma;
				break;
			case mostrar_grados:
				if (con_alarma) *menu = config_grados;
				else *menu = alarma;
				*angulo_cero = 0;
				ADXL345_Apagar ();
				break;
			case mostrar_distancia:
				if (con_alarma) *menu = config_distancia;
				else *menu = alarma;
				*distancia = 0;
				ENCODER_Desactivo ();
				break;
			}
			return 1;
		case op_d:
			if (*menu == config_grados || *menu == config_distancia) toggle_op_d (*menu);
			else return 0;
			break;
		case op_numeral: break;
		}
		toggle_op_c (*menu);
		*op = op_c;
	}
	return 0;
}
