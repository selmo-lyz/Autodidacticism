#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

class TreeNode {
 public:
    TreeNode(string val, int max_num_child = 2) {
        this->label = val;
        this->num_child = 0;
        this->max_num_child = max_num_child;
        this->decision.assign(max_num_child, "\0");
        this->p_child.assign(max_num_child, NULL);
    }

    void setLabel(string val) {
        if (val.length() > 0) {
            this->label = val;
        }
    }

    void addChild(string val, int max_num_child, string opt) {
        if (num_child <= this->max_num_child) {
            decision[num_child] = opt;
            p_child[num_child++] = new TreeNode(val, max_num_child);
        }
    }

    void addChild(TreeNode* node, string opt) {
        if (num_child <= this->max_num_child) {
            decision[num_child] = opt;
            p_child[num_child++] = node;
            return;
        }
        cout << "ERROR: FULL\n";
    }

    string getLabel(void) {
        return label;
    }

    string getDecision(int index) {
        if (index < num_child) {
            return decision[index];
        }
        return "";
    }

    TreeNode* getChild(int index) {
        if (index < num_child) {
            return p_child[index];
        }
        return NULL;
    }

    int getNumChild(void) {
        return num_child;
    }

 private:
    string label;
    int num_child, max_num_child;
    vector<string> decision;
    vector<TreeNode*> p_child;
};

class DecisionTree {
 public:
    DecisionTree(vector<vector<string>> d, vector<string> list) {
        D = d;
        attribute_list = list;
        decision_tree_root = NULL;
    }

    void run(void) {
        decision_tree_root = generate_decision_tree(D, attribute_list);

        if (!decision_tree_root) {
            cout << "決策樹建立失敗\n\n";
        }
    }

    void Preorder(void) {
        if (!decision_tree_root) {
            cout << "決策樹為 NULL，請先以 run() 建立決策樹\n\n";
        } else {
            PreorderRecursive(decision_tree_root);
        }
    }

    void decision(vector<vector<string>> d) {
        int num_attr = attribute_list.size();
        int num_tuple = d.size();

        for (int r = 0; r < num_tuple; r++) {
            // 依照 Decision Tree 分類資料 d[r]
            TreeNode* p = decision_tree_root;
            while (true) {
                // 取得該節點中 attribute 對應的行索引值
                int index;
                for (index = 0; index < num_attr; index++) {
                    if (p->getLabel() == attribute_list[index]) {
                        break;
                    }
                }
                
                // 依照節點與資料的 attribute 將其分類
                for (int i = 0; i < p->getNumChild(); i++) {
                    if (d[r][index] == p->getDecision(i)) {
                        p = p->getChild(i);
                        break;
                    }
                }

                if (p->getNumChild() == 0) {
                    cout << "第" << r << "筆資料屬於" << p->getLabel() << endl;
                    break;
                }
            }
        }
    }

 private:
    vector<vector<string>> D;
    vector<string> attribute_list;
    TreeNode* decision_tree_root;

    void PreorderRecursive(TreeNode* p = NULL, int l = 0) {
        if (!p) {
            return;
        }

        for (int i = 0; i < l; i++) {
            cout << "\t";
        }

        cout << "Node Label: " << p->getLabel() << endl;
        for (int i = 0; i < p->getNumChild(); i++) {
            PreorderRecursive(p->getChild(i), l + 1);
        }
    }

