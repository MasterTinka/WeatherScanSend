//                  БИБЛИОТЕКИ
#include <Arduino.h>
#include <iarduino_AM2320.h>  // Датчик
#include <Ethernet.h>           // Интернет
#include <SPI.h>

//                    ИНИЦИАЛИЗАЦИЯ ИНТЕРНЕРТА
byte mac[] = { 0xDE, 0xAD, 0x0L, 0xF0, 0x14, 0x48 };

EthernetClient client;

//                    ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ
iarduino_AM2320 sensor;       // Объект датчика

void setup() {
  Ethernet.begin(mac);        // Начать работу с интернетом
  Serial.begin(9600);         // Начать UART
  sensor.begin();             // Начать работу с датчиком
  delay(1000);
}

String send_data()            // Отслыка данных
{
  String data = "#DE:AD:0L:F0:14:48\n#T1#"; // MAC-Адрес 
  data += sensor.tem;                       // Температура
  data += "\n#T2#";                         // Второй датчик
  data += sensor.hum;                       // Влажность
  data += "\n##";                           // Конец передачи
  return data;                              // Вернуть строку
}

void loop() 
{
  if(client.connect("narodmon.ru", 8283));              // Если соединение успешно
  {
    sensor.read();                                      // Прочитать данные с дачтика температуры
    String StringData = send_data();                    // Строка заполняется данными
    char data[StringData.length()];                     // Создание C-строки
    StringData.toCharArray(data, StringData.length());  // Перевод из STD-строки в C-строку
    client.write(data);                                 // Отправка на сервер данных
    client.disconnect();                                // Отключится от сервера
  }
  delay(300000);                                        // Подождать 5 минут
}