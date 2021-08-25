#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iterator>

#include <thread>
#include <math.h>

using namespace std;
class Edge {

private:
	string first_vertex;
	string second_vertex;
	int weight;
	string region;

public:
	string get_first_vertex() {
		return first_vertex;
	}
	string get_second_vertex() {
		return second_vertex;
	}
	int get_weight() {
		return weight;
	}
	string get_region() {
		return region;
	}

	Edge(string first, string second, string reg, int w) {
		first_vertex = first;
		second_vertex = second;
		region = reg;
		weight = w;
	}

	Edge() {
		first_vertex = "";
		second_vertex = "";
		weight = -1;
		region = "";
	}
};


class ReadWriter
{

private:

	fstream fin;
	fstream fout;
	fstream fout_h1;
	fstream fout_h2;

public:

	~ReadWriter()
	{
		fin.close();
		fout.close();
		fout_h1.close();
		fout_h2.close();
	}

	ReadWriter()
	{
		fin.open("optic_edges.txt", std::ios::in);
		fout.open("m_input.txt", std::ios::out);
		fout_h1.open("h1.txt", std::ios::out);
		fout_h2.open("h2.txt", std::ios::out);
	}



	void closeOpenH2() {
		fout_h2.close();
		fout_h2.open("h2.txt", std::ios::app);
	}
	//чтение массива ребер из файла
	void readArray(vector<Edge>& arr)
	{
		string first, second;
		int weight;
		string region;
		while (fin >> first >> second >> region >> weight)
		{
			//надо удалить петли из графа
			if (first != second)
				arr.push_back(Edge(first, second, region, weight));
		}
	}


	void writeMatrix(vector<vector<int>>& matrix) {
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[0].size() - 1; j++)
			{
				fout << matrix[i][j] << ' ';
			}
			fout << matrix[i][matrix[0].size() - 1] << '\n';

		}
	}

	void writeH1(int t, int b) {
		fout_h1 << t << ' ' << b << '\n';
	}
	void writeH2(int t, int b) {
		fout_h2 << t << ' ' << b << '\n';
	}
};


class TwoPath {

private:
	string first_vertex;
	string second_vertex;
	string third_vertex;
	int w1;
	int w2;

public:
	string get_first_vertex() {
		return first_vertex;
	}
	string get_second_vertex() {
		return second_vertex;
	}
	string get_third_vertex() {
		return third_vertex;
	}
	int get_w1() {
		return w1;
	}
	int get_w2() {
		return w2;
	}

	TwoPath(string first, string second, string third, int ww1, int ww2) {
		first_vertex = first;
		second_vertex = second;
		third_vertex = third;
		w1 = ww1;
		w2 = ww2;
	}

	TwoPath() {
		first_vertex = "";
		second_vertex = "";
		third_vertex = "";
		w1 = 0;
		w2 = 0;
	}
};

ReadWriter rw;
vector<Edge> edges;
vector<TwoPath> twopaths;
vector<TwoPath> twopaths_different;



int amountOf2paths() {
	//ищем два последовательных ребра графа
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges.size(); j++) {
			if (edges[i].get_second_vertex() == edges[j].get_first_vertex()) {
				//cout << edges[i].get_first_vertex() << " - " << edges[i].get_second_vertex() << " - " <<
					//edges[j].get_second_vertex() << '\n';
				twopaths.push_back(TwoPath(edges[i].get_first_vertex(), edges[i].get_second_vertex(),
					edges[j].get_second_vertex(), edges[i].get_weight(), edges[j].get_weight()));
			}
		}
	}
	return twopaths.size();
}

void creationOfSquares(vector<vector<string>>& squares, vector<int>& squares_time) {

	//ищем сколько 2 путей отличаются ТОЛЬКО средней вершинкой
	for (int i = 0; i < twopaths.size(); i++) {
		
		for (int j = i; j < twopaths.size(); j++) {
			if (twopaths[i].get_first_vertex() == twopaths[j].get_first_vertex() &&
				twopaths[i].get_third_vertex() == twopaths[j].get_third_vertex() &&
				twopaths[i].get_second_vertex() != twopaths[j].get_second_vertex()) {
				//Нашли квадрат
				vector<string> sq;
				sq.push_back(twopaths[i].get_first_vertex());//a
				sq.push_back(twopaths[i].get_second_vertex());//b
				sq.push_back(twopaths[i].get_third_vertex());//c
				sq.push_back(twopaths[j].get_second_vertex());//b'
				squares.push_back(sq);
				squares_time.push_back(max(
					max(twopaths[i].get_w1(), twopaths[i].get_w2()),
					max(twopaths[j].get_w1(), twopaths[j].get_w2())));
			}
		}
	}
}


