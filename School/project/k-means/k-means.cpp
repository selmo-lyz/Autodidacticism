/*
 * 說明：
 *   可透過修改標有 Method 1. 與 Method 2. 區段的程式，
 *   切換判斷是否繼續執行 k-means 的方法。
 */
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>

using namespace std;

class DataPoint {
    /*
     * 儲存各項資料的 property 與所屬的 cluster
     */
public:
    void reserveProp(int size) {
        prop.reserve(size + 1);
    }

    double getProp(int index) {
        return prop[index];
    }

    int getCluster(void) {
        return no_cluster;
    }

    void setProp(double val) {
        prop.push_back(val);
    }

    void setCluster(int no) {
        no_cluster = no;
    }

private:
    /*
     * prop: 資料屬性
     * no_cluster: 資料所屬群集
     */
    vector<double> prop;
    int no_cluster;
};

class KMeans {
    /*
     * 執行 K-Means 演算法相關操作的類別
     */
public:
    KMeans(double **data, int num_data, int num_prop, int num_cluster) {
        this->num_data = num_data;
        this->num_prop = num_prop;
        this->num_cluster = num_cluster;

        this->data.reserve(num_data + 1);
        for (int i = 0; i < num_data; i++) {
            this->data[i].reserveProp(num_prop);
            for (int j = 0; j < num_prop; j++) {
                this->data[i].setProp(data[i][j]);
            }
        }
    }

    void run(void) {
        /*
         * 執行 k-means
         */
        init_means();

        counter = 0;
        changed = true;
        while (changed) {
            changed = false;

            classification();
            update_means();

            counter++;
            //output(to_string(counter));  // 輸出過程中的資訊
        }
    }

    void show(void) {
        /*
         * 顯示結果
         */
        for (int i = 0; i < num_data; i++) {
            cout << "資料 " << i << " 屬於第 "
                 << data[i].getCluster() << " 類\n";
        }

        cout << "共 " << counter << " 輪\n";
    }

    void output(string name) {
        /*
         * 將各 data 的座標、其所屬的 cluster 與當時的各點 mean 輸出成檔案
         */
         name += ".csv";
        ofstream fout(name.c_str());

        for (int i = 0; i < num_prop; i++) {
            fout << "prop" << i << ',';
        }
        fout << "cluster" << endl;


        for (int i = 0; i < num_data; i++) {
            for (int j = 0; j < num_prop; j++) {
                fout << data[i].getProp(j) << ',';
            }
            fout << data[i].getCluster() << endl;
        }

        for (int i = 0; i < num_cluster; i++) {
            for (int j = 0; j < num_prop; j++) {
                fout << mean[i][j] << ',';
            }
            fout << i << endl;
        }

        fout.close();
    }

    void output_without_mean(string name) {
        /*
         * 將各 data 的座標、其所屬的 cluster 輸出成檔案
         */
         name += ".csv";
        ofstream fout(name.c_str());

        for (int i = 0; i < num_prop; i++) {
            fout << "prop" << i << ',';
        }
        fout << "cluster" << endl;

        for (int i = 0; i < num_data; i++) {
            for (int j = 0; j < num_prop; j++) {
                fout << data[i].getProp(j) << ',';
            }
            fout << data[i].getCluster() << endl;
        }

        fout.close();
    }

private:
    /*
     * changed: 判斷是否該再次執行 k-means
     * num_data: 資料數量
     * num_prop: 資料屬性數量
     * num_cluster: 群集數量
     * counter: k-means 執行輪數
     * data: 資料
     * mean: 平均值
     */
    bool changed;
    int num_data, num_prop, num_cluster, counter;
    vector<DataPoint> data;
    vector<vector<double>> mean;

    void init_means(void) {
        /*
         * 從資料中隨機取 k 個來初始化 k 個平均數
         */
        mean.reserve(num_cluster + 1);
        for (int i = 0; i < num_cluster; i++) {
            mean[i].reserve(num_prop + 1);
        }

        // 取隨機數的工具
        random_device rd;
        default_random_engine gen = default_random_engine(rd());
        uniform_int_distribution<int> dis(0, num_data - 1);

        // temp: 已最為初始平均值的資料
        int *temp = new int[num_cluster];
        for (int i = 0; i < num_cluster; i++) {
            // 防止選取到相同資料的機制
            bool flag;
            do {
                flag = false;

                temp[i] = dis(gen);
                for (int j = 0; j < i; j++) {
                    flag |= temp[j] == temp[i];
                }
            } while (flag);

            for (int j = 0; j < num_prop; j++) {
                mean[i].push_back(data[temp[i]].getProp(j));
            }

            // 輸出選取結果
            cout << "選用第 " << temp[i] << " 項資料\n";
            cout << "mean[" << i << "]: ";
            for (int j = 0; j < num_prop; j++) {
                cout << mean[i][j] << ", ";
            }
            cout << endl;
        }
        delete temp;

        cout << "初始化 mean 完成\n\n";
    }

