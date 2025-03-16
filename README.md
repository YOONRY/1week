https://youtu.be/alHJsZcv0D4?si=Yj-HD-j2aSYUaTnh

# 프로젝트 시연 유튜브 링크



# p5.js & Arduino 프로젝트

## 프로젝트 개요
이 프로젝트는 **p5.js**와 **Arduino**를 활용하여 웹과 물리적 디바이스가 상호작용하는 과제물입니다. 
웹 기반 인터페이스(p5.js)와 아두이노를 연결하여 실시간 시각화, 제어를 구현하였습니다.

---

## 사용된 기술
- **Arduino**: 물리적인 입력(버튼) 및 출력(LED) 제어
- **p5.js**: 웹 기반 시각화 및 인터페이스 구축
- **Web Serial API**: 웹과 아두이노 간의 직렬 통신
- **TaskScheduler (Arduino 라이브러리)**: 아두이노에서 효율적인 멀티태스킹 구현


사용된 기술

Arduino: 물리적인 입력(버튼, 센서) 및 출력(LED, 서보모터) 제어

p5.js: 웹 기반 시각화 및 인터페이스 구축

Web Serial API: 웹과 아두이노 간의 직렬 통신

TaskScheduler (Arduino 라이브러리): 아두이노에서 효율적인 멀티태스킹 구현

![KakaoTalk_20250316_170641680](https://github.com/user-attachments/assets/347b33e5-6301-43a3-a4c7-3030cacefa04)
사용 된 회뢰의 실물 사진

![image](https://github.com/user-attachments/assets/b2c87e03-96c6-4d6c-a4e7-da960ffac318)

이해를 돕기 위한 이미지

##  하드웨어 구성 (핀 연결)
<table>
  <tr>
    <th>구성 요소</th>
    <th>핀 번호</th>
  </tr>
  <tr>
    <td>🔴 빨간 LED</td>
    <td>DIGITAL 4</td>
  </tr>
  <tr>
    <td>🟡 노란 LED</td>
    <td>DIGITAL 9</td>
  </tr>
  <tr>
    <td>🟢 초록 LED</td>
    <td>DIGITAL 5</td>
  </tr>
  <tr>
    <td>Emergency 버튼</td>
    <td>DIGITAL 6</td>
  </tr>
  <tr>
    <td>Blinking 버튼</td>
    <td>DIGITAL 7</td>
  </tr>
  <tr>
    <td>ONOFF 버튼 3</td>
    <td>DIGITAL 8</td>
  </tr>
  <tr>
    <td>🎚 가변저항</td>
    <td>아날로그 A0</td>
  </tr>
</table>

### 버튼 및 신호 디폴트 상태

- 버튼 입력은 디지털 핀(PULLDOWN 저항 필요)

- 버튼을 누르지 않을 때: HIGH (5V)

- 버튼을 눌렀을 때: LOW (0V)

- LED 출력은 PWM 제어 가능 (AnalogWrite 사용)

##  주요 기능
### 1 **신호등 제어 시스템** 
- **아두이노의 LED를 p5.js에서 제어**
- **비상 모드 (Emergency)**: 버튼 클릭 시 빨간불 고정
- **블링크 모드 (Blinking)**: 모든 LED가 깜빡이는 기능
- **전원 ON/OFF** 기능 지원
- **p5.js UI에서 신호등 상태 실시간 확인**

### 2 **실시간 센서 데이터 시각화**
- **가변 저항 데이터 시각화**
- **웹 UI에서 실시간 데이터 모니터링**

### 3 **서보모터 원격 제어** 
- **웹 UI에서 슬라이더를 움직이면 LED 주기 변경**
- **실시간 반응 속도 개선을 위한 Web Serial API 적용**
---

##  실행 방법
### **아두이노 설정**
1. **아두이노 IDE**에서 프로젝트 코드를 업로드합니다.
2. **TaskScheduler등 필요한 라이브러리**를 설치합니다.
3. 시리얼 모니터를 통해 데이터가 정상적으로 출력되는지 확인합니다.

### **p5.js 실행**
1. `index.html` 파일을 웹 브라우저에서 실행합니다.
2. UI에서 "연결하기" 버튼을 클릭해 **Arduino와 Web Serial**을 연결합니다.
3. 센서 데이터를 실시간으로 확인하거나, 슬라이더바로 아두이노를 제어합니다.

   ![image](https://github.com/user-attachments/assets/250ace37-1a4d-4192-9cb2-0d01a42d2f96)
   웹 UI 예시




참고자료

p5.js 공식 문서: https://p5js.org

Arduino 공식 사이트: https://www.arduino.cc

Web Serial API: https://developer.mozilla.org/en-US/docs/Web/API/Serial

챗 gpt : https://chatgpt.com/



