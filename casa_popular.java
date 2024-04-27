import java.net.*;
import java.io.*;

public class CasaPopularEficiente {

    // Configuring DSB
    static int ONE_WIRE_BUS = 4;
    static int deviceCount = 0;

    // Configuring ultraviolet sensor
    static float sensor = 0.0f;
    static float raw = 0.0f;
    static float sensorUV;

    // Configuring DHT
    static int DHT22_PIN1 = 8; // bathroom
    static int DHT22_PIN2 = 9; // bedroom 1
    static int DHT22_PIN3 = 7; // bedroom 2
    static int DHT22_PIN4 = 11; // living room
    static int DHT22_PIN5 = 12; // kitchen

    static int humi1, temp1, humi2, temp2, humi3, temp3, humi4, temp4, humi5, temp5;

    public static void main(String[] args) {
        // Setup code here...

        // Initialize sensors
        System.out.println("Locating devices...");
        System.out.print("Found ");
        deviceCount = sensors.getDeviceCount();
        System.out.println(deviceCount + " devices.");

        // Connect to WiFi
        connectToWiFi();

        while (true) {
            // Obtaining temperatures from DSBs
            sensors.requestTemperatures();

            // Show temperature of each sensor
            for (int i = 0; i < deviceCount; i++) {
                float tempC = sensors.getTempCByIndex(i);
                float tempC1 = sensors.getTempCByIndex(0);
                float tempC2 = sensors.getTempCByIndex(1);

                System.out.println("DSB1: " + tempC1 + " ºC");
                System.out.println("DSB2: " + tempC2 + " ºC");
            }

            // Obtaining temperatures from DHTs
            readDHT(DHT22_PIN1);
            readDHT(DHT22_PIN2);
            readDHT(DHT22_PIN3);
            readDHT(DHT22_PIN4);
            readDHT(DHT22_PIN5);

            // Obtaining UV index from ultraviolet sensor
            for (int i = 0; i < 5; i++) {
                raw += analogRead(A7);
            }
            System.out.println("Valor medio da leitura: " + raw / 5);
            sensorUV = raw / 1024.0f * 5.0f;
            sensorUV = sensor / 5;
            System.out.println("Nivel UV: " + sensorUV);

            // Establish TCP connection with ThingSpeak
            // Prepare GET string
            String getStr = "GET /update?api_key=3FSOOWSNE2XEJZY7&field1&field2&field3&field4&field5&field6=0";
            // Send data length

            // Send data to ThingSpeak
            sendDataToThingSpeak(getStr);

            // Send data to Google Sheets
            sendDataToGoogleSheets();

            // Delay between updates
            delay(15000);
        }
    }

    static void readDHT(int pin) {
        DHT.read22(pin);
        int humi = DHT.humidity;
        int temp = DHT.temperature;

        System.out.println("DHT (umidade): " + humi + " %");
        System.out.println("DHT (temperatura): " + temp + " ºC");
    }

    static void connectToWiFi() {
        // Code to connect to WiFi
    }

    static void sendDataToThingSpeak(String getStr) {
        try {
            String cmd = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80";
            // Establish TCP connection

            // Send data length

            // Send data
            System.out.println(getStr);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void sendDataToGoogleSheets() {
        try {
            // Code to send data to Google Sheets
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void delay(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