    string information_gain(vector<vector<string>> d, vector<string> list) {
        int num_attr = list.size();
        int num_tuple = d.size();

        // 計算 Info(D)
        double d_info = 0;
        map<string, double> cls;
        for (int i = 0; i < num_tuple; i++) {
            // 若 d[i][num_attr - 1] 的 class 不存在於計算陣列中，
            // 則將其加入陣列。
            if (!cls.count(d[i][num_attr - 1])) {
                cls[d[i][num_attr - 1]] = 0;
            }

            // 計算 d[i][num_attr - 1] class 的出現次數
            cls[d[i][num_attr - 1]] += 1;
        }

        for (map<string, double>::iterator i = cls.begin(); i != cls.end(); ++i) {
            double temp = i->second / num_tuple;
            d_info += - (temp * log2(temp));
        }

        cls.clear();

        // 計算 Info_{attribute}(D)
        vector<double> attr_info;
        // 不須計算 attribute Class，故為 num_attr - 1
        attr_info.assign(num_attr - 1, 0);
        map<string, double> attr_cls;
        for (int i = 0; i < num_attr - 1; i++) {
            // 計算 attribute 中 class 的種類與其個數
            for (int j = 0; j < num_tuple; j++) {
                if (!attr_cls.count(d[j][i])) {
                    attr_cls[d[j][i]] = 0;
                }

                attr_cls[d[j][i]] += 1;
            }

            for (map<string, double>::iterator k = attr_cls.begin(); k != attr_cls.end(); ++k) {
                // 計算 Info(D_{class of attribute})
                int sum_num_cls = 0;
                for (int j = 0; j < num_tuple; j++) {
                    // 若 attribute 的 class 與欲尋找的不同，則跳到下個 tuple
                    if (d[j][i] != k->first) {
                        continue;
                    }

                    if (!cls.count(d[j][num_attr - 1])) {
                        cls[d[j][num_attr - 1]] = 0;
                    }

                    // 計算 d[i][num_attr - 1] class 的出現次數
                    cls[d[j][num_attr - 1]] += 1;

                    sum_num_cls += 1;
                }

                double temp = 0;
                for (map<string, double>::iterator j = cls.begin(); j != cls.end(); ++j) {
                    temp -= ((j->second/sum_num_cls)*log2(j->second/sum_num_cls));
                }

                cls.clear();

                attr_info[i] += (k->second / num_tuple) * temp;
            }
            attr_cls.clear();
        }

        // 找出 Gain(attribute) 最大者的索引值
        int max = 0;
        for (int i = 1; i < attr_info.size(); i++) {
            if ((d_info - attr_info[i]) > (d_info - attr_info[max])) {
                max = i;
            }
        }

        return list[max];
    }

    TreeNode* generate_decision_tree(vector<vector<string>> d, vector<string> attribute_list) {
        // 1.
        TreeNode* n;
        int num_tuple = d.size();
        int num_attr = attribute_list.size();

        // 2.
        bool diff = false;
        string temp_cls = d[0][num_attr - 1];
        for (int i = 0; i < num_tuple; i++) {
            if (temp_cls != d[i][num_attr - 1]) {
                diff = true;
                break;
            }
        }

        if (!diff) {
            n = new TreeNode(temp_cls, 0);
            // 3.
            //cout << "leaf: " << temp_cls << endl;
            return n;
        }

        // 4.
        if (attribute_list.empty()) {
            map<string, double> cls;
            for (int r = 0; r < num_tuple; r++) {
                if (!cls.count(d[r][num_attr - 1])) {
                    cls[d[r][num_attr - 1]] = 0;
                }
                cls[d[r][num_attr - 1]] += 1;
            }

            string val_leaf;
            int max = 0;
            for (map<string, double>::iterator i = cls.begin(); i != cls.end(); ++i) {
                if (i->second > max) {
                    max = i->second;
                    val_leaf = i->first;
                }
            }

            n = new TreeNode(val_leaf, 0);
            // 5.
            //cout << "leaf: " << val_leaf << endl;
            return n;
        }

        // 6.
        string splitting_criterion = information_gain(d, attribute_list);
        //cout << "node: " << splitting_criterion << endl;

        // 8. & 9.
        int index;
        for (index = 0; index < num_attr; index++) {
            if (attribute_list[index] == splitting_criterion) {
                attribute_list.erase(attribute_list.begin() + index);
                num_attr -= 1;
                break;
            }
        }

        // 7.
        map<string, double> attr_cls;
        for (int r = 0; r < num_tuple; r++) {
            if (!attr_cls.count(d[r][index])) {
                attr_cls[d[r][index]] = 0;
            }

            attr_cls[d[r][index]] += 1;
        }

        n = new TreeNode(splitting_criterion, attr_cls.size());

        // 10.
        for (map<string, double>::iterator j = attr_cls.begin(); j != attr_cls.end(); ++j) {
            // 11.
            vector<string> temp;
            temp.assign(num_attr, "\0");
            vector<vector<string>> dj;
            dj.assign(j->second, temp);
            int num_data_dj = 0;

            for (int r = 0; r < num_tuple; r++) {
                if (d[r][index] == j->first) {
                    int d_col = 0;
                    for (int col = 0; col < num_attr; col++, d_col++) {
                        if (d_col == index) d_col += 1;
                        dj[num_data_dj][col] = d[r][d_col];
                    }
                    num_data_dj += 1;
                }
            }

            // 12.
            if (j->second == 0) {  // 13.
                map<string, double> cls;
                for (int r = 0; r < num_tuple; r++) {
                    if (!cls.count(d[r][num_attr - 1])) {
                        cls[d[r][num_attr - 1]] = 0;
                    }
                    cls[d[r][num_attr - 1]] += 1;
                }

                string val_leaf;
                int max = 0;
                for (map<string, double>::iterator i = cls.begin(); i != cls.end(); ++i) {
                    if (i->second > max) {
                        max = i->second;
                        val_leaf = i->first;
                    }
                }

                n->addChild(val_leaf, 0, j->first);
                //cout << "leaf: " << val_leaf << endl;
                cls.clear();
            } else {  // 14.
                n->addChild(generate_decision_tree(dj, attribute_list), j->first);
            }
        }

        attr_cls.clear();

        // 15.
        return n;
    }
};

