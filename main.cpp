#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>

using namespace std;

class Graph {
    int num_vert;
    int num_edges;
    int id_begin;
    int id_end;
    vector<vector<int>> edges;
    vector<vector<double>> widths;

    static int random_number() {
        const int MAX_NUM = 1e6;
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, MAX_NUM);
        return dist(mt);
    }

    struct Node {
        Node *left;
        Node *right;
        int value;
        int min;
        int max;

        Node(Node *left, Node *right, int value, int min, int max) :
                left(left), right(right), value(value), min(min), max(max) {}

        void add_edges_to_owner_vec(vector<int> &vec, int owner) {
            if (min + 1 == max) {
                if (min >= owner) {
                    vec.push_back(min);
                }
            }
            if (left != nullptr) {
                left->add_edges_to_owner_vec(vec, owner);
            }
            if (right != nullptr) {
                right->add_edges_to_owner_vec(vec, owner);
            }
        }
    };

    struct Neighbours {
        int num_vert;
        Node *root;
        vector<int> edges;
        int owner;

        Neighbours(int num_vert, int owner) : num_vert(num_vert),
                                              root(new Node(nullptr, nullptr, 0, 0, num_vert)), owner(owner) {}

        void add_edge(int vert) {
            if (vert < owner) {
                _add(vert);
            } else {
                _add(vert - 1);
            }
        }

        void _add(int vert) {
            Node *cur = root;
            while (cur->min < cur->max) {
                ++cur->value;

                if (cur->min + 1 == cur->max) {
                    break;
                }

                int mid = (cur->max + cur->min) / 2;
                if (vert < mid) {
                    if (cur->left == nullptr) {
                        cur->left = new Node(nullptr, nullptr, 0, cur->min, mid);
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == nullptr) {
                        cur->right = new Node(nullptr, nullptr, 0, mid, cur->max);
                    }
                    cur = cur->right;
                }
            }
        }

        int get_random_node() {
            int random_node_num = _get_rand();
            if (random_node_num >= owner) {
                ++random_node_num;
            }
            return random_node_num;
        }

        int _get_rand() {
            if (root->value == num_vert) {
                return -1;
            }

            Node *cur = root;
            while (cur->min < cur->max) {
                if (cur->min + 1 == cur->max) {
                    return -2;
                }

                int mid = (cur->max + cur->min) / 2;
                int free_left = mid - cur->min - ((cur->left == nullptr) ? 0 : (cur->left)->value);
                int free_right = cur->max - mid - ((cur->right == nullptr) ? 0 : (cur->right)->value);

                int rand = random_number() % (free_left + free_right);

                if (rand < free_left) {
                    if (cur->left == nullptr) {
                        return (cur->min + random_number() % (mid - cur->min));
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == nullptr) {
                        return (mid + random_number() % (cur->max - mid));
                    }
                    cur = cur->right;
                }
            }
            return -2;
        }

        void fill_vector() {
            root->add_edges_to_owner_vec(edges, owner);
            for (int i = 0; i < edges.size(); ++i) {
                if (edges[i] >= owner) {
                    ++edges[i];
                }
            }
        }
    };

    struct Degrees {
        int num_vert;
        Node *root;
        vector<int> degrees;

        Degrees(int num_vert) : num_vert(num_vert),
                                root(new Node(nullptr, nullptr, 0, 0, num_vert)) {
            degrees.resize(num_vert, 0);
        }

        void increase_degree(int vert) {
            ++degrees[vert];
            Node *cur = root;
            while (cur->min < cur->max) {
                if (degrees[vert] == num_vert - 1) {
                    ++cur->value;
                }
                if (cur->min + 1 == cur->max) {
                    return;
                }

                int mid = (cur->max + cur->min) / 2;
                if (vert < mid) {
                    if (cur->left == nullptr) {
                        cur->left = new Node(nullptr, nullptr, 0, cur->min, mid);
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == nullptr) {
                        cur->right = new Node(nullptr, nullptr, 0, mid, cur->max);
                    }
                    cur = cur->right;
                }
            }
        }

        int get_random_node() {
            if (root->value == num_vert) {
                return -1;
            }
            Node *cur = root;
            while (cur->min < cur->max) {
                if (cur->min + 1 == cur->max) {
                    return cur->min;
                }
                int mid = (cur->max + cur->min) / 2;
                int free_left = mid - cur->min - ((cur->left == nullptr) ? 0 : cur->left->value);
                int free_right = cur->max - mid - ((cur->right == nullptr) ? 0 : cur->right->value);

                int rand = random_number() % (free_left + free_right);
                if (rand < free_left) {
                    if (cur->left == nullptr) {
                        return (cur->min + random_number() % (mid - cur->min));
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == nullptr) {
                        return (mid + random_number() % (cur->max - mid));
                    }
                    cur = cur->right;
                }
            }
            return -2;
        }
    };

    void generate_tree(vector<Neighbours> &neighbours, Degrees &degrees) {
        for (int i = 0; i < num_vert; ++i) {
            neighbours.push_back(Neighbours(num_vert - 1, i));
        }

        for (int next_v = 1; next_v < num_vert; ++next_v) {
            int v = random_number() % next_v;
            neighbours[next_v].add_edge(v);
            neighbours[v].add_edge(next_v);
            degrees.increase_degree(v);
            degrees.increase_degree(next_v);
        }
    }

    void generate_graph() {
        vector <Neighbours> neighbours;
        Degrees degrees(num_vert);

        generate_tree(neighbours, degrees);

        for (int i = 0; i < num_edges - num_vert + 1; ++i) {
            int v = degrees.get_random_node();
            int w = neighbours[v].get_random_node();
            neighbours[w].add_edge(v);
            neighbours[v].add_edge(w);
            degrees.increase_degree(v);
            degrees.increase_degree(w);
        }

        for (auto vert: neighbours) {
            vert.fill_vector();
            edges.push_back(vert.edges);
        }
    }

    void generate_widths() {
        for (int i = 0; i < num_vert; ++i) {
            for (int j = 0; j < edges[i].size(); ++j) {
                widths[i].push_back(random_number());
            }
        }
        for (int i = num_vert - 1; i >= 0; --i) {
            for (int j = 0; j < edges[i].size(); ++j) {
                edges[edges[i][j]].push_back(i);
                widths[edges[i][j]].push_back(widths[i][j]);
            }
        }
    }

    void generate_ids() {
        id_begin = random_number() % num_vert;
        id_end = random_number() % num_vert;

        while (id_end == id_begin) {
            id_end = random_number() % num_vert;
        }
    }

    void write_file(string file_name) {
        ofstream file;
        file.open(file_name);

        file << id_begin << " " << id_end;
//         file << num_vert << " " << num_edges;
        for (int i = 0; i < edges.size(); ++i) {
            file << '\n';
            file << i + 1 << " :";
            for (int j = 0; j < edges[i].size(); ++j) {
                file << " " << edges[i][j] + 1 << "-" << widths[i][j];
            }
        }

        file.close();
    }

    void read_file(string file_name) {
        ifstream file;
        file.open(file_name);

        std::string line;

        getline(file, line);
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == ' ') {
                num_vert = stoi(line.substr(0, i));
                num_edges = stoi(line.substr(i + 1, line.size()));

                break;
            }
        }

        edges.resize(num_vert);
        widths.resize(num_vert);

        while (getline(file, line)) {
            int cur_vert = 0, last_ind = 0, cnt_values = 0;
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == ' ') {
                    last_ind = i + 3;
                    cur_vert = stoi(line.substr(0, i)) - 1;

                    break;
                }
            }

            for (int i = last_ind; i <= line.size(); ++i) {
                if (line[i] == '-') {
                    edges[cur_vert].push_back(stoi(line.substr(last_ind, i - last_ind)) - 1);
                    last_ind = i + 1;
                }
                if (line[i] == ' ' || i == line.size()) {
                    widths[cur_vert].push_back(stod(line.substr(last_ind, i - last_ind)));
                    ++cnt_values;
                    last_ind = i + 1;
                }
            }
        }

        file.close();
    }

    void change_widths(double x) {
        for (int i = 0; i < widths.size(); ++i) {
            for (int j = 0; j < widths[i].size(); ++j) {
                widths[i][j] += x;
            }
        }
    }

    void dfs(vector<bool> &is_used, int vertex) {
        is_used[vertex] = true;
        for (int i = 0; i < edges[vertex].size(); ++i) {
            if (widths[vertex][i] < 0) {
                continue;
            }
            if (is_used[edges[vertex][i]]) {
                continue;
            }
            dfs(is_used, edges[vertex][i]);
        }
    }

    bool find_path() {
        int num_vert = edges.size();

        vector<bool> is_used(num_vert, false);

        dfs(is_used, id_begin);

        return is_used[id_end];
    }

    int bin_search(vector<double> &widths_sort) {
        int left = 0, right = num_edges - 1;
        int mid;

        while (left < right) {
            mid = (left + right) / 2;

            change_widths(-widths_sort[mid + 1]);

            if (find_path()) {
                left = mid + 1;
            } else {
                right = mid;
            }

            change_widths(widths_sort[mid + 1]);
        }

        return left;
    }

