#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iterator>


using namespace std;

class ReadWriter
{

private:

	fstream fin;
	fstream f;
	fstream fout;

public:

	~ReadWriter()
	{
		fin.close();
		fout.close();
		f.close();
	}

	ReadWriter()
	{
		fin.open("matrix.txt", std::ios::in);
		//f.open("m.txt", ios::in | ios::out);
		f.open("m.txt", ifstream::binary | ios::in | ios::out);
		fout.open("intervals.txt", ios::out);
	}



	void GotoLine(std::fstream& file, int num, int size) {
		//file.seekg(0);
		//file.seekg((num ) * (size) + num / 8 + num / 16);
		file.seekg((num) * (size), ios::beg);
		//file.seekg(num * (size + 2));
	}


	//чтение матрицы размера size*size из файла
	void readMatrix(vector<vector<int>>& arr, int size)
	{
		for (int i = 0; i < size; i++) {
			vector<int> v(size, 0);
			arr.push_back(v);
			for (int j = 0; j < size; j++) {
				fin >> arr[i][j];
			}
		}

	}

	void FromByte(char* buffer, bool* b, int size)
	{
		for (int j = 0; j < size; j++) {
			char c = buffer[j];
			for (int i = 0; i < 8; ++i)
				//b.push_back((c & (1 << i)) != 0);
				b[j*8 + i] = ((c & (1 << i)) != 0);
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


/*

class Column
{
public:
	vector<int> arr;

	Column(int size)
	{
		//нужно создать столько бит, чтобы хватило для заполнения size 0 и 1
		//каждый int по 32 бита
		int amount = size / 32 + 1;
		for (int am = 0; am < amount; am++) {
			arr.push_back(0);
		}
	}


	Column(int size, int i, int j)
	{
		//нужно создать столько бит, чтобы хватило для заполнения size 0 и 1
		//каждый int по 32 бита
		int amount = size / 32 + 1;
		for (int am = 0; am < amount; am++) {
			arr.push_back(0);
		}
		//arr[0] - самый старший массив бит
		int num = i / 32; //номер числа int в векторе
		int num2 = i % 32; //номер бита в числе
		int mask = 1 << num2;
		arr[num] |= mask;

		num = j / 32;
		num2 = j % 32;
		mask = 1 << num2;
		arr[num] |= mask;
	}

	Column(int size, int i, int j, int k)
	{
		//нужно создать столько бит, чтобы хватило для заполнения size 0 и 1
		//каждый int по 32 бита
		int amount = size / 32 + 1;
		for (int am = 0; am < amount; am++) {
			arr.push_back(0);
		}
		//arr[0] - самый старший массив бит
		int num = i / 32; //номер числа int в векторе
		int num2 = i % 32; //номер бита в числе
		int mask = 1 << num2;
		arr[num] |= mask;

		num = j / 32;
		num2 = j % 32;
		mask = 1 << num2;
		arr[num] |= mask;

		num = k / 32;
		num2 = k % 32;
		mask = 1 << num2;
		arr[num] |= mask;
	}

	Column(int size, int i, int j, int k, int l)
	{
		//нужно создать столько бит, чтобы хватило для заполнения size 0 и 1
		//каждый int по 32 бита
		int amount = size / 32 + 1;
		for (int am = 0; am < amount; am++) {
			arr.push_back(0);
		}
		//arr[0] - самый старший массив бит
		int num = i / 32; //номер числа int в векторе
		int num2 = i % 32; //номер бита в числе
		int mask = 1 << num2;
		arr[num] |= mask;

		num = j / 32;
		num2 = j % 32;
		mask = 1 << num2;
		arr[num] |= mask;

		num = k / 32;
		num2 = k % 32;
		mask = 1 << num2;
		arr[num] |= mask;

		num = l / 32;
		num2 = l % 32;
		mask = 1 << num2;
		arr[num] |= mask;
	}

	int getLow() {
		//найти старший бит, которые не равен нулю
		for (int i = arr.size() - 1; i >= 0; i--) {
			if (arr[i] != 0) {
				//надо найти старший ненулевой бит этого числа
				for (int j = 31; j >= 0; j--) {
					int mask = 1 << j;
					if ((mask | arr[i]) == arr[i]) {
						return i*32 + j;
					}
				}
			}
		}
		return -1;
	}

	void Add(Column& j) {
		for (int i = 0; i < arr.size(); i++) {
			this->arr[i] = this->arr[i] ^ j.arr[i];
		}
	}
};

*/



int n;


vector<vector<int>> m_space;
vector<int> times; //время появления каждого симплекса. Размер вектора = кол-во симплексов
//vector<pair<int, int>> intervals;
ReadWriter rw;

int vertexes;
int lines;
int triangles;
int tetrahedrons;

void amount_of_lines() {
	lines = 0;
	for (int i = 0; i < m_space.size(); i++) {
		for (int j = i + 1; j < m_space.size(); j++) {
			if (m_space[i][j] != INT32_MAX) {
				lines++; //нашли отрезок
			}
		}
	}
}
void amount_of_triangles() {
	int n = m_space.size();
	triangles = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (m_space[i][j] != INT32_MAX) { //нашлось ребро
				for (int k = max(i, j) + 1; k < n; k++) {
					if (m_space[j][k] != INT32_MAX && m_space[i][k] != INT32_MAX) {
						triangles++;
					}
				}
			}
		}
	}

}
void amount_of_tetrahedrons() {
	int n = m_space.size();
	tetrahedrons = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (m_space[i][j] != INT32_MAX) { //нашлось ребро
				for (int k = j + 1; k < n; k++) {
					if (m_space[j][k] != INT32_MAX && m_space[i][k] != INT32_MAX) { //нашелся треугольник
						for (int l = k + 1; l < n; l++) {
							if (m_space[l][k] != INT32_MAX && m_space[l][i] != INT32_MAX && m_space[l][j] != INT32_MAX) {
								tetrahedrons++;
							}
						}
					}
				}
			}
		}
	}
}