void load_data_2D(
    ifstream& file,
    vector<string>& header,
    vector<vector<string>>& data,
    int c,
    int r = 1
    ) {
    string str;
    cout << "資料屬性表: ";  // for Debug
    for (int i = 0; i < c - 1; i++){
        getline(file, str, ',');
        header[i] = str;
        cout << header[i] << ", ";  // for Debug
    }
    getline(file, str, '\n');
    header[c - 1] = str;
    cout << header[c - 1] << endl;  // for Debug

    for (int i = 0; i < r; i++) {
        cout << "資料 " << i << " 屬性: ";  // for Debug
        for (int j = 0; j < c - 1; j++) {
            getline(file, str, ',');
            data[i][j] = str;
            cout << data[i][j] << ", ";  // for Debug
        }
        getline(file, str, '\n');
        data[i][c - 1] = str;
        cout << data[i][c - 1] << endl;  // for Debug
    }
}

void load_data_1D(
    ifstream& file,
    vector<string>& header,
    int c
    ) {
    string str;
    cout << "資料: ";  // for Debug
    for (int i = 0; i < c - 1; i++){
        getline(file, str, ',');
        header[i] = str;
        cout << header[i] << ", ";  // for Debug
    }
    getline(file, str, '\n');
    header[c - 1] = str;
    cout << header[c - 1] << endl;  // for Debug
}

int main(int argc, char *argv[]) {
    /*
     * row: 資料筆數
     * col: 資料屬性數量
     */
    int row = atoi(argv[2]), col = atoi(argv[3]);

    /*
     * 讀取訓練資料
     */
    vector<vector<string>> data;
    vector<string> attribute_list;
    attribute_list.assign(col, "\0");
    data.assign(row, attribute_list);

    ifstream file(argv[1]);
    load_data_2D(file, attribute_list, data, col, row);
    file.close();

    cout << "初始化 data 完成\n\n";

    DecisionTree dt(data, attribute_list);
    // 建立 Decision Tree
    dt.run();
    // Preorder Trace
    dt.Preorder();

    // 重建 Decision Tree
    //dt.load_decision_tree(file);

    // 讀取測試資料
    vector<string> temp;
    vector<vector<string>> input;
    temp.assign(col - 1, "\0");
    input.assign(36, temp);
    file.open("input.csv");
    load_data_2D(file, attribute_list, input, col - 1, 36);

    // 以 Decision Tree 分類 input 中的資料
    dt.decision(input);
}