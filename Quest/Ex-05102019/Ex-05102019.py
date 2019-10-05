import pandas as pd
import openpyxl
from openpyxl.utils.dataframe import dataframe_to_rows
from openpyxl.styles import NamedStyle, Alignment, Font
from openpyxl.drawing.image import Image

# --------------------
# 轉換表格排列格式
# --------------------

# 變數說明
# input_df: 儲存讀取進來的 xlsx 資料
# cols: 原資料中所需的欄位 (column/header) 名稱
# rows: 新資料中所需的列 (row/index) 名稱
# init_row_pos: cols 欄依序該對應到哪個 rows 列的索引值
# num_cols_in_row: 新資料中，每列的資料個數數
# row_pos: 已存入新資料的原資料筆數
input_df = pd.read_excel('input.xlsx')
cols = [' 學號', ' 姓名', ' 地址', ' 聯絡電話1', ' 聯絡電話2', ' 導師姓名']
rows = ['學號', '照片', '中文姓名', '英文姓名', '出生年月日', '住址', '住家電話', '手機號碼', '指導教授', '畢業學校', '']
init_row_pos = [0, 2, 5, 6, 7, 8]
num_cols_in_row = 6
row_pos = 0

# 過濾原資料中無用資料欄
input_df = input_df[cols]

# output_df: 新資料的表格
output_df = pd.DataFrame()

# times: 依 num_cols_in_row 將原資料分次填入暫存區段的次數
times = int(input_df.shape[0] / num_cols_in_row) + 1
for i in range(0, times):
    # output_temp: 新資料表格的暫存區段
    output_temp = pd.DataFrame(index=rows, columns=[index for index in range(0,num_cols_in_row)])
    # 依 cols 順序取出原資料，並將其寫入暫存區段
    for col in range(0, len(init_row_pos)):
        input_col = input_df[cols[col]]
        # 將該欄 num_cols_in_row 個資料依 init_row_pos 填入新資料暫存的列
        for j in range(0, num_cols_in_row):
            # 若指定的欄位超過範圍，則換下一列
            if (row_pos + j) > (input_df.shape[0] - 1):
                break;
            output_temp.iloc[init_row_pos[col], j] = input_col.iloc[row_pos + j]

    if row_pos == 0:
        # 首次將暫存加入實際輸出
        output_df = output_temp
    else:
        # 將暫存加入實際輸出
        output_df = pd.concat([output_df, output_temp])

    # 更新已填入資料個數
    row_pos += num_cols_in_row
    
# Debug
print(output_df)
print(output_df.shape)

# --------------------
# 設定格式
# --------------------

# 以 openpyxl 的格式使用 output_df
wb = openpyxl.Workbook()
ws = wb.active

for r in dataframe_to_rows(output_df, index=True, header=False):
    ws.append(r)

# 設定全儲存格的格式
all_cell = NamedStyle(name='all_cell')
all_cell.alignment = Alignment(horizontal='center', vertical='center', wrap_text=True)
all_cell.font = Font(name='Calibri', size=11)

for r in range(1, ws.max_row):
    ws.row_dimensions[r].height = 30
    for c in range(1, ws.max_column + 1):
        ws.cell(r, c).style = all_cell

# 設定格式：欄
for c in ('B', 'C', 'D', 'E', 'F', 'G'):
    ws.column_dimensions[c].width = 23 
# 設定格式：標籤欄
ws.column_dimensions['A'].width = 11
# 設定格式：照片
for r in range(3, ws.max_row,11):
    ws.row_dimensions[r].height = 160
# 設定格式：住址
for r in range(7, ws.max_row,11):
    ws.row_dimensions[r].height = 50

# 插入相片
# magni: 圖片放大倍率
magni = 1.5;
#for r in range(3, ws.max_row, 11)
    #for c in range(2, ws.max_column + 1):
        #file_name = str(ws.cell(r-1, c).value)
        #img = Image("./images/" + file_name + ".jpg")
        #img.width = 106 * magni # pixel
        #img.height = 132.3 * magni # pixel
        #ws.add_image(img, ws.cell(r, c).coordinate)

# Debug
for c in range(2, ws.max_column + 1):
    file_name = str(ws.cell(2, c).value)
    img = Image("./images/" + file_name + ".png")
    img.width = 106 * magni # pixel
    img.height = 132.3 * magni # pixel
    ws.add_image(img, ws.cell(3, c).coordinate)

# 儲存表格
wb.save("output.xlsx")