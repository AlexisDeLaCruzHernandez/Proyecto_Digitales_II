#include "definitions.h"

void tecla_num (opcion_t *op, menu_t *menu, uint8_t *numeros_configuracion, uint8_t *boton) {
	switch (*menu) {
	case inicio: case alarma: case mostrar_grados: case mostrar_distancia: break;
	case config_general:
		if (*numeros_configuracion == 1) {
			if (*op == op_a) contraste += (*boton - 48) * 10;
			else volumen_config += (*boton - 48) * 10;
			OLED_RectanguloRelleno (50 * (*op) - 39, 28, 1, 11, Apagar_Pixel);
			OLED_EscribirNumero (50 * (*op) - 39, 28, *boton - 48, Encender_Pixel);
		}
		else {
			if (*op == op_a) contraste += *boton - 48;
			else volumen_config += *boton - 48;
			OLED_RectanguloRelleno (50 * (*op) - 31, 28, 1, 11, Apagar_Pixel);
			OLED_EscribirNumero (50 * (*op) - 31, 28, *boton - 48, Encender_Pixel);
		}
		(*numeros_configuracion) ++;
		if (*numeros_configuracion == 3) {
			*numeros_configuracion = 0;
			if (*op == op_a) {
				toggle_op_a (*menu);
				OLED_Contraste (contraste * 255 / 99);
			}
			else {
				toggle_op_b (*menu);
				volumen = volumen_config;
			}
			cambio_de_config = 1;
			*op = ninguno;
		}
		break;
	case config_grados:
		switch (*numeros_configuracion) {
		case 1:
			if (*boton == '0' || *boton == '1') {
				angulo_alarma += (*boton - 48) * 1000;
				OLED_RectanguloRelleno (21, 26, 1, 11, Apagar_Pixel);
				OLED_EscribirNumero (21, 26, *boton - 48, Encender_Pixel);
				(*numeros_configuracion) ++;
			}
			break;
		case 2:
			if ((angulo_alarma == 1000 && (*boton - 48) <= 8) || angulo_alarma == 0) {
				angulo_alarma += (*boton - 48) * 100;
				OLED_RectanguloRelleno (29, 26, 1, 11, Apagar_Pixel);
				OLED_EscribirNumero (29, 26, *boton - 48, Encender_Pixel);
				(*numeros_configuracion) ++;
			}
			break;
		case 3:
			if ((angulo_alarma == 1800 && (*boton - 48) == 0) || angulo_alarma < 1800) {
				angulo_alarma += (*boton - 48) * 10;
				OLED_RectanguloRelleno (37, 26, 1, 11, Apagar_Pixel);
				OLED_EscribirNumero (37, 26, *boton - 48, Encender_Pixel);
				(*numeros_configuracion) ++;
			}
			break;
		case 4:
			if ((angulo_alarma == 1800 && (*boton - 48) == 0) || angulo_alarma < 1800) {
				angulo_alarma += *boton - 48;
				OLED_RectanguloRelleno (48, 26, 1, 11, Apagar_Pixel);
				OLED_EscribirNumero (48, 26, *boton - 48, Encender_Pixel);
				(*numeros_configuracion) ++;
			}
			break;
		}
	    if (*numeros_configuracion == 5) {
	    	*numeros_configuracion = 0;
	    	angulo_alarma = angulo_alarma * signo_angulo_alarma;
	    	toggle_op_a (*menu);
	    	cambio_de_config = 1;
	    	*op = ninguno;
	    }
		break;
	case config_distancia:
		if (*numeros_configuracion < 4) {
			distancia_alarma += (*boton - 48) * pow (10, 4 - (*numeros_configuracion));
			OLED_RectanguloRelleno (20 + 8 * (*numeros_configuracion), 26, 1, 11, Apagar_Pixel);
			OLED_EscribirNumero (20 + 8 * (*numeros_configuracion), 26, *boton - 48, Encender_Pixel);
		}
		else {
			distancia_alarma += *boton - 48;
			OLED_RectanguloRelleno (55, 26, 1, 11, Apagar_Pixel);
			OLED_EscribirNumero (55, 26, *boton - 48, Encender_Pixel);
		}
		(*numeros_configuracion) ++;
	    if (*numeros_configuracion == 5) {
	    	*numeros_configuracion = 0;
	    	if (unidad_distancia_alarma == 100) {
	    		distancia_alarma = distancia_alarma * unidad_distancia_alarma;
	    		// Si está en metros se guarda en centimetros la alarma
	    	}
	    	toggle_op_a (*menu);
	    	cambio_de_config = 1;
	    	*op = ninguno;
	    }
		break;
	}
}