    void classification(void) {
        /*
         * 計算資料與 k 個平均值的差距，並將其歸類在差距最小值的群集
         */

        for (int i = 0; i < num_data; i++) {
            /*
             * min: 資料與平均值當前的最小平方和
             * next_cluster: 更新後資料的群集
             */
            double min = -1;
            int next_cluster = 0;

            for (int j = 0; j < num_cluster; j++) {
                // sum: 資料 i 與平均值 j 的平方和
                double sum = 0;

                for (int k = 0; k < num_prop; k++) {
                    sum += (mean[j][k] - data[i].getProp(k)) *
                           (mean[j][k] - data[i].getProp(k));
                }

                // 判斷最小平方和是否大於當前，若是則更新最小值、所屬群集
                if (min > sum || min < 0) {
                    min = sum;
                    next_cluster = j;
                }
            }

            /*
             * Method 1.
             * 以所屬群集是否改變，判斷是否該繼續執行 k-means
             */
            if (data[i].getCluster() != next_cluster) {
                changed = 1;
                data[i].setCluster(next_cluster);
            }

            // Degug: 顯示更新後的 cluster
            /*
            cout << "資料 " << i << " 屬於第 "
                 << data[i].getCluster() << " 類\n";
            */
        }

        cout << "更新 cluster 完成\n\n";
    }

    void update_means(void) {
        /*
         * 計算重新歸類後各群集資料的加總平均，並以此更新各群集的平均值
         */

        /*
         * sum: 儲存各群集單一屬性的加總與資料個數
         * row_sum: vector sum 裡的元素
         */
        vector<vector<double>> sum;
        vector<double> row_sum;
        row_sum.assign(2, 0);
        sum.assign(num_cluster, row_sum);

        // error: 儲存各群集平均值更新前後單一屬性的平方差
        vector<double> error;
        error.assign(num_cluster, 0);
        for (int j = 0; j < num_prop; j++) {
            for (int k = 0; k < num_cluster; k++) {
                sum[k][0] = 0;
                sum[k][1] = 0;
                error[k] = 0;
            }
            cout << "暫存陣列初始化\n";

            for (int i = 0; i < num_data; i++) {
                sum[data[i].getCluster()][0] += data[i].getProp(j);
                sum[data[i].getCluster()][1]++;
            }
            cout << "取得各群集的第" << j << "個屬性加總\n";

            // 更新平均值
            for (int i = 0; i < num_cluster; i++) {
                // 防止 x / 0 = +/-nan
                cout << "sum[" << i << "][1] = " << sum[i][1] << endl;
                if (sum[i][1] > 0) {
                    double temp = sum[i][0] / sum[i][1];

                    error[i] += (mean[i][j] - temp) * (mean[i][j] - temp);
                    cout << "error: " << error[i] << endl;
                    
                    mean[i][j] = temp;
                }
            }

            /*
             * Method 2.
             * 以是否有點變動過大，判斷是否繼續執行 k-means
             */
            /*
            for (int i = 0; i < num_cluster; i++) {
                if (error[i] > 0.01) {
                    changed = true;
                }
            }
            */

            // Debug: 顯示更新後的平均值
            for (int i = 0; i < num_cluster; i++) {
                cout << "mean[" << i << "]: ";
                for (int l = 0; l < num_prop; l++) {
                    cout << mean[i][l] << ", ";
                }
                cout << endl;
            }
        }

        cout << "更新 means 完成\n\n";
    }
};

int main() {
    /*
     * row: 資料筆數
     * col: 資料屬性數量
     * k: 群集數量
     */
    int row = 110, col = 2, k = 10;

    /*
     * 讀取檔案資料
     */
    double **data;
    data = new double*[row];
    for (int i = 0; i < row; i++) {
        data[i] = new double[col];
    }

    ifstream file("data.csv");

    string str;
    getline(file, str, '\n');
    for (int i = 0; i < row; i++) {
        //cout << "資料 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < col - 1; j++) {
            getline(file, str, ',');
            data[i][j] = atof(str.c_str());
            //cout << data[i][j] << ", ";  // for Debug
        }
        getline(file, str, '\n');
        data[i][col - 1] = atof(str.c_str());
        //cout << data[i][col - 1] << endl;  // for Debug
    }

    cout << "初始化 data 完成\n\n";

    /*
     * 執行 K-Means
     */
    KMeans km(data, row, col, k);
    km.run();
    km.show();
    //km.output_without_mean("result");
}
