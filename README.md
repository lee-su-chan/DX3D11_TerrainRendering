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
F3: 상호작용 재생 켜기/끄기  
F4: 셀 분할 스키마 시각화 켜기/끄기

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

### 2017-01-07(Tutorial 6 : Terrain Normal Mapping)
-이번 단계는 가까이서만 확인할 수 있으므로 헤드라인 사진은 없습니다.
#### *Texture X, ColorMapping X*
![no texture colormapping](https://cloud.githubusercontent.com/assets/13383741/21742300/08996e26-d52f-11e6-9020-1ab6aea3890e.PNG)
#### *Texture O, ColorMapping X*
![have texture no colormapping](https://cloud.githubusercontent.com/assets/13383741/21742301/08c468ec-d52f-11e6-983e-dce4673c890a.PNG)
#### *Texture O, ColorMapping O*
![have texture colormapping](https://cloud.githubusercontent.com/assets/13383741/21742302/08ecdee4-d52f-11e6-8670-fd7ead45d656.PNG)
1. 지형 노말 맵핑 구현  
2. 이제부터 한 단계가 끝나면 개발자 노트를 쓸 것입니다.  

#### My Thinks  
이번 단계에선 노말 맵핑이라는 것을 배웠다.
내가 정확히 이해했는지는 모르겠지만 아마 한 텍스쳐를 위한 NormalMap 텍스쳐가 준비되어야 하는 것 같다.
그리고 노말 맵핑이란 NormalMap 텍스쳐를 이용하여 법선이란 의미의 Tangent와
종법선이란 의미의 Binormal을 이용하여 지형의 한 픽셀마다의 조명 정보를 얻어 렌더링 하는 기법인 것 같다.

전 단계에선 지형에 가까이 가면 뭉개진 느낌이 났는데 이번 단계에서 노말 맵핑을 하고 나니
눈에 잘 띄지는 않아도 가까이 가면 그 형태가 온전히 이루어져 있는것이 보여졌다.

정말 DirectX는 내가 무엇을 상상하든 그 이상을 보여주는 것 같다. 그래서 하면 할수록 재밌다.
HeightBitmap을 가지고 지형을 만들었을 때나 텍스쳐를 입히고 조명을 주고 컬러 맵핑을 했을때나
하나 같이 마지막에 실행을 했을땐 항상 감탄했었다.
앞으로 더 해야할 단계들이 남았지만 실행창을 보았을때의 느낌은 무뎌지지 않을 것 같다.

### 2017-01-09(Tutorial 7 : Sky Domes)
#### *Headline*
![1](https://cloud.githubusercontent.com/assets/13383741/21773265/bfc222ac-d6d1-11e6-8952-efec542a2681.PNG)
#### *LookAtSky*
![2](https://cloud.githubusercontent.com/assets/13383741/21773266/bfe9d22a-d6d1-11e6-8e98-e698e427ce13.PNG)
1. 스카이돔 렌더링 구현

#### Developer Notes
이번 단계에서는 하늘을 그렸다.  
스카이돔의 여러 정보를 가지고있는 txt파일을 읽어들여서 데이터를 입력했는데  
rasterTek은 Maya를 이용하여 obj형식의 모델을 만들었다고 한다.  
내가 Maya를 몰라서 그런 걸 수도 있지만 obj형식으로 내보낸 파일이 어떻게 txt파일이 되었는지는 미지수이다.  
  
확실히 전 단계까지는 주위가 온통 까만색이어서 신경쓰였는데  
이번 단계에서 하늘을 그리니까 왠지 신이 된 기분이 들었다.  
  
나만 느끼는 걸 수도 있지만 일반적인 도스창을 보며 하는 프로그래밍도 없는 것을 만드는 느낌이 들어서 신 같았는데  
그래픽스 프로그래밍는 좀더 명확한 결과물이 있어서 그런지 유난히 신이 된 기분이다.  
  
다음번엔 실시간으로 시간이 지남에따라 하늘색이 바뀌는 것을 구현할 예정이다.  

### 2017-01-11(Update Function)
#### *LookAtNewUI*
![1](https://cloud.githubusercontent.com/assets/13383741/21847840/5dc60d86-d841-11e6-8fdf-3d0c3fe34b81.PNG)
1. ApexColor, CenterColor 수치 확인을 위한 인터페이스 구현

#### Developer Notes
오늘은 UI추가를 했는데 기존에 있던 코드들이 어떻게 돌아가는지  
확실하게 이해하는데에는 스스로가 무언가를 생각해서 그걸 직접  
구현 해보는게 좋고 구현 능력도 느는 것 같다.  
  
새로 만든 인터페이스 덕에 앞으로의 실시간 하늘색 변경 구현을  
좀더 편하게 개발할 수 있을 것이다.  

### 2017-01-12(Tutorial 8 : Raw Height Maps)
#### *Headline*
![2](https://cloud.githubusercontent.com/assets/13383741/21894200/a4660ab2-d921-11e6-8423-e0afc9e71251.PNG)
#### *Topview*
![3](https://cloud.githubusercontent.com/assets/13383741/21894202/a4922b4c-d921-11e6-9491-5056c224f3b5.PNG)
1.  16비트 RAW파일을 이용한 지형 렌더링 구현(세부적인 높이 조절)   

#### Developer Notes
오늘은 새로운 Colormap.bmp파일과 Heightmap.r16파일을 이용하여  
전보다 더 세부적인 지형을 렌더링 하였다.  
  
기존에 있던 Setup.txt의 지형 가로 세로 수치들이 255에서 1025로 증가하였고  
스케일링의 수치도 12.0에서 300.0으로 증가하였다.  
  
이제는 Heightmap파일로 비트맵 형식은 최대 범위는 2의 8승이였다면  
Raw16 형식은 2의 16승까지 가능함으로 매우 세부적인 높이 묘사가 가능한 것 같다.  
Colormap파일도 Heightmap파일과 같이 더 세부적인 정보를 가지고 있는 파일로 바꾸었다.  
  
Heightmap파일과 Colormap파일의 용량들이 193KB에서 각각 2MB, 3MB로 바뀐걸 보면  
이전과는 비교할 수 없을만큼의 데이터들이 있는 듯 하다.  
  
그리고 이번 단계부터 렌더링양이 방대하게 많아져서 FPS가 많이 떨어지기 때문에  
외장그래픽을 잡지 못하는 버그를 고쳐야 할 것 같다.  

### 2017-01-16(Tutorial 9 : Terrain Cells)
#### *Headline*
![1](https://cloud.githubusercontent.com/assets/13383741/21964535/fdfc0b3c-db90-11e6-8d43-ad4d8c1eec45.PNG)
#### *Topview*
![2](https://cloud.githubusercontent.com/assets/13383741/21964536/fdfc0cae-db90-11e6-9483-1665c6a9da35.PNG)
#### *Closely*
![3](https://cloud.githubusercontent.com/assets/13383741/21964565/89d271dc-db91-11e6-8537-f1e3a578c2fc.PNG)
1. 지형 셀 분할 스키마 구현(한 셀에 32개의 쿼드)  
2. 조작키 추가(F4)

#### Developer Notes
이번에는 지형 셀 분할 스키마를 구현했다.  
  
이 단계에서 한 것은 지형 렌더링의 최적화 부분인 것 같다.  
rasterTek이 말하길 지형은 대부분 대규모 데이터이므로 효율적으로  
처리해야 한다고 말했다.  
  
또 지형을 셀로 분할하는 주된 이유는 보이는 것만 렌더링하여 효율성을  
높이기 위함이라고 한다. 하지만 이번 단계에서는 셀 분할과 시각화까지만 진행 되었다.  
  
앞으로의 단계에서 최적화를 할지 안할지는 모르지만 이부분은 굉장히 중요한 부분인것 같다.  
  
내가 지금까지 무언가를 개발하면서 성능을 그렇게까지 중요시하는 프로그램이 없었는데  
이번에 이렇게 하게 되면서 성능과 품질의 타협을 배우고 있는 것 같다.  

### 2017-01-18(Tutorial 10 : Terrain Cell Culling)
#### *LookAtNewUI*
![1](https://cloud.githubusercontent.com/assets/13383741/22067691/ac6bcd9c-ddd5-11e6-8373-151ccdf7749f.PNG)
1. UI 추가  
2. 보이지 않는 셀 컬링 구현  

#### Developer Notes
이번 단계에서는 지형 셀 컬링을 구현했다.  
  
화면에서 보이지 않는 셀은 처리하지 않으며, 컬링의 기준을 셀 단위로 보기 때문에  
조금이라도 셀이 화면에 걸친다면 그 셀도 모두 처리한다.  
  
화면에 셀이 걸치는지 안걸치는지를 체크하는 클래스를 만들었다.  
내적등의 계산이 있어서 그 연산들이 어떻게 되는지는 정확히 모르지만  
나름 cocos2d-x에서 여러 충돌처리를 구현 해봤었기 때문에  
어떤 느낌인지는 알고 있다.(OBB는 구현하지 못했다.. 작년 얘기니까 지금은 구현 할 수 있을지도 모른다.)  
  
프로그래머에게 수학은 유리하고 일을 편하게 할 수 있는 무기인 것 같다.  
그래픽스 프로그래머라면 더욱 더 수학이 필요하다는 걸 서서히 느끼고 있다.  
최소한의 지식마저도 내가 이해 할 수 있을지 없을지도 모른다.  
이 단계 끝마치고 내 앞에 커다란 장벽이 꽤 가까이 있을 수도 있다고 생각했다.  