//из точек в отрезки
void create_dim1() {
	cout << "dim1 matrix\n";

	//проходимся по всех отрезкам и заполняем матрицу
	for (int i = 0; i < m_space.size(); i++) {
		for (int j = i + 1; j < m_space.size(); j++) {
			if (m_space[i][j] != INT32_MAX) {
				bool* col = new bool[n];
				for (int count = 0; count < n; count++) {
					col[count] = 0;
				}
				col[i] = 1;
				col[j] = 1;
				rw.writeColumn(col, n);
				delete[] col;
				//matrix[lines] - i, j бит единицы, все остальные нули 
				//время появления комплекса
				times.push_back(m_space[i][j]); //время = длина отрезка
			}
		}
	}

}


//из отрезков в треугольники
void create_dim2() {
	cout << "dim2 matrix\n";

	//проходимся по всем треугольникам и заполняем матрицу
	for (int i = 0; i < vertexes; i++) {
		for (int j = i + 1; j < vertexes; j++) {
			if (m_space[i][j] != INT32_MAX) { //нашлось ребро
				for (int k = j + 1; k < vertexes; k++) {
					if (m_space[j][k] != INT32_MAX && m_space[i][k] != INT32_MAX) {
						bool* col = new bool[n];
						for (int count = 0; count < n; count++) {
							col[count] = 0;
						}
						col[vertexes + i] = 1;
						col[vertexes + j] = 1;
						col[vertexes + k] = 1;
						rw.writeColumn(col, n);
						delete[] col;
						//время появления комплекса
						times.push_back(max(max(m_space[i][j], m_space[i][k]), m_space[j][k])); //время = max(грани треугольника)

					}
				}
			}
		}
	}
}


