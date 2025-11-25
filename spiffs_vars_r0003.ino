

void  spiffs_vars_begin() {
  //if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
  if (!SPIFFS.begin(true)) { // questa modifica ha risolto l'errore di mount della SPI https://github.com/espressif/arduino-esp32/issues/638
    //otalog("SPIFFS Mount Failed");
    return;
  }
}

bool writeFile_(fs::FS &fs, const char *path, const char *message) {
  bool result=false;
  File file = fs.open(path, FILE_WRITE);

  if (!file) return result;
  if (file.print(message)) result=true; 
  else result=false;
  file.close();

  return result;
}

String readFile_(fs::FS &fs, const char *path) {
  String result="";
  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    return result;
  }

  while (file.available()) {
    char c=file.read();
    result=result+String(c);
  }
  file.close();

  return result;
}


bool setFileVarString(String varname, String value){
  String path = "/"+varname;
  writeFile_(SPIFFS, path.c_str(), value.c_str());
  String b = getFileVarString(varname);
  return (getFileVarString(varname).equals(value));
}

String getFileVarString(String varname){
  String path = "/"+varname;
  return readFile_(SPIFFS, path.c_str());
}
