#include "definitions.h"

uint8_t tecla_d (opcion_t *op, menu_t *menu, gravedad_t *gravedad) {
	if (*menu != alarma && *menu != config_general) {
		switch (*op) {
		case ninguno: break;
		case op_a:
			if (*menu == inicio) toggle_op_a (*menu);
			else return 0;
			break;
		case op_b:
			if (*menu == inicio) toggle_op_b (*menu);
			else return 0;
			break;
		case op_c:
			if (*menu != inicio) toggle_op_c (*menu);
			else return 0;
			break;
		case op_d:
			switch (*menu) {
			case alarma: case config_general: break;
			case inicio:
				*menu = config_general;
				break;
			case config_grados:
				*menu = mostrar_grados;
				ADXL345_Encender (&(*gravedad));
				break;
			case config_distancia:
				*menu = mostrar_distancia;
				ENCODER_Activo ();
				PINT_EnableCallback (PINT);
				break;
			case mostrar_grados: case mostrar_distancia:
				toggle_op_d (*menu);
				*op = ninguno;
				return 0;
			}
			return 1;
			case op_numeral:
				OLED_ToggleRectangulo (101, 19, 126, 47);
				break;
		}
		toggle_op_d (*menu);
		*op = op_d;
	}
	return 0;
}