//из треугольников в тетраэдры
void create_dim3() {
	cout << "dim3 matrix\n";

	//проходимся по всем тетраэдрам и заполняем матрицу
	for (int i = 0; i < vertexes; i++) {
		for (int j = i + 1; j < vertexes; j++) {
			if (m_space[i][j] != INT32_MAX) { //нашлось ребро
				for (int k = j + 1; k < vertexes; k++) {
					if (m_space[j][k] != INT32_MAX && m_space[i][k] != INT32_MAX) { //нашелся треугольник
						for (int l = k + 1; l < vertexes; l++) {
							if (m_space[l][k] != INT32_MAX && m_space[l][i] != INT32_MAX && m_space[l][j] != INT32_MAX) { //нашелся тетраэдр
								bool* col = new bool[n];
								for (int count = 0; count < n; count++) {
									col[count] = 0;
								}
								col[vertexes + lines + i] = 1;
								col[vertexes + lines + j] = 1;
								col[vertexes + lines + k] = 1;
								col[vertexes + lines + l] = 1;
								rw.writeColumn(col, n);
								delete[] col;
								//время появления комплекса
								times.push_back(
									max(
										max(
											max(m_space[i][j], m_space[i][k]),
											max(m_space[i][l], m_space[j][k])
										),
										max(m_space[j][l], m_space[k][l])
									)
								); //время = max(ребра тетаэдра)

							}
						}
					}
				}
			}
		}
	}
}


