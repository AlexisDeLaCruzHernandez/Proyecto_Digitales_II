#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include "OLED_128x64_SSD1306.h"
#include "ADXL345.h"
#include "ENCODER.h"
#include "ALARMA.h"
#include "FLASH.h"
#include "images.h"

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"

#define PUERTO_0 0
#define FILA_1 16
#define FILA_2 17
#define FILA_3 18
#define FILA_4 19
#define COLUMNA_1 20
#define COLUMNA_2 21
#define COLUMNA_3 22
#define COLUMNA_4 23

typedef enum {
	ninguno = 0,
	op_a = 1,
	op_b = 2,
	op_c = 3,
	op_d = 4,
	op_numeral = 5,
} opcion_t;

typedef enum {
	distancias = 1,
	grados = 2,
} modo_t;

typedef enum {
	inicio = 1,
	alarma = 2,
	config_general = 3,
	config_grados = 4,
	mostrar_grados = 5,
	config_distancia = 6,
	mostrar_distancia = 7,
} menu_t;

void Delay_ms (uint8_t mseg);
void GPIO_Inicializacion (void);
void SysTick_Inicializacion (void);
void I2C_Inicializacion (void);
uint8_t TECLADO_Lectura (uint8_t Fila_Ini, uint8_t Columna_Ini, uint8_t Fila_Fin, uint8_t Columna_Fin);
void OLED_BarraEscritura (uint8_t X, uint8_t Y);

void toggle_op_a (menu_t menu);
void toggle_op_b (menu_t menu);
void toggle_op_c (menu_t menu);
void toggle_op_d (menu_t menu);
void mostrar_menu (menu_t menu, uint8_t *positivo, uint8_t *negativo, uint8_t *centimetros, uint8_t *metros);
void menu_independiente (menu_t menu, opcion_t op, uint8_t numeros_configuracion, uint8_t con_alarma,
						 gravedad_t *gravedad, int16_t *angulo, int16_t *angulo_resultante,
						 int16_t angulo_cero, int32_t *distancia, uint8_t *positivo, uint8_t *negativo,
						 uint8_t *metros, uint8_t *centimetros);

/*
 * op: opcion que está activa en el menu
 * menu: menu que se está mostrando
 * con_alarma: indica si se usa la alarma o no
 * modo: indica si es medicion de distancia o de angulo
 * numeros_configuracion: indica el numero que se está inigresando por teclado
 */
uint8_t tecla_a (opcion_t *op, menu_t *menu, uint8_t *con_alarma, modo_t *modo, uint8_t *numeros_configuracion);
uint8_t tecla_b (opcion_t *op, menu_t *menu, uint8_t *con_alarma, modo_t *modo, uint8_t *numeros_configuracion,
				 int16_t *angulo_cero, int16_t angulo, int32_t *distancia, gravedad_t *gravedad);
uint8_t tecla_c (opcion_t *op, menu_t *menu, uint8_t con_alarma, int16_t *angulo_cero, int32_t *distancia);
uint8_t tecla_d (opcion_t *op, menu_t *menu, gravedad_t *gravedad);
void tecla_num (opcion_t *op, menu_t *menu, uint8_t *numeros_configuracion, uint8_t *boton);
void tecla_numeral (opcion_t *op, menu_t menu);

extern volatile uint8_t flag_tick;
extern volatile uint16_t flag_tick_barra;
extern uint8_t teclas[];
extern uint8_t volumen, volumen_config, contraste, version, cambio_de_config;
extern int32_t distancia_alarma;
extern uint8_t unidad_distancia_alarma;
extern int16_t angulo_alarma;
extern int8_t signo_angulo_alarma;

#endif /* DEFINITIONS_H_ */