public:
    void fill_graph_data(string option, string argv2, string argv3, string file_name) {
        if (option == "-g") {
            num_vert = stoi(argv2);
            num_edges = stoi(argv3);

            widths.resize(num_vert);

            generate_graph();
            generate_widths();
            generate_ids();

            write_file(file_name);
        }

        if (option == "-t") {
            id_begin = stoi(argv2) - 1;
            id_end = stoi(argv3) - 1;

            read_file(file_name);
        }
    }

    int find_max_allow_width() {
        vector<double> widths_sort;

        for (int i = 0; i < widths.size(); ++i) {
            for (int j = 0; j < widths[i].size(); ++j) {
                if (i > edges[i][j]) {
                    continue;
                }
                widths_sort.push_back(widths[i][j]);
            }
        }

        sort(widths_sort.begin(), widths_sort.end());

        double min_width = widths_sort[0];

        for (int i = 0; i < widths_sort.size(); ++i) {
            widths_sort[i] -= min_width;
        }

        change_widths(-min_width);

        int max_ind_edge = bin_search(widths_sort);

        return widths_sort[max_ind_edge];
    }
};

int main(int argc, char* argv[]) {
    Graph planet;
    planet.fill_graph_data(argv[1], argv[2], argv[3], argv[4]);

    cout << "max width is " << planet.find_max_allow_width();
    return 0;
}