void sortMatrix(vector<int>& times, int size) {

	int am_col = vertexes + lines + triangles + tetrahedrons;
	for (int i = 0; i < am_col - 1; i++) {
		for (int j = 0; j < am_col - i - 1; j++) {
			if (times[j] > times[j + 1]) {
				// меняем элементы местами
				int temp = times[j];
				bool* vec_temp = new bool[n];
				for (int count = 0; count < n; count++) {
					vec_temp[count] = 0;
				}
				rw.readColumn(j, vec_temp, n); // с номером j хранится в temp

				times[j] = times[j + 1];
				bool* vec_jp1 = new bool[n];
				for (int count = 0; count < n; count++) {
					vec_jp1[count] = 0;
				}
				rw.readColumn(j + 1, vec_jp1, n);
				rw.changeColumn(j, vec_jp1, n); // j <- j + 1

				times[j + 1] = temp;
				rw.changeColumn(j + 1, vec_temp, n); // j+1 <- temp=j
				delete[] vec_temp;
				delete[]  vec_jp1;
			}
		}
		cout << i << " out of " << am_col - 1 << '\n';
	}

	/*
	int n = vertexes + lines + triangles + tetrahedrons;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (times[j] > times[j + 1]) {
				// меняем элементы местами
				int temp = times[j];
				vector<bool> vec_temp;
				rw.readColumn(j, vec_temp, size); // с номером j хранится в temp

				times[j] = times[j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(j + 1, vec_jp1, size);
				rw.changeColumn(j, vec_jp1, size); // j <- j + 1

				times[j + 1] = temp;
				rw.changeColumn(j + 1, vec_temp, size); // j+1 <- temp=j


				//надо поменять местами элементы vertexes + j, vertexes + j+1 в каждом столбце
				// меняем элементы местами
				for (int col_ind = 0; i < n; i++) {
					vector<bool> col;
					rw.readColumn(col_ind, col, size);

					bool temp_bool = col[j];
					col[j] = col[j + 1];
					col[j + 1] = temp_bool;

					rw.changeColumn(col_ind, col, size);
				}
			}
		}
	}*/



	/*
	int n = vertexes + lines + triangles + tetrahedrons;

	for (int i = 0; i < lines - 1; i++) {
		for (int j = 0; j <lines - i - 1; j++) {
			if (times[vertexes + j] > times[vertexes + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + j] = times[vertexes + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + j, vec_jp1, size); // j <- j + 1

				times[vertexes + j + 1] = temp;
				rw.changeColumn(vertexes + j + 1, vec_temp, size); // j+1 <- temp=j

				//надо поменять местами элементы vertexes + j, vertexes + j+1 в каждом столбце
				// меняем элементы местами
				for (int col_ind = 0; i < n; i++) {
					vector<bool> col;
					rw.readColumn(col_ind, col, size);

					bool temp_bool = col[j];
					col[j] = col[j + 1];
					col[j + 1] = temp_bool;

					rw.changeColumn(col_ind, col, size);
				}
			}
		}
	}

	for (int i = 0; i < triangles - 1; i++) {
		for (int j = 0; j < triangles - i -1; j++) {
			if (times[vertexes + lines + j] > times[vertexes + lines + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + lines + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + lines + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + lines + j] = times[vertexes + lines + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + lines + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + lines + j, vec_jp1, size); // j <- j + 1

				times[vertexes + lines + j + 1] = temp;
				rw.changeColumn(vertexes + lines + j + 1, vec_temp, size); // j+1 <- temp=j


				for (int col_ind = 0; i < n; i++) {
					vector<bool> col;
					rw.readColumn(col_ind, col, size);

					bool temp_bool = col[lines + j];
					col[lines + j] = col[lines + j + 1];
					col[lines + j + 1] = temp_bool;

					rw.changeColumn(col_ind, col, size);
				}
			}
		}
	}

	for (int i = 0; i < tetrahedrons - 1; i++) {
		for (int j = 0; j < tetrahedrons - i - 1; j++) {
			if (times[vertexes + lines + triangles + j] > times[vertexes + lines + triangles + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + lines + triangles + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + lines + triangles + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + lines + triangles + j] = times[vertexes + lines + triangles + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + lines + triangles + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + lines + triangles + j, vec_jp1, size); // j <- j + 1

				times[vertexes + lines + triangles + j + 1] = temp;
				rw.changeColumn(vertexes + lines + triangles + j + 1, vec_temp, size); // j+1 <- temp=j

				for (int col_ind = 0; i < n; i++) {
					vector<bool> col;
					rw.readColumn(col_ind, col, size);

					bool temp_bool = col[ lines + triangles + j];
					col[ lines + triangles + j] = col[lines + triangles + j + 1];
					col[ lines + triangles + j + 1] = temp_bool;

					rw.changeColumn(col_ind, col, size);
				}
			}
		}
	}
	*/


	/*
	int n = vertexes + lines + triangles + tetrahedrons;

	for (int i = 0; i < lines - 1; i++) {
		for (int j = 0; j < lines - i - 1; j++) {
			if (times[vertexes + j] > times[vertexes + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + j] = times[vertexes + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + j, vec_jp1, size); // j <- j + 1

				times[vertexes + j + 1] = temp;
				rw.changeColumn(vertexes + j + 1, vec_temp, size); // j+1 <- temp=j


			}
		}
	}

	for (int i = 0; i < triangles - 1; i++) {
		for (int j = 0; j < triangles - i - 1; j++) {
			if (times[vertexes + lines + j] > times[vertexes + lines + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + lines + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + lines + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + lines + j] = times[vertexes + lines + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + lines + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + lines + j, vec_jp1, size); // j <- j + 1

				times[vertexes + lines + j + 1] = temp;
				rw.changeColumn(vertexes + lines + j + 1, vec_temp, size); // j+1 <- temp=j


			}
		}
	}

	for (int i = 0; i < tetrahedrons - 1; i++) {
		for (int j = 0; j < tetrahedrons - i - 1; j++) {
			if (times[vertexes + lines + triangles + j] > times[vertexes + lines + triangles + j + 1]) {
				// меняем элементы местами
				int temp = times[vertexes + lines + triangles + j];
				vector<bool> vec_temp;
				rw.readColumn(vertexes + lines + triangles + j, vec_temp, size); // с номером j хранится в temp

				times[vertexes + lines + triangles + j] = times[vertexes + lines + triangles + j + 1];
				vector<bool> vec_jp1;
				rw.readColumn(vertexes + lines + triangles + j + 1, vec_jp1, size);
				rw.changeColumn(vertexes + lines + triangles + j, vec_jp1, size); // j <- j + 1

				times[vertexes + lines + triangles + j + 1] = temp;
				rw.changeColumn(vertexes + lines + triangles + j + 1, vec_temp, size); // j+1 <- temp=j

			}
		}
	}
	*/


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
	for (int j = 0; j < am_col; j++) {
		bool* j_col = new bool[n];
		for (int count = 0; count < n; count++) {
			j_col[count] = 0;
		}
		rw.readColumn(j, j_col, n);
		bool exit = false;
		while (!exit) { //если за один проход не нашелся ни один подходящий столбец, то выходим
			exit = true;
			for (int i = 0; i < j; i++) {
				bool* i_col = new bool[n];
				for (int count = 0; count < n; count++) {
					i_col[count] = 0;
				}
				rw.readColumn(i, i_col, n);

				int low_j = low(j_col);
				int low_i = low(i_col);

				if (low_i != -1 && low_i == low_j) {
					exit = false; //если есть хотя бы один подходящий столбец
					add(i_col, j_col);
					//пошаговый вывод матрицы для проверки
					/*cout << "\n\n";
					for (int i = 0; i < matrix.size(); i++) {
						for (int j = 0; j < matrix[i].size(); j++) {
							cout << matrix[i][j] << ' ';
						}
						cout << '\n';
					}*/
				}
				delete[] i_col;
			}
		}

		rw.changeColumn(j, j_col, n);

		delete[] j_col;
	}
}



