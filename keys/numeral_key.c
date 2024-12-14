#include "definitions.h"

void tecla_numeral (opcion_t *op, menu_t menu) {
	if (menu == inicio) {
		switch (*op) {
		case ninguno: break;
		case op_a:
			toggle_op_a (menu);
			break;
		case op_b:
			toggle_op_b (menu);
			break;
		case op_c: break;
		case op_d:
			toggle_op_d (menu);
			break;
		case op_numeral:
			*op = ninguno;
			if (cambio_de_config == 1) {
				FLASH_Guardado (contraste, volumen, &version, angulo_alarma, distancia_alarma);
				cambio_de_config = 0;
				for (uint8_t i = 0; i < 5; i ++) {
					OLED_ToggleRectangulo (101, 19, 126, 47);
					OLED_Refresco ();
					Delay_ms (250);
					if (i != 4) INDICADOR_Enciendo (TIEMPO_INDICACION, 99);
				}
				return;
			}
		}
		OLED_ToggleRectangulo (101, 19, 126, 47);
		*op = op_numeral;
	}
}
