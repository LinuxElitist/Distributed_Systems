#pragma once

#include <vector>
#include <string>
#include <cstdio>

struct Article
{
public:
  std::string type, orig, org, content;

  /* Split based on ';'. Should be 4 parts */
  Article(std::string art);

  /* Returns all possible unique combinations of super categories */
  std::vector<Article> getSuperCats() const;

  std::string getWhole() const;
  void print() const;

private:

  void stringsplit(
			  const std::vector<std::string> &in, int i, std::string prev,
			  std::vector<Article> &outs) const;
};

/* Comparator struct used by set */
bool operator<(const Article &lhs, const Article &rhs);
