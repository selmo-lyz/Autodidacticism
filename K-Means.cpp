/*
 * Name: K-Means
 * 
 * 記錄:
 * - 20190918:
 *   雖然可以成功分類幾次資料，但大多時候會分類錯誤。
 */
#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace std;

int main(void) {
    int num_data, num_prop, num_kind;
    int *kind;
    double **data, **mean;

    /*
     * 讀取檔案資料，初始化資料個數、資料屬性個數、種類個數
     */
    char arr[81] = {"\0"};
    ifstream file("data.csv");

    file.getline(arr, 9);
    string str = arr;

    // 取出被括號分隔的資料
    int cp = str.find(',');
    num_data = atof(str.substr(0, cp).c_str());
    str = str.substr(cp + 1, str.length() - cp);

    cp = str.find(',');
    num_prop = atof(str.substr(0, cp).c_str());
    str = str.substr(cp + 1, str.length() - cp);

    cp = str.find(',');
    num_kind = atof(str.substr(0, cp).c_str());

    cout << "資料個數: " << num_data 
         << ", 資料屬性個數: " << num_prop 
         << ", 種類個數: " << num_kind << endl;

    /*
     * 讀取檔案資料，初始化資料內容
     */
    data = new double*[num_data];
    kind = new int[num_data];
    for (int i = 0; i < num_data; i++) {
        data[i] = new double[num_prop];
        kind[i] = 0;
    }


    for (int i = 0; i < num_data; i++) {
        file.getline(arr, 80);
        str = arr;

        cout << "資料 " << i << " 屬性: ";
        for (int j = 0; j < num_prop; j++) {
            cp = str.find(',');
            data[i][j] = atof(str.substr(0, cp).c_str());
            str = str.substr(cp + 1, str.length() - cp);

            cout << data[i][j] << ", ";
        }
        cout << endl;
    }
    cout << "初始化 data 陣列完成\n\n";

    /*
     * 從資料中隨機取 k 個來初始化 k 個平均數
     */
    mean = new double*[num_kind];
    for (int i = 0; i < num_kind; i++) {
        mean[i] = new double[num_prop];
    }

    // 取隨機數的工具
    random_device rd;
    default_random_engine gen = default_random_engine(rd());
    uniform_int_distribution<int> dis(0, num_data - 1);

    int *temp = new int[num_kind];
    for (int i = 0; i < num_kind; i++) {
        bool flag;
        // 防止選取到相同資料的機制
        do {
            flag = 0;
            temp[i] = dis(gen);
            for (int j = 0; j < i; j++) {
                flag |= temp[j] == temp[i];
            }
        } while (flag);

        mean[i] = data[temp[i]];

        cout << "選用第 " << temp[i] << " 項資料\n";
        cout << "mean[" << i << "]: ";
        for (int j = 0; j < num_prop; j++) {
            cout << mean[i][j] << ", ";
        }
        cout << endl;
    }
    cout << "初始化 mean 陣列完成\n\n";

    /*
     * 若資料完全未被更新則代表已收斂
     */
    int counter = 0;
    bool changed = 0;
    do {
        cout << "開始新一輪\n";

        changed = 0;

        /*
         * 計算資料與 k 個平均值的差距，並將其歸類在距最小值的那類
         */
        for (int i = 0; i < num_data; i++) {
            double sum = 0.0, min = -1.0;
            int next_kind = 0;
            for (int j = 0; j < num_kind; j++) {
                sum = 0.0;
                for (int k = 0; k < num_prop; k++) {
                    sum += (mean[j][k] - data[i][k]) * (mean[j][k] - data[i][k]);
                }

                if (min > sum || min < 0) {
                    min = sum;
                    next_kind = j;
                }

                cout << "資料 " << i << " 與第 " 
                     << j << " 項平均值差為: " << sum <<endl;
            }
            changed |= kind[i] != next_kind;
            kind[i] = next_kind;

            cout << "資料 " << i << " 屬於第 " << next_kind << " 類\n";
        }
        cout << "分類完成\n\n";

        /*
         * 計算重新歸類後，每類的加總平均，並以此更新該類的平均值
         */
        double (*sum)[2] = new double[num_kind][2];
        for (int j = 0; j < num_prop; j++) {
            for (int k = 0; k < num_kind; k++) {
                sum[k][0] = 0;
                sum[k][1] = 0;
            }
            cout << "暫存陣列初始化\n";

            for (int i = 0; i < num_data; i++) {
                sum[kind[i]][1] += data[i][j];
                sum[kind[i]][0]++;
            }
            cout << "取得屬性加總\n";

            for (int i = 0; i < num_kind; i++) {
                mean[i][j] = sum[i][1] / sum[i][0];
            }
        }
        cout << "更新 Mean 陣列完成\n";

        counter++;
    } while (changed);

    /*
     * 顯示結果
     */
    for (int i = 0; i < num_data; i++) {
        cout << "資料 " << i << " 屬於第 " << kind[i] << " 類\n";
    }

    cout << "共 " << counter << " 輪\n";
}
