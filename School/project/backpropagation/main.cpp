#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <random>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

class Layer {
  public:
    Layer(int unit, int input_dim = 0/*, activation = 'sigmoid'*/) {
        this->unit = unit;
        this->input_dim = input_dim;

        if (input_dim > 0) {
            random_init_1D_vec(weight, input_dim * unit);
            random_init_1D_vec(bias, unit);
            input.assign(input_dim, 0.0);
            output.assign(unit, 0.0);
        }
    }

    void run(vector<double> input) {
        for (int i = 0; i < unit; i++) {
            output[i] = 0 - bias[i];
            for (int j = 0; j < input_dim; j++)
                output[i] += weight[i * input_dim + j] * input[j];
            output[i] = activation_func(output[i]);
        }
    }

    void set_input_dim(int val) {
        if (val > 0) {
            this->input_dim = val;
            random_init_1D_vec(weight, input_dim * unit);
            random_init_1D_vec(bias, unit);
            input.assign(input_dim, 0.0);
            output.assign(unit, 0.0);
        }
    }

    void set_weight(vector<double> vec) {
        this->weight = vec;
    }

    void set_bias(vector<double> vec) {
        this->bias = vec;
    }

    void set_input(vector<double> vec) {
        this->input = vec;
    }

    int get_unit(void) {
        return unit;
    }

    int get_input_dim(void) {
        return input_dim;
    }

    vector<double> get_weight(void) {
        return weight;
    }

    vector<double> get_bias(void) {
        return bias;
    }

    vector<double> get_output(void) {
        return output;
    }

    vector<double> get_input(void) {
        return input;
    }

    //void init_weight_and_bias(vector<double> p, vector<double> q) {
        //weight = p;
        //bias = q;
    //}

  private:
    vector<double> weight, bias, input, output;
    int input_dim, unit;

    void random_init_1D_vec(vector<double>& vec, int len) {
        // 取隨機數的工具
        random_device rd;
        default_random_engine generator(rd());
        uniform_real_distribution<double> unif(0.0, 10.0);

        vec.assign(len, 0.0);
        for (int i = 0; i < len; i++) {
            vec[i] = unif(generator);
        }
    }

    double activation_func(double var) {
        return 1 / (1 + exp(-var));
    }
};

class Model {
  public:
    void add(Layer layer) {
        vector<double> temp;
        temp.assign(layer.get_unit(), 0.0);
        error.reserve(error.size() + 1);
        error.push_back(temp);
        grad_bias.reserve(grad_bias.size() + 1);
        grad_bias.push_back(temp);

        if (layers.empty() && layer.get_input_dim() > 0) {
            // 以下課本範例數值
            //vector<double> p;
            //p.assign(4, 0.0);
            //p[0] = 1;
            //p[1] = -1;
            //p[2] = -1;
            //p[3] = 1;
            //layer.set_weight(p);
            //p.assign(2, 0.0);
            //p[0] = 1;
            //p[1] = 1;
            //layer.set_bias(p);
            // 以上課本範例數值

            temp.assign(layer.get_unit() * layer.get_input_dim(), 0.0);
            grad_weight.reserve(grad_weight.size() + 1);
            grad_weight.push_back(temp);
            
            layers.push_back(layer);
            return;
        }

        layer.set_input_dim(layers.back().get_unit());

        temp.assign(layer.get_unit() * layer.get_input_dim(), 0.0);
        grad_weight.reserve(grad_weight.size() + 1);
        grad_weight.push_back(temp);

        // 以下課本範例數值
        //vector<double> pt;
        //pt.assign(2, 0.0);
        //pt[0] = 1;
        //pt[1] = 1;
        //layer.set_weight(pt);
        //pt.assign(1, 0.0);
        //pt[0] = 1;
        //layer.set_bias(pt);
        // 以上課本範例數值

        layers.push_back(layer);
    }

