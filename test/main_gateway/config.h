
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

// Configuración de delays para solicitudes de datos (más conservador en producción)
//#define DELAY_BETWEEN_ATMOSPHERIC_REQUESTS 2000  /**< @brief Delay entre solicitudes atmosféricas en milisegundos (2 segundos) */
//#define DELAY_BETWEEN_GROUND_REQUESTS 5000       /**< @brief Delay entre solicitudes de suelo/GPS en milisegundos (5 segundos) */
#define DELAY_BETWEEN_NODES 200                 /**< @brief Delay entre procesamiento de nodos en milisegundos (1.5 segundos) */
#define DELAY_BEFORE_RETRY_ATMOSPHERIC 2000       /**< @brief Delay antes de reintentar solicitudes atmosféricas (500ms) */
#define DELAY_BEFORE_RETRY_GROUND 10000           /**< @brief Delay antes de reintentar solicitudes de suelo/GPS (1.5 segundos) */

#define RH_MESH_MAX_MESSAGE_LEN 50


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
#define RFM95_INT 15 // d8

#define RFM95_RST 16 // d0

/**
 * @def RADIO_RESET_DELAY_MS
 * @brief Tiempo de espera después del reset del módulo radio en milisegundos.
 */
#define RADIO_RESET_DELAY_MS 1000 // 1 segundo

/**
 * @def RADIO_MAX_FAILURES
 * @brief Número máximo de fallos de transmisión antes de resetear el módulo radio.
 */
#define RADIO_MAX_FAILURES 5  


// RTC DS1302 (3-wire interface)
#define RTC_CLK 18  // Pin CLK del DS1302 (D3 en ESP8266)
#define RTC_DAT 19  // Pin DAT del DS1302 (D2 en ESP8266) 
#define RTC_RST 4  // Pin RST del DS1302 (D0 en ESP8266)
