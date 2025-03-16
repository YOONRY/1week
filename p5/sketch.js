let serialPort;
let reader;
let textDecoder;
let incomingData = "";
let trafficLightState = "OFF";
let blinkState = false;
let lastBlinkTime = 0;

async function connectSerial() {
    try {
        serialPort = await navigator.serial.requestPort();
        await serialPort.open({ baudRate: 115200 });
        textDecoder = new TextDecoderStream();
        reader = serialPort.readable.pipeThrough(textDecoder).getReader();
        readSerialData();
    } catch (error) {
        console.error("⚠ 연결 실패:", error);
    }
}

async function readSerialData() {
    try {
        while (true) {
            const { value, done } = await reader.read();
            if (done) {
                reader.releaseLock();
                break;
            }
            if (value) {
                incomingData += value;
                processSerialData();
            }
        }
    } catch (error) {
        console.error("⚠️ 읽기 실패:", error);
    }
}

let ledBrightness = 255; // 초기 밝기값 저장용 변수

function processSerialData() {
    let stateStart = incomingData.indexOf("<");
    let stateEnd = incomingData.indexOf(">");
    let brightnessStart = incomingData.indexOf("[BRIGHTNESS:");
    let brightnessEnd = incomingData.indexOf("]");

    while (stateStart !== -1 && stateEnd !== -1 && stateStart < stateEnd) {
        trafficLightState = incomingData.substring(stateStart + 1, stateEnd).trim();
        document.getElementById("lightState").innerText = trafficLightState;
        incomingData = incomingData.substring(stateEnd + 1);
        stateStart = incomingData.indexOf("<");
        stateEnd = incomingData.indexOf(">");
    }

    while (brightnessStart !== -1 && brightnessEnd !== -1 && brightnessStart < brightnessEnd) {
        let brightnessValue = incomingData.substring(brightnessStart + 12, brightnessEnd);
        ledBrightness = parseInt(brightnessValue); // 숫자형으로 저장
        document.getElementById("brightnessValue").innerText = ledBrightness;
        incomingData = incomingData.substring(brightnessEnd + 1);
        brightnessStart = incomingData.indexOf("[BRIGHTNESS:");
        brightnessEnd = incomingData.indexOf("]");
    }
}

function setup() {
    createCanvas(200, 400);
    frameRate(20);
}
async function sendTimings() {
    const red = parseFloat(document.getElementById("redSlider").value) * 1000;
    const green = parseFloat(document.getElementById("greenSlider").value) * 1000;

    const encoder = new TextEncoder();
    const writer = serialPort.writable.getWriter();
    await writer.write(encoder.encode(`<SET,${red},${green}>`));
    writer.releaseLock();

    document.getElementById("redValue").innerText = (red / 1000).toFixed(1);
    document.getElementById("greenValue").innerText = (green / 1000).toFixed(1);
}

async function sendTimings() {
    const red = parseFloat(document.getElementById("redSlider").value) * 1000;
    const yellow = parseFloat(document.getElementById("yellowSlider").value) * 1000;
    const green = parseFloat(document.getElementById("greenSlider").value) * 1000;

    const encoder = new TextEncoder();
    const writer = serialPort.writable.getWriter();
    await writer.write(encoder.encode(`<SET,R:${red},Y:${yellow},G:${green}>`));
    writer.releaseLock();

    document.getElementById("redValue").innerText = (red/1000).toFixed(1);
    document.getElementById("yellowValue").innerText = (yellow/1000).toFixed(1);
    document.getElementById("greenValue").innerText = (green/1000).toFixed(1);
}

function draw() {
    background(230);

    if (millis() - lastBlinkTime > (trafficLightState === "GREEN_BLINK" ? 200 : 500)) {
        blinkState = !blinkState;
        lastBlinkTime = millis();
    }

    switch (trafficLightState) {
        case "RED": drawLights(true, false, false); break;
        case "YELLOW": drawLights(false, true, false); break;
        case "GREEN": drawLights(false, false, true); break;
        case "GREEN_BLINK": drawLights(false, false, blinkState); break;
        case "BLINKING": drawLights(blinkState, blinkState, blinkState); break;
        case "EMERGENCY": drawLights(true, false, false); break; // 🚨 빨강으로만 점등
        case "OFF": drawLights(false, false, false); break;
        default: drawLights(false, false, false); break;
    }
}

function drawLights(redOn, yellowOn, greenOn) {
    fill(0);
    rect(50, 50, 100, 300, 20);

    fill(redOn ? 'red' : '#555');
    ellipse(100, 120, 60, 60);

    fill(yellowOn ? 'yellow' : '#555');
    ellipse(100, 200, 60, 60);

    fill(greenOn ? 'green' : '#555');
    ellipse(100, 280, 60, 60);
}
