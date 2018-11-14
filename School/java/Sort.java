/*
操作說明：
    在見到 "Elements in array a are: " 時，即可輸入欲排序的多個數字，
    (輸入的數字須在每個數字其間加上一個空格)
    在此，若輸入的數字其中有一個以上並非整數，則會警告並重新開始輸入，
    當輸入皆無誤時，程式將開始排序並將排序的結果輸出。
*/
import java.util.Scanner;
import java.io.*;
import java.lang.Integer;
import java.lang.NumberFormatException;

public class H2_110616034
{
    public static void main(String args[])
    {
        // 主程式迴圈，使程式能執行一次以上，直到強制終止
        while (true)
        {
            Scanner sc = new Scanner(System.in);
            int a[], min;

            System.out.print("Elements in array a are: ");
            /*
            偵測輸入是否無法轉換為整數
            若是，則警告並重新開始輸入
            */
            try
            {
                /*
                將輸入字串 "分隔" 成字串陣列，並 "轉換" 成整數陣列
                作法源自以下網址：
                    https://www.javaworld.com.tw/jute/post/view?bid=29&id=289910
                    作者：XI
                */
                String input = sc.nextLine();
                String strArray[] = input.split(" ");
                a = new int[strArray.length];

                // 將字串陣列轉換成整數陣列
                for (int i = 0; i < a.length; i++)
                {
                    a[i] = Integer.parseInt(strArray[i]);
                }
            }
            catch (NumberFormatException e)
            {
                System.out.println("Error: Can't input other type except integer.\n");
                // 重新開始輸入欲排序陣列
                continue;
            }

            /*
            用以進行排序的雙層迴圈
            */
            // 用以移動該次迴圈的最小值位置
            for (int i = 0; i < a.length - 1; i++)
            {
                min = i;
                // 用以移動比較位置，比較在最小值位置右方的所有值是否比最小值更小
                for (int j = i + 1; j < a.length; j++)
                {
                    // 若在陣列右方有比最小值更小的值，則將該索引指派給 min 變數
                    if (a[j] < a[min])
                    {
                        min = j;
                    }
                }

                int temp = a[i];
                a[i] = a[min];
                a[min] = temp;
            }

            // 列印出排序完成的陣列
            for (int i = 0; i < a.length; i++)
            {
                System.out.print(a[i] + " ");
            }
            System.out.print("\n");
        }
    }
}
