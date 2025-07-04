
#define DEBUGPRINTS false

#define MAX_NODES 250
#define NUMERO_MUESTRAS_ATMOSFERICAS 8
#define CANTIDAD_PAQUETES_ALMACENAR
#define CANTIDAD_MUESTRAS_SUELO 2
#define TIMEOUTGRAL 1500            /**< @brief timeout para espera recepcion de datos se usa en requestAmospheric y grondGps entre otros */
#define INTERVALOANNOUNCE 120000    /**< @brief Intervalo en milisegundos (2 min) para announce*/
#define INTERVALOATMOSPHERIC 480000 /**< @brief Intervalo en milisegundos (8 minutos) para request atmospheric*/

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

/**
 * @brief Frecuencia LoRa configurada en MHz.
 *
 * Este define establece la frecuencia de operación para la comunicación LoRa.
 * Debe elegirse en función de las regulaciones regionales y otros nodos de la red.
 */
#define FRECUENCI_CONF 415.0

/**
 * @brief Nivel de potencia de transmisión para LoRa en dBm.
 *
 * Este define establece la potencia de transmisión del módulo LoRa.
 * Un valor más alto generalmente significa un mayor alcance pero consume más energía.
 * La potencia máxima para el RFM95 es típicamente de 20 dBm.
 */
#define POWER_LORA 23

// RTC
#define I2C_SDA 4  // d2
#define I2C_SCL 16 // d0
