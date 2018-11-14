/*
操作說明：
    當出現 "請輸入四位數字（四位數皆不相同）: " 時，即代表可以開始猜測，
    輸入的數字必須為四個不相同的 0 ~ 9 整數，否則會顯示錯誤訊息，
    若輸入數字符合規定，則會與答案進行比對，當比對完成後，會顯示該次結果，如
        XAXB, X:0~4的數目 A:猜對數值及位置 B:猜對數值但位置有錯
    若結果為 4A （即正確答案）或猜測次數至 10 次，則會顯示答案與結尾訊息

    額外功能：可輸入 0000 使用僅有一次的提示，來顯示其中一位數。
*/
import java.util.Scanner;

public class H3_110616034
{
    public static void main(String args[])
    {
        int t = 1,
             ab[] = {0, 0},
             answer[] = {0, 0, 0, 0},
             input[] = {0, 0, 0, 0};
        boolean hintUsed[] = {false};
        Scanner sc = new Scanner(System.in);
        String str;

        // 產生題目
        generQ(answer);

        System.out.println("*****猜數字遊戲*****");
        System.out.println("!注意：可輸入 0000 使用僅有一次的提示\n");
        // 限制猜測次數為 10 次
        while (t <= 10)
        {
            System.out.print("請輸入四位數字（四位數皆不相同）: ");
            str = sc.nextLine();
            // 檢查輸入是否符合規定，若否，則重新輸入
            if (!isOK(str, answer, hintUsed)) continue;
            // 將字串轉換為整數陣列
            for (int i = 0; i < 4; i++)
                input[i] = str.charAt(i) - 48;

            check(answer, input, ab);
            // 若 A == 4 (代表猜測正確)，則跳出猜測迴圈
            if (ab[0] == 4) break;

            t++;
        }

        System.out.print("\nThe answer is ");
        // 印出解答
        for (int i = 0; i < 4; i++)
            System.out.print(answer[i]);

        // 依照猜測正確與否印出相對訊息
        if (ab[0] == 4)
            System.out.println("\nBingo!!!，Thanks for playing!");
        else
            System.out.println("\nYou Lose~，Thanks for playing!");
    }

    public static void generQ(int answer[])
    {
        // 隨機產生四位數皆不同的數字
        // 移動將要產生的位數
        for (int i = 0; i < 4; i++)
        {
            answer[i] = (int)(Math.random() * 10);
            // 檢查產生的位數在前面是否出現過
            for (int j = i-1; j >= 0; j--)
            {
                // 若產生的位數曾出現過，則再次產生並從頭比較
                if (answer[j] == answer[i])
                {
                    answer[i] = (int)(Math.random() * 10);
                    j = i-1;
                }
            }
        }

        System.out.println("\n題目產生完畢，準備開始遊戲...\n");
    }

    public static boolean isOK(String str, int ans[], boolean used[])
    {
        // 檢查輸入是否為四位數，若否，則回傳 false
        if (str.length() != 4)
        {
            System.out.println("!ERROR: 輸入需為四位數\n");
            return false;
        }

        /*
        檢查輸入字串是否存在不為 0 ~ 9 的字元
        計算 0 的數量以用於請求提示
        */
        int counter = 0;
        for (int i = 0; i < 4; i++)
        {
            // 檢查字元範圍是否不在 0 ~ 9
            if (str.charAt(i) < '0' || str.charAt(i) > '9')
            {
                System.out.println("!ERROR: 輸入需由 0 ~ 9 的整數組成\n");
                return false;
            }

            // 檢查字元是否為 0，若是則將 counter + 1
            if (str.charAt(i) == '0') counter++;
        }

        // 檢查是否輸入 4 個 0 且尚未使用提示，若是則提示其中一個位數
        if (counter == 4 && !used[0])
        {
            int r = (int)(Math.random() * 10) % 4;

            System.out.print("Hint: ");
            // 顯示提示
            for (int i = 0; i < 4; i++)
            {
                // 檢查是否為需提示字元，若是則提示，若否則輸出 '*'
                if (i == r)
                    System.out.print(ans[i]);
                else
                    System.out.print('*');
            }
            System.out.println();

            used[0] = true;
            return false;
        }

        // 檢查輸入是否為四位數皆不同，若否，則將 isAllDiff 改為 false
        boolean isAllDiff = true;
        // 移動被檢查的位數
        for (int i = 0; i < 3; i++)
        {
            // 移動用來檢查的位數
            for (int j = i+1; j < 4; j++)
            {
                // 當四位數中有兩位數相同，則將 isAllDiff 改為 false
                if (str.charAt(i) == str.charAt(j))
                    isAllDiff = false;
            }
        }

        // 檢查四位數是否皆不同的flag是否為true，若否，則回傳 false
        if (!isAllDiff)
        {
            System.out.println("!ERROR: 輸入需四位數皆為不同\n");
            return false;
        }

        return true;
    }

    public static void check(int answer[], int input[], int ab[])
    {
        ab[0] = 0;
        ab[1] = 0;
        // 移動輸入被比較的位數
        for (int i = 0; i < 4; i++)
        {
            // 移動答案被比較的位數
            for (int j = 0; j < 4; j++)
            {
                // 比較數字是否相同
                if (input[i] == answer[j])
                {
                    // 比較位數是否相同，若是則 A + 1，若否則 B + 1
                    if (i == j) ab[0]++;
                    else ab[1]++;
                }
            }
        }

        System.out.println("該次比較結果為: " + ab[0] + "A" + ab[1] + "B");
    }
}
