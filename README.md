# 🔐 Fechadura Eletrônica com ESP32 e RFID


Este projeto implementa uma fechadura eletrônica controlada por ESP32, utilizando leitor RFID (RC522) para autenticação. Ao aproximar um cartão ou chaveiro RFID autorizado, a fechadura é liberada. O sistema também pode registrar os acessos em uma planilha do Google Sheets via conexão Wi-Fi.


![image](https://github.com/user-attachments/assets/5aa61893-2474-45ca-befd-5b6df37315b2)


# 🛠️ Tecnologias Utilizadas

- ESP32

- RFID RC522

- Servo motor

- Google Sheets + Google Apps Script (para registro de acessos)

- Arduino IDE

# 🔧 Materiais Necessários

| Item                | Quantidade |
| ------------------- | ---------- |
| ESP32               | 1          |
| Módulo RFID RC522   | 1          |
| Cartões/Tags RFID   | 1 ou mais  |
| Servo motor ou relé | 1          |
| Jumpers             | Vários     |
| Fonte 5V            | 1          |


# ⚙️ Como Funciona
O ESP32 se conecta ao Wi-Fi.

Ao aproximar um cartão RFID:

- O código UID é lido.

- Se o UID for autorizado, o servo motor é ativado.

- O nome, data e hora são enviados a uma planilha do Google Sheets via requisição HTTP (opcional).

- Se o cartão não for autorizado, o acesso é negado.

# 🚀 Instalação
Instale a biblioteca MFRC522 e WiFi no Arduino IDE.

Clone este repositório: https://github.com/GustavoLealDev/Projeto-IOT.git


# 📒 Registro na Planilha

Os dados registrados devem ser:

![image](https://github.com/user-attachments/assets/5149ac22-276f-4b14-b075-57dc42eb0992)


