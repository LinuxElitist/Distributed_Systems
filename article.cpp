/*This source code mainly involves parsing the article 
and has helper functions for splitting string and check
the string categories */

/*
References : 

1) For comparing structs we employed the idea from here
https://stackoverflow.com/questions/5740310/no-operator-found-while-
comparing-structs-in-c

2) Efficient method of splitting strings and storing them -https://www.fluentcpp.com
/2017/04/21/how-to-split-a-string-in-c/ 

*/


#include "article.h"
#include <cstdio>
#include <tuple>

using std::string;
using std::tie; //using tie creating tuple for references
using std::vector;


bool operator<(const Article &lhs, const Article &rhs) {

    return tie(lhs.type, lhs.orig, lhs.org) < tie(rhs.type, rhs.orig, rhs.org);
}


vector<string> split(string str, char tok) {

    vector<string> strings;
    string tmp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == tok) {
            strings.push_back(tmp);
            tmp = "";
        } else {
            tmp += str[i];
        }
    }

    strings.push_back(tmp);

    return strings;
}

string Article::fullString() const {
    return type + ";" + orig + ";" + org + ";" + content;
}

Article::Article(string art) {
    auto strings = split(art, ';');

    int s = strings.size();

    type = orig = org = content = "";

    if (s > 0)
        type = strings[0];
    if (s > 1)
        orig = strings[1];
    if (s > 2)
        org = strings[2];
    if (s > 3)
        content = strings[3];
}

vector<Article> Article::getCategory() const {

    vector<Article> out;
    vector<string> in;

    in.push_back(type);
    in.push_back(orig);
    in.push_back(org);

    stringsplit(in, 0, "", out);
    return out;
}


void Article::stringsplit(const vector<string> &input, int j, string prev,
                          vector<Article> &outputs) const {

    for (j; j < input.size(); j++) {

        if (input[j] != "") {

            stringsplit(input, j + 1, prev + input[j] + ";", outputs);
        }
        prev += ";";
    }

    outputs.push_back(Article(prev));
}

void Article::print() const {
    printf("%s \n", fullString().c_str());
}

