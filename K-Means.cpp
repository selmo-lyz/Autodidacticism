#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>

using namespace std;

class DataPoint {
    /*
     * 用以儲存各項資料的 property 與所屬的 cluster
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
    vector<double> prop;
    int no_cluster;
};

class KMeans {
    /*
     * 用以執行 K-Means 演算法相關操作的類別
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
         * 重複執行演算法，直到資料的 cluster 完全未被更新
         */
        init_means();

        counter = 0;
        changed = true;
        while (changed) {
            changed = false;

            classification();
            update_means();

            counter++;
            output(to_string(counter));
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
         name += ".dat";
        ofstream fout(name.c_str());

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
            fout << endl;
        }

        fout.close();
    }

private:
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

        int *temp = new int[num_cluster];
        for (int i = 0; i < num_cluster; i++) {
            bool flag;
            // 防止選取到相同資料的機制
            do {
                flag = 0;

                temp[i] = dis(gen);
                for (int j = 0; j < i; j++) {
                    flag |= temp[j] == temp[i];
                }
            } while (flag);

            // 可考慮使用 vector 的 assign(vector, end)
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
         * 計算資料與 k 個平均值的差距，並將其歸類在距最小值的那類
         */
        for (int i = 0; i < num_data; i++) {
            double min = -1;
            int next_cluster = 0;

            for (int j = 0; j < num_cluster; j++) {
                double sum = 0;

                for (int k = 0; k < num_prop; k++) {
                    sum += (mean[j][k] - data[i].getProp(k)) *
                        (mean[j][k] - data[i].getProp(k));
                }

                if (min > sum || min < 0) {
                    min = sum;
                    next_cluster = j;
                }
            }

            if (data[i].getCluster() != next_cluster) {
                changed = 1;
                data[i].setCluster(next_cluster);
            }

            //Degug
            cout << "資料 " << i << " 屬於第 "
                 << data[i].getCluster() << " 類\n";
        }

        cout << "更新 cluster 完成\n\n";
    }

    void update_means(void) {
        /*
         * 計算重新歸類後每類的加總平均，並以此更新該類的平均值
         */
        vector<double> row_sum;
        row_sum.assign(2, 0);
        vector<vector<double>> sum;
        sum.assign(num_cluster, row_sum);

        for (int j = 0; j < num_prop; j++) {
            for (int k = 0; k < num_cluster; k++) {
                sum[k][0] = 0;
                sum[k][1] = 0;
            }
            cout << "暫存陣列初始化\n";

            for (int i = 0; i < num_data; i++) {
                sum[data[i].getCluster()][0] += data[i].getProp(j);
                sum[data[i].getCluster()][1]++;
            }
            cout << "取得屬性加總\n";

            for (int i = 0; i < num_cluster; i++) {
                mean[i][j] = sum[i][0] / sum[i][1];
            }

            // Debug
            for (int i = 0; i < num_cluster; i++) {
                cout << "mean[" << i << "]: ";
                for (int l = 0; l < num_prop; l++) {
                    cout << mean[i][l] << ", ";
                }
                cout << endl;
            }
        }

        cout << "更新 mean 完成\n\n";
    }
};

int main(void) {
    /*
     * 讀取檔案資料
     */
    double **data;
    data = new double*[10];
    for (int i = 0; i < 10; i++) {
        data[i] = new double[2];
    }

    ifstream file("data.csv");

    for (int i = 0; i < 10; i++) {
        cout << "資料 " << i << " 屬性: ";  // for Debug
        string str;
        for (int j = 0; j < 2 - 1; j++) {
            getline(file, str, ',');
            data[i][j] = atof(str.c_str());
            cout << data[i][j] << ", ";  // for Debug
        }
        getline(file, str, '\n');
        data[i][1] = atof(str.c_str());
        cout << data[i][1] << endl;  // for Debug
    }

    cout << "初始化 data 完成\n\n";

    /*
     * 執行 K-Means
     */
    KMeans km(data, 10, 2, 3);
    km.run();
    km.show();
}
