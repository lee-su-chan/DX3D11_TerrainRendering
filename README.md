# DX3D11_TerrainRendering

## Origin
http://www.rastertek.com/tutdx11s2ter.html

## Diagram
![pic4023](https://cloud.githubusercontent.com/assets/13383741/21643936/5a80712a-d2ce-11e6-8b76-9aca403e47dd.gif)

## Operation keys
↑: 전진  
↓: 후진  
← →: 회전  
A: 왼쪽이동  
D: 오른쪽이동  
W: 상승  
X: 하강  
PgUp: 시점 위 회전  
PgDn: 시점 아래 회전  
F1: 디스플레이 인터페이스 켜기/끄기  
F2: 와이어프레임 켜기/끄기  

## Report

### 2016-12-29(Tutorial 1 : Grid and Camera Movement)  
![default](https://cloud.githubusercontent.com/assets/13383741/21547893/439056c0-ce2c-11e6-9f80-9c2fae5e415a.PNG)  
1. 기본 지형 렌더링 구현  
2. 카메라 이동 구현  
3. 디스플레이 인터페이스 구현

### 2016-12-30(Tutorial 2 : Bitmap Height Maps)
#### *Wireframe*  
![wireframe](https://cloud.githubusercontent.com/assets/13383741/21567214/ba62a1ba-ceed-11e6-8453-7070b9fc1bbd.PNG)
#### *Solidframe*  
![solid](https://cloud.githubusercontent.com/assets/13383741/21567219/c1226594-ceed-11e6-9f2a-ee273e215f7c.PNG)
1. 비트맵 파일을 이용한 높이 맵 지형 구현  
2. 카메라 좌표, 각도 버그 수정  
3. 디스플레이 인터페이스 버그 수정  
4. 조작키 변경 및 추가  

### 2017-01-02(Tutorial 3 : Terrain Texturing)
#### *Headline*
![1](https://cloud.githubusercontent.com/assets/13383741/21581944/7825ce4c-d089-11e6-927e-b860b64155c9.PNG)  
#### *Ground Texture*
![2](https://cloud.githubusercontent.com/assets/13383741/21581949/b57295f0-d089-11e6-9312-64b7246fa785.PNG)  
#### *Test Texture*
![3](https://cloud.githubusercontent.com/assets/13383741/21581950/c5ae3690-d089-11e6-8396-ce0cd662f308.PNG)  
1. 텍스쳐 맵핑 구현  
  
### 2017-01-03(Tutorial 4 : Terrain Lighting)
#### *Ground Texture*  
![1](https://cloud.githubusercontent.com/assets/13383741/21688770/69298b7e-d3b1-11e6-972a-9c0ba21f9257.PNG)
#### *Test Texture*
![testtexture](https://cloud.githubusercontent.com/assets/13383741/21609034/7cb7a946-d203-11e6-8cd2-c286baa33ac7.PNG)
1. 조명 효과 구현

### 2017-01-05(Tutorial 5 : Color Mapped Terrain)
#### *Headline*
![2](https://cloud.githubusercontent.com/assets/13383741/21688771/692b50c6-d3b1-11e6-8b7b-92616636beb7.PNG)
1. 비트맵 파일을 이용한 색상 맵핑 구현
