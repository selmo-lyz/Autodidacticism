# 萬年曆

## 目錄

- [定年中 M 月 D 日 星期幾](#定年中 M 月 D 日 星期幾)
- [年曆的循環](#年曆的循環)
- [曆法改革](#曆法改革)
- [年曆的循環 - 英美年曆：1 A.D. ~ 1752 A.D.](#年曆的循環 - 英美年曆：1 A.D. ~ 1752 A.D.)

採英美曆法  
輸入：Y 年 M 月 D 日  
輸出：該日為星期幾 與 該年該月的月曆  

## 定年中 M 月 D 日 星期幾

Premise:  
各星期的 index 為 0：日, 1：一, 2：二, ..., 6：六  
該年 M 月 1 日的前一天是星期 W

Conclusion:
M 月 1 日則是星期 W + 1，且 M 月 N 日是星期 W + N  

由此可知，  
若令該年的 M 月 1 日的前一天為星期 W，則 M 月的星期換算公式為 (W + N) % 7，並且  
若將一年的 W 值轉換為 int [12] 的陣列，即可透過輸入 M 月 D 日來輸出星期幾  

## 年曆的循環

一月  
日 一 二 三 四 五 六  
1  2  3  4  5  6  7  
......  
29 30 31 1  2  3  4  
5  6  7  8  9  10 11  
......  
十二月  
......  
26 27 28 29 30 1  2  
3  4  5  6  7  8  9  
10 11 12 13 14 15 16  
17 18 19 20 21 22 23  
24 25 26 27 28 29 30  
31 1  2  3  4  5  6  

Premise:  
因為一年有 365 天，而 365 % 7 = 1，因此每年的各月 W 值皆會是前年加一，  
又因閏年有 366 天，而 366 % 7 = 2，因此二月後各月 W 值皆會是前年加二。

而閏年的規則為：

1. 4 年一閏
2. 100 年不閏
3. 400 年一閏

Conclusion:  
(W[M-1] + (Y-1753) + (Y/4) - (Y/100) + (Y/400) - ?(若 Y 為閏年且 M < 3) 1 : 0) % 7

由此可知，  
若令 1753 A.D. M 月的 W 值為 W[M-1]，則可求得 Y A.D. M 月的 W 值為  
(W[M-1] + (Y-1753) + (Y/4) - (Y/100) + (Y/400) - ?(若 Y 為閏年且 M < 3) 1 : 0) % 7

> ##### Note
> 因為 1753 A.D. 才開始現在的閏年規則，故起始需為 1753 A.D.

## 曆法改革

### 45 B.C. ~ 1582 A.D.

- 45 B.C. 開始實施儒略曆  
  （註：1 A.D. 之前為 0 B.C.，故西元前 1 年為 0 B.C.）  
  （註： 4/03/01 以前，閏年不固定
- 1582 年曾經改為格里曆，且由於儒略曆有誤差 10 天，故 1582/10/04 的隔天為 1582/10/15，  
  45 B.C. ~ 1 A.D. 少算 2 天，1 A.D. ~ 1582 A.D. 多算 12 天

> #### Note
> 1582/10/15 以前的日期因為一些原因，故無法查詢

### 1582 A.D. ~ 1752 A.D.

- 英美曆法 1752/09/02 是星期三，但隔天是 1752/09/14 星期四  
  （註：在 Y <= 1752 年，每四年一潤，無其他規則）
- 中國曆法 1752/12/18 是星期四，但隔天是 1912/01/01 星期一  
  （註：在 Y <= 1911 年，每四年一潤，無其他規則）

> ##### Note
> 因為英國在 1752/09 才使用格里曆，誤差已達到 11 天，故以此調整。

### 英美曆法：1753 A.D. ~ Now

閏年的規則為：

1. 4 年一潤
2. 100 年不潤
3. 400 年一潤

## 年曆的循環 - 英美年曆：1 A.D. ~ 1752 A.D.

因為現行的閏年規則於 1753 A.D. 開始，故在此之前需套用其他公式，  
並且因為在 1752 A.D. 09/02 (三)時，為了調整 11 天的誤差，故隔天為 1752/09/14。

Premise:  
因為一年有 365 天，而 365 % 7 = 1，因此每年的各月 W 值皆會是前年加一，  
又因閏年有 366 天，而 366 % 7 = 2，因此二月後各月 W 值皆會是前年加二。  
而閏年的規則為：4 年一閏  
在 1752 A.D. 09/02 (三)時，為了調整 11 天的誤差，故隔天為 1752/09/14。

Conclusion:

```java
if (y == 1752 && m == 9)
  輸出 1752/09 的月曆
else
  (W[M-1] + (Y-1) + (Y/4) - ?(若 Y 為閏年且 M < 3) 1 : 0) % 7
```

由此可知，  
若選擇 1752/09，則輸出其專用的月曆  
若令 1 A.D. M 月的 W 值為 W[M-1]，則可求得 Y A.D. M 月的 W 值為  
(W[M-1] + (Y-1) + (Y/4) - ?(若 Y 為閏年且 M < 3) 1 : 0) % 7


> ##### Note
> 1 A.D. 之前為 0 B.C.，故在此以 1 A.D. 為起始

|日 一 二 三 四 五 六|  
|      1  2  14 15 16|  
|17 18 19 20 21 22 23|  
|24 25 26 27 28 29 30|

## 參考資料

日期換算公式(http://dhcp.tcgs.tc.edu.tw/c/p015.htm)
曆法改革(https://people.cs.nctu.edu.tw/~tsaiwn/introcs/03_Labs/Lab08/)