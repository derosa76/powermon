String WS_helpMessage(){
  String msg="";//online da: "+elapsed_time_online()+"\r\n";
  msg=msg+"IP:"+WiFi.localIP().toString()+"\r\n";
  msg=msg+"DNS name = Hostname ='"+String(doc["dns_name"])+"'\r\n";
  msg=msg+"available commands:\r\n";
  msg=msg+String(doc["dns_name"])+".local/?cmd=setdns;nuovo_nome_dns\r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=setdescription;x;description \r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=setrnota;x;r \r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=setgpio;do_n;gpio    // set del GPIO da associare al DO\r\n"; ////////<<<<<<<<<--------------
  // msg=msg+String(doc["dns_name"])+".local/?cmd=setnegation;do_n;negation  //negation può essere 0 (non negato) o 1 (output negato)\r\n"; ////////<<<<<<<<<--------------
  // msg=msg+"\r\n";
  // msg=msg+String(doc["dns_name"])+".local/?cmd=set_do_description;x;description \r\n";
  // //msg=msg+String(doc["dns_name"])+".local/?cmd=set_timer_seconds;x;start_time_seconds_from_midnight;duration_seconds \r\n";
  // msg=msg+"\r\n";
  // msg=msg+String(doc["dns_name"])+".local/?cmd=set_timer;x;start_time(hh:mm));duration_seconds;period_hours\r\n";
  // msg=msg+String(doc["dns_name"])+".local/?cmd=open;x;duration_seconds;period_hours (test che fa partire adesso e rimane memorizzato)"+"\r\n";
  // msg=msg+"\r\n";
  // msg=msg+String(doc["dns_name"])+".local/?cmd=reset_program --> rst programmazione. NON descr dei DO"+"\r\n";
  // msg=msg+"\r\n";
  // msg=msg+String(doc["dns_name"])+".local/log"+"\r\n";
  // msg=msg+"\r\n";
  //msg=msg+"disponibili x=0..11\r\n";
  //msg=msg+do_get_status_all();
  //msg=msg+String(doc["dns_name"])+".local/?cmd=set_kp;n;val\r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=set_ki;n;val\r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=set_kd;n;val\r\n";
  //msg=msg+String(doc["dns_name"])+".local/?cmd=get_pv;0 --> restituisce un json es. {\"pv\": 25.3}\r\n";
  //msg=msg+get_all_measurements_details();

  String s;
  serializeJsonPretty(doc,s);
  msg=msg+s+"\r\n";
  msg=msg+"\r\nfreemem: " + String(esp_get_free_heap_size()) + " bytes";
  return msg;
}


