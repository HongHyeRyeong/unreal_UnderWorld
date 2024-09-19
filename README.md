# Breakout abyss
![image](https://github.com/user-attachments/assets/e8227d33-de1f-49ad-97ef-e1364fe0a233)

## 개요
* 개발 기간: 2024.03 – 2024.07
* 개발 인원: 클라이언트(홍혜령), 기획(오영주)
* 개발 환경: Unreal5, C++

## 다운로드
https://drive.google.com/file/d/1hipUKr26XhzzvY9L8sQxOepl-rcc7nhw/view?usp=sharing

## 게임 설명
플레이어가 생명력을 모두 소진하기 전까지 적을 피하며, 기계를 수리하여 탈출하는 서바이벌 게임이다.

* 적을 피하거나 적의 공격에 반격하며, 동시에 주어진 임무를 수행해야 하는 플레이로부터 느낄 수 있는 스릴감.
* 스테이지 별로 다른 장르로 변경되어, 플레이어의 빠른 적응이 필요하여 신선함과 재미를 느낄 수 있음.

## 게임 플레이
### 플레이어
플레이어는 아이템을 획득하여 스탯을 올리거나, 기계를 수리하거나, 감옥을 탈출할 수 있다.
* 획득한 착용 아이템(모자, 가방)은 인벤토리를 열어 확인 가능하다. 또한 다른 아이템으로 바꿔 착용할 수 있다.
* 스테이지를 클리어할수록 레벨이 높은 아이템을 획득할 수 있다.
* 스태미나를 통해 뛰기가 가능하다.

|플레이어|
|---|
|![image](https://github.com/user-attachments/assets/d586066a-c4d9-421f-979f-c365f7c9db2e)|

### 기계 수리
스테이지 내 모든 기계를 수리하면 다음 스테이지로 넘어간다.

|기계 수리|기계 수리 완료|
|---|---|
|![image](https://github.com/user-attachments/assets/596a0d01-1b7e-4802-9971-238fcaf01400)|![image](https://github.com/user-attachments/assets/5621d6b5-6b53-4b81-948f-e6f59a6306bd)|

### 살인마
* 살인마는 텔레포트를 하며 기계 주변을 순회한다. 종종 덫을 설치하기도 한다.
* 순회 중 플레이어를 발견하면 바로 추적을 시작한다.
* 플레이어와 가까워지면 공격을 하며 이때 플레이어는 반격이 가능하다.

|살인마 순회|살인마 추적|
|---|---|
|![image](https://github.com/user-attachments/assets/df099857-62f2-4f79-9f48-da5b11b3a42e) ![image](https://github.com/user-attachments/assets/d4747e80-5108-409c-a46b-b11514766c96)|![image](https://github.com/user-attachments/assets/69d3c2bb-d175-4571-a8c8-9825b94462e4)|

|살인마 공격|생존자 반격|
|---|---|
|![image](https://github.com/user-attachments/assets/0712e597-0ad8-4c8c-91bc-3dea58a09082)|![image](https://github.com/user-attachments/assets/7a725c88-23b5-49b6-8e96-edfeb4de30ba)|

살인마의 공격을 피하거나 몰래 공격도 가능하다.

|생존자 피하기|생존자 공격|
|---|---|
|![image](https://github.com/user-attachments/assets/8b166095-184a-457c-bc84-685d5e39a263)|![image](https://github.com/user-attachments/assets/6d965825-22aa-4756-ab94-4009b6f4a289)|

모든 생명력을 잃었을 때 열쇠 아이템이 있다면 감옥에 갇히고, 없다면 스테이지 실패로 다시 시작하게 된다.

|감옥|
|---|
|![image](https://github.com/user-attachments/assets/57bbc088-93c7-4570-885b-5ed02f0b2d76)|

### 최종 스테이지
최종 스테이지는 이전 스테이지와 다르게 디펜스를 해야 한다.
* 플레이어는 한 건물에 갇히게 되고 그 안에서 아이템 획득, 기계 수리를 통해 탈출해야 한다.
* 살인마는 건물 내 문을 통해 계속해서 침입을 시도한다.
* 살인마와 문의 거리에 따라 문의 빛이 다르게 난다.
* 플레이어는 문 앞의 버튼을 통해 문을 닫을 수 있으며 닫힌 문으론 살인마가 들어오지 못한다.


|살인마 침입 시도|생존자 문 닫기|살인마 침입 성공|
|---|---|---|
|![image](https://github.com/user-attachments/assets/5edc04a4-e11d-4d1b-92c8-d027edd31906)|![image](https://github.com/user-attachments/assets/4c10df9c-3503-4bd0-8340-24182db1fedc)|![image](https://github.com/user-attachments/assets/60712873-fa26-4705-9d0d-a8dc6c80e5ac)|
