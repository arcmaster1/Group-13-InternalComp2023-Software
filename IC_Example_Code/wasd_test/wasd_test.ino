#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

//README:
//Install Libary "ESPAsyncWebServer" before compiling through Arduino IDE Libary Manager
//Else it will not compile.

const char* ssid = "SBRT";
const char* password = "Robotic$3";

// Define motor pins
const int motorA1 = 15; 
const int motorA2 = 2;
const int motorB1 = 32;
const int motorB2 = 33;

AsyncWebServer server(80);


String html2 = R"RAW(
<!DOCTYPE html>
<head>
<title>ESP32 Direction Controller</title>
</head>
<body>
<h1>ESP32 Direction Controller</h1>
<p>Use WASD keys to control the direction:</p>
<p>W - Forward, A - Left, S - Backward, D - Right</p>

<script>
// Function to send the request to the ESP32
function sendDirection(direction) {
  // Replace with the IP address of your ESP32
  var ip = window.location.host;
  var espUrl = 'http://';
  espUrl = espUrl + ip;
  espUrl = espUrl + '/control';

  // Assemble the full URL with the direction parameter
  var fullUrl = espUrl + '?dir=' + direction;

  // Send the request
  fetch(fullUrl)
    .then(response => response.text())
    .then(data => console.log(data))
    .catch(error => console.error('Error:', error));
}

// Event listener for keydown to start movement
document.addEventListener('keydown', function(event) {
  if(event.key === 'w' || event.key === 'W') {
    sendDirection('forward');
  } else if(event.key === 'a' || event.key === 'A') {
    sendDirection('left');
  } else if(event.key === 's' || event.key === 'S') {
    sendDirection('backward');
  } else if(event.key === 'd' || event.key === 'D') {
    sendDirection('right');
  }
});

// Event listener for keyup to stop movement
document.addEventListener('keyup', function(event) {
  if(['w', 'W', 'a', 'A', 's', 'S', 'd', 'D'].includes(event.key)) {
    sendDirection('stop');
  }
});
</script>
</body>
</html>
)RAW";

void moveForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void moveBackward() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void turnLeft() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void turnRight() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void stopCar() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  stopCar();  // Ensure car is stopped on startup

  Serial.begin(115200);
  // WiFi.softAP(ssid, password);
  // //   while (WiFi.status() != WL_CONNECTED) {
  // //   delay(1000);
  // //   Serial.println("Connecting to WiFi...");
  // // }
  // Serial.println(WiFi.localIP());
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected! \nPaste this IP address into your browser to control your robot:");
  Serial.println(WiFi.localIP());

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html2);
  });

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    String direction = request->getParam("dir")->value();
    if (direction == "forward") {
      moveForward();
    } else if (direction == "backward") {
      moveBackward();
    } else if (direction == "left") {
      turnLeft();
    } else if (direction == "right") {
      turnRight();
    } else if (direction == "stop") {
      stopCar();
    }
    request->send(200);
  });

  server.begin();
}

void loop() {
  
  
}


