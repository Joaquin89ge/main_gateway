// RadioManager.cpp
#include "radio_manager.h"

RadioManager::RadioManager(uint8_t address)
    : driver(RFM95_CS, RFM95_INT), manager(driver, address), failureCount(0)
{
}

bool RadioManager::init()
{
  Serial.println("[DEBUG] RadioManager::init INICIO");
  // Configurar el pin de RESET como OUTPUT
  pinMode(RFM95_RST, OUTPUT);
  // Iniciar SPI
  
  while (!manager.init())
  {
    Serial.print(" ");
    Serial.println("RF95 MESH init failed");
    // Baja el pin RST para poner el módulo en reset
    digitalWrite(RFM95_RST, LOW);
    delay(10); // Mantenerlo bajo por un corto tiempo
    // Sube el pin RST para liberar el módulo del reset
    digitalWrite(RFM95_RST, HIGH);
    delay(10); // Darle tiempo al módulo para arrancar
    delay(100);
  }

  Serial.print("RF95 MESH init okay");
  Serial.println("[DEBUG] RadioManager::init FIN");
  return true;
}

bool RadioManager::sendMessage(uint8_t to, uint8_t *data, uint8_t len, uint8_t flag)
{
    Serial.println("[DEBUG] RadioManager::sendMessage INICIO");
    
    // Envía el mensaje y espera un acuse de recibo.
    // RH_ROUTER_ERROR_NONE indica una transmisión y acuse de recibo exitosos.
 
    uint8_t result = manager.sendtoWait(data, len, to, flag);
    
    if (result == RH_ROUTER_ERROR_NONE)
    {
        // Éxito: resetear contador de fallos
        resetFailureCounter();
        return true;
    }
    else
    {
        // Fallo: incrementar contador y verificar si necesita reset
        if (handleTransmissionFailure()) {
            Serial.println("[RadioManager] Módulo reseteado automáticamente");
        }
        return false;
    }
}

bool RadioManager::recvMessage(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag)
{
  uint8_t dest;
  uint8_t id;
  // Intenta recibir un mensaje reconocido.
  if (manager.recvfromAck(buf, len, from, &dest, &id, flag))
  {
    return true; // Mensaje recibido con éxito.
  }
  return false; // No se recibió ningún mensaje o falló el acuse de recibo.
}

bool RadioManager::recvMessageTimeout(uint8_t *buf, uint8_t *len, uint8_t *from, uint8_t *flag, uint16_t timeout)
{
  uint8_t dest;      // Variable interna para la dirección de destino
  uint8_t messageId; // Variable interna para el ID del mensaje de RadioHead

  // Llama a manager.recvfromAckTimeout con los parámetros adecuados
  // Los punteros 'from', 'dest', 'messageId' y 'flags' deben ser proporcionados a la función.
  // En tu caso, necesitas el 'from' y el 'flag' que se pasan,
  // y 'dest' y 'messageId' se pueden usar como variables temporales internas.
  // La función recvfromAckTimeout es:
  // bool recvfromAckTimeout(uint8_t* buf, uint8_t* len, uint16_t timeout, uint8_t* from = NULL, uint8_t* dest = NULL, uint8_t* id = NULL, uint8_t* flags = NULL);

  // Aquí, 'flag' del parámetro de tu función se mapea a 'flags' de recvfromAckTimeout
  // y 'from' de tu función se mapea a 'from' de recvfromAckTimeout.
  // 'dest' y 'messageId' son variables temporales que no necesitas devolver.
  if (manager.recvfromAckTimeout(buf, len, timeout, from, &dest, &messageId, flag))
  {
    // Mensaje recibido y reconocido con éxito dentro del tiempo
    return true;
  }
  // No se recibió un mensaje reconocido dentro del tiempo
  return false;
}

/**
 * @brief Método de marcador de posición para actualizar el estado interno del gestor de radio.
 *
 * Actualmente, la librería RHMesh maneja la mayoría de sus procesos internos (como
 * retransmisión, actualizaciones de la tabla de enrutamiento y manejo de acuses de recibo) automáticamente.
 * Este método se proporciona como un marcador de posición vacío para futuras expansiones o
 * si son necesarias tareas periódicas específicas relacionadas con la gestión de radio.
 * Por ejemplo, podría usarse para la gestión de energía personalizada, sondeo de estado,
 * o lógica de enrutamiento avanzada si el comportamiento predeterminado de RHMesh es insuficiente.
 */
void RadioManager::update()
{
  // RHMesh maneja internamente el reintento y enrutamiento.
  // Este método se reserva para futuras ampliaciones si se requiere.
}

bool RadioManager::handleTransmissionFailure()
{
    failureCount++;
    Serial.printf("[RadioManager] Fallo de transmisión #%d/%d\n", failureCount, RADIO_MAX_FAILURES);
    
    if (failureCount >= RADIO_MAX_FAILURES) {
        Serial.println("[RadioManager] Máximo de fallos alcanzado, reseteando módulo...");
        forceRadioReset();
        return true;
    }
    return false;
}

void RadioManager::resetFailureCounter()
{
    if (failureCount > 0) {
        Serial.printf("[RadioManager] Reset contador de fallos (era %d)\n", failureCount);
        failureCount = 0;
    }
}

uint8_t RadioManager::getFailureCount() const
{
    return failureCount;
}

void RadioManager::forceRadioReset()
{
    Serial.println("[RadioManager] Iniciando reset forzado del módulo radio...");
    
    // Reset del módulo usando el pin RST
    digitalWrite(RFM95_RST, LOW);
    delay(10); // Mantener reset por 10ms
    digitalWrite(RFM95_RST, HIGH);
    delay(RADIO_RESET_DELAY_MS); // Esperar a que el módulo se estabilice
    
    // Reinicializar el módulo
    if (manager.init()) {
        Serial.println("[RadioManager] Reset exitoso, módulo reinicializado");
        failureCount = 0; // Resetear contador después del reset exitoso
    } else {
        Serial.println("[RadioManager] ERROR: Fallo en reinicialización después del reset");
    }
}
