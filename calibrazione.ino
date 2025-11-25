

void calibration_run(float vrms_, float p, int i) {
  


  if (isnan(vrms_) || isnan(p) || p<=0 || vrms_<=0) {
    //Serial.println("calibration_run(): vrms="+String(vrms,4)+ "p="+String(p,1)+" --> return per condizione verificata -->'(isnan(vrms) || isnan(p) || p<= || vrms<=0)'");
    return;
  }

  float k=p/vrms_;

  String vrms__="vrms"+String(i);
  String k_ ="k"+String(i);

  for(int i = 1; i < doc[vrms__].size(); i++) {
    float vrms1 = doc[vrms__][i-1].as<float>();
    float vrms2 = doc[vrms__][i].as<float>();
    float k1 = doc[k_][i-1].as<float>();
    float k2 = doc[k_][i].as<float>();
    
    if (isnan(vrms1) || isnan(vrms2)) {
      continue;
    }
    
    if (vrms1 == vrms2) {
      continue;
    }
    
    if ((vrms_ >= vrms1) && (vrms_ < vrms2)) {
      float vrms_avg=(vrms1+vrms2)/2;
      if (i==1){
        if (vrms_>=vrms_avg){ //il k trovato per il punto 1 lo applico anche al punto 0. Riempio l'array 1 sola volta. Poi vedrò l'equazione esternamente se quadra, o con lo stesso feedback del programma.
          if(k1<0) doc[k_][0]=k;
          if(k2<0) doc[k_][1]=k;
        }
      }
      if (i>1){
        if (vrms_<vrms_avg) if(k1<0) doc[k_][i-1]=k;
        if (vrms_>=vrms_avg) if(k2<0) doc[k_][i]=k;
      }
    }
  }

  json_file_update(false);


}



String getCalibrationData(int i) {
  String result = "";

  // Intestazione
  result += "i,vrms,power,stddev,samples\n";

  String vrms_="vrms"+String(i);
  String k_ ="k"+String(i);

  // Iterate through all data points
  for(int i = 0; i < doc[vrms_].size(); i++) {
      result += String(i) + ",";
      result += String(doc[vrms_][i].as<float>(), 3) + ",";  // 3 decimali per vrms
      result += String(doc[k_][i].as<float>(), 1) + ",";     // 1 decimale per power
      result += "\n";
  }

  return result;
}


float getPowerFromCalibration(float vrms,int i) {

  String vrms_="vrms"+String(i);
  String k_ ="k"+String(i);

    // Controllo validità input
    if (isnan(vrms)) {
        return NAN;
    }

    // Cerca l'intervallo corretto nella tabella
    for(int i = 1; i < doc[vrms_].size(); i++) {
        float vrms1 = doc[vrms_][i-1].as<float>();
        float vrms2 = doc[vrms_][i].as<float>();
        
        // Verifica validità dei valori
        if (isnan(vrms1) || isnan(vrms2)) {
            continue;
        }
        
        // Se trovo l'intervallo giusto
        if ((vrms >= vrms1) && (vrms < vrms2)) {
            float k1 = doc[k_][i-1].as<float>();
            float k2 = doc[k_][i].as<float>();
            
            // Verifica validità dei valori di potenza
            if (isnan(k1) || isnan(k2)) {
                return NAN;
            }
            
            // Evita divisione per zero
            if (vrms1 == vrms2) {
                return vrms*k1;
            }
            
            // Interpolazione lineare
            return vrms*(k1 + (k2 - k1) * (vrms - vrms1) / (vrms2 - vrms1));
        }
    }
    
    // Se la tensione è fuori range, restituisci NAN
    return NAN;
}