bool StartAndEndExist(TwoPath& tp) {
	for (int i = 0; i < twopaths_different.size(); i++) {
		string a = twopaths_different[i].get_first_vertex();
		string b = tp.get_first_vertex();
		if (twopaths_different[i].get_first_vertex() == tp.get_first_vertex() &&
			twopaths_different[i].get_third_vertex() == tp.get_third_vertex()) {
			return true;
		}
	}
	return false;
}

void modifyTwoPaths() {
	//преобразуем список 2 путей, чтобы не было одинаковых путей с совпадающими началами и концами
	for (int i = 0; i < twopaths.size(); i++) {
		if (!StartAndEndExist(twopaths[i])) {
			twopaths_different.push_back(twopaths[i]);
		}
	}
}

int amountOfSemiEdges() {
	modifyTwoPaths();

	int res = 0;
	bool flag;
	//ищем сколько 2 путей таких, что нет ребра от первой к последней
	for (int i = 0; i < twopaths_different.size(); i++) {
		flag = true;
		if (i % 100 == 0) {
			cout << i << " out of " << twopaths_different.size() << '\n';
		}
		for (int j = 0; j < edges.size(); j++) {
			if (edges[j].get_first_vertex() == twopaths_different[i].get_first_vertex() &&
				edges[j].get_second_vertex() == twopaths_different[i].get_third_vertex()) {
				flag = false; // not a semi edge
				break;
			}
		}
		if (flag) //a semi edge 
			res++;
	}
	return res;
}


void creationOfTriangles(vector<vector<string>>& triangles, vector<int>& triangles_time) {
	//ищем сколько 2 путей таких, что ЕСТЬ ребро от первой к последней
	for (int i = 0; i < twopaths.size(); i++) {
		if (i % 100 == 0) {
			cout << i << " out of " << twopaths.size() << '\n';
		}
		for (int j = 0; j < edges.size(); j++) {
			if (edges[j].get_first_vertex() == twopaths[i].get_first_vertex() &&
				edges[j].get_second_vertex() == twopaths[i].get_third_vertex()) {
				//нашли треугольник
				vector<string> tr;
				tr.push_back(twopaths[i].get_first_vertex());
				tr.push_back(twopaths[i].get_second_vertex());
				tr.push_back(twopaths[i].get_third_vertex());
				triangles.push_back(tr);

				//находим максимальное ребро в треугольнике по весу
				triangles_time.push_back(max(
					max(twopaths[i].get_w1(), twopaths[i].get_w2()), edges[j].get_weight()));
			}
		}
	}

	//Надо отсортировать треугольники по времени их появления
	int temp;
	vector<string> temp_vec;
	for (int i = 0; i < triangles.size() - 1; i++) {
		for (int j = 0; j < triangles.size() - i - 1; j++) {
			if (triangles_time[j] > triangles_time[j + 1]) {
				// меняем время появления местами
				temp = triangles_time[j];
				triangles_time[j] = triangles_time[j + 1];
				triangles_time[j + 1] = temp;
				// меняем сами треугольники местами
				temp_vec = triangles[j];
				triangles[j] = triangles[j + 1];
				triangles[j + 1] = temp_vec;

			}
		}
	}
}


