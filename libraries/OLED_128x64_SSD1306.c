#include "../libraries/OLED_128x64_SSD1306.h"

#include "fsl_i2c.h"
#include "LPC845.h"

/********************************************************************************************************************************
 * Buffer del display
 * En nuestro caso 128 x 64 = 8192 pixeles, cada 8 bits son
 * 8192 / 8 = 1024 bytes
 *******************************************************************************************************************************/
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];
/********************************************************************************************************************************
 * Funciones
 *******************************************************************************************************************************/
static void OLED_Comando (uint8_t comando) {
	i2c_master_transfer_t xfer = {0};

	xfer.data = (uint8_t *)&comando;
	xfer.dataSize = sizeof(comando);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x0; // Indica que es comando
	xfer.subaddressSize = 1;
	I2C_MasterTransferBlocking (I2C1, &xfer);
}

static void OLED_Data (uint8_t *data) {
	i2c_master_transfer_t xfer = {0};

	xfer.data = data;
	xfer.dataSize = sizeof(OLED_Buffer);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x40; // Indica que escribe en RAM
	xfer.subaddressSize = 1;
	I2C_MasterTransferBlocking (I2C1, &xfer);
}

void OLED_Inicio (void) {
	OLED_Comando (OLED_SETDISPLAYOFF); // Apagar display
	OLED_Comando (OLED_SETDISPLAYCLOCKDIVOSCILLATORFREQUENCY); // Seleccionar frecuencia
	OLED_Comando (0x80); // Frecuencia recomendada
	OLED_Comando (OLED_SETMULTIPLEXRATIO); // Elegir multiplexación
	OLED_Comando (OLED_HEIGHT - 1);	// Multiplexación para las 64 lineas
	OLED_Comando (OLED_SETDISPLAYOFFSET); // Configuramos el offset
	OLED_Comando (0x0); // Sin offset
	OLED_Comando (OLED_SETDISPLAYSTARTLINE | 0x0); // Elegimos la linea de inicio (0)
	OLED_Comando (OLED_SETCHARGEPUMP); // Configuramos la alta tensión
	OLED_Comando (0x14); // Usar 3,3V para generar el alto voltaje
	OLED_Comando (OLED_SETMEMORYADDRESSINGMODE); // Elegimos modo de direccionamiento
	OLED_Comando (0x00); // Direccionamiento horizontal
	OLED_Comando (OLED_SETSEGMENTREMAP | 0x1);
	OLED_Comando (OLED_COMSCANLOWTOP);
	OLED_Comando (OLED_SETCOMPINS);
	OLED_Comando (0x12);
	OLED_Comando (OLED_SETCONTRAST); // Ajustamos contraste
	OLED_Comando (206); // Contraste en 206
	OLED_Comando (OLED_SETPRECHARGEPERIOD);
	OLED_Comando (0xF1);
	OLED_Comando (OLED_SETVCOMLEVEL);
	OLED_Comando (0x40);
	OLED_Comando (OLED_ENTIREDISPLAYON_OFF);
	OLED_Comando (OLED_SETNORMALDISPLAY);
	OLED_Comando (OLED_DEACTIVATE_SCROLL);
	OLED_Comando (OLED_SETCOLUMNADDRESS);
	OLED_Comando (0);
	OLED_Comando (OLED_WIDTH - 1);
	OLED_Comando (OLED_SETPAGEADDRESS);
	OLED_Comando (0);
	OLED_Comando (OLED_HEIGHT / 8 - 1);
	OLED_Comando (OLED_SETDISPLAYON);
	OLED_Comando (OLED_SETLOWERCOLUMN | 0x0);
	OLED_Comando (OLED_SETHIGHERCOLUMN | 0x0);
	OLED_Comando (OLED_SETDISPLAYSTARTLINE | 0x0);
	OLED_BorrarBuffer (); // Vaciamos el buffer
}

