#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iterator>


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
	fstream f;
	fstream f2;
	fstream fout;

public:

	~ReadWriter()
	{
		fin.close();
		fout.close();
		f.close();
		f2.close();
	}

	ReadWriter()
	{
		fin.open("optic_edges.txt", std::ios::in);
		//f.open("m.txt", ios::in | ios::out);
		f.open("m.txt", ifstream::binary | ios::in | ios::out);
		f2.open("m2.txt", ifstream::binary | ios::out);
		fout.open("intervals.txt", ios::out);
	}

	

	void GotoLine(std::fstream& file, int num, int size) {
		//file.seekg(0);
		//file.seekg((num ) * (size) + num / 8 + num / 16);
		file.seekg((num) * (size), ios::beg);
		//file.seekg(num * (size + 2));
	}


	void clear2File() {
		f2.close();
		f2.open("m2.txt", ifstream::binary | ios::out);
	}

	//чтение массива ребер из файла
	void readArray(vector<Edge>& arr)
	{
		string first, second;
		int weight;
		string region;
		while (fin >> first >> second >> region >> weight)
		{
			arr.push_back(Edge(first, second, region, weight));
		}
	}

	void FromByte(char* buffer, bool* b, int size)
	{
		for (int j = 0; j < size; j++) {
			char c = buffer[j];
			for (int i = 0; i < 8; ++i)
				//b.push_back((c & (1 << i)) != 0);
				b[j * 8 + i] = ((c & (1 << i)) != 0);
		}
	}

	void readColumn(int number, bool* res, int size) {

		/*while (size % 8 != 0) {
			size++;
		}*/
		int a = size / 8;

		char* buffer = new char[a];
		GotoLine(f, number, a);
		f.read(buffer, a);
		FromByte(buffer, res, a);
		delete[] buffer;

		/*string s;
		GotoLine(f, number, a);
		f >> s;
		//getline(f, s); //считываем строку

		for (int i = 0; i < s.length(); i++) {
			FromByte(s[i], res, size);
			size -= 8;
		}*/
	}

	void ToByte(bool* b, int size, char* buffer)
	{
		int count = 0;
		for (int j = 0; j < size; j += 8) {
			char c = 0;
			for (int i = 0; i < 8; ++i)
				if (b[j + i]) {
					c |= 1 << i;
				}
			buffer[count] = c;
			count++;
		}
	}


	void writeColumn(bool* col, int size) {

		//f.seekg(f.end);

		//Надо дописать в конец нулевые значения, чтобы размер был кратен 8

		/*while (size % 8 != 0) {
			col.push_back(0);
		}*/

		char* buffer = new char[size / 8];
		ToByte(col, size, buffer);

		//f.write(buffer, col.size() / 8);
		//buffer[col.size() / 8] = '\n';
		//for (int i = 0; i < col.size() / 8; i++) {
			//f << buffer[i];
		f.write(buffer, size / 8);

		//f.write(buffer, col.size() / 8);
	//}
	//f << '\n';


		delete[] buffer;
	}


	void writeColumn2(bool* col, int size) {
		char* buffer = new char[size / 8];
		ToByte(col, size, buffer);

		f2.write(buffer, size / 8);
		delete[] buffer;
	}

	void changeColumn(int num, bool* col, int size) {

		/*while (col.size % 8 != 0) {
			col.push_back(0);
		}*/

		char* buffer = new char[size / 8];
		ToByte(col, size, buffer);

		//f.write(buffer, col.size() / 8);
		//buffer[col.size() / 8] = '\n';
		//for (int i = 0; i < col.size() / 8; i++) {
			//f << buffer[i];
		GotoLine(f, num, size / 8);
		f.write(buffer, size / 8);
		f.seekg(f.tellg(), ios::beg);
		delete[] buffer;
	}





	void writeInterval(int a, int b) {
		fout << a << ' ' << b << '\n';
	}


	void outMatrix(int size, int n) {

		bool* v = new bool[n];
		for (int i = 0; i < size; i++) {
			for (int count = 0; count < n; count++) {
				v[count] = 0;
			}

			readColumn(i, v, n);

			for (int j = 0; j < n; j++) {
				cout << v[j] << ' ';
			}
			cout << '\n';
		}

		delete[] v;
	}
};


