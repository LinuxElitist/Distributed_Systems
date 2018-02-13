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
  for (int i = 0; i < str.length(); i++)
    {
      if (str[i] == tok)
	{
	  strings.push_back(tmp);
	  tmp = "";
	}
      else
	{
	  tmp += str[i];
	}
    }

  strings.push_back(tmp);

  return strings;
} 

string Article::fullString() const {
  return type + ";" + orig + ";"+ org + ";"+ content;
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


void Article::stringsplit(const vector<string> &in, int i, string prev,
			  vector<Article> &outs) const {

  for (i; i < in.size(); i++) {
      stringsplit(in, i + 1, prev + in[i] + ";", outs);
      prev += ";";
    }

  outs.push_back(Article(prev));
}

void Article::print() const {
  printf("%s\n", fullString().c_str());
}