void OLED_Contraste (uint8_t contraste) {
	OLED_Comando (OLED_SETCONTRAST); // Ajustamos contraste
	OLED_Comando (contraste);
}

void OLED_Refresco (void) {
	OLED_Comando (0xB0); // Pagina 0
	OLED_Comando (((0 & 0xF0) >> 4) | 0x10); // Parte alta de la columna
	OLED_Comando ((0 & 0x0f) | 0x00); // Parte baja de la columna
	OLED_Data (&OLED_Buffer[0]); // Mostramos el buffer en pantalla
}

void OLED_BorrarBuffer (void) {
	memset (OLED_Buffer, 0, sizeof(OLED_Buffer)); // 0 en el buffer
}

void OLED_SetPixel (uint8_t X, uint8_t Y, uint8_t Estado) {
	// Verificamos que se encuentra en el rango del display
	if(X >= OLED_WIDTH || Y >= OLED_HEIGHT) {
		// No hacer nada
	}
	else {
		switch(Estado) {
			case Apagar_Pixel:
				// Colocamos 0 en la posición del buffer
				OLED_Buffer [X + (Y / 8) * OLED_WIDTH] &= ~(1 << (Y & 7));
				break;
			case Encender_Pixel:
				// Colocamos 1 en la posición del buffer
				OLED_Buffer [X + (Y / 8) * OLED_WIDTH] |= (1 << (Y & 7));
				break;
		}
	}
}

void OLED_TogglePixel (uint8_t X, uint8_t Y) {
	if(X >= OLED_WIDTH || Y >= OLED_HEIGHT) {
		// No hacer nada
	}
	else {
		OLED_Buffer[X + (Y / 8) * OLED_WIDTH] ^= (1 << (Y % 8));
	}
}

