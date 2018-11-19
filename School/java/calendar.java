/*
�ާ@�����G
    ��J���d�ߪ��~, ��, ��A�Y�i��X�ӤѬP���X�P�Ӧ~�Ӥ몺���]�]�t�P���^
    ���G�ӵ{����ܪ��O"�^����k"
�ѦҸ�ơG
    ������⤽���G
    http://dhcp.tcgs.tc.edu.tw/c/p015.htm
    ��k�ﭲ�G
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
        System.out.println("���G�ӵ{���ϥΪ��O�^����k");
        System.out.print("�п�J���d�ߪ��~���]�榡�GY M D�^�G");
        y = sc.nextInt();
        m = sc.nextInt();
        d = sc.nextInt();

        calender(y, m, d);
    }

    public static void calender(int year, int month, int date)
    {
        Formatter fm = new Formatter(System.out);
        // �� 1752/09 �ק�~�t�ɩҥΪ����
        String[] cal175209 =
        {
            "1", "2", "14", "15", "16", "17", "18", "19", "20",
            "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"
        };
        String[] day = {"��", "�@", "�G", "�T", "�|", "��", "��"};
        int oldW[] = {5, 1, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3};
        int newW[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
        int w, numDay, today;

        // �P�_�Ӥ릳�X��
        if (month == 4 || month == 6 ||  month == 9 || month == 11)
            numDay = 30;
        else if (month == 2)
            numDay = 28;
        else
            numDay = 31;

        // �P�_��J���~���O�_�����T
        if (year < 1 || month < 1 || month > 12 || date < 1 || date > 31 || numDay == 30 && date == 31 || month == 2 && date > 28)
        {
            System.out.println("!ERROR: �п�J���T���~���");
            return;
        }

        // �P�_��J�� year �O�_ >= 1753�A�Ǧ��M�Τ��P�������k�]�s/�¶|�~�W�h�^
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

        // �� today �N�� "��骺�P�� - 1"
        today = (w + date) % 7;

        /*
        ��X�Ӧ~�Ӥ몺���P�Ӥ�P���X
        */
        System.out.println("\n�� �@ �G �T �| �� ��");

        // �P�_��J�~��O�_�� 1752/09�A�Y�O�h��X�M�Τ��
        if (year == 1752 && month == 9)
        {
            // ��X�M�Τ��
            System.out.print("      ");
            for (int i = 0; i < 19; i++)
            {
                fm.format("%-3s", cal175209[i]);

                // �P�_�O�_��X�ܬP�����A�Y�O�h����
                if (((i + 2) % 7) == 6)
                    System.out.print("\n");

                // �P�_��J����O�_����Ӥ���A���Ǧ��P�_�P���X
                if (date == Integer.parseInt(cal175209[i]))
                    today = (i + 2) % 7;
            }

            // �P�_��J����O�_���s�b
            if (date >= 3 && date <= 13)
                System.out.println("\n\n!ERROR: ���s�b�����");
            else
                System.out.println(
                    "\n\n" + year + " �~ " + month + " �� " + date +
                    " �� ���P��" + day[today]
                );
        }
        else
        {
            // ��X�@���e���Ů�
            for (int i = 0; i <= w && w < 6; i++)
                System.out.print("   ");

            // �� w �N��Ӥ�Ĥ@�Ѫ��P�� - 1
            ++w;
            // �}�l��X���
            for (int i = 1; i <= numDay; i++, w++)
            {
                fm.format("%-3s", i);

                // �P�_�O�_��X�ܬP�����A�Y�O�h����
                if ((w % 7) == 6)
                    System.out.print("\n");
            }

            System.out.println(
                "\n\n" + year + " �~ " + month + " �� " + date + " �� ���P��" +
                day[today]
            );
        }
    }
}
