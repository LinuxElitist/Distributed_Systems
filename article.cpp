#include "article.h"
#include <cstdio>
#include <tuple>

using std::vector;
using std::string;
using std::tie;


vector<string> split(string s, char delim)
{
  vector<string> strings;
  string tmp = "";
  for (int i = 0; i < s.length(); i++)
    {
      if (s[i] == delim)
	{
	  strings.push_back(tmp);
	  tmp = "";
	}
      else
	{
	  tmp += s[i];
	}
    }

  strings.push_back(tmp);

  return strings;
}

bool operator<(const Article &lhs, const Article &rhs)
{
  return tie(lhs.type, lhs.orig, lhs.org) < tie(rhs.type, rhs.orig, rhs.org);
}

/* Implementation */
/* Split based on ';'. Should be 4 parts */
Article::Article(string art)
{
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

vector<Article> Article::getSuperCats() const
{
  vector<string> in;
  vector<Article> out;

  in.push_back(type);
  in.push_back(orig);
  in.push_back(org);

  stringsplit(in, 0, "", out);
  return out;
}


string Article::getWhole() const
{
  return type + ";" + orig + ";"+ org + ";"+ content;
}

void Article::print() const
{
  printf("%s\n", getWhole().c_str());
}


void Article::stringsplit(
				 const vector<string> &in, int i, string prev,
				 vector<Article> &outs) const
{
  for (i; i < in.size(); i++)
    {
      stringsplit(in, i + 1, prev + in[i] + ";", outs);
      prev += ";";
    }

  outs.push_back(Article(prev));
}
