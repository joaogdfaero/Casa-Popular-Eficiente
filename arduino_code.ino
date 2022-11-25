float sensor = 0.0;
float raw    = 0.0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
    for (int i=0;i <5;i++){
        raw += analogRead(A7);  
    }
    Serial.print("Valor medio da leitura: ");
    Serial.println(raw/5);
    sensor = raw/1024.0*5.0;
    sensor = sensor/5;
    Serial.print("Nivel UV: ");
    Serial.println(sensor);
    raw = 0.0;
    delay(1500);
}
