#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iterator>

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
	fstream fout_edges;

public:

	~ReadWriter()
	{
		fin.close();
		fout.close();
		fout_h1.close();
		fout_h2.close();
		fout_edges.close();
	}

	ReadWriter()
	{
		fin.open("optic_edges.txt", std::ios::in);
		fout.open("m_input.txt", std::ios::out);
		fout_h1.open("h1.txt", std::ios::out);
		fout_h2.open("h2.txt", std::ios::out);
		fout_edges.open("edges_am.txt", std::ios::out);
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
	void write_num(int n) {
		fout_edges << n << '\n';
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
int rang; //Ранг матрицы
vector<int> firstIndexes; //Индексы ведущий элементов строк матрицы

int amountOf2paths() {
	//ищем два последовательных ребра графа
	for (int i = 0; i < edges.size(); i++) {
		for (int j = 0; j < edges.size(); j++) {
			if (edges[i].get_second_vertex() == edges[j].get_first_vertex() &&
				edges[i].get_first_vertex() != edges[j].get_second_vertex()) { //обратные ребра не подходят
				//cout << edges[i].get_first_vertex() << " - " << edges[i].get_second_vertex() << " - " <<
					//edges[j].get_second_vertex() << '\n';
				twopaths.push_back(TwoPath(edges[i].get_first_vertex(), edges[i].get_second_vertex(),
					edges[j].get_second_vertex(), edges[i].get_weight(), edges[j].get_weight()));
			}
		}
	}
	return twopaths.size();
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

int get_index_of_first_el(vector<double>& line, double eps) {
	for (int i = 0; i < line.size(); i++) {
		if (abs(line[i]) > eps)
			return i;
	}
	return -1; //строка нулевая
}

//coef - int, так как в матрице только 1 и -1
void sub_lines(vector<double>& line1, const vector<double>& line2, double coef) {
	int n = line1.size(); //элементов
	for (int i = 0; i < n; i++) {
		line1[i] = line1[i] - coef * line2[i];
	}
}

void get_rank_gauss(double eps, vector<vector<double>>& a) {
	int m = a.size(); //строк
	int n = a[0].size(); //столбцов

	if (m == 1) { //если пока что только одна строка
		firstIndexes.push_back(get_index_of_first_el(a[0], eps));
		if (firstIndexes[0] == -1)
			rang = 0;
		else
			rang = 1;
	}
	else {
		bool exit = false;
		int index;
		while (!exit) {
			exit = true; //выходим, если нет строки, которую нужно вычесть
			//берем последнюю строку
			//находим индекс первого ненулевого элемента
			index = get_index_of_first_el(a[m - 1], eps);
			if (index != -1) {
				//находим строку, у которой ведущий элемент с тем же индексом
				for (int i = 0; i < a.size() - 1; i++) {
					if (firstIndexes[i] == index) { //нашелся ведущий элемент другой строки a[i], которую надо вычесть теперь
						exit = false;

						//обнуляем текущий элемент в строке
						//вычитаем строки с коэффициентом line1[first]/line2[first]
						sub_lines(a[m - 1], a[i], a[m - 1][index] * 1.0 / a[i][index]);
						break;
					}
				}
			}
		}
		//если вдруг ведущий элемент не нашелся, то элемент становится ведущим окончательно 
		//(из строки больше ничего вычитать не надо)
		firstIndexes.push_back(index);

		//если строка занулилась, то она лз с остальными, то есть ранг НЕ увеличивается
		if (index != -1) {
			rang++; //лнз строка
		}
	}

}



void matrix_output(vector<vector<int>>& matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[0].size(); j++)
		{
			cout << matrix[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}


void set_triangle_matrix(vector<double>& a, vector<string>& tr) {
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

void set_square_matrix(vector<double>& a, vector<string>& sq) {
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
void deltaMatrix2(vector<vector<double>>& matrix, int max,
	vector<vector<string>>& triangles, vector<int>& triangles_time,
	vector<vector<string>>& squares, vector<int>& squares_time) {
	//в матрице будет столбцов столько, сколько ребер есть в графе
	//То есть edges.size()


	for (int w = 1; w <= max; w++) {
		cout << w << " out of " << max << '\n';

		//теперь пройдемся по всем треугольникам
		//у которых triangles_time равен w
		//занесем эти треугольники в матрицу
		for (int i = 0; i < triangles_time.size(); i++) {
			if (triangles_time[i] == w) {
				vector<double>a(edges.size(), 0);//этот вектор будем добавлять в матрицу
				set_triangle_matrix(a, triangles[i]);
				matrix.push_back(a);

				get_rank_gauss(0.0001, matrix); //как только строка появилась, пересчитываем ранг
			}
		}



		//теперь пройдемся по всем квадратам
		for (int i = 0; i < squares_time.size(); i++) {
			if (squares_time[i] == w) {
				vector<double>a(edges.size(), 0);//этот вектор будем добавлять в матрицу
				set_square_matrix(a, squares[i]);
				matrix.push_back(a);

				get_rank_gauss(0.0001, matrix); //как только строка появилась, пересчитываем ранг
			}
		}


		//записываем ранг
		rw.writeH1(w, rang);
	}
}






bool vert_is_exist(string& v, vector<string>& arr) {
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == v)
			return true;
	}
	return false;
}


//Составим матрицу из путей из A0 (То есть из вершин-элементов)
void deltaMatrix1(vector<vector<double>>& matrix, vector<string>& vert, int max) {
	//в матрице будет столбцов столько, сколько вершин есть в графе

	int r = -1;
	for (int w = 1; w <= max; w++) {

		cout << w << " out of " << max << '\n';
		bool flag = false;
		//теперь пройдемся по всем ребрам
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i].get_weight() == w) {
				vector<double> a(vert.size(), 0);
				flag = true;
				for (int j = 0; j < vert.size(); j++) {

					if (vert[j] == edges[i].get_second_vertex()) {
						a[j] = 1;
					}
					else if (vert[j] == edges[i].get_first_vertex()) {
						a[j] = -1;;
					}
				}
				matrix.push_back(a); //добавляем строку
				//как только добавили строку, пересчитываем ранг
				get_rank_gauss(0.0001, matrix);
			}
		}
		//получили матрицу matrix

		//записываем ранг
		rw.writeH1(w, rang);

	}

}




int main()
{
	rw.readArray(edges);
	//найдем максимум из всех весов ребер
	int max = edges[0].get_weight();
	for (int i = 1; i < edges.size(); i++) {
		if (max < edges[i].get_weight())
			max = edges[i].get_weight();
	}


	/*vector<string> vert;
	//создаем список вершин графа
	for (int i = 0; i < edges.size(); i++) {
		string v1 = edges[i].get_first_vertex();
		string v2 = edges[i].get_second_vertex();

		if (!vert_is_exist(v1, vert))
			vert.push_back(v1);
		if (!vert_is_exist(v2, vert))
			vert.push_back(v2);
	}*/

	/*cout << "twopaths ...\n";
	amountOf2paths();
	cout << "triangles ...\n";
	vector<vector<string>> triangles;
	vector<int> triangles_time;
	creationOfTriangles(triangles, triangles_time);
	cout << "squares ...\n";
	vector<vector<string>> squares;
	vector<int> squares_time;
	creationOfSquares(squares, squares_time);
	vector<vector<double>> matrix;
	deltaMatrix2(matrix, max, triangles, triangles_time, squares, squares_time);
	*/
	/*vector<vector<double>> matrix;
	deltaMatrix1(matrix, vert, max);*/
	for (int i = 1; i <= max; i++) {
		int count = 0;
		for (int ed = 0; ed < edges.size(); ed++) {
			if (edges[ed].get_weight() <= i) {
				count++;
			}
		}
		rw.write_num(count);
	}
}