void get_rank(double EPS, vector<vector<int>>& a, int w) {
	int m = a.size();
	int n = a[0].size();

	/*int rank = max(n, m);
	vector<char> line_used(n);
	for (int i = 0; i < m; ++i) {
		int j;
		for (j = 0; j < n; ++j)
			if (!line_used[j] && abs(a[j][i]) > EPS)
				break;
		if (j == n)
			--rank;
		else {
			line_used[j] = true;
			for (int p = i + 1; p < m; ++p)
				a[j][p] /= a[j][i];
			for (int k = 0; k < n; ++k)
				if (k != j && abs(a[k][i]) > EPS)
					for (int p = i + 1; p < m; ++p)
						a[k][p] -= a[j][p] * a[k][i];
		}
	}

	return rank;*/

	int i, j, k, l;
	double r;

	i = 0; j = 0;
	while (i < m && j < n) {
		if (i % 100 == 0)
			cout << "\t\t" << i << " out of " << m << '\n';
		// Инвариант: минор матрицы в столбцах 0..j-1
		//   уже приведен к ступенчатому виду, и строка
		//   с индексом i-1 содержит ненулевой эл-т
		//   в столбце с номером, меньшим чем j

		// Ищем максимальный элемент в j-м столбце,
		// начиная с i-й строки
		r = 0.0;
		for (k = i; k < m; ++k) {
			if (abs(a[k][j]) > r) {
				l = k;      // Запомним номер строки
				r = abs(a[k][j]); // и макс. эл-т
			}
		}
		if (r <= EPS) {
			// Все элементы j-го столбца по абсолютной
			// величине не превосходят eps.
			// Обнулим столбец, начиная с i-й строки
			for (k = i; k < m; ++k) {
				a[k][j] = 0.0;
			}
			++j;      // Увеличим индекс столбца
			continue; // Переходим к следующей итерации
		}

		if (l != i) {
			// Меняем местами i-ю и l-ю строки
			for (k = j; k < n; ++k) {
				r = a[i][k];
				a[i][k] = a[l][k];
				a[l][k] = (-r); // Меняем знак строки
			}
		}

		// Утверждение: fabs(a[i*n + k]) > eps

		// Обнуляем j-й столбец, начиная со строки i+1,
		// применяя элем. преобразования второго рода
		for (k = i + 1; k < m; ++k) {
			r = (-a[k][j] / a[i][j]);

			// К k-й строке прибавляем i-ю, умноженную на r
			a[k][j] = 0.0;
			for (l = j + 1; l < n; ++l) {
				a[k][l] += r * a[i][l];
			}
		}

		++i; ++j;   // Переходим к следующему минору
	}

	//return i; // Возвращаем число ненулевых строк
	rw.closeOpenH2();
	rw.writeH2(w, i);
}


//составим матрицу из всех элементов из Omega2, То есть квадратов и треугольников
void matrix2(vector<vector<int>>& matrix) {
	//в матрице будет строк столько, сколько путей через 3 вершины есть в графе
	//То есть twopaths.size()
	vector<int>a;
	for (int i = 0; i < twopaths.size(); i++) {
		matrix.push_back(a);
	}

	//теперь пройдемся по всем треугольникам

	//ищем сколько 2 путей таких, что ЕСТЬ ребро от первой к последней
	for (int i = 0; i < twopaths.size(); i++) {
		for (int j = 0; j < edges.size(); j++) {
			if (edges[j].get_first_vertex() == twopaths[i].get_first_vertex() &&
				edges[j].get_second_vertex() == twopaths[i].get_third_vertex()) {
				//нашли треугольник v1 = twopaths[i].get_first_vertex(), v2 = twopaths[i].get_second_vertex(),
			   //v3 = twopaths[i].get_third_vertex()
			   //нужно поставить единицу в строку с номером i, а во все остальные поставить нули
				for (int k = 0; k < i; k++) {
					matrix[k].push_back(0);
				}
				for (int k = i + 1; k < twopaths.size(); k++) {
					matrix[k].push_back(0);
				}
				matrix[i].push_back(1);
			}
		}
	}


	//теперь пройдемся по всем квадратам
	//ищем сколько 2 путей отличаются ТОЛЬКО средней вершинкой
	for (int i = 0; i < twopaths.size(); i++) {
		if (i % 10 == 0) {
			cout << i << " out of " << twopaths.size() << '\n';
		}
		for (int j = i; j < twopaths.size(); j++) {
			if (twopaths[i].get_first_vertex() == twopaths[j].get_first_vertex() &&
				twopaths[i].get_third_vertex() == twopaths[j].get_third_vertex() &&
				twopaths[i].get_second_vertex() != twopaths[j].get_second_vertex()) {
				//Нашли квадрат с v1 = twopaths[i].get_first_vertex()
				//v2 = twopaths[i].get_second_vertex()
				//v3 = twopaths[i].get_third_vertex()
				//v2' = twopaths[j].get_second_vertex()

				//надо поставить 1 в строку i, и поставить -1 в строку j
				//в другие строки поставить нули
				for (int k = 0; k < twopaths.size(); k++) {
					if (k == i)
						matrix[k].push_back(1);
					else if (k == j)
						matrix[k].push_back(-1);
					else
						matrix[k].push_back(0);
				}
			}
		}
	}

	//получили матрицу matrix
}


