// Send data to laptop using USB via Serial
// Accelerometer reading is in (m/s^2)
// Gyroscope reading is in (degree/s) but we integrated it about time and get the result in ° (degree) from initial state.
#include <Wire.h>
#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C
#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18
#define LED 2

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
  I2CwriteByte(MAG_ADDRESS, 0x0A, 0x01);
}

int i=50;
int count=0;
float x1=0;
float x2=0;
float x3=0;
float start_time;
float end_time;
float time_delay=0;
float x_COUNT1=0;
float y_COUNT1=0;
float z_COUNT1=0;
float x_COUNT2=0;
float y_COUNT2=0;
float z_COUNT2=0;

void loop() {
  //Store initial time
  start_time = millis();
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  //Calibration
  while(--i>=0){
    //To read the initial data
    int16_t axx = -(Buf[0] << 8 | Buf[1]);
    int16_t ayy = -(Buf[2] << 8 | Buf[3]);
    int16_t azz = Buf[4] << 8 | Buf[5];
    int16_t gxx = -(Buf[8] << 8 | Buf[9]);
    int16_t gyy = -(Buf[10] << 8 | Buf[11]);
    int16_t gzz = Buf[12] << 8 | Buf[13];
    float ax = axx * 9.81 / 2048;
    float ay = ayy * 9.81 / 2048;
    float az = azz * 9.81 / 2048;
    float gx = gxx / 16.4;
    float gy = gyy / 16.4;
    float gz = gzz / 16.4;
    digitalWrite(LED,HIGH);
    x_COUNT1+=ax;
    y_COUNT1+=ay;
    z_COUNT1+=az;
    x_COUNT2+=gx;
    y_COUNT2+=gy;
    z_COUNT2+=gz;
    delay(100);
    if(i==0){
      digitalWrite(LED,LOW);
    }
  }
  //To read the data
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];
  int16_t gx = -(Buf[8] << 8 | Buf[9]);
  int16_t gy = -(Buf[10] << 8 | Buf[11]);
  int16_t gz = Buf[12] << 8 | Buf[13];
  float ax_mss = ax * 9.81 / 2048 -x_COUNT1/50;
  float ay_mss = ay * 9.81 / 2048 -y_COUNT1/50;
  float az_mss = az * 9.81 / 2048 -z_COUNT1/50+9.81;
  //To calculate the angle using integration about time
  float gx_dps = (gx / 16.4  -x_COUNT2/50)* time_delay/1000;
  float gy_dps = (gy / 16.4  -y_COUNT2/50)* time_delay/1000;
  float gz_dps = (gz / 16.4  -z_COUNT2/50)* time_delay/1000;
  x1+=round(gx_dps*100)/100;
  x2+=round(gy_dps*100)/100;
  x3+=round(gz_dps*100)/100;
  //Store end time
  end_time = millis();
  //Calculate the delay (Approx 12-14ms)
  time_delay = end_time - start_time;
  count++;
  if(count==50){
    //Sending data is in the format of {Accel X: 0.004121  Accel Y: -0.071851  Accel Z: 9.812561  Deg X: 0.01  Deg Y: 0.01  Deg Z: 0.02}
    Serial.print("Accel X: "); Serial.print(ax_mss, 6); Serial.print("\t");
    Serial.print("Accel Y: "); Serial.print(ay_mss, 6); Serial.print("\t");
    Serial.print("Accel Z: "); Serial.print(az_mss, 6); Serial.print("\t");
    Serial.print("Deg X: "); Serial.print(x1, 2); Serial.print("\t");
    Serial.print("Deg Y: "); Serial.print(x2, 2); Serial.print("\t");
    Serial.print("Deg Z: "); Serial.print(x3, 2);
    Serial.println("");
    if(x1>360 || x1<-360){
      x1=0;
    }
    if(x2>360 || x2<-360){
      x2=0;
    }
    if(x3>360 || x3<-360){
      x3=0;
    }
    count=0;
  }
}
