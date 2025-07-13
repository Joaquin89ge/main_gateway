#include "rtc_manager.h" // Incluye la cabecera de tu clase para acceder a sus declaraciones

// Constructor de la clase
RtcManager::RtcManager()
{
    // El constructor se puede dejar vacío si la inicialización se hace en begin()
}

// Método para inicializar el RTC. Debe llamarse en setup().
bool RtcManager::begin()
{
    Wire.begin(I2C_SDA, I2C_SCL); // Inicializa I2C con los pines definidos
    DEBUG_PRINTLN("Inicializando RTC...");
    if (!rtc.begin())
    {
        DEBUG_PRINTLN("¡Error! No se encontró el módulo RTC. Revisa las conexiones.");
        return false; // Indica fallo en la inicialización
    }

    // Opcional: Comprobar si el RTC perdió energía y ajustar la hora
    if (rtc.lostPower())
    {
        DEBUG_PRINTLN("RTC perdió energía, ¡ajustando la hora al momento de la compilación!");
        // Ajusta el RTC a la fecha y hora de compilación.
        // ¡Recuerda comentar esta línea después de la primera subida para que el reloj mantenga su hora!
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // Puedes usar la fecha y hora actual del sistema si tienes conectividad a internet:
        // rtc.adjust(DateTime(2025, 6, 19, 16, 45, 0)); // Ejemplo: Año, Mes, Día, Hora, Minuto, Segundo
    }
    DEBUG_PRINTLN("RTC inicializado correctamente.");
    return true; // Indica éxito en la inicialización
}

// --- Funciones para obtener la hora y minutos en uint8_t ---

// Devuelve la hora actual (0-23) como uint8_t
uint8_t RtcManager::getHora()
{
    DateTime now = rtc.now();
    return now.hour();
}

// Devuelve los minutos actuales (0-59) como uint8_t
uint8_t RtcManager::getMinutos()
{
    DateTime now = rtc.now();
    return now.minute();
}

// Devuelve los segundos actuales (0-59) como uint8_t
uint8_t RtcManager::getSegundos()
{
    DateTime now = rtc.now();
    return now.second();
}

// --- Función para devolver la fecha en un formato considerado apropiado ---

// Devuelve la fecha en formato "DD/MM/YYYY" como String
String RtcManager::getFecha_DDMMYYYY()
{
    DateTime now = rtc.now();
    String fecha = "";
    if (now.day() < 10)
        fecha += "0";
    fecha += now.day();
    fecha += "/";
    if (now.month() < 10)
        fecha += "0";
    fecha += now.month();
    fecha += "/";
    fecha += now.year();
    return fecha;
}

// Devuelve la fecha en formato "YYYY-MM-DD" como String
String RtcManager::getFecha_YYYYMMDD()
{
    DateTime now = rtc.now();
    String fecha = "";
    fecha += now.year();
    fecha += "-";
    if (now.month() < 10)
        fecha += "0";
    fecha += now.month();
    fecha += "-";
    if (now.day() < 10)
        fecha += "0";
    fecha += now.day();
    return fecha;
}

// --- Función para devolver fecha y tiempo (horas, minutos y segundos) en string ---

// Devuelve la fecha y hora completas en formato "DD/MM/YYYY HH:MM:SS" como String
String RtcManager::getFechaHoraCompleta()
{
    DateTime now = rtc.now();
    String fechaHora = getFecha_DDMMYYYY(); // Reutiliza la función de fecha
    fechaHora += " ";
    if (now.hour() < 10)
        fechaHora += "0";
    fechaHora += now.hour();
    fechaHora += ":";
    if (now.minute() < 10)
        fechaHora += "0";
    fechaHora += now.minute();
    fechaHora += ":";
    if (now.second() < 10)
        fechaHora += "0";
    fechaHora += now.second();
    return fechaHora;
}