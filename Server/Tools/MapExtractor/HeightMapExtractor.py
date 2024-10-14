from PIL import Image
import numpy as np

def image_to_text(image_path, output_file):
    # 이미지 열기
    img = Image.open(image_path)
    print(f"이미지 모드: {img.mode}")
    img_array = np.array(img, dtype=np.uint16)
    
    width, height = img.size
    print(f"Image size: {width}x{height}")
    # 이미지의 픽셀 데이터를 numpy 배열로 변환
    
    normalized_array = img_array / 65535.0
    # 텍스트 파일로 저장
    with open(output_file, 'w') as f:
        for row in normalized_array:
            line = ''.join(f"{int(value * 255):03}" for value in row)  # 3자리로 포맷팅하여 붙여쓰기
            f.write(line + '\n')



path = ('HeightMap.png')
image_to_text(path, '../../MapFiles/MainMapHeight.txt')