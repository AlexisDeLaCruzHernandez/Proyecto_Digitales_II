#include "ADXL345.h"

void ADXL345_Comando (uint8_t direccionRegistro, uint8_t valor) {
    i2c_master_transfer_t masterXfer;

    uint8_t data[2];
    data[0] = direccionRegistro; // Dirección del registro
    data[1] = valor;             // Valor a escribir

    masterXfer.slaveAddress = ADDRESS_ADXL345;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = data;
    masterXfer.dataSize = sizeof(data);
    masterXfer.flags = kI2C_TransferDefaultFlag;

    I2C_MasterTransferBlocking(I2C1, &masterXfer);
}

void ADXL345_Configuracion (void) {
	ADXL345_Comando (INT_ENABLE, 0x80); // Interrupción data ready activada en INT1
	ADXL345_Comando (BW_RATE, 0x09); // 50Hz
	ADXL345_Comando (DATA_FORMAT, 0x00); // Formato
	ADXL345_Comando (OFSZ, 50); // Offset eje Z

	gpio_pin_config_t in_config = {kGPIO_DigitalInput};

	//GPIO_PortInit (GPIO, PUERTO_0); // Ya inicializado en otro lado
	GPIO_PinInit (GPIO, PUERTO_0, INT_ADXL345, &in_config);
}

void ADXL345_Encender (gravedad_t *gravedad) {
	ADXL345_Comando (POWER_CTL, ADXL345_ON);
	for (uint8_t i = 0; i < 10; i ++) {
		gravedad->gravedades_x[i] = 0;
		gravedad->gravedades_y[i] = 0;
		gravedad->gravedades_z[i] = 0;
	}
	gravedad->suma_gx = 0;
	gravedad->suma_gy = 0;
	gravedad->suma_gz = 0;
	gravedad->indice = 0;
}

void ADXL345_Apagar (void) {
	ADXL345_Comando (POWER_CTL, ADXL345_OFF);
}

int16_t ADXL345_Angulo (gravedad_t *gravedad) {
	int16_t x, y, z, angulo;
	float gx, gy, gz;
	uint8_t lecturas[] = {0, 0, 0, 0, 0, 0};
	uint8_t REGISTRO_DATAX0 = 0x32;

	I2C_MasterStart (I2C1, ADDRESS_ADXL345, kI2C_Write);
	I2C_MasterWriteBlocking (I2C1, &REGISTRO_DATAX0, 1, 0);
	I2C_MasterRepeatedStart (I2C1, ADDRESS_ADXL345, kI2C_Read);
	I2C_MasterReadBlocking (I2C1, &lecturas[0], 6, 0);

	x = (((int16_t)lecturas[1]) << 8 | lecturas[0]);
	gx = (float) x / 256;
	y = (((int16_t)lecturas[3]) << 8 | lecturas[2]);
	gy = (float) y / 256;
	z = (((int16_t)lecturas[5]) << 8 | lecturas[4]);
	gz = (float) z / 256;
	gravedad->suma_gx -= gravedad->gravedades_x[gravedad->indice];
	gravedad->gravedades_x[gravedad->indice] = gx;
	gravedad->suma_gx += gravedad->gravedades_x[gravedad->indice];
	gravedad->suma_gy -= gravedad->gravedades_y[gravedad->indice];
	gravedad->gravedades_y[gravedad->indice] = gy;
	gravedad->suma_gy += gravedad->gravedades_y[gravedad->indice];
	gravedad->suma_gz -= gravedad->gravedades_z[gravedad->indice];
	gravedad->gravedades_z[gravedad->indice] = gz;
	gravedad->suma_gz += gravedad->gravedades_z[gravedad->indice];
	gravedad->indice += 1;
	if (gravedad->indice == 10) gravedad->indice = 0;

	angulo = (int16_t) (acos ((gravedad->suma_gz / 10) /
			sqrt (pow (gravedad->suma_gx / 10, 2) + pow (gravedad->suma_gy / 10, 2) +
			pow (gravedad->suma_gz / 10, 2))) * 10 * (180 / PI));
	if (gravedad->suma_gx / 10 < 0) return -angulo;
	else return angulo;
}