//vector<vector<int>> m_space;
vector<Edge> edges;
vector<int> times; //время появления каждого симплекса. Размер вектора = кол-во симплексов
//vector<pair<int, int>> intervals;
ReadWriter rw;

vector<string> vertexes_arr;


int vertexes;
int lines;
int triangles;
int tetrahedrons;
int n;


bool vertex_is_exist(const string& vert) {
	for (int i = 0; i < vertexes_arr.size(); i++) {
		if (vertexes_arr[i] == vert)
			return true;
	}
	return false;
}

int get_index(const string& vert) {
	for (int i = 0; i < vertexes_arr.size(); i++) {
		if (vertexes_arr[i] == vert)
			return i;
	}
	return -1; //вершина не нашлась impossible
}

void create_vertexes() {
	for (int i = 0; i < edges.size(); i++) {
		if (!vertex_is_exist(edges[i].get_first_vertex())) {
			vertexes_arr.push_back(edges[i].get_first_vertex());
		}
		if (!vertex_is_exist(edges[i].get_second_vertex())) {
			vertexes_arr.push_back(edges[i].get_second_vertex());
		}
	}
}


void amount_of_lines() {
	lines = edges.size();
}


//из точек в отрезки
void create_dim1() {
	cout << "dim1 matrix\n";

	//проходимся по всех отрезкам и заполняем матрицу
	for (int i = 0; i < edges.size(); i++) {

		bool* col = new bool[n];
		for (int count = 0; count < n; count++) {
			col[count] = 0;
		}
		//получаем номер вершины по ее значению

		col[get_index(edges[i].get_first_vertex())] = 1;
		col[get_index(edges[i].get_second_vertex())] = 1;
		rw.writeColumn(col, n);
		delete[] col;
		//matrix[lines] - i, j бит единицы, все остальные нули 
		//время появления комплекса
		times.push_back(edges[i].get_weight()); //время = длина отрезка

	}

}




//всего есть 
//вершин: size
//отрезков: lines
//треугольников: triangles
//тетраэдров: tetrahedrons



//low(num = номер столбца)


int low(bool* col) {
	for (int i = n - 1; i >= 0; i--) {
		if (col[i])
			return i;
	}
	return -1;

}




//add column i to column j
void add(const bool* col_i, bool* col_j) {

	for (int k = 0; k < n; k++) {
		if ((col_i[k] == 1 && col_j[k] == 1) || col_i[k] == 0 && col_j[k] == 0) {
			col_j[k] = 0;
		}
		else {
			col_j[k] = 1;
		}
	}

}


//алгоритм приведения матрицы к нужной форме
void reducing() {
	int am_col = vertexes + lines + triangles + tetrahedrons;
	bool** array_of_cols = new bool*[am_col];

	for (int j = 0; j < am_col; j++) {
		array_of_cols[j] = new bool[n];
		for (int count = 0; count < n; count++) {
			array_of_cols[j][count] = 0;
		}
		rw.readColumn(j, array_of_cols[j], n);
	}


	for (int j = 0; j < am_col; j++) {
		if (j % 50 == 0)
			cout << j << " out of " << am_col << '\n';
		if (j == 10000) {
			rw.clear2File();
			for (int i = 0; i < am_col; i++) {
				
				rw.writeColumn2(array_of_cols[i], n);
			}
			cout << " columns have written " << j << '\n';
		}

		///bool* j_col = new bool[n];
		/*for (int count = 0; count < n; count++) {
			j_col[count] = 0;
		}
		rw.readColumn(j, j_col, n);*/

		bool exit = false;
		while (!exit) { //если за один проход не нашелся ни один подходящий столбец, то выходим
			exit = true;
			for (int i = 0; i < j; i++) {
				/*bool* i_col = new bool[n];
				for (int count = 0; count < n; count++) {
					i_col[count] = 0;
				}
				rw.readColumn(i, i_col, n);*/

				int low_j = low(array_of_cols[j]);
				int low_i = low(array_of_cols[i]);

				if (low_i != -1 && low_i == low_j) {
					exit = false; //если есть хотя бы один подходящий столбец
					add(array_of_cols[i], array_of_cols[j]);
					//пошаговый вывод матрицы для проверки
					/*cout << "\n\n";
					for (int i = 0; i < matrix.size(); i++) {
						for (int j = 0; j < matrix[i].size(); j++) {
							cout << matrix[i][j] << ' ';
						}
						cout << '\n';
					}*/
				}
				//delete[] i_col;
			}
		}

		//rw.changeColumn(j, j_col, n);
		//delete[] j_col;
	}

	for (int j = 0; j < am_col; j++) {
		rw.changeColumn(j, array_of_cols[j], n);
	}
	for (int j = 0; j < am_col; j++) {
		delete[] array_of_cols[j];
	}
	delete[] array_of_cols;
}



