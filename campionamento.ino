
float vrms[4]={0,0,0,0};//variabile globale misura attuale - semplificando a 1 solo input
float f_signal[4]={50,50,50,50}; //Hz frequenza segnale stimata, non la ricalcolo perchè è complesso trovare il modo da farlo bene
float f_sampling[4]={400,400,400,400}; // frequenza campionamento stimata (non intesa al secondo, ma ogni 1.2 secondi (60 periodi))
float avg_long[4];
unsigned long last_1200_=0;

unsigned long sample_counter[4]={0,0,0,0};

void campiona_setup(){
  adc.setCompareChannels(ch[0]);
  avg_long[0]=3.3/2; //valore iniziale media mobile lunga
  avg_long[1]=3.3/2;
  avg_long[2]=3.3/2;
  avg_long[3]=3.3/2;
}


void campiona_all(){
  //se passo il secondo, aggiorno sampling counter facendo la media su 5 secondi (sono sample/secondo, per quello specifico ingresso)
  if ((millis()/1200) > last_1200_){
    last_1200_=millis()/1200;
    for (int i=0;i<4;i++){
      f_sampling[i]=(f_sampling[i]*4+sample_counter[i])/5.0; //in realtà equivale a quanti samples ogni 300 ms
      sample_counter[i]=0;
    }
  }

  int n=(millis()%1200)/300; //0,1,2,3 switch ogni 300 ms (15 periodi a 50 Hz)
  if (ch_actual!=ch[n]){
    adc.setCompareChannels(ch[n]);
    ch_actual=ch[n];
  }
  campiona(n);
}


void campiona(int adc_channel) {

  sample_counter[adc_channel]++;

  //const int N_avg_periods=50; //numero di periodi che deve coprire il calcolo del valore medio



  //misura
  float v = adc.getResult_V();
  

  //calcolo media (mobile) degli ultimi N_avg_periods periodi
  //se centro la media su un numero di periodi sarà molto più stabile che scegliere una lunghezza slegata dalla durata periodi
  //int N_long=round(N_avg_periods*(f_sampling[adc_channel]*4/f_signal[adc_channel]));
  int N_long=2*f_sampling[adc_channel];//2.4 secondi
  avg_long[adc_channel]=(avg_long[adc_channel]*(N_long-1)+v)/N_long; //media mobile di lungo periodo (su N_avg_periods periodi) della tensione misurata

  //calcolo vrms
  float v_=v-avg_long[adc_channel];
  float v_2=v_*v_;
  float vrms2=vrms[adc_channel]*vrms[adc_channel];
  vrms[adc_channel]=sqrt((vrms2*(N_long-1)+v_2)/N_long); //media mobile di lungo periodo  (su N_avg_periods periodi) di vrms

}