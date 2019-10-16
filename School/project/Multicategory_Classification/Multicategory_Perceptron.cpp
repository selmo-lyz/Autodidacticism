#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class MulticategoryPerceptron {
public:
    MulticategoryPerceptron(double **input_data, int num_data, int num_prop, int num_cls, double lr = 1) {
        // num_prop 中包含 x_i, 0 <= i < n 加上 class 的數量
        this->num_data = num_data;
        this->num_prop = num_prop;
        this->num_cls = num_cls;
        this->learning_rate = lr;

        // 需加上恆等於 1 的 x_0
        vector<double> prop;
        prop.assign(num_prop + 1, 0);
        this->data.assign(num_data, prop);
        for (int i = 0; i < num_data; i++) {
            // 初始化資料
            for (int j = 0; j < num_prop - 1; j++) {
                this->data[i][j] = input_data[i][j];
            }
            // 令 data 為 [x_(n-1), ..., x_1, x_0 = 1, cls]
            this->data[i][num_prop - 1] = 1;
            this->data[i][num_prop] = input_data[i][num_prop -1];
        }


        this->weight.reserve(num_cls);
        for (int i = 0; i < num_cls; i++) {
            // 需加上 w_0 並減去 cls 的空間
            this->weight[i].reserve(num_prop);
            for (int j = 0; j < num_prop; j++) {
                // 令 weight 為 [w_(n-1), ..., w_1, w_0]
                this->weight[i].push_back(0);
            }
        }
    }

    void run(void) {
        counter = 0;
        changed = true;
        while (changed) {
            changed = false;

            for (int i = 0; i < num_data; i++) {
                vector<double> inner_product;
                inner_product.assign(num_cls, 0);

                // inner product with every weight
                for (int j = 0; j < num_cls; j++) {
                    for (int k = 0; k < num_prop; k++) {
                        inner_product[j] += weight[j][k] * data[i][k];
                    }

                    // Debug
                    //cout << "x[" << num_data*counter + i << "] & w_{" << j
                        //<< "}'s inner_product = " << inner_product[j] << endl;
                }

                // update every weight
                bool mismatch = false;
                for (int j = 0; j < num_cls; j++) {
                    if (j == data[i][num_prop]) continue;

                    // decrease the weight whose inner product greater than
                    // the weight which i th data belongs to
                    if (inner_product[data[i][num_prop]] <= inner_product[j]) {
                        for (int k = 0; k < num_prop; k++) {
                            weight[j][k] -= learning_rate * data[i][k];
                        }

                        mismatch = true;
                        changed = true;
                    }
                }
                // increase the weight which i th data belong to
                // if its inner product less than others
                if (mismatch) {
                    for (int k = 0; k < num_prop; k++) {
                        weight[data[i][num_prop]][k] += learning_rate * data[i][k];
                    }
                }

                // Debug
                //show();
                //cout << endl;
            }

            counter++;
        }
    }

    void show(void) {
        cout << "weight: \n";
        for (int i = 0; i < num_cls; i++) {
            for (int j = 0; j < num_prop - 1; j++) {
                cout << weight[i][j] << ", ";
            }
            cout << weight[i][num_prop - 1] << endl;
        }
    }

    void output_weight(string name) {
        name = name.substr(0, name.length() - 4);
        name += "_result.csv";
        ofstream fout(name.c_str());

        for (int i = 0; i < num_cls; i++) {
            for (int j = 0; j < num_prop - 1; j++) {
                fout << weight[i][j] << ", ";
            }
            fout << weight[i][num_prop - 1] << endl;
        }

        fout.close();
    }

private:
    int num_data, num_prop, num_cls, counter;
    vector<vector<double>> data;
    vector<vector<double>> weight;
    double learning_rate;
    bool changed;
};

int main(int argc, char *argv[]) {
    /*
     * row: 資料筆數
     * col: 資料屬性數量
     */
    int row = atoi(argv[2]), col = atoi(argv[3]), num_cls = atoi(argv[4]);

    /*
     * 讀取檔案資料
     */
    double **data;
    data = new double*[row];
    for (int i = 0; i < row; i++) {
        data[i] = new double[col];
    }

    ifstream file(argv[1]);

    string str;
    for (int i = 0; i < row; i++) {
        cout << "資料 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < col - 1; j++) {
            getline(file, str, ',');
            data[i][j] = atof(str.c_str());
            cout << data[i][j] << ", ";  // for Debug
        }
        getline(file, str, '\n');
        data[i][col - 1] = atof(str.c_str());
        cout << data[i][col - 1] << endl;  // for Debug
    }

    cout << "初始化 data 完成\n\n";

    /*
     * 執行 MulticategoryPerceptron
     */
    MulticategoryPerceptron mp(data, row, col, num_cls, 0.005);
    mp.run();
    mp.show();
    mp.output_weight(argv[1]);

    for (int i = 0; i < row; i++)
        delete data[i];
}