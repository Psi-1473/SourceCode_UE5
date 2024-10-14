import pandas as pd
import json

# 엑셀 파일 읽기
file_path = 'NpcDialouge.xlsx'
xls = pd.ExcelFile(file_path)

# 모든 시트의 데이터 읽기
all_data = pd.read_excel(file_path, sheet_name = None)

# 모든 데이터 출력 (예시로 첫 번째 시트 출력)
for sheet_name, data in all_data.items():
    print(f"Sheet name: {sheet_name}")
    print(data)

combined_data = pd.concat(all_data.values(), ignore_index=True)

# JSON으로 변환
json_data = combined_data.to_json(orient='records', force_ascii=False)

# JSON 파일로 저장
with open('../../../MyRpg/Content/Json/NpcDialogueData.json', 'w', encoding='utf-8') as json_file:
    json.dump(json.loads(json_data), json_file, ensure_ascii=False, indent=4)

print("모든 시트를 합쳐서 JSON 파일로 저장했습니다.")