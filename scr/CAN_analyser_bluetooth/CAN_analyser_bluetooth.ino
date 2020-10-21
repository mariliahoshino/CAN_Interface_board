//This project is a TransReceiver Bluetooth with CAN Analyser
//Can send and receive data in a CAN bus network
//Licensed under MIT License
//
// Marília Zeniti Hoshino
//mailto:mariliahoshino@hotmail.com
//https://www.linkedin.com/in/mariliahoshino/
//https://github.com/mariliahoshino
//https://mariliahoshino.wixsite.com/cvitae
//https://www.instagram.com/mari.zeniti/


//library CAN.h  https://github.com/sandeepmistry/arduino-CAN
#include <CAN.h>
#include "BluetoothSerial.h"
BluetoothSerial ESP_BT; //Object for Bluetooth

int dadosbl[13], id_packet, x, dados;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  ESP_BT.begin("R_ESP32CAN_TR"); //Name of your Bluetooth Signal
  Serial.println("CAN Transmitter and Receiver");    
  if (!CAN.begin(250E3)) {  // start the CAN bus at 250 kbps   // can change baudrate
    Serial.println("Starting CAN failed!");
    while (1);
  }
}
void loop() {


//////////////////////////////////////////////////////////////
///////////////////RECEIVER///////////////////////////////////
//////////////////////////////////////////////////////////////

  int packetSize = CAN.parsePacket();
  id_packet=(CAN.packetId());
  if (packetSize) {
    // received a packet
    Serial.print("Received ");
    ESP_BT.print("Received ");

    if (CAN.packetExtended()) {  //verifica se o pacote é estendido
      Serial.print("extended ");
      ESP_BT.print("Extended ");
    }

    if (CAN.packetRtr()) {
      //Verifica se o pacote é um pacote remoto (Requisição de dados), neste caso não há dados
      // Remote transmission request, packet contains no data
      Serial.print("RTR ");
      ESP_BT.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);
    ESP_BT.print("packet with id 0x");
    ESP_BT.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {                 //se o pacote recebido é de requisição, indicamos o comprimento solicitado
      Serial.print(" and requested length ");
      Serial.println(CAN.packetDlc());     //obtem o DLC (Data Length Code, que indica o comprimento dos dados)
    } else {
      Serial.print(" and length ");   // aqui somente indica o comprimento recebido
      Serial.println(packetSize);
      ESP_BT.print(" and length ");
      ESP_BT.println(packetSize);
      ESP_BT.print("  ");

      while (CAN.available()) {
        dados = CAN.read();
        Serial.print(dados, HEX);
        Serial.print("\t");
        ESP_BT.print(dados, HEX);
        ESP_BT.print("  ");
              }
      Serial.println();
      ESP_BT.println();
    }

    Serial.println();
    ESP_BT.println();
    
  }

//////////////////////////////////////////////////////////////
/////////////////////SENDER///////////////////////////////////
//////////////////////////////////////////////////////////////
  
  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
      {
            Serial.print("Sended:");
            for (x=0; x<=12; x++){
              dadosbl[x] = ESP_BT.read(); //Read what we recevive
               Serial.print(dadosbl[x], HEX); Serial.print("  "); 
              ESP_BT.print(dadosbl[x], HEX);
            }
  
  ///////////////////////////////////////////////////////////////
  // Usando o CAN 2.0 padrão
  //Envia um pacote: o id tem 11 bits e identifica a mensagem (prioridade, evento)
  //o bloco de dados deve possuir até 8 bytes
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
            
            id_packet= (dadosbl[2] + (dadosbl[1]*256));
            Serial.println("");
            ESP_BT.println("");
            Serial.println("id packet////////////////////////////");
            Serial.print(dadosbl[1]);Serial.print("*256 + "); Serial.print(dadosbl[2]);Serial.print(" = "); Serial.print(id_packet);Serial.println(" base10");
            CAN.beginPacket(id_packet);  //0x185 = 389(10)
            if(1<=dadosbl[0]){
            CAN.write(dadosbl[3]);
            } if(2<=dadosbl[0]){
            CAN.write(dadosbl[4]);
            } if(3<=dadosbl[0]){
            CAN.write(dadosbl[5]);
            } if(4<=dadosbl[0]){
            CAN.write(dadosbl[6]);
            } if(5<=dadosbl[0]){
            CAN.write(dadosbl[7]);
            } if(6<=dadosbl[0]){
            CAN.write(dadosbl[8]);
            } if(7<=dadosbl[0]){
            CAN.write(dadosbl[9]);
            } if(8<=dadosbl[0]){
            CAN.write(dadosbl[10]);
            }
            CAN.endPacket();
      }


//This project is Standar CAN, but is possible use extended CAN
  //Usando CAN 2.0 Estendido
  //Envia um pacote: o id tem 29 bits e identifica a mensagem (prioridade, evento)
  //o bloco de dados deve possuir até 8 bytes

  // send extended packet: id is 29 bits, packet can contain up to 8 bytes of data

  /*
  Serial.print("Sending extended packet ... ");

  CAN.beginExtendedPacket(0xabcdef);  //id 11259375 decimal ( abcdef em hexa) = 24 bits preenchidos até aqui
  CAN.write('w');
  CAN.write('o');
  CAN.write('r');
  CAN.write('l');
  CAN.write('d');
  CAN.endPacket();  //encerra o pacote para envio

  Serial.println("done");

  delay(1000);
  */
}
