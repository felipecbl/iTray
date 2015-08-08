//************************************
// Help (Auxiliar) functions
//************************************

// Return a substring between 2 substrings
String getValue(String webString, String startString, String endString) {
  if (webString.indexOf(startString) != -1) {
    int sStart = webString.indexOf(startString) + startString.length();
    int sEnd = webString.indexOf(endString, sStart);

    return  webString.substring(sStart, sEnd);
  }
  return "";
}

// Check if first value is bigger than the second
boolean isBigger(int firstNumber, int secondNumber) {
  if (firstNumber > secondNumber) {
    return true;
  }
  return false;
}

// Print Serial if debug mode is defined
void logln(String toPrint){
  #ifdef DEBUG
    Serial.println(String(toPrint));
  #endif
}

void log(String toPrint){
  #ifdef DEBUG
    Serial.print(String(toPrint));
  #endif
}

