
//************************************
// EEPROM functions
//************************************

void setNetworkId(String nId){
  setEeprom( 0, nId );
}

String getNetworkId(){
  return getEeprom(0, 32);
}

void setNetworkPassword(String nPassword){
  setEeprom( 32, nPassword );
}

String getNetworkPassword(){
  return getEeprom(32, 64);
}

void setLastAngle(int angle){
  setEeprom( 96, String(angle) );
}

int getLastAngle(){
  return getEeprom(96, 3).toInt();
}

void setEeprom(int address, String data){
  Serial.print("Saving to EEPROM: ");
  Serial.println(data);

  for (int i = 0; i < data.length(); ++i){
    EEPROM.write(address+i, data[i]);
  }
  EEPROM.commit();
}

String getEeprom(int address, int dataLength){
  Serial.print("Loading from EEPROM: ");
  String value = "";

  for (int i = address; i < address + dataLength; ++i){
    if(EEPROM.read(i) != 0xFF){
      value += char(EEPROM.read(i)); 
    }
  }
  Serial.println(value);

  return value;
}