    void init(void) {
        vector<double> p;
        vector<double> pt;
        // 取隨機數的工具
        random_device rd;
        default_random_engine generator(rd());
        uniform_real_distribution<double> unif(0.0, 10.0);

        for (
            list<Layer>::reverse_iterator it = layers.rbegin();
            it != layers.rend();
            ++it
            ) {
            if (it == layers.rbegin()) {
                pt.assign(it->get_input_dim() * it->get_unit(), 0.0);
                for (int i = 0; i < it->get_input_dim() * it->get_unit(); i++) {
                    pt[i] = unif(generator);
                }
                it->set_weight(pt);

                pt.assign(it->get_unit(), 0.0);
                for (int i = 0; i < it->get_unit(); i++) {
                    pt[i] = unif(generator);
                }
                it->set_bias(pt);

                // 以下課本範例數值
                //pt.assign(2, 0.0);
                //pt[0] = 1;
                //pt[1] = 1;
                //it->set_weight(pt);
                //pt.assign(1, 0.0);
                //pt[0] = 1;
                //it->set_bias(pt);
                // 以上課本範例數值

                continue;
            }

            pt.assign(it->get_input_dim() * it->get_unit(), 0.0);
            for (int i = 0; i < it->get_input_dim() * it->get_unit(); i++) {
                pt[i] = unif(generator);
            }
            it->set_weight(pt);

            pt.assign(it->get_unit(), 0.0);
            for (int i = 0; i < it->get_unit(); i++) {
                pt[i] = unif(generator);
            }
            it->set_bias(pt);
            // 以下課本範例數值
            //p.assign(4, 0.0);
            //p[0] = 1;
            //p[1] = -1;
            //p[2] = -1;
            //p[3] = 1;
            //it->set_weight(p);
            //p.assign(2, 0.0);
            //p[0] = 1;
            //p[1] = 1;
            //it->set_bias(p);
            // 以上課本範例數值
        }

        for (int i = 0; i < grad_weight.size(); i++) {
            for (int j = 0; j < grad_weight[i].size(); j++) {
                grad_weight[i][j] = 0;
            }
        }

        for (int i = 0; i < grad_bias.size(); i++) {
            for (int j = 0; j < grad_bias[i].size(); j++) {
                grad_bias[i][j] = 0;
            }
        }

    }

    double fit(vector<vector<double>> x_train, vector<vector<double>> y_train, int epochs, int batch_size) {
        learning_rate = 5;
        init();
        //print();

        for (int i = 0; i < epochs; i++) {
            for (int j = 0; j < batch_size; j++) {

                // 代入一筆 training data 訓練
                vector<double> input = x_train[j];
                /*
                cout << "Input: ";
                for (int k = 0; k < input.size(); k++)
                    cout << input[k] << ", ";
                cout << endl;
                */

                int l = 0;
                for (list<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
                    it->run(input);
                    it->set_input(input);
                    input = it->get_output();

                    /*
                    cout << "Output " << ++l << " : ";
                    for (int index = 0; index < input.size(); index++) {
                        cout << input[index] << ", ";
                    }
                    cout << "\n";
                    */
                }
                //cout << "\n";

                // 計算誤差
                int pre_input_dim, pre_unit;
                vector<double> output, pre_weight, pre_error;

                list<Layer>::reverse_iterator rit = layers.rbegin();
                vector<vector<double>>::reverse_iterator rvit = error.rbegin();
                while (rit != layers.rend()) {
                    output = rit->get_output();

                    if (rit == layers.rbegin()) {
                        for (int k = 0; k < rvit->size(); k++) {
                            rvit->at(k) = output[k] * (1 - output[k]) * (y_train[j][k] - output[k]);
                            //cout << rvit->at(k) << " : "  << output[k] << " : " << y_train[j][k] << endl;
                        }
                    } else {
                        for (int k = 0; k < rvit->size(); k++) {
                            rvit->at(k) = output[k] * (1 - output[k]);
                            double temp = 0.0;
                            int index = k;
                            for (int l = 0; l < pre_error.size(); l++) {
                                temp += pre_weight[index] * pre_error[l];
                                index += pre_input_dim;
                                //cout << pre_weight[l * pre_input_dim] << " : " << pre_error[l] << "!!!  \n\n";
                            }
                            rvit->at(k) *= temp;
                        }
                    }

                    pre_weight = rit->get_weight();
                    pre_error.assign(rvit->begin(), rvit->end());
                    pre_unit = rit->get_unit();
                    pre_input_dim = rit->get_input_dim();

                    ++rit;
                    ++rvit;
                }

                /*
                 * Debug：印出誤差值
                 */
                /*
                l = 0;
                for (int i = 0; i < error.size(); i++) {
                    cout << "Error " << ++l << " : ";
                    for (int j = 0; j < error[i].size(); j++) {
                        cout << error[i][j] << ", ";
                    }
                    cout << endl;
                }
                cout << endl;
                */

                // 計算修正量
                rit = layers.rbegin();
                rvit = error.rbegin();
                vector<vector<double>>::reverse_iterator rw = grad_weight.rbegin();
                vector<vector<double>>::reverse_iterator rb = grad_bias.rbegin();
                while (rit != layers.rend()) {
                    vector<double> vec_input = rit->get_input();
                    for (int i = 0; i < rit->get_unit(); i++) {
                        rb->at(i) += -learning_rate * rvit->at(i);

                        for (int j = 0; j < rit->get_input_dim(); j++) {
                            rw->at(i * rit->get_input_dim() + j) += learning_rate * rvit->at(i) * vec_input[j];
                        }
                    }

                    ++rit;
                    ++rvit;
                    ++rw;
                    ++rb;
                }

                /*
                 * Debug：印出 Gradient
                 */
                /*
                l = 0;
                for (int i = 0; i < grad_weight.size(); i++) {
                    cout << "Grad_Weight " << ++l << " : ";
                    for (int j = 0; j < grad_weight[i].size(); j++) {
                        cout << grad_weight[i][j] << ", ";
                    }
                    cout << endl;
                }
                cout << endl;

                l = 0;
                for (int i = 0; i < grad_bias.size(); i++) {
                    cout << "Grad_Bias " << ++l << " : ";
                    for (int j = 0; j < grad_bias[i].size(); j++) {
                        cout << grad_bias[i][j] << ", ";
                    }
                    cout << endl;
                }
                cout << endl;
                */
            }

            // 用 graident 更新 Weight
            list<Layer>::iterator lit = layers.begin();
            vector<vector<double>>::iterator vwit = grad_weight.begin();
            vector<vector<double>>::iterator vbit = grad_bias.begin();
            while (lit != layers.end()) {
                vector<double> wt = lit->get_weight();
                vector<double> bt = lit->get_bias();

                for (int i = 0; i < wt.size(); i++) {
                    wt[i] += vwit->at(i);
                }

                for (int i = 0; i < bt.size(); i++) {
                    bt[i] += vbit->at(i);
                }

                lit->set_weight(wt);
                lit->set_bias(bt);

                ++lit;
                ++vwit;
                ++vbit;
            }

            //print();

            //cout << "一次學習循環完成\n\n";
        }
        //cout << endl;
        return loss_function(x_train, y_train);
    }