void reading_intervals() {
	int am_col = vertexes + lines + triangles + tetrahedrons;
	vector<int> lows(n); //вектор всех нижних граней
	for (int i = 0; i < am_col; i++) {

		if (i % 50 == 0)
			cout << i << " out of " << am_col << '\n';
		bool* col = new bool[n];
		for (int count = 0; count < n; count++) {
			col[count] = 0;
		}
		rw.readColumn(i, col, n);
		lows[i] = low(col);
		delete[] col;
	}



	bool is_paired;

	//vertexes - lines
	for (int i = 0; i < vertexes; i++) { //тут ищем positive simplexes

		if (lows[i] == -1) {
			is_paired = false;
			//ищем для него negative match
			for (int j = vertexes; j < vertexes + lines; j++) {
				if (lows[j] != -1 && i == lows[j]) {
					if (times[i] != times[j]) {
						rw.writeInterval(times[i], times[j]);
						//cout << i << ' ' << j << '\n';
						is_paired = true;
					}
				}
			}
			if (!is_paired) {
				rw.writeInterval(times[i], INT32_MAX);
				//cout << i << '\n';
			}
		}

	}

	//lines - triangles
	for (int i = vertexes; i < vertexes + lines; i++) { //тут ищем positive simplexes

		if (lows[i] == -1) {
			is_paired = false;
			//ищем для него negative match
			for (int j = vertexes + lines; j < vertexes + lines + triangles; j++) {
				if (lows[j] != -1 && i == lows[j]) {
					if (times[i] != times[j]) {
						rw.writeInterval(times[i], times[j]);
						//cout << i << ' ' << j << '\n';
						is_paired = true;
					}
				}
			}
			if (!is_paired) {
				rw.writeInterval(times[i], INT32_MAX);
				//cout << i << '\n';
			}
		}

	}

	//triangles - tetrahedrons
	for (int i = vertexes + lines; i < vertexes + lines + triangles; i++) { //тут ищем positive simplexes

		if (lows[i] == -1) {
			is_paired = false;
			//ищем для него negative match
			for (int j = vertexes + lines + triangles; j < vertexes + lines + triangles + tetrahedrons; j++) {
				if (lows[j] != -1 && i == lows[j]) {
					if (times[i] != times[j]) {
						rw.writeInterval(times[i], times[j]);
						//cout << i << ' ' << j << '\n';
						is_paired = true;
						break;
					}
				}
			}
			if (!is_paired) {
				rw.writeInterval(times[i], INT32_MAX);
				//cout << i << '\n';
			}
		}

	}
}

void process() {
	vertexes = 3738;

	cout << "Reading array \n";
	rw.readArray(edges);
	//массив уже отсортирован

	cout << "ed size = " << edges.size() << '\n';
	create_vertexes();
	for (int i = 0; i < vertexes; i++) {
		times.push_back(0);
	}

	amount_of_lines();

	n = vertexes + lines; //размер столбцов
	while (n % 8 != 0) {
		n++;
	}
	cout << "n = " << n << '\n';
	cout << "lines = " << lines << '\n';
	cout << "tr = " << triangles << '\n';
	cout << "tetr = " << tetrahedrons << '\n';
	//lines = 0;
	tetrahedrons = 0;
	triangles = 0;
	for (int i = 0; i < vertexes; i++)
	{
		bool* vec = new bool[n];
		for (int count = 0; count < n; count++) {
			vec[count] = 0;
		}
		rw.writeColumn(vec, n);
		delete[] vec;
	}

	create_dim1();

	cout << "reducing\n";
	reducing();

	cout << "reading intervals\n\n\n";

	reading_intervals();
}



int main()
{
	process();
}
