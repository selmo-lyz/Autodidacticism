/*
操作說明：
    輸入欲查詢的年, 月, 日，即可輸出該天星期幾與該年該月的月曆（包含星期）
    註：該程式顯示的是"英美曆法"
參考資料：
    日期換算公式：
    http://dhcp.tcgs.tc.edu.tw/c/p015.htm
    曆法改革：
    https://people.cs.nctu.edu.tw/~tsaiwn/introcs/03_Labs/Lab08/
*/
import java.util.Scanner;
import java.util.Formatter;
import java.lang.Integer;

public class H4_110616034
{
    public static void main(String args[])
    {
        int y, m , d;
        Scanner sc = new Scanner(System.in);
        System.out.println("註：該程式使用的是英美曆法");
        System.out.print("請輸入欲查詢的年月日（格式：Y M D）：");
        y = sc.nextInt();
        m = sc.nextInt();
        d = sc.nextInt();

        calender(y, m, d);
    }

    public static void calender(int year, int month, int date)
    {
        Formatter fm = new Formatter(System.out);
        // 為 1752/09 修改誤差時所用的月曆
        String[] cal175209 =
        {
            "1", "2", "14", "15", "16", "17", "18", "19", "20",
            "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"
        };
        String[] day = {"日", "一", "二", "三", "四", "五", "六"};
        int oldW[] = {5, 1, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3};
        int newW[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
        int w, numDay, today;

        // 判斷該月有幾天
        if (month == 4 || month == 6 ||  month == 9 || month == 11)
            numDay = 30;
        else if (month == 2)
            numDay = 28;
        else
            numDay = 31;

        // 判斷輸入的年月日是否不正確
        if (year < 1 || month < 1 || month > 12 || date < 1 || date > 31 || numDay == 30 && date == 31 || month == 2 && date > 28)
        {
            System.out.println("!ERROR: 請輸入正確的年月日");
            return;
        }

        // 判斷輸入的 year 是否 >= 1753，藉此套用不同的換算方法（新/舊閏年規則）
        if (year >= 1753)
        {
            w = (newW[month - 1] + (year - 1753) + ((year - 1752) / 4) -
                ((year - 1700) / 100) + ((year - 1600) / 400) -
                ((year % 400 == 0 || year % 4 == 0 && year % 100 != 0) &&
                (month < 3) ? 1 : 0)) % 7;
        }
        else
        {
            w = (oldW[month - 1] + (year - 1) + (year / 4) -
                ((year % 400 == 0 || year % 4 == 0 && year % 100 != 0) &&
                (month < 3) ? 1 : 0)) % 7;
        }

        // 使 today 代表 "當日的星期 - 1"
        today = (w + date) % 7;

        /*
        輸出該年該月的月曆與該日星期幾
        */
        System.out.println("\n日 一 二 三 四 五 六");

        // 判斷輸入年月是否為 1752/09，若是則輸出專用月曆
        if (year == 1752 && month == 9)
        {
            // 輸出專用月曆
            System.out.print("      ");
            for (int i = 0; i < 19; i++)
            {
                fm.format("%-3s", cal175209[i]);

                // 判斷是否輸出至星期六，若是則換行
                if (((i + 2) % 7) == 6)
                    System.out.print("\n");

                // 判斷輸入日期是否等於該日期，並藉此判斷星期幾
                if (date == Integer.parseInt(cal175209[i]))
                    today = (i + 2) % 7;
            }

            // 判斷輸入日期是否不存在
            if (date >= 3 && date <= 13)
                System.out.println("\n\n!ERROR: 不存在此日期");
            else
                System.out.println(
                    "\n\n" + year + " 年 " + month + " 月 " + date +
                    " 日 為星期" + day[today]
                );
        }
        else
        {
            // 輸出一號前的空格
            for (int i = 0; i <= w && w < 6; i++)
                System.out.print("   ");

            // 使 w 代表該月第一天的星期 - 1
            ++w;
            // 開始輸出日期
            for (int i = 1; i <= numDay; i++, w++)
            {
                fm.format("%-3s", i);

                // 判斷是否輸出至星期六，若是則換行
                if ((w % 7) == 6)
                    System.out.print("\n");
            }

            System.out.println(
                "\n\n" + year + " 年 " + month + " 月 " + date + " 日 為星期" +
                day[today]
            );
        }
    }
}
