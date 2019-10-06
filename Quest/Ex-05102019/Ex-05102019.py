# 注意事項
# 圖檔要放在 ./images 底下

import pandas as pd
from openpyxl import Workbook
from openpyxl.utils.dataframe import dataframe_to_rows
from openpyxl.styles import NamedStyle, Alignment, Font, Border, Side
from openpyxl.drawing.image import Image
from os.path import isfile

# --------------------
# 轉換表格排列格式
# --------------------

# 變數說明
# input_df: 儲存讀取進來的 xlsx 資料
# input_df_fresher: 儲存讀取進來的 xlsx 資料 part 2
# cols: 原資料中所需的欄位 (column/header) 名稱
# rows: 新資料中所需的列 (row/index) 名稱
# num_spec_row: 在 rows 當中作為分隔列的個數
# init_row_pos: cols 欄依序該對應到哪個 rows 列的索引值
# num_cols_in_row: 新資料中，每列的資料個數數
# row_pos: 已存入新資料的原資料筆數
input_df = pd.read_excel('input.xlsx')
input_df_fresher = pd.read_excel('fresher.xlsx')
output_name = "output.xlsx"
cols = [' 學號', ' 姓名', ' 地址', ' 聯絡電話1', ' 聯絡電話2', ' 導師姓名']
extend_cols = ['出生年月日', '入學前畢業學校']
rows = ['學號', '照片', '中文姓名', '英文姓名', '出生年月日', '住址', '住家電話', '手機號碼', '指導教授', '畢業學校', '']
num_spec_row = 1
init_row_pos = [0, 2, 5, 6, 7, 8, 4, 9]
num_cols_in_row = 6
row_pos = 0

# 過濾原資料中無用資料欄
input_df = input_df[cols]

# 在原資料中增加額外欄位
for i in range(0, len(extend_cols)):
    input_df[extend_cols[i]] = [0 for x in range(0, input_df.shape[0])]
cols = cols + extend_cols

# 以學號識別該資料是否為新生，並在原資料中添加新生資料且刪除非新生者
input_df.set_index(cols[0], inplace=True)
input_df_fresher.set_index("學號", inplace=True)
for i in input_df.index:
    exist = False
    for j in input_df_fresher.index:
        if i == j:
            exist = True
            for k in range(0, len(extend_cols)):
                input_df.ix[i, extend_cols[k]] = input_df_fresher.ix[i, extend_cols[k]]
    if exist == False:
        input_df = input_df.drop(i)
input_df.reset_index(inplace=True)

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
wb = Workbook()
ws = wb.active
for r in dataframe_to_rows(output_df, index=True, header=False):
    ws.append(r)
# 全儲存格格式
all_cell = NamedStyle(name='all_cell')
all_cell.alignment = Alignment(horizontal='center', vertical='center', wrap_text=True)
all_cell.font = Font(name='Calibri', size=11)
# 儲存格邊框格式
#border_l = Border(left=Side(border_style='thin'))
#border_r = Border(right=Side(border_style='thin'))
border_t = Border(top=Side(border_style='medium'), bottom=Side(border_style='thin'))
border_b = Border(top=Side(border_style='thin'), bottom=Side(border_style='medium'))
border_m = Border(bottom=Side(border_style='thin'))
# magni: 圖片放大倍率
magni = 1.5;
# 各標籤於 row 的位置
row_pos_pic = 0
row_pos_ssn = 0
row_pos_addr = 0

for r in range(1, len(rows) + 1):
    if ws.cell(r, 1).value == "照片":
        row_pos_pic = r
    if ws.cell(r, 1).value == "學號":
        row_pos_ssn = r
    if ws.cell(r, 1).value == "住址":
        row_pos_addr = r

# 設定全儲存格的格式
for r in range(1, ws.max_row):
    ws.row_dimensions[r].height = 30
    for c in range(1, ws.max_column + 1):
        ws.cell(r, c).style = all_cell

# 設定格式：欄
for c in range(66, 66 + ws.max_column + 1):
    ws.column_dimensions[chr(c)].width = 23 

# 設定格式：標籤欄
ws.column_dimensions['A'].width = 11

# 設定格式：照片
for r in range(row_pos_pic, ws.max_row,11):
    ws.row_dimensions[r].height = 160

# 設定格式：住址
for r in range(row_pos_addr, ws.max_row,11):
    ws.row_dimensions[r].height = 50

# 設定格式：border
for r in range(2, ws.max_row):
    for c in range(1, ws.max_column + 1):
        ws.cell(r, c).border = border_m

for r in range(2, ws.max_row, 11):
    for c in range(1, ws.max_column + 1):
        ws.cell(r, c).border = border_t
        ws.cell(r + 9, c).border = border_b

# 插入相片
for r in range(0, ws.max_row, len(rows)):
    for c in range(2, ws.max_column + 1):
        file_name = "./images/" + str(ws.cell(r + row_pos_ssn, c).value) + ".jpg"
        if (isfile(file_name)):
            img = Image(file_name)
            img.width = 106 * magni # pixel
            img.height = 132.3 * magni # pixel
            ws.add_image(img, ws.cell(r + row_pos_pic, c).coordinate)

# Debug
#for c in range(2, ws.max_column + 1):
    #file_name = str(ws.cell(2, c).value)
    #img = Image("./images/" + file_name + ".png")
    #img.width = 106 * magni # pixel
    #img.height = 132.3 * magni # pixel
    #ws.add_image(img, ws.cell(3, c).coordinate)

# 儲存表格
wb.save(output_name)