void reading_intervals() {
	int am_col = vertexes + lines + triangles + tetrahedrons;
	vector<int> lows(n); //вектор всех нижних граней
	for (int i = 0; i < am_col; i++) {
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

	/*
	for (int i = 0; i < n; i++) {
		if (lows[i] != -2) { //уже парный
			if (lows[i] == -1) { //undefined
				//найти такое k, что low(k) == i. Тогда k - конец, i - старт
				bool is_paired = false;
				for (int k = i + 1; k < n; k++) {
					if (lows[k] == i) {
						//intervals.push_back({ times[i], times[k] });
						rw.writeInterval(times[i], times[k]);
						lows[i] = -2; lows[k] = -2;
						cout << i << ' ' << k << '\n';
						is_paired = true;
						break;
					}
				}
				if (!is_paired) {
					//intervals.push_back({ times[i], INT32_MAX });  // [i; +inf) - интервал
					rw.writeInterval(times[i], INT32_MAX);
					lows[i] = -2;
					cout << i << '\n';
				}
			}
			else {
				//надо найти такое j, что j = low(i). Тогда j - старт, i - конец
				///for (int j = 0; j < n; j++) {
				//for (int j = i + 1; j < n; j++) {
				for (int j = 0; j < i; j++) {
					if (j == lows[i]) {
						//intervals.push_back({ times[j], times[i] });
						rw.writeInterval(times[j], times[i]);
						lows[i] = -2; lows[j] = -2;
						cout << j << ' ' << i << '\n';
						break;
					}
				}
			}
		}
	}*/
}

void process() {
	vertexes = 3738;

	cout << "Reading matrix \n";
	rw.readMatrix(m_space, vertexes);


	for (int i = 0; i < vertexes; i++) {
		times.push_back(0);
	}

	amount_of_lines();
	//amount_of_triangles();
	//amount_of_tetrahedrons();

	n = vertexes + lines + triangles + tetrahedrons; //размер столбцов
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
	//create_dim2();
	//create_dim3();

	//rw.outMatrix(vertexes + lines + triangles + tetrahedrons, n);
	//rw.outMatrix(vertexes + lines + triangles + tetrahedrons);
	


	cout << "sorting\n";

	sortMatrix(times, vertexes + lines + triangles + tetrahedrons);
	//rw.outMatrix(vertexes + lines + triangles + tetrahedrons, n);

	cout << "reducing\n";
	reducing();

	//rw.outMatrix(vertexes + lines + triangles + tetrahedrons);
	//rw.outMatrix(vertexes + lines + triangles + tetrahedrons, n);


	cout << "reading intervals\n\n\n";

	reading_intervals();
}



int main()
{
	process();
	/*vector<bool> res;
	rw.readColumn(1, res, 1);
	for (int i = 0; i < res.size(); i++)
		cout << res[i] << ' ';*/
}
