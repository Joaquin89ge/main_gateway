
#define DEBUG_MODE 

#ifdef DEBUG_MODE

#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define MAX_NODES 250
#define NUMERO_MUESTRAS_ATMOSFERICAS 8

#define CANTIDAD_MUESTRAS_SUELO 2
#define TIMEOUTGRAL 2000           /**< @brief timeout para espera recepcion de datos se usa en requestAmospheric y grondGps entre otros */
#define INTERVALOANNOUNCE 5000     /**< @brief Intervalo en milisegundos (5 seg) para announce*/
#define INTERVALOATMOSPHERIC 30000 /**< @brief Intervalo en milisegundos (1 minutos) para request atmospheric*/

#else

#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define MAX_NODES 250
#define NUMERO_MUESTRAS_ATMOSFERICAS 8

#define CANTIDAD_MUESTRAS_SUELO 2
#define TIMEOUTGRAL 1500            /**< @brief timeout para espera recepcion de datos se usa en requestAmospheric y grondGps entre otros */
#define INTERVALOANNOUNCE 120000    /**< @brief Intervalo en milisegundos (2 min) para announce*/
#define INTERVALOATMOSPHERIC 480000 /**< @brief Intervalo en milisegundos (8 minutos) para request atmospheric*/

#endif


#define MAC_STR_LEN_WITH_NULL 18 

// lora
/**
 * @brief Pin Chip Select (CS) para el módulo LoRa RFM95.
 *
 * Este pin se utiliza para habilitar/deshabilitar la comunicación con el módulo RFM95
 * a través del bus SPI. Corresponde al pin digital D3 en algunas placas de desarrollo.
 */
#define RFM95_CS 2 // d4

/**
 * @brief Pin de Interrupción (INT) para el módulo LoRa RFM95.
 *
 * Este pin es utilizado por el módulo RFM95 para señalar eventos (por ejemplo, paquete recibido)
 * al microcontrolador. Corresponde al pin digital D1 en algunas placas de desarrollo.
 */
#define RFM95_INT 5 // d1


// RTC
#define I2C_SDA 4  // d2
#define I2C_SCL 16 // d0
