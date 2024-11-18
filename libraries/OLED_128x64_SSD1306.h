/*
 * Se definen todos los comandos, el addres, el ancho y alto y los prototipos de las funciones
 * del display OLED 128x64 con SSD1306
 * Se incluye el archivo con la fuente de 7x7 pixeles
 * Inspirado en fsl_SSD1306_I2C.h de
 * https://community.nxp.com/t5/LPC-Microcontrollers/LPC845-SSD1306-IIC-I2C-APPLICATION-EXAMPLE/m-p/1209656
 */

#ifndef OLED_128X64_SSD1306_H_
#define OLED_128X64_SSD1306_H_

#include <math.h>
#include "fsl_common.h"

/*******************************************************************************************************************************
 * Comandos del SSD1306
 ******************************************************************************************************************************/
#define OLED_SETCONTRAST							0x81
#define OLED_ENTIREDISPLAYON_OFF					0xA4
#define OLED_ENTIREDISPLAYON_ON	 					0xA5
#define OLED_SETNORMALDISPLAY 						0xA6
#define OLED_SETINVERSEDISPLAY 						0xA7
#define OLED_SETDISPLAYOFF 							0xAE
#define OLED_SETDISPLAYON							0xAF
#define OLED_SETDISPLAYOFFSET 						0xD3
#define OLED_SETCOMPINS 							0xDA
#define OLED_SETVCOMLEVEL 							0xDB
#define OLED_SETDISPLAYCLOCKDIVOSCILLATORFREQUENCY	0xD5
#define OLED_SETPRECHARGEPERIOD						0xD9
#define OLED_SETMULTIPLEXRATIO						0xA8
#define OLED_SETLOWERCOLUMN							0x00
#define OLED_SETHIGHERCOLUMN						0x10
#define OLED_SETDISPLAYSTARTLINE					0x40
#define OLED_SETMEMORYADDRESSINGMODE 				0x20
#define OLED_SETCOLUMNADDRESS						0x21
#define OLED_SETPAGEADDRESS							0x22
#define OLED_COMSCANTOPLOW 							0xC0
#define OLED_COMSCANLOWTOP 							0xC8
#define OLED_SETSEGMENTREMAP 						0xA0
#define OLED_SETCHARGEPUMP 							0x8D
#define OLED_ACTIVATE_SCROLL 						0x2F
#define OLED_DEACTIVATE_SCROLL 						0x2E
#define OLED_SET_VERTICAL_SCROLL_AREA				0xA3
#define OLED_RIGHT_HORIZONTAL_SCROLL 				0x26
#define OLED_LEFT_HORIZONTAL_SCROLL 				0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 	0x2A
/*******************************************************************************************************************************
 * Address del SSD1306
 ******************************************************************************************************************************/
#define SSD1306_ADDRESS								0x3C
/*******************************************************************************************************************************
 * Ancho y alto del display
 ******************************************************************************************************************************/
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
/*******************************************************************************************************************************
 * Selección de enceder/apagar display
 ******************************************************************************************************************************/
enum _OLED_Pixel {
	Apagar_Pixel,
	Encender_Pixel,
};
/*******************************************************************************************************************************
 * Prototipos de las funciones
 ******************************************************************************************************************************/

/*
 * Inicialización del display
 * No tiene parametros
 */
void OLED_Inicio (void);

/*
 * Cambia el contraste del display
 * Tiene 1 parametro:
 * Contraste: valor a cambiar
 */
void OLED_Contraste (uint8_t contraste);

/*
 * Refresco del display
 * Envia a pantalla el buffer de datos
 * No tiene parametros
 */
void OLED_Refresco (void);

/*
 * Borra el buffer
 * Llena el buffer con todos ceros
 * No tiene parametros
 */
void OLED_BorrarBuffer (void);

/*
 * Setea un pixel en especifico
 * Tiene 3 parametros:
 * X: coordenada en eje X
 * Y: coordenada en eje Y
 * Estado: encender o apagar pixel
 */
void OLED_SetPixel (uint8_t X, uint8_t Y, uint8_t Estado);

/*
 * Cambia el estado de un pixel
 * Tiene 2 parametros:
 * X: coordenada en eje X
 * Y: coordenada en eje Y
 */
void OLED_TogglePixel (uint8_t X, uint8_t Y);

/*
 * Escribe un numero en el buffer
 * Tiene 4 parametros:
 * X: coordenada en el eje X
 * Y: coordenada en el eje Y
 * numero: numero a escribir
 * Estado: encender o apagar pixeles
 */
void OLED_EscribirNumero (uint8_t X, uint8_t Y, uint8_t numero, uint8_t Estado);

/*
 * Escribe en un rectangulo relleno del Buffer
 * Tiene 5 parametros:
 * X: coordenada en eje X
 * Y: coordenada en eje Y
 * Ancho: ancho del rectangulo
 * Alto: alto del rectangulo
 * Estado: encender o apagar pixeles
 */
void OLED_RectanguloRelleno (uint8_t X, uint8_t Y, uint8_t Ancho, uint8_t Alto, uint8_t Estado);

/*
 * Escribe en un rectangulo vacio del Buffer
 * Tiene 5 parametros:
 * X: coordenada en eje X
 * Y: coordenada en eje Y
 * Ancho: ancho del rectangulo
 * Alto: alto del rectangulo
 * Estado: encender o apagar pixeles
 */
void OLED_RectanguloVacio (uint8_t X, uint8_t Y, uint8_t Ancho, uint8_t Alto, uint8_t Estado);

/*
 * Cambia el estado de los pixeles de un rectangulo
 * Tiene 4 parametros:
 * X_Ini: punto inicial del rectangulo en X
 * Y_Ini: punto inicial del rectangulo en Y
 * X_Fin: punto final del rectangulo en X
 * Y_Fin: punto final del rectangulo en Y
 */
void OLED_ToggleRectangulo (uint8_t X_Ini, uint8_t Y_Ini, uint8_t X_Fin, uint8_t Y_Fin);

/*
 * Dibuja una recta en el Buffer
 * Tiene 5 parametros:
 * X_Ini: posición inicial en X
 * Y_Ini: posición inicial en Y
 * X_Fin: posición final en X
 * Y_Fin: posición final en Y
 * Estado: encender o apagar pixeles
 */
void OLED_DibujarRecta (uint8_t X_Ini, uint8_t Y_Ini, uint8_t X_Fin, uint8_t Y_Fin, uint8_t Estado);

/*
 * Dibuja un archo de circunferencia en el Buffer
 * Tiene 6 parametros:
 * X: centro de arco en eje x
 * Y: centro de arco en eje y
 * Radio: radio del arco
 * Ang_Ini: angulo inicial
 * Ang_Fin: angulo final
 * Estado: encender o apagar pixeles
 */
void OLED_DibujarArco (uint8_t X, uint8_t Y, uint8_t Radio, int16_t Ang_Ini_Por_Diez, int16_t Ang_Fin_Por_Diez, uint8_t Estado);

/*
 * Copia una "imagen" en el buffer
 * Guarda en el buffer los datos del buffer de la imagen
 * Tiene 2 parametros:
 * Imagen: contiene la información de todos los pixeles de la imagen
 * Tamaño: tamaño de la imagen
 */

void OLED_CopiarImagen (const uint8_t *Imagen, uint16_t Tamaño);

#endif /* OLED_128X64_SSD1306_H_ */
