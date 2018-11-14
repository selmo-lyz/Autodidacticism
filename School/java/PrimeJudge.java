/*
操作說明：
    程式啟動後，
    見到 "請輸入數字 (大於 0 的整數): " 即可輸入要進行判斷的數字，
    若該數字是質數則顯示 "是"，反之則顯示 "否"，
    若輸入<=零的數字則會顯示錯誤訊息 "!ERROR: 請輸入大於零的整數"，
    除了錯誤訊息外，之後皆會顯示 "該數字的因式分解"。
*/
import java.util.Scanner;

public class H1_110616034
{
    public static void main(String args[])
    {
        Scanner sc = new Scanner(System.in);
        int input;
        
        while (true)
        { // 主程式迴圈，使程式能執行一次以上，直到強制終止
            System.out.println("\n此程式會判斷輸入的數字（大於零的整數）是否為質數。");
            System.out.print("請輸入數字（大於 0 的整數): ");
            input = sc.nextInt();

            int result = PrimeTest(input);
            /*
            判斷質數檢驗的結果，
            1 代表 質數
            -1 代表 輸入 <= 0 的數字
            0 代表 非質數
            */
            if (result == 1)
                System.out.println("是");
            else if (result == -1)
                System.out.println("!ERROR: 請輸入大於零的整數");
            else
                System.out.println("否");

            // 判斷輸入是否符合規定，若符合則輸出其因式分解
            if (result != -1)
                factoring(input);
        }
    }

    public static int PrimeTest(int parm)
    {
        /*
        判斷參數是否為質數，若參數為
        質數 : 回傳 1
        合數 : 回傳 0
        == 1 : 回傳 0
        <= 0 的整數 : 回傳 -1
        */
        if (parm <= 0)
        {
            return -1;
        }
        else if (parm == 2)
        {
            return 1;
        }
        else if (parm == 1 || (parm % 2) == 0)
        {
            return 0;
        }
        else if (parm > 0)
        { // 在此過濾除了 "1", "2", "2 的倍數" 的參數
            /*
            尋找參數的因數，範圍為 3 開始到 參數的開方，
            每次間隔 1 個數是為了避免再次檢查 2 的倍數
            */
            for (int i = 3; i <= Math.sqrt(parm); i += 2)
                // 確認 i 是否為參數的因數，若是則回傳 0 代表並非質數
                if ((parm % i) == 0) return 0;

            return 1;
        }
    }

    public static void factoring(int parm)
    {
        int divisor = 1;

        System.out.print(parm + " = " + divisor++);
        /*
        列出參數的部分質因數分解，
        若參數不為 1 且 非質數，則進入迴圈進行質因數分解
        */
        while (parm != 1 && PrimeTest(parm) == 0)
        {
            /*
            若參數可被除數整除，則列出參數的質因數 (即除數)
            */
            while ((parm % divisor) == 0)
            {
                System.out.print(" X " + divisor);
                parm /= divisor;
            }
            divisor++;
        }

        // 列出最後的質因數，因為 1 已在函式最前面顯示，故不必再列出
        if (parm != 1)
            System.out.print(" X " + parm);

        System.out.println();
    }
}