void OLED_EscribirNumero (uint8_t X, uint8_t Y, uint8_t numero, uint8_t Estado) {
	if (X < OLED_WIDTH - 6 && Y < OLED_HEIGHT - 11 && numero < 10) {
		switch (numero) {
			case 0:
				OLED_DibujarRecta (X + 0, Y + 3, X + 0, Y + 8, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 10, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 2, Estado);
				OLED_DibujarRecta (X + 2, Y + 9, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 2, Estado);
				OLED_DibujarRecta (X + 4, Y + 9, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 1, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 3, X + 6, Y + 8, Estado);
				break;
			case 1:
				OLED_DibujarRecta (X + 0, Y + 4, X + 0, Y + 5, Estado);
				OLED_DibujarRecta (X + 1, Y + 3, X + 1, Y + 5, Estado);
				OLED_DibujarRecta (X + 2, Y + 2, X + 2, Y + 4, Estado);
				OLED_DibujarRecta (X + 3, Y + 1, X + 3, Y + 3, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 0, X + 5, Y + 11, Estado);
				OLED_DibujarRecta (X + 6, Y + 0, X + 6, Y + 11, Estado);
				break;
			case 2:
				OLED_DibujarRecta (X + 0, Y + 2, X + 0, Y + 3, Estado);
				OLED_DibujarRecta (X + 0, Y + 9, X + 0, Y + 11, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 3, Estado);
				OLED_DibujarRecta (X + 1, Y + 8, X + 1, Y + 11, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 2, Estado);
				OLED_DibujarRecta (X + 2, Y + 7, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 6, X + 3, Y + 8, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 2, Estado);
				OLED_DibujarRecta (X + 4, Y + 5, X + 4, Y + 7, Estado);
				OLED_DibujarRecta (X + 4, Y + 10, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 1, X + 5, Y + 6, Estado);
				OLED_DibujarRecta (X + 5, Y + 10, X + 5, Y + 11, Estado);
				OLED_DibujarRecta (X + 6, Y + 2, X + 6, Y + 5, Estado);
				OLED_DibujarRecta (X + 6, Y + 10, X + 6, Y + 11, Estado);
				break;
			case 3:
				OLED_DibujarRecta (X + 0, Y + 2, X + 0, Y + 3, Estado);
				OLED_DibujarRecta (X + 0, Y + 8, X + 0, Y + 9, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 3, Estado);
				OLED_DibujarRecta (X + 1, Y + 8, X + 1, Y + 10, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 2, Estado);
				OLED_DibujarRecta (X + 2, Y + 9, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 5, X + 3, Y + 5, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 2, Estado);
				OLED_DibujarRecta (X + 4, Y + 4, X + 4, Y + 6, Estado);
				OLED_DibujarRecta (X + 4, Y + 9, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 1, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 2, X + 6, Y + 4, Estado);
				OLED_DibujarRecta (X + 6, Y + 6, X + 6, Y + 9, Estado);
				break;
			case 4:
				OLED_DibujarRecta (X + 0, Y + 6, X + 0, Y + 9, Estado);
				OLED_DibujarRecta (X + 1, Y + 4, X + 1, Y + 9, Estado);
				OLED_DibujarRecta (X + 2, Y + 3, X + 2, Y + 6, Estado);
				OLED_DibujarRecta (X + 2, Y + 8, X + 2, Y + 9, Estado);
				OLED_DibujarRecta (X + 3, Y + 2, X + 3, Y + 4, Estado);
				OLED_DibujarRecta (X + 3, Y + 8, X + 3, Y + 9, Estado);
				OLED_DibujarRecta (X + 4, Y + 1, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 0, X + 5, Y + 11, Estado);
				OLED_DibujarRecta (X + 6, Y + 8, X + 6, Y + 9, Estado);
				break;
			case 5:
				OLED_DibujarRecta (X + 0, Y + 0, X + 0, Y + 5, Estado);
				OLED_DibujarRecta (X + 0, Y + 8, X + 0, Y + 9, Estado);
				OLED_DibujarRecta (X + 1, Y + 0, X + 1, Y + 5, Estado);
				OLED_DibujarRecta (X + 1, Y + 8, X + 1, Y + 10, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 1, Estado);
				OLED_DibujarRecta (X + 2, Y + 4, X + 2, Y + 5, Estado);
				OLED_DibujarRecta (X + 2, Y + 9, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 4, X + 3, Y + 5, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 1, Estado);
				OLED_DibujarRecta (X + 4, Y + 4, X + 4, Y + 6, Estado);
				OLED_DibujarRecta (X + 4, Y + 9, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 0, X + 5, Y + 1, Estado);
				OLED_DibujarRecta (X + 5, Y + 5, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 0, X + 6, Y + 1, Estado);
				OLED_DibujarRecta (X + 6, Y + 6, X + 6, Y + 9, Estado);
				break;
			case 6:
				OLED_DibujarRecta (X + 0, Y + 2, X + 0, Y + 9, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 10, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 2, Estado);
				OLED_DibujarRecta (X + 2, Y + 4, X + 2, Y + 6, Estado);
				OLED_DibujarRecta (X + 2, Y + 9, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 4, X + 3, Y + 5, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 1, Estado);
				OLED_DibujarRecta (X + 4, Y + 4, X + 4, Y + 6, Estado);
				OLED_DibujarRecta (X + 4, Y + 9, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 0, X + 5, Y + 2, Estado);
				OLED_DibujarRecta (X + 5, Y + 5, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 1, X + 6, Y + 2, Estado);
				OLED_DibujarRecta (X + 6, Y + 6, X + 6, Y + 9, Estado);
				break;
			case 7:
				OLED_DibujarRecta (X + 0, Y + 0, X + 0, Y + 1, Estado);
				OLED_DibujarRecta (X + 1, Y + 0, X + 1, Y + 1, Estado);
				OLED_DibujarRecta (X + 1, Y + 9, X + 1, Y + 11, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 1, Estado);
				OLED_DibujarRecta (X + 2, Y + 7, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 5, X + 3, Y + 9, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 1, Estado);
				OLED_DibujarRecta (X + 4, Y + 3, X + 4, Y + 7, Estado);
				OLED_DibujarRecta (X + 5, Y + 0, X + 5, Y + 5, Estado);
				OLED_DibujarRecta (X + 6, Y + 0, X + 6, Y + 3, Estado);
				break;
			case 8:
				OLED_DibujarRecta (X + 0, Y + 2, X + 0, Y + 4, Estado);
				OLED_DibujarRecta (X + 0, Y + 7, X + 0, Y + 9, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 10, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 1, Estado);
				OLED_DibujarRecta (X + 2, Y + 5, X + 2, Y + 6, Estado);
				OLED_DibujarRecta (X + 2, Y + 10, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 5, X + 3, Y + 6, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 1, Estado);
				OLED_DibujarRecta (X + 4, Y + 5, X + 4, Y + 6, Estado);
				OLED_DibujarRecta (X + 4, Y + 10, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 1, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 2, X + 6, Y + 4, Estado);
				OLED_DibujarRecta (X + 6, Y + 7, X + 6, Y + 9, Estado);
				break;
			case 9:
				OLED_DibujarRecta (X + 0, Y + 2, X + 0, Y + 5, Estado);
				OLED_DibujarRecta (X + 0, Y + 9, X + 0, Y + 10, Estado);
				OLED_DibujarRecta (X + 1, Y + 1, X + 1, Y + 6, Estado);
				OLED_DibujarRecta (X + 1, Y + 9, X + 1, Y + 11, Estado);
				OLED_DibujarRecta (X + 2, Y + 0, X + 2, Y + 2, Estado);
				OLED_DibujarRecta (X + 2, Y + 5, X + 2, Y + 7, Estado);
				OLED_DibujarRecta (X + 2, Y + 10, X + 2, Y + 11, Estado);
				OLED_DibujarRecta (X + 3, Y + 0, X + 3, Y + 1, Estado);
				OLED_DibujarRecta (X + 3, Y + 6, X + 3, Y + 7, Estado);
				OLED_DibujarRecta (X + 3, Y + 10, X + 3, Y + 11, Estado);
				OLED_DibujarRecta (X + 4, Y + 0, X + 4, Y + 2, Estado);
				OLED_DibujarRecta (X + 4, Y + 5, X + 4, Y + 7, Estado);
				OLED_DibujarRecta (X + 4, Y + 9, X + 4, Y + 11, Estado);
				OLED_DibujarRecta (X + 5, Y + 1, X + 5, Y + 10, Estado);
				OLED_DibujarRecta (X + 6, Y + 2, X + 6, Y + 9, Estado);
				break;
		}
	}
}

