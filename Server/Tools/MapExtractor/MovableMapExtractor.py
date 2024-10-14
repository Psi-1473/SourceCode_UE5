from PIL import Image
import numpy as np

def image_to_text(image_path, output_file):
    # 이미지 열기
    img = Image.open(image_path)
    print(f"이미지 모드: {img.mode}")
    img_array = np.array(img);
    width, height = img.size
    print(f"Image size: {width}x{height}")
    # 이미지의 픽셀 데이터를 numpy 배열로 변환
    
    binary_array = (img_array > 0).astype(int)
    # 텍스트 파일로 저장
    with open(output_file, 'w') as f:
        for row in binary_array:
            line = ''.join(str(value) for value in row)  # 각 값을 문자열로 변환하여 공백으로 구분
            f.write(line + '\n')



path = ('MoveInfo.png')
image_to_text(path, '../../MapFiles/MoveInfo.txt')