    void print(void) {
        int l = 1;
        for (list<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
            vector<double> p = it->get_weight(), pt = it->get_bias();
            cout << "Weight " << l << " : ";
            for (int i = 0; i < p.size(); i++) {
                cout << p[i] << ", ";
            }
            cout << "\n";

            cout << "Bias " << l++ << " : ";
            for (int i = 0; i < pt.size(); i++) {
                cout << pt[i] << ", ";
            }
            cout << "\n\n";
        }
    }

    void output_weight(void) {
        // 輸出 NN 的 Weight
        ofstream fout("output_weight.csv");
        list<Layer>::iterator lit = layers.begin();
        while (lit != layers.end()) {
            vector<double> wt = lit->get_weight();
            vector<double> bt = lit->get_bias();
            
            for (int i = 0; i < wt.size(); i++) {
                fout << wt[i] << ",";
            }
            
            for (int i = 0; i < bt.size() - 1; i++) {
                fout << bt[i] << ",";
            }
            fout << bt.back() << "\n";
            
            ++lit;
        }
        
        fout.close();
    }

    void input_weight(void) {
        // 輸入 NN 的 Weight
        ifstream fin("output_weight.csv");
        list<Layer>::iterator lit = layers.begin();
        int index_layers = 1;
        while (lit != layers.end()) {
            string str;
            int indim = lit->get_input_dim(), unit = lit->get_unit();
            vector<double> tmp;
            tmp.assign(indim*unit, 0);
            cout << "Layer " << index_layers << " Weight: ";
            for (int i = 0; i < indim*unit; i++) {
                getline(fin, str, ',');
                tmp[i] = atof(str.c_str());
                cout << tmp[i] << ", ";
            }
            lit->set_weight(tmp);

            cout << endl;
            
            tmp.assign(unit, 0);
            cout << "Layer " << index_layers++ << " Bias: ";
            for (int i = 0; i < unit - 1; i++) {
                getline(fin, str, ',');
                tmp[i] = atof(str.c_str());
                cout << tmp[i] << ", ";
            }
            getline(fin, str, '\n');
            tmp[unit - 1] = atof(str.c_str());
            cout << tmp[unit - 1] << "";
            lit->set_bias(tmp);
            
            cout << endl;

            ++lit;
        }
    }
    
    double test(vector<vector<double>> testing_data, vector<vector<double>> label) {
        double acc = loss_function(testing_data, label);
        return acc;
    }    