void OLED_RectanguloRelleno (uint8_t X, uint8_t Y, uint8_t Ancho, uint8_t Alto, uint8_t Estado) {
	if ((X + Ancho) >= OLED_WIDTH || (Y + Alto) >= OLED_HEIGHT){
		// No hacer nada
	}
	else {
		for (uint8_t i = X; i <= X + Ancho; i ++) {
			for (uint8_t j = Y; j <= Y + Alto; j ++) {
				OLED_SetPixel (i, j, Estado);
			}
		}
	}
}

void OLED_RectanguloVacio (uint8_t X, uint8_t Y, uint8_t Ancho, uint8_t Alto, uint8_t Estado) {
	if ((X + Ancho) > OLED_WIDTH || (Y + Alto) > OLED_HEIGHT){
		// No hacer nada
	}
	else {
	    // Dibujar la línea superior
	    for (uint8_t i = 0; i < Ancho; i++) {
	    	OLED_SetPixel(X + i, Y, Estado);
	    }
	    // Dibujar la línea inferior
	    for (uint8_t i = 0; i < Ancho; i++) {
	    	OLED_SetPixel(X + i, Y + Alto - 1, Estado);
	    }
	    // Dibujar la línea izquierda
	    for (uint8_t i = 0; i < Alto; i++) {
	    	OLED_SetPixel(X, Y + i, Estado);
	    }
	    // Dibujar la línea derecha
	    for (uint8_t i = 0; i < Alto; i++) {
	    	OLED_SetPixel(X + Ancho - 1, Y + i, Estado);
	    }
	}
}

