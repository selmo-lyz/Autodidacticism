#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Perceptron {
public:
    Perceptron(double **input_data, int num_data, int num_prop, int lr = 1) {
        // num_prop 中包含 x_i, 0 <= i < n 加上 class 的數量
        this->num_data = num_data;
        this->num_prop = num_prop;
        this->learning_rate = lr;

        this->data.reserve(num_data);
        for (int i = 0; i < num_data; i++) {
            // 需加上恆等於 1 的 x_0
            this->data[i].reserve(num_prop + 1);
            // 初始化資料
            for (int j = 0; j < num_prop - 1; j++) {
                this->data[i].push_back(input_data[i][j]);
            }
            // 令 data 為 [x_(n-1), ..., x_1, x_0 = 1, cls]
            this->data[i].push_back(1);
            this->data[i].push_back(input_data[i][num_prop - 1]);
        }


        // 需加上 w_0 並減去 cls 的空間
        this->weight.reserve(num_prop);
        for (int i = 0; i < num_prop; i++) {
            // 令 weight 為 [w_(n-1), ..., w_1, w_0]
            this->weight.push_back(0);
        }
    }

    void run(void) {
        counter = 0;
        changed = true;
        while (changed) {
            changed = false;

            for (int i = 0; i < num_data; i++) {
                double inner_product = 0, y;

                // inner product
                for (int j = 0; j < num_prop; j++) {
                    inner_product += weight[j] * data[i][j];
                }
                cout << "x[ " << num_data*counter + i 
                     << " ] and w's inner_product = " << inner_product << endl;

                // checking classification is right
                bool update = 
                    (data[i][num_prop] > 0 && inner_product <= 0) ||
                    (data[i][num_prop] < 0 && inner_product >= 0);

                if (update) {
                    changed = true;

                    // update weight
                    cout << "weight: ";
                    for (int j = 0; j < num_prop; j++) {
                        weight[j] += data[i][num_prop] * learning_rate * data[i][j];
                        cout << weight[j] << ", ";
                    }
                    cout << endl << endl;
                }
            }

            counter++;
        }
    }

    void show(void) {
        cout << "weight: ";
        for (int i = 0; i < num_prop - 1; i++) {
            cout << weight[i] << ", ";
        }
        cout << weight[num_prop - 1] << endl;
    }

private:
    int num_data, num_prop, counter;
    vector<vector<double>> data;
    vector<double> weight;
    double learning_rate;
    bool changed;

    void output_weight(string name) {
        name += ".csv";
        ofstream fout(name.c_str());

        for (int i = 0; i < num_prop; i++) {
            fout << weight[i] << endl;
        }

        fout.close();
    }
};

int main(int argc, char *argv[]) {
    /*
     * row: 資料筆數
     * col: 資料屬性數量
     */
    int row = atoi(argv[2]), col = atoi(argv[3]);

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
     * 執行 Perceptron
     */
    Perceptron perc(data, row, col);
    perc.run();
    perc.show();

    for (int i = 0; i < row; i++)
        delete data[i];
}