void matrix_output(vector<vector<int>>& matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[0].size(); j++)
		{
			cout << matrix[i][j] << ' ';
		}
		cout << '\n';
	}

}


/*void set_triangle_matrix(vector<vector<int>>& matrix, string& v1, string& v2, string& v3, int counter) {
	for (int i = 0; i < edges.size(); i++) {
		if ((edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v2) ||
			(edges[i].get_first_vertex() == v2 && edges[i].get_second_vertex() == v3)) {
			matrix[i][counter] += 1;
		}
		else if (edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v3) {
			matrix[i][counter] -= 1;
		}
	}
}*/
void set_triangle_matrix(vector<int>& a, vector<string>& tr) {
	string v1 = tr[0];
	string v2 = tr[1];
	string v3 = tr[2];
	for (int i = 0; i < edges.size(); i++) {
		if ((edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v2) ||
			(edges[i].get_first_vertex() == v2 && edges[i].get_second_vertex() == v3)) {
			a[i] += 1;
		}
		else if (edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v3) {
			a[i] -= 1;
		}
	}
}

void set_square_matrix(vector<int>& a, vector<string>& sq) {
	string v1 = sq[0];
	string v2 = sq[1];
	string v3 = sq[2];
	string v22 = sq[3];
	for (int i = 0; i < edges.size(); i++) {
		if ((edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v2) ||
			(edges[i].get_first_vertex() == v2 && edges[i].get_second_vertex() == v3)) {
			a[i] += 1;
		}
		else if ((edges[i].get_first_vertex() == v1 && edges[i].get_second_vertex() == v22) ||
			(edges[i].get_first_vertex() == v22 && edges[i].get_second_vertex() == v3)) {
			a[i] -= 1;
		}
	}
}



//Составим матрицу из путей из A1 (То есть из ребер элементов)
void deltaMatrix2(vector<vector<int>>& matrix, int max,
	vector<vector<string>>& triangles, vector<int>& triangles_time,
	vector<vector<string>>& squares, vector<int>& squares_time) {
	//в матрице будет столбцов столько, сколько ребер есть в графе
	//То есть edges.size()

	int r = -1;
	std::vector <std::thread> th_vec;
	//for (int w = 1; w <= max; w++) {
	for (int w = 1; w <= max - 1; w++) {
		cout << w << " out of " << max << '\n';
		bool flag = false;

		//теперь пройдемся по всем треугольникам
		//у которых triangles_time равен w
		//занесем эти треугольники в матрицу
		for (int i = 0; i < triangles_time.size(); i++) {
			if (triangles_time[i] == w) {
				vector<int>a(edges.size(), 0);//этот вектор будем добавлять в матрицу
				set_triangle_matrix(a, triangles[i]);
				matrix.push_back(a);
				flag = true; 
			}
		}



		//теперь пройдемся по всем квадратам
		for (int i = 0; i < squares_time.size(); i++) {
			if (squares_time[i] == w) {
				vector<int>a(edges.size(), 0);//этот вектор будем добавлять в матрицу
				set_square_matrix(a, squares[i]);
				matrix.push_back(a);
				flag = true;
			}
		}

		//получили матрицу matrix
		if (w >= 17) {
			//посчитаем ее ранг и запишем
			if (flag) {
				//отправляем в отдельный поток
				th_vec.push_back(thread(get_rank, 0.0000001, matrix, w));
				//r = get_rank(0.0000001, matrix);
			}
			
		}
		if (th_vec.size() == 4)
			break;
	}
	//всего 4 потока
	for (int i = 0; i < th_vec.size(); ++i) {
		th_vec.at(i).join();
	}
}








