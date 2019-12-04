#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <cmath>

using namespace std;

class Unit {
  public:
    Unit(int num = 1, double min = -1, double max = 1) {
        /*
         * num: Weight 的數量
         * min: weight 隨機值的最小值
         * max: weight 隨機值的最大值
         */
        if (num > 0) {
            output = 0;
            weight.assign(num, 0.0);
            delta_w.assign(num, 0.0);

            // 取隨機數的工具
            random_device rd;
            default_random_engine generator(rd());
            uniform_real_distribution<double> unif(min, max);

            // 以隨機值初始化 weight
            for (int i = 0; i < num; i++) {
                weight[i] = unif(generator);
            }
        } else {
            cerr << "number must greater than 0\n";
        }
    }

    vector<double> get_delta_w(void) {
        return delta_w;
    }

    vector<double> get_weight(void) {
        return weight;
    }

    double get_output(void) {
        return output;
    }

    void set_weight(vector<double> vec) {
        if (vec.size() == weight.size()) {
            weight = vec;
        }
    }

    void set_delta_w(vector<double> vec) {
        if (vec.size() == delta_w.size()) {
            delta_w = vec;
        }
    }

    void set_output(double val) {
        output = val;
    }

  private:
    vector<double> weight, delta_w;
    double output;
};

class Topology2D {
  public:
    Topology2D(int row, int col, int num_weight, double min = -1, double max = 1) {
        /*
         * row: 二維網路拓撲的列數
         * col: 二維網路拓撲的行數
         * num_weight: 每個單元的 weight 數量
         * min: weight 隨機值的最小值
         * max: weight 隨機值的最大值
         */
        if ((row > 0 && col > 0) && (num_weight > 0) && (min < max)) {
            // 設定網路拓撲大小
            vector<Unit> temp(col, Unit());
            topo.assign(row, temp);
            // 初始化單元中 weight 的數量與其值
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    temp[j] = Unit(num_weight, min, max);
                }
                topo[i] = temp;
            }
        }
    }

    void train(vector<vector<double>> x_train,
               vector<double> y_train,
               int epochs,
               double radius,
               double r_rate,
               double learning_rate = 1.0,
               double lr_rate = 0.5,
               bool converge = false
              ) {
        /*
         * x_train: training_data
         * epochs: 最多執行次數
         * radius: 鄰近半徑
         * r_rate: 鄰近半徑縮小率
         * learning_rate: 學習速率
         * lr_rate: 學習速率縮小率
         */
        bool keep = true;
        for (int time = 0; time < epochs && keep; time++) {
            // 暫存當前 epoch 中各單元的 weight 值
            vector<vector<double>> t1(topo[0].size(), topo[0][0].get_weight());
            vector<vector<vector<double>>> pre_weight(topo.size(), t1);
            for (int row = 0; row < topo.size(); row++) {
                pre_weight.reserve(pre_weight.size() + 1);
                pre_weight[row].reserve(topo[row].size());

                for (int col = 0; col < topo[row].size(); col++) {
                    pre_weight[row][col] = topo[row][col].get_weight();
                }
            }

            for (int index_data = 0; index_data < x_train.size(); index_data++) {
                // 找出優勝單元 topo(i,j)
                int winner[2] = {0, 0};    // 優勝單元拓撲座標
                double min = 99999;            // 當前與 x_trian(i,j) 鄰近距離最小值
                for (int row = 0; row < topo.size(); row++) {
                    for (int col = 0; col < topo[row].size(); col++) {
                        // 初始化當前單元輸出值爲 0.0
                        topo[row][col].set_output(0.0);
                        vector<double> wt = topo[row][col].get_weight();

                        // 計算鄰近距離
                        double temp = 0;
                        for (int i = 0; i < x_train[index_data].size(); i++) {
                            temp += pow(x_train[index_data][i] - wt[i], 2);
                        }

                        // 若較當前最小值小，則更新優勝單元座標與當前最小值
                        if (temp < min) {
                            winner[0] = row;
                            winner[1] = col;
                            min = temp;
                        }
                    }
                }
                //cout << "Winner is (" << winner[1] << "," << winner[0] << ") - Unit "
                    //<< winner[0] * topo.size() + winner[1] + 1 << endl;

                // 令優勝單元輸出爲 1
                topo[winner[0]][winner[1]].set_output(1.0);

                // TODO: 若 delta_w 不需要，則刪除其相關程式
                // 計算修正量 delta W 後，更新 Weight
                for (int row = 0; row < topo.size(); row++) {
                    for (int col = 0; col < topo[row].size(); col++) {
                        vector<double> wt = topo[row][col].get_weight();
                        vector<double> delta_wt = topo[row][col].get_delta_w();

                        for (int i = 0; i < delta_wt.size(); i++) {
                            // 計算修正量後
                            delta_wt[i] = learning_rate * 
                                (x_train[index_data][i] - wt[i]) * 
                                neightbor_func(
                                    pow(col-winner[1],2)+pow(row-winner[0],2),
                                    radius
                                );
                            // 更新 weight
                            wt[i] += delta_wt[i];
                        }

                        // 更新修正量 delta W
                        topo[row][col].set_delta_w(delta_wt);
                        // 更新 Weight
                        topo[row][col].set_weight(wt);
                    }
                }
            }

            // 縮小學習速率與鄰近半徑
            learning_rate = learning_rate * lr_rate;
            radius = radius * r_rate;

            // 若不想使用收斂作爲結束判定，則繼續下一輪
            if (!converge) continue;

            // 計算各單元經歷一個 epoch 後 weight 的誤差
            keep = false;
            for (int row = 0; row < topo.size(); row++) {
                for (int col = 0; col < topo[row].size(); col++) {
                    double error = 0;
                    vector<double> wt = topo[row][col].get_weight();

                    for (int i = 0; i < wt.size(); i++) {
                        error += sqrt(pow(wt[i] - pre_weight[row][col][i], 2));
                    }

                    // 判斷各單元 weight 的誤差是否過大
                    if (error > 0.0000000001) {
                        keep = true;
                    }
                }
            }
        }

        set_region(x_train, y_train);
        cout << "Done!\n\n";
    }

    double test(vector<vector<double>> x_train, vector<double> y_train) {
        double acc = 0.0;
        ofstream fout("output_test_result.csv");

        for (int index_data = 0; index_data < x_train.size(); index_data++) {
            // 找出優勝單元 topo(i,j)
            int winner[2] = {0, 0};    // 優勝單元拓撲座標
            double min = 99999;            // 當前與 x_trian(i,j) 鄰近距離最小值
            for (int row = 0; row < topo.size(); row++) {
                for (int col = 0; col < topo[row].size(); col++) {
                    vector<double> wt = topo[row][col].get_weight();

                    // 計算鄰近距離
                    double temp = 0;
                    for (int i = 0; i < x_train[index_data].size(); i++) {
                        temp += pow(x_train[index_data][i] - wt[i], 2);
                    }

                    // 若較當前最小值小，則更新優勝單元座標與當前最小值
                    if (temp < min) {
                        winner[0] = row;
                        winner[1] = col;
                        min = temp;
                    }
                }
            }

            //cout << "No." << index_data + 1
                 //<<" winner is (" << winner[1] << "," << winner[0] << ") - Unit "
                 //<< winner[0] * topo.size() + winner[1] + 1 << endl;

            //cout << topo[winner[0]][winner[1]].get_output() << ":" << y_train[index_data] << endl;
            if (topo[winner[0]][winner[1]].get_output() == y_train[index_data]) {
                acc++;
            } else {
                if (y_train[index_data] == 0.0) {
                    cout << topo[winner[0]][winner[1]].get_output() << " : Class 0\n";
                } else {
                    cout << topo[winner[0]][winner[1]].get_output() << " : Class " << y_train[index_data] << endl;
                }
            }
            
            // 輸出原二維座標，並輸出映射到的單元
            //char cls = 65;
            //cls += winner[0] * topo.size() + winner[1];
            //fout << x_train[index_data][1] << ',' << x_train[index_data][0] << ','
                 //<< cls << endl;
            
            // 輸出映射後的座標
            fout << winner[1] << ',' << winner[0] << endl;
        }

        fout.close();
        cout << "Accuracy: " << acc/x_train.size() * 100 << '%' << endl;
    }

    void output_weight(void) {
        ofstream fout("output_weight.csv");
        for (int row = 0; row < topo.size(); row++) {
            for (int col = 0; col < topo[row].size(); col++) {
                vector<double> wt = topo[row][col].get_weight();
                for (int i = 0; i < wt.size() - 1; i++) {
                    fout << wt[i] << ',';
                }
                fout << wt[wt.size() - 1] << endl;
            }
        }
        fout.close();
    }

    void input_weight(void) {
        ifstream fin("output_weight.csv");
        for (int row = 0; row < topo.size(); row++) {
            for (int col = 0; col < topo[row].size(); col++) {
                string str = "";
                vector<double> wt = topo[row][col].get_weight();
                for (int i = 0; i < wt.size() - 1; i++) {
                    getline(fin, str, ',');
                    wt[i] = atof(str.c_str());
                    cout << wt[i] << ", ";  // for Debug
                }
                getline(fin, str, '\n');
                wt[wt.size() - 1] = atof(str.c_str());
                cout << wt[wt.size() - 1] << endl;  // for Debug

                topo[row][col].set_weight(wt);
            }
        }
        fin.close();
    }

    void print(void) {
        // Debug：印出各單元的 weight 值
        cout << "print:\n";
        for (int i = 0; i < topo.size(); i++) {
            for (int j = 0; j < topo[i].size(); j++) {
                vector<double> w = topo[i][j].get_weight();

                for (int k = 0; k < w.size(); k++) {
                    cout << w[k] << ", ";
                }
                cout << endl;
            }
        }
    }

    void output_region(void) {
        ofstream fout("output_result_region.csv");
        for (int row = 0; row < topo.size(); row++) {
            for (int col = 0; col < topo[row].size(); col++) {
                fout << col << "," << row << ",Class" << topo[row][col].get_output() << endl;
            }
        }
        fout.close();
    }

    void set_region(vector<vector<double>> x_train, vector<double> y_train) {
        // 初始化單元輸出
        for (int row = 0; row < topo.size(); row++) {
            for (int col = 0; col < topo[row].size(); col++) {
                // 初始化當前單元輸出值爲 0.0
                topo[row][col].set_output(0.0);
            }
        }

        for (int index_data = 0; index_data < x_train.size(); index_data++) {
            // 找出優勝單元 topo(i,j)
            int winner[2] = {0, 0};    // 優勝單元拓撲座標
            double min = 99999;        // 當前與 x_trian(i,j) 鄰近距離最小值
            for (int row = 0; row < topo.size(); row++) {
                for (int col = 0; col < topo[row].size(); col++) {
                    vector<double> wt = topo[row][col].get_weight();

                    // 計算鄰近距離
                    double temp = 0;
                    for (int i = 0; i < x_train[index_data].size(); i++) {
                        temp += pow(x_train[index_data][i] - wt[i], 2);
                    }

                    // 若較當前最小值小，則更新優勝單元座標與當前最小值
                    if (temp < min) {
                        winner[0] = row;
                        winner[1] = col;
                        min = temp;
                    }
                }
            }
            
            topo[winner[0]][winner[1]].set_output(y_train[index_data]);
        }
    }

  private:
    vector<vector<Unit>> topo;

    //inline double vector_dis(Unit winner, Unit unit) {
        //// distance 的平方 = 兩向量相減後的 norm 平方
        //double temp = 0;
        //vector<double> w = winner.get_weight(), u = unit.get_weight();
        //for (int i = 0; i < w.size(); i++) {
            //temp += pow(w[i] - u[i], 2);
        //}
        //return temp;
    //}

    inline double neightbor_func(double dis, double radius) {
        // 以斗笠帽函數爲鄰近函數
        return exp(- dis / radius);
    }
};