void OLED_ToggleRectangulo (uint8_t X_Ini, uint8_t Y_Ini, uint8_t X_Fin, uint8_t Y_Fin) {
	uint8_t X_menor = X_Ini, X_mayor = X_Fin, Y_menor = Y_Ini, Y_mayor = Y_Fin;

	if (X_Ini >= OLED_WIDTH || Y_Ini >= OLED_HEIGHT || X_Fin >= OLED_WIDTH || Y_Fin >= OLED_HEIGHT) {
			// No hacer nada
	}
	else {
		if (X_Ini > X_Fin) {
			X_mayor = X_Ini;
			X_menor = X_Fin;
		}
		if (Y_Ini > Y_Fin) {
			Y_mayor = Y_Ini;
			Y_menor = Y_Fin;
		}
		for (uint8_t x = X_menor; x <= X_mayor; x ++) {
			for (uint8_t y = Y_menor; y <= Y_mayor; y ++) {
				OLED_TogglePixel (x, y);
			}
		}
	}
}

void OLED_DibujarRecta (uint8_t X_Ini, uint8_t Y_Ini, uint8_t X_Fin, uint8_t Y_Fin, uint8_t Estado) {
	int16_t dx = abs (X_Fin - X_Ini);
	int16_t dy = abs (Y_Fin - Y_Ini);
	int8_t sx = (X_Ini < X_Fin) ? 1 : -1;
	int8_t sy = (Y_Ini < Y_Fin) ? 1 : -1;
	int16_t err = dx - dy;
	int16_t e2;

	if (X_Ini >= OLED_WIDTH || Y_Ini >= OLED_HEIGHT || X_Fin >= OLED_WIDTH || Y_Fin >= OLED_HEIGHT) {
				// No hacer nada
	}
	else {
		while (1) {
        	OLED_SetPixel (X_Ini, Y_Ini, Estado);
        	if (X_Ini == X_Fin && Y_Ini == Y_Fin) break;
        	e2 = 2 * err;
        	if (e2 > -dy) {
            	err -= dy;
            	X_Ini += sx;
        	}
        	if (e2 < dx) {
            	err += dx;
            	Y_Ini += sy;
        	}
    	}
	}
}

void OLED_DibujarArco (uint8_t X, uint8_t Y, uint8_t Radio, int16_t Ang_Ini_Por_Diez, int16_t Ang_Fin_Por_Diez, uint8_t Estado) {
	uint8_t x, y;
	int16_t angulo_menor = Ang_Ini_Por_Diez, angulo_mayor = Ang_Fin_Por_Diez;

	if (Ang_Ini_Por_Diez > Ang_Fin_Por_Diez) {
		angulo_menor = Ang_Fin_Por_Diez;
		angulo_mayor = Ang_Ini_Por_Diez;
	}
	while (angulo_menor <= angulo_mayor) {
		x = X + (int16_t)(Radio * cos(angulo_menor * (3.141592654 / 180.0) / 10));
		y = Y - (int16_t)(Radio * sin(angulo_menor * (3.141592654 / 180.0) / 10));
		OLED_SetPixel (x, y, Estado);
		angulo_menor += 5;
	}
}

void OLED_CopiarImagen (const uint8_t *Imagen, uint16_t Tamaño) {
	uint16_t CpyBuffer;

	OLED_BorrarBuffer ();
	for(CpyBuffer = 0; CpyBuffer < Tamaño; CpyBuffer ++) {
		OLED_Buffer[CpyBuffer] = *(Imagen + CpyBuffer);
	}
}
