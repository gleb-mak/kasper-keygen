#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

void Generate_4_offsets(const vector<int>::iterator src, vector<int>& dst) {
	int num_1 = *src;
	int num_2 = *(src + 1);
	int num_3 = *(src + 2);
	vector<int> result(4);
	result[0] = num_1 >> 2;
	result[1] = ((num_1 & 3) << 4) | (num_2 >> 4);
	result[2] = ((num_2 & 0xf) << 2) | (num_3 >> 6);
	result[3] = num_3 & 0x3f;
	copy(begin(result), end(result), back_inserter(dst));
}

void Offsets_to_key(const vector<int>& offsets, string& key) {
	string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (int tmp : offsets) {
		key += base[tmp];
	}
}

int main() {
    string email;
    cout << "Enter your email:\n";
    cin >> email;
    int len_email = email.length();
    string key;
    vector<int> offsets;
    vector<int> generated = {0x0, 0x0};
   // первые 2 числа - любые
    //третье должно равнятся 2 в степени len_email, если len_email<8
    if (len_email < 8) {
    	generated.push_back(pow(2, len_email));
    }
    //3е должно заканчиваться на две единицы в бинарной записи, если len_email >= 8
    else {
    	generated.push_back(0x3);
    	generated.push_back((len_email - 8) | 0xc0); //4ое число
    }
    copy(begin(email), end(email), back_inserter(generated));//дальше должен идти сам  email
    generated.push_back(0x1f);//и в конце 1F, чтобы успешно выйти из функции
    for (int i = 0; i < generated.size() % 3; ++i) { //т.к. длина key кратна 4, то длина generated должна быть кратна 3
    	generated.push_back(0x0);// дополняем generated нулями, чтобы длина generated была кратна 3
    }
    for (int i = 0; i < generated.size() / 3; ++i) { //для каждой троки из generated получаем 4 смещения и записываем их в конец offsets
    	Generate_4_offsets(begin(generated) + 3 * i, offsets);
    }
    Offsets_to_key(offsets, key);// из смещений получаем сами буквы
    cout << "Your key: " << key;
    return 0;
}