void load_data_2D(
    ifstream& file,
    vector<vector<double>>& data,
    vector<double>& label,
    int c,
    int r = 1
    ) {
    string str;

    for (int i = 0; i < r; i++) {
        cout << "資料 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < c; j++) {
            getline(file, str, ',');
            data[i][j] = atof(str.c_str());
            cout << data[i][j] << ", ";  // for Debug
        }
        cout << endl;    // for Debug

        cout << "標籤 " << i << " 屬性: ";  // for Debug
        getline(file, str, '\n');
        label[i] = atof(str.c_str());
        cout << label[i]<< endl;  // for Debug
    }
}

int main(int argc, char *argv[]) {
    /*
     * 指令：./執行檔名 1. 2. 3. 4.
     * 參數說明：
     * 1. 放在 training_data 底下的 training data 檔案名，需有副檔名 csv
     * 2. training data 行數
     * 3. training data 屬性數
     * 附註：
     * 需改寫 testing data 行數變數
     */

    /*
     * row: 資料筆數
     * col: 資料屬性數量 (including target label)
     */
    int row = atoi(argv[2]), col = atoi(argv[3]);

    /*
     * 讀取訓練資料
     */
    vector<vector<double>> training_data;
    vector<double> temp, label(row, 0.0);
    temp.assign(col, 0.0);
    training_data.assign(row, temp);

    string training_data_name = "./training_data/";
    training_data_name.append(argv[1]);
    ifstream file(training_data_name);

    load_data_2D(file, training_data, label, col, row);

    file.close();
    cout << "初始化 data 完成\n\n";

    /*
     * Training
     */
    /*
     * row: 二維網路拓撲的列數
     * col: 二維網路拓撲的行數
     * num_weight: 每個單元的 weight 數量
     * min: weight 隨機值的最小值
     * max: weight 隨機值的最大值
     */
    Topology2D topo2D(9, 9, col, -1, 1);
    /*
     * x_train: training_data
     * epochs: 最多執行次數
     * radius: 鄰近半徑
     * r_rate: 鄰近半徑縮小率
     * learning_rate: 學習速率
     * lr_rate: 學習速率縮小率
     * converge: 使訓練到收斂爲止
     */
    topo2D.train(training_data, label, 1000, 2, 0.8, 1, 0.5, false);

    topo2D.output_weight();
    topo2D.output_region();

    /*
     * 讀取測試資料
     */
    int num_testing_data = 30;
    vector<vector<double>> testing_data;
    temp.assign(col, 0.0);
    testing_data.assign(num_testing_data, temp);

    string testing_data_name = "./testing_data/";
    testing_data_name.append(argv[1]);
    file.open(testing_data_name);

    load_data_2D(file, testing_data, label,  col, num_testing_data);

    file.close();
    cout << "初始化 data 完成\n\n";

    /*
     * Testing
     */
    topo2D.test(testing_data, label);
}