  private:
    double learning_rate;
    list<Layer> layers;
    vector<vector<double>> error, grad_weight, grad_bias;
    
    double loss_function(vector<vector<double>> training_data, vector<vector<double>> label) {
        double correct = 0;
        for (int r = 0; r < training_data.size(); r++) {
            vector<double> input = training_data[r];
            for (list<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
                it->run(input);
                it->set_input(input);
                input = it->get_output();
            }

            int current = 0;
            vector<double> result = layers.back().get_output();
            for (int c = 0; c < label[c].size(); c++) {
                cout << result[c] << " : " << label[r][c] << endl;
                //if (result[c] > 0.8) {
                    //result[c] = 1;
                //} else if (result[c] < 0.2) {
                    //result[c] = 0;
                //}
                
                if (abs(result[c] - label[r][c]) < 0.1) {
                    current++;
                    //cout << "current: " << current << endl;
                }
            }

            if (current == label[r].size()) {
                correct++;
                //cout << "correct: " << correct << endl;
            }
        }
        cout << "Accuracy: " << correct / training_data.size() * 100 << "%" << endl;
        return correct / training_data.size() * 100;
    }
};

void load_data_2D(
    ifstream& file,
    vector<vector<double>>& target,
    vector<vector<double>>& data,
    int c,
    int num_label,
    int r = 1
    ) {
    string str;

    for (int i = 0; i < r; i++) {
        cout << "資料 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < c - num_label; j++) {
            getline(file, str, ',');
            data[i][j] = atof(str.c_str());
            cout << data[i][j] << ", ";  // for Debug
        }
        cout << endl;

        cout << "標籤 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < num_label - 1; j++) {
            getline(file, str, ',');
            target[i][j] = atof(str.c_str());
            cout << target[i][j] << ", ";  // for Debug
        }
        getline(file, str, '\n');
        target[i][num_label - 1] = atof(str.c_str());
        cout << target[i][num_label - 1] << endl;  // for Debug
    }
}

int main(int argc, char *argv[]) {
    /*
     * 指令：./執行檔名 1. 2. 3. 4.
     * 參數說明：
     * 1. 放在 training_data 底下的 training data 檔案名，需有副檔名 csv
     * 2. training data 行數
     * 3. training data 屬性數
     * 4. training data 標籤數
     * 需改寫 testing data 行數變數
     */

    /*
     * row: 資料筆數
     * col: 資料屬性數量 (including target label)
     */
    int row = atoi(argv[2]), col = atoi(argv[3]), num_label = atoi(argv[4]);

    /*
     * 讀取訓練資料
     */
    vector<vector<double>> training_data, target;
    vector<double> temp;
    temp.assign(col-num_label, 0.0);
    training_data.assign(row, temp);
    temp.assign(num_label, 0.0);
    target.assign(row, temp);

    string training_data_name = "./training_data/";
    training_data_name.append(argv[1]);
    ifstream file(training_data_name);
    load_data_2D(file, target, training_data, col, num_label, row);
    file.close();

    cout << "初始化 data 完成\n\n";

    /*
     * 建構 Neural Network
     */
    Model model;
    //model.add(Layer(2, 2));
    //model.add(Layer(1));
    model.add(Layer(3, 4));
    //model.add(Layer(3));
    //model.print();    // Debug: 印出各層的 weight
    //model.compile(/*loss = 'E = (1/2) * sum_{j}(T_{j} - A_{j})^{2}, optimizer = 'BP'*/);

    /*
     * Training
     */
    int batch_size = row, epochs = 1;
    double max = -1;
    int times = 0;
    double acc;
    for (int epochs = 1; epochs <= 100; epochs++) {
        cout << epochs << endl;
        acc = model.fit(training_data, target, epochs, batch_size);
        
        if (max < acc) {
            max = acc;
            times = epochs;
            model.output_weight();
        }
    }

    //model.fit(training_data, target, times, batch_size);
    //model.output_weight();

    cout << max << " : " << times << endl;

    /*
     * Testing
     */
    int num_testing_data = 6;
    vector<vector<double>> testing_data;
    temp.assign(col-num_label, 0.0);
    testing_data.assign(num_testing_data, temp);
    temp.assign(num_label, 0.0);
    target.assign(num_testing_data, temp);

    string testing_data_name = "./testing_data/";
    testing_data_name.append(argv[1]);
    ifstream ftest(testing_data_name);
    load_data_2D(ftest, target, testing_data, col, num_label, num_testing_data);
    ftest.close();

    model.input_weight();

    model.test(testing_data, target);
}