#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <TimeLib.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>
#include <ESP32Servo.h> 

// ======= Configurações da rede e pinos =======
const char* ssid = "CRF";                         // Nome da rede Wi-Fi
const char* password = "leallyra";                // Senha da rede Wi-Fi
const char* host = "script.google.com";           //Google Apps Script
const char* googleScriptId = "AKfycbzUx7yOfb8NndcbUj1bht7KR22vhXRHBdOzvCINxLI3QLP1dfZyWeWEDuiNthBDnaiT";
const int httpsPort = 443; 

#define SS_PIN 5
#define RST_PIN 27
#define SERVO_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600);
Servo servo;
String ultimoEstado = ""; // Guarda último estado (ENTRADA ou SAÍDA)

// Certificado digital da Google (HTTPS) 
const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
"27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
"Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
"TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
"qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
"szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
"Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
"MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
"wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
"aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
"VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
"C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
"QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
"h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
"7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
"ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
"MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
"Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
"6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
"0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
"2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
"bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
"-----END CERTIFICATE-----\n";

// ID para nome das pessoas
String getNomeByUID(String uid) {
  if (uid == "8F E9 15 1F") return "Leonardo Silva";
  if (uid == "14 09 B1 A3") return "Gustavo Leal";
  return "Visitante Desconhecido";
}

// Codifica texto para URL (GET)
String urlEncode(const String& str) {
  String encoded = "";
  for (char c : str) {
    if (isalnum(c)) encoded += c;
    else if (c == ' ') encoded += '+';
    else encoded += "%" + String("0123456789ABCDEF"[(c >> 4) & 0xF]) + String("0123456789ABCDEF"[c & 0xF]);
  }
  return encoded;
}

// Envia dados para a planilha
void enviarParaGoogleSheets(String nome, String data, String hora, String tipo) {
  WiFiClientSecure client;
  client.setCACert(root_ca);

  if (!client.connect(host, httpsPort)) return;   // Conecta ao Google Script

  // Monta a URL com os parâmetros
  String url = "/macros/s/" + String(googleScriptId) + "/exec?nome=" + urlEncode(nome) +
               "&data=" + urlEncode(data) + "&hora=" + urlEncode(hora) + "&tipo=" + urlEncode(tipo);

  client.print("GET " + url + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n");

  while (client.connected() && !client.available()) delay(10);
  while (client.available()) client.read();
  client.stop();
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  servo.attach(SERVO_PIN);
  servo.write(0);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  timeClient.begin();
}

void loop() {
  // Verifica se há novo cartão RFID
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  // Lê e UID do cartão
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += (rfid.uid.uidByte[i] < 0x10 ? " 0" : " ") + String(rfid.uid.uidByte[i], HEX);
  }
  uid.trim(); uid.toUpperCase();

  // Identifica a pessoa pelo UID
  String nome = getNomeByUID(uid);

  // Atualiza a hora atual pela internet
  timeClient.update();
  unsigned long epoch = timeClient.getEpochTime(); // Tempo em segundos
  String data = String(day(epoch)) + "/" + String(month(epoch)) + "/" + String(year(epoch));
  String hora = timeClient.getFormattedTime();

  // Alterna tipo entre ENTRADA e SAÍDA
  String tipo = (ultimoEstado == "SAIDA") ? "ENTRADA" : "SAIDA";
  ultimoEstado = tipo;

  // Se o nome for conhecido, libera o acesso com o servo
  if (nome != "Visitante Desconhecido") {
    servo.write(tipo == "ENTRADA" ? 90 : 0);  // Abre ou fecha a "porta"
    Serial.println("Acesso autorizado - " + tipo);
  } else {
    Serial.println("Acesso negado!");
  }

  // Mostra os dados no monitor serial
  Serial.printf("UID: %s\nNome: %s\nData: %s\nHora: %s\nTipo: %s\n",
    uid.c_str(), nome.c_str(), data.c_str(), hora.c_str(), tipo.c_str());

  // Envia os dados para o Google Sheets
  enviarParaGoogleSheets(nome, data, hora, tipo);

  delay(2000);
  rfid.PICC_HaltA();
}