bool vert_is_exist(string& v, vector<string>& arr) {
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == v)
			return true;
	}
	return false;
}

/*
//Составим матрицу из путей из A0 (То есть из вершин-элементов)
void deltaMatrix1(vector<vector<int>>& matrix, vector<string>& vert, int max) {
	//в матрице будет строк столько, сколько вершин есть в графе

	vector<int>a;
	for (int i = 0; i < vert.size(); i++) {
		matrix.push_back(a);
	}

	for (int w = 1; w <= max; w++) {

		cout << w << " out of " << max << '\n';
		//теперь пройдемся по всем ребрам
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].get_weight() == w) {
				for (int j = 0; j < vert.size(); j++) {

					if (vert[j] == edges[i].get_second_vertex()) {
						matrix[j].push_back(1);
					}
					else if (vert[j] == edges[i].get_first_vertex()) {
						matrix[j].push_back(-1);
					}
					else
						matrix[j].push_back(0);
				}
			}
		}
		//получили матрицу matrix

		//теперь считаем ранг и записываем
		int r = get_rank(0.0000001, matrix);
		rw.writeH1(w, r);
	}

}




int findDet(vector<vector<int>>& a, int n) { //Рекурсивная функция вычисления определителя матрицы
	if (n == 1)
		return a[0][0];
	else if (n == 2)
		return a[0][0] * a[1][1] - a[0][1] * a[1][0];
	else {
		int d = 0;
		for (int k = 0; k < n; k++) {
			vector<vector<int>> m(n - 1);
			for (int i = 0; i < n - 1; i++) {
				vector<int> vec(n - 1);
				m[i] = vec;
			}
			for (int i = 1; i < n; i++) {
				int t = 0;
				for (int j = 0; j < n; j++) {
					if (j == k)
						continue;
					m[i - 1][t] = a[i][j];
					t++;
				}
			}
			d += pow(-1, k + 2) * a[0][k] * findDet(m, n - 1);
		}
		return d; //Возвращаем определитель матрицы
	}
}


bool index_is_exist(int v, vector<int>& arr) {
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == v)
			return true;
	}
	return false;
}

void insert_to_vector(vector<int>& vec, int a) {
	bool flag = false;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] > a)
		{
			vec.insert(vec.begin() + i, a);
			flag = true;
			break;
		}
	}
	if (!flag)
		vec.push_back(a);
}

void create_minor(int a, int b,
	vector<int> indexes_of_lines,
	vector<int> indexes_of_columns,
	vector<vector<int>>& minor, vector<vector<int>>& matrix) {


	insert_to_vector(indexes_of_lines, a);
	insert_to_vector(indexes_of_columns, b);


	//создать окаймляющий минор с предыдущем минором на пересечении строк и столбцов (данные векторы) 
	//и добавочной строкой a и столбцом b
	vector<int> vec(indexes_of_columns.size()); //колво столбцов + столбец b
	for (int i = 0; i < indexes_of_lines.size(); i++) {
		minor.push_back(vec);
		for (int j = 0; j < indexes_of_columns.size(); j++) {
			minor[i][j] = matrix[indexes_of_lines[i]][indexes_of_columns[j]];
		}
	}
}


int get_rank_minors(vector<vector<int>>& matrix) {
	int m = matrix.size();
	int n = matrix[0].size();
	//находим сначала ненулевой минор порядка 1 - Он гарантированно есть
	int a, b;
	bool flag;
	for (a = 0; a < m; a++) {
		flag = false;
		for (b = 0; b < n; b++) {
			if (matrix[a][b] != 0)
			{
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}

	//Запомнили индексы ненулевого элемента a, b
	vector<int> indexes_of_lines;
	vector<int> indexes_of_columns;
	indexes_of_lines.push_back(a);
	indexes_of_columns.push_back(b);
	flag = true;
	while (flag && indexes_of_lines.size() < min(m, n)) {
		cout << indexes_of_lines.size() << " out of " << min(m, n) << '\n';
		flag = false; //остается false, если все миноры равны нулю
		for (int i = 0; i < m; i++) {
			if (!index_is_exist(i, indexes_of_lines)) {
				for (int j = 0; j < n; j++) {
					if (!index_is_exist(j, indexes_of_columns)) {
						vector<vector<int>> minor;
						create_minor(i, j,
							indexes_of_lines, indexes_of_columns,
							minor,
							matrix);
						//посчитать определитель
						int det = findDet(minor, minor.size());
						if (det != 0)
						{
							flag = true; //нашли окаймляющий минор не равный нулю
							insert_to_vector(indexes_of_lines, i);
							insert_to_vector(indexes_of_columns, j);
							break;
						}
					}
				}
			}
			if (flag)
				break;
		}
		if (!flag)
			return indexes_of_lines.size(); //ранг матрицы нашелся
	}
	return min(m, n);
}

*/


