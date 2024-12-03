int SIR;
int dist;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  SIR = analogRead(A0);
  delay(200);
  dist = pow(10, log10(SIR / 1821.2) / -0.65);
  Serial.print("Medicion= ");
  Serial.print(SIR);
  Serial.print("Distancia= ");
  Serial.println(dist);
}
