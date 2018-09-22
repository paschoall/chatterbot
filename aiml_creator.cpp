#include <bits/stdc++.h>

#define OUTPUT_FILE "output.aiml"

#define debug if(true)

using namespace std;

struct variants {
	vector<string> v;
	variants(){}
	variants(string file) {
		ifstream in (file + ".set", ios::in);
		if(in.is_open() == false) throw "Error opening file";
		while(in.eof() == false) {
			string s;
			getline(in, s);
			v.push_back(s);
		}
		in.close();
	}

	string operator[](int pos) {
		if(pos > v.size()) {
			cerr << "Position " << pos << "is invalid\n";
			return "";
		}
		return v[pos];
	}

	int size() {
		return v.size();
	}
};
int flag = 0;

map<string, variants> var;

string create_pattern(string & que, string & ans) {
	string res;
	res += "\t<category>\n";
	res += "\t\t<pattern> ";
	

	res += que;
	res += "</pattern>\n\t\t<template> ";
	res += ans;
	res += "</template>\n\t</category>\n\n";

	return res;
}

void build_question(vector<string> & list, string & que, int p, string s, string & ans) {
	if(p == que.size()) {
		list.push_back(create_pattern(s, ans));
		if(flag == 0) {
			ans = "<srai> " + s + " </srai>";
			flag = 1;
		}
		return;
	}

	if(que[p] == '?') {
		string x;
		for(int i = p+1; que[i] != '?'; ++i) x += que[i];

		if(var.count(x) == 0) try {
			var[x] = variants(x);
		} catch(const char * message) {
			cout << message << endl;
			exit(0);
		}
		for(int i = 0; i < var[x].size(); ++i)
			build_question(list, que, p + x.size() + 2, s + var[x][i], ans);
	} else build_question(list, que, p+1, s + que[p], ans);
}
vector<string> processLine(string que, string ans) {
	vector<string> v;

	flag = 0;

	build_question(v, que, 0, "", ans);
	return v;
}

int main(int argc, char * argv[]) {
	string res = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<aiml>\n";

	if(argc != 2) {
		printf("Usage: %s <input text file>\n", argv[0]);
		return 0;
	}

	string file = string(argv[1]);

	ifstream in (file, ios::in);
	ofstream out (OUTPUT_FILE, ios::out);


	if(in.is_open() and out.is_open()) {
		while(in.eof() == false) {
			string question, answer;
			
			while(question.size() == 0 or question[0] == '\n') getline(in, question);
			while(answer.size() == 0 or answer[0] == '\n') getline(in, answer);
			debug cout << question << " " << answer << endl;
			vector<string> ret = processLine(question, answer);
			for(string s : ret) res += s;

		}
	} else {
		printf("Erro ao abrir os arquivos\n");
		return 0;
	}

	res += "</aiml>\n";

	out << res;
	in.close();
	out.close();
}