int main()
{
	rw.readArray(edges);
	//найдем максимум из всех весов ребер
	int max = edges[0].get_weight();
	for (int i = 1; i < edges.size(); i++) {
		if (max < edges[i].get_weight())
			max = edges[i].get_weight();
	}


	vector<string> vert;
	//создаем список вершин графа
	for (int i = 0; i < edges.size(); i++) {
		string v1 = edges[i].get_first_vertex();
		string v2 = edges[i].get_second_vertex();

		if (!vert_is_exist(v1, vert))
			vert.push_back(v1);
		if (!vert_is_exist(v2, vert))
			vert.push_back(v2);
	}




	cout << "2 paths...\n";
	int p2 = amountOf2paths();
	//cout << "semi edges...\n";
	//int se = amountOfSemiEdges();
	//cout << "triangles...\n";
	//int tr = amountOfTriangles();
	//cout << "squares...\n";
	//int squares = amountOfSquares();
	cout << "2paths amount = " << p2 << '\n';
	//cout << "semi edges amount = " << se << '\n';
	//cout << "triangles amount = " << tr << '\n';
	//cout << "squares amount = " << squares << '\n';

	//составим матрицу из всех элементов из A2, То есть квадратов и треугольников
	//cout << "\n\n\n =================matrix omega2=================\n\n";

	//matrix2(matrix);
	//cout << "\n\n rank of omega2 = " << get_rank(0.0000001, matrix) << "\n\n";

	/*cout << "\n\n =================matrix delta omega1=================\n\n";
	vector<vector<int>> matrix1;
	deltaMatrix1(matrix1, vert);
	//matrix_output(matrix1);
	int dOmega1_rank = get_rank(0.0000001, matrix1);
	//matrix_output(matrix1);
	cout << "\n rank of d omega1 = " << dOmega1_rank << "\n\n";


	cout << "\n\n =================matrix delta omega2=================\n\n";
	vector<vector<int>> matrix2;
	deltaMatrix2(matrix2);
	int dOmega2_rank = get_rank(0.0000001, matrix2);
	cout << "\n rank of d omega2 = " << dOmega2_rank << "\n\n";



	cout << "\n\n\n dim H0 = dim Omega0 - dim d Omega 0 - dim d Omega 1 =    " <<
		vert.size() - 0 - dOmega1_rank << "\n\n";
	cout << "\n\n\n dim H1 = dim Omega1 - dim d Omega 1 - dim d Omega 2 =    " <<
		edges.size() - dOmega1_rank - dOmega2_rank << "\n\n";
		*/

		/*cout << "\n\n =================matrix delta omega2=================\n\n";

		vector<vector<int>> matrix2;
		deltaMatrix2(matrix2);
		rw.writeMatrix(matrix2);*/
		//cout << "\n\n =================rank delta omega2=================\n\n";
		//int dOmega2_rank = get_rank_minors(matrix2);
		//cout << "\n rank of d omega2 = " << dOmega2_rank << "\n\n";
	vector<vector<string>> triangles;
	vector<int> triangles_time;
	creationOfTriangles(triangles, triangles_time); //создаем треугольники для создания матрицы
	vector<vector<string>> squares;
	vector<int> squares_time;
	creationOfSquares(squares, squares_time); //создаем треугольники для создания матрицы

	vector<vector<int>> matrix2;
	deltaMatrix2(matrix2, max, triangles, triangles_time, squares, squares_time);
}
