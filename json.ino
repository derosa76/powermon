

String file_mirror="";

unsigned int json_doc_check_time_interval=10000;// ms
unsigned long json_doc_check_time_last=0;


String descr[]={"consumo totale","-","-","-"};

void json_file_setup(boolean erase_config){
  String file_content=getFileVarString(jsonFileVarName);
  Serial.println("json_file_setup(): file_content="+file_content);
  if ((!erase_config) && (file_content.length()>0)) deserialize_from_file();
  else{
    JsonArray data;
    doc["dns_name"] = "";
    data = doc["description"].to<JsonArray>(); for (int i=0;i<4;i++) data.add(descr[i]);
    //data = doc["vrms0"].to<JsonArray>(); for (int i=0;i<=400;i++) data.add(i*0.001); for (int i=401;i<=600;i++) data.add(0.4+i*0.003);
    data = doc["vrms0"].to<JsonArray>(); data.add(0);data.add(0.01);data.add(1); 
    data = doc["vrms1"].to<JsonArray>(); data.add(0);data.add(0.01);data.add(1); 
    data = doc["vrms2"].to<JsonArray>(); data.add(0);data.add(0.01);data.add(1); 
    data = doc["vrms3"].to<JsonArray>(); data.add(0);data.add(0.01);data.add(1); 
    //data = doc["k0"].to<JsonArray>(); for (int i=0;i<=600;i++) data.add(-1);//valore medio (che va nel calcolo)
    data = doc["k0"].to<JsonArray>(); data.add(9874.5);data.add(9143.7);data.add(9143.7);
    data = doc["k1"].to<JsonArray>(); data.add(9874.5);data.add(9143.7);data.add(9143.7);
    data = doc["k2"].to<JsonArray>(); data.add(9874.5);data.add(9143.7);data.add(9143.7);
    data = doc["k3"].to<JsonArray>(); data.add(9874.5);data.add(9143.7);data.add(9143.7);
    serialize_to_file();
  }
}

//travasa il json nel file se l'oggetto è differente dal file (quindi se è stato modificato)
void json_file_update(bool force_now){
  if (force_now || (millis()>=json_doc_check_time_last+json_doc_check_time_interval)) {
    Serial.println("json_file_update(): verifica file in esecuzione");
    json_doc_check_time_last=millis();
    String s;
    serializeJson(doc,s);
    if(!file_mirror.equals(s)) serialize_to_file();
    else Serial.println("json_file_update(): file già aggiornato.");
  }
}

//travasa l'oggetto json nel file
void serialize_to_file(){
  String s;
  serializeJson(doc,s);
  Serial.println("serialize_to_file(): file <-- "+s);
  setFileVarString(jsonFileVarName, s);
  file_mirror=s;
}

void deserialize_from_file(){
  String s=getFileVarString(jsonFileVarName);
  DeserializationError error = deserializeJson(doc, s);
  if (error) {Serial.print(F("deserialize_from_file() - deserializeJson() failed: ")); Serial.println(error.f_str());}
  else {
    file_mirror=s;
    Serial.println("deserialize_from_file():file --> "+s);
  }
}