String WS_cmd_interpreter(String cmd){
  //otalog("interpretazione comando:"+cmd);

  String s0=getValue(cmd,';',0);

  if (s0.equals("setdns")){
  //  otalog("settaggio DNS name..."); 
    String DNS_name=getValue(cmd,';',1);
    doc["dns_name"]=DNS_name;// {if(setFileVarString("dns_name", DNS_name)) {
  //  otalog("new DNS:"); 
  //  otalog(String(doc["dns_name"]));
    
    reboot=true;
    return ("cmd: "+cmd+" executed \r\n ESP reboot...");
  }


  //  if (s0.equals("set_timer")){
  //   String do_n_string=getValue(cmd,';',1);
  //   int do_n_int=do_n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
    
  //   String do_start_string=getValue(cmd,';',2);
  //   String ore=getValue(do_start_string,':',0);
  //   String minuti=getValue(do_start_string,':',1);
  //   int ore_int=ore.toInt(); // If no valid conversion could be performed because the String doesn’t start with a digit, a zero is returned. 
  //   int minuti_int = minuti.toInt();

  //   String do_duration_sec_string=getValue(cmd,';',3);
  //   int do_duration_sec_int=do_duration_sec_string.toInt(); // If no valid conversion could be performed because the String doesn’t start with a digit, a zero is returned.

  //   String do_period_hours_string=getValue(cmd,';',4);
  //   double do_period_hours=do_period_hours_string.toDouble();

  //   //long last_midnight_epoch = time_object.getEpoch()-getSecondsFromMidnight();
  //   int last_midnight_epoch = (time_object.getEpoch()/(24*60*60))*(24*60*60);
  //   doc["start_epoch"][do_n_int]=last_midnight_epoch+ore_int*3600+minuti_int*60;

  //   doc["duration"][do_n_int]=do_duration_sec_int;

  //   doc["period"][do_n_int]=do_period_hours;
  //   double do_period_hours_stored=doc["period"][do_n_int];//get_DO_period_hours(do_n_int);

  //   String msg="set start time="+String(ore_int)+":"+String(minuti_int)+" - durata="+String(do_duration_sec_int)+" sec\r\n";
  //   msg=msg+"Start seconds from midnight="+String(ore_int*3600+minuti_int*60) + " - Duration(s)="+String(do_duration_sec_int)+", period="+String(do_period_hours_stored)+"\r\n";
  //   return (msg+"cmd: "+cmd+" executed");
  // }

  //  if (s0.equals("open")){ ///?cmd=open;x;duration_seconds;period_hours (test che fa partire adesso e rimane memorizzato)
  //   String do_n_string=getValue(cmd,';',1);
  //   int do_n_int=do_n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
    
  //   String do_duration_sec_string=getValue(cmd,';',2);
  //   int do_duration_sec=do_duration_sec_string.toInt(); // If no valid conversion could be performed because the String doesn’t start with a digit, a zero is returned. 

  //   String do_period_hours_string=getValue(cmd,';',3);
  //   double do_period_hours=do_period_hours_string.toDouble();

  //   doc["start_epoch"][do_n_int]=time_object.getEpoch();
  //   int start_epoch_stored=doc["start_epoch"][do_n_int];

  //   doc["duration"][do_n_int]=do_duration_sec;
  //   int do_duration_sec_stored=doc["duration"][do_n_int];//get_DO_duration_seconds(do_n_int);

  //   doc["period"][do_n_int]=do_period_hours;
  //   double do_period_hours_stored=doc["period"][do_n_int];//get_DO_period_hours(do_n_int);

  //   String msg="DO N. "+String(do_n_int)+" - set start epoch="+String(start_epoch_stored)+", duration="+String(do_duration_sec_stored)+", period="+String(do_period_hours_stored)+"\r\n";
  //   msg="cmd: "+cmd+" executed! "+msg;
  //   otalog(msg);
  //   return (msg);
  // }

//msg=msg+String(doc["dns_name"])+".local/?cmd=setdescription;x;description \r\n";
 if (s0.equals("setdescription")){
    String n_string=getValue(cmd,';',1);
    int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
    String description=getValue(cmd,';',2);
    doc["description"][n_int]=description;
    return ("cmd: "+cmd+" executed");
  }

  //   if (s0.equals("setrnota")){
  //   String n_string=getValue(cmd,';',1);
  //   int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
  //   String r_string=getValue(cmd,';',2);
  //   double r_double=r_string.toDouble();
  //   doc["rnota"][n_int]=(float)r_double;
  //   otalog("doc[\"rnota\"]["+n_string+"]="+r_string);
  //   return ("cmd: "+cmd+" executed");
  // }

//    if (s0.equals("setgpio")){
//     String gpio_n_string=getValue(cmd,';',1);
//     int gpio_n_int=gpio_n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String gpio=getValue(cmd,';',2);
//     int gpio_int=gpio.toInt();
//     doc["gpio"][gpio_n_int]=gpio_int;
//     otalog("doc[\"dgpio\"]["+String(gpio_n_int)+"]="+String(gpio_int)+" -"+String(doc["description"][gpio_int])+" -");
//     return ("cmd: "+cmd+" executed");
//   }



//    if (s0.equals("set_kp")){
//     String n_string=getValue(cmd,';',1);
//     int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String val=getValue(cmd,';',2);
//     double val_double=val.toDouble();
//     doc["kp"][n_int]=val_double;
//     otalog("doc[\"kp\"]["+String(n_int)+"]="+String(val_double)+" -"+String(doc["description"][n_int])+" -");
//     return ("cmd: "+cmd+" executed");
//   }

//   if (s0.equals("set_ki")){
//     String n_string=getValue(cmd,';',1);
//     int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String val=getValue(cmd,';',2);
//     double val_double=val.toDouble();
//     doc["ki"][n_int]=val_double;
//     otalog("doc[\"ki\"]["+String(n_int)+"]="+String(val_double)+" -"+String(doc["description"][n_int])+" -");
//     return ("cmd: "+cmd+" executed");
//   }

//   if (s0.equals("set_kd")){
//     String n_string=getValue(cmd,';',1);
//     int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String val=getValue(cmd,';',2);
//     double val_double=val.toDouble();
//     doc["kd"][n_int]=val_double;
//     otalog("doc[\"kd\"]["+String(n_int)+"]="+String(val_double)+" -"+String(doc["description"][n_int])+" -");
//     return ("cmd: "+cmd+" executed");
//   }

//   if (s0.equals("set_sp")){
//     String n_string=getValue(cmd,';',1);
//     int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String val=getValue(cmd,';',2);
//     double val_double=val.toDouble();
//     doc["sp"][n_int]=val_double;
//     otalog("doc[\"sp\"]["+String(n_int)+"]="+String(val_double)+" -"+String(doc["description"][n_int])+" -");
//     return ("cmd: "+cmd+" executed");
//   }

// //"http://192.168.1.37/?cmd=get_pv;0 --> restituisce un json es. {"pv": 25.3}"
//   if (s0.equals("get_pv")){
//     String n_string=getValue(cmd,';',1);
//     int n_int=n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String res="{\"pv\": "+String(pv[n_int],3)+"}";
//     otalog("richiesta temperatura N."+n_string+" evasa: pv["+n_string+"] --> "+res);
//     return (res);
//   }

  //"http://192.168.1.37/?cmd=get_pv;0 --> restituisce un json es. {"pv": 25.3}"
  if (s0.equals("get_all")){
    JsonDocument doc_;
    JsonArray data_;
    data_ = doc_["description"].to<JsonArray>(); data_.add(doc["description"][0]);
    data_ = doc_["vrms"].to<JsonArray>(); data_.add(vrms[0]);data_.add(vrms[1]);data_.add(vrms[2]);data_.add(vrms[3]);
    data_ = doc_["p"].to<JsonArray>(); data_.add(getPowerFromCalibration(vrms[0],0));data_.add(getPowerFromCalibration(vrms[1],1));data_.add(getPowerFromCalibration(vrms[2],2));data_.add(getPowerFromCalibration(vrms[3],3));
    String s;
    serializeJson(doc_,s);
    return (s);
  }






//   //local/?cmd=setnegation;do_n;negation
//     if (s0.equals("setnegation")){
//     String do_n_string=getValue(cmd,';',1);
//     int do_n_int=do_n_string.toInt(); //If no valid conversion could be performed because the String doesn’t start with a integer number, a zero is returned.
//     String negation=getValue(cmd,';',2);
//     int negation_int=negation.toInt();
//     if (negation_int==0 || negation_int==1){
//       doc["do_negation"][do_n_int]=negation_int;
//       return ("cmd: "+cmd+" executed");
//     }
//     return ("cmd: "+cmd+" failed because negation value is not valid (shall be 0 or 1)");
//   }

//    if (s0.equals("reset_program")){
//     for (int i=0;i<12;i++){
//       doc["start_epoch"][i]=0;
//       doc["duration"][i]=0;
//       doc["period"][i]=0;
//     }
//     return ("cmd: "+cmd+" executed");
//   }

  return "cmd not recognized";
}
