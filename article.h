/* Needed so that include header is included only once during compilation*/
#pragma once

#include <vector>
#include <string>
#include <cstdio>

struct Article
{
public:

  // varible declarations for the article when split
  //Eg:-"Sports;Someone;UMN;contents"

  std::string type, orig, org, content;
  /* Splitting based on ';'in 4 parts */
  Article(std::string art);

  /* Returns all possible unique combinations of super categories */
  std::vector<Article> getCategory() const;
  std::string fullString() const;
  void print() const;
  void stringsplit(const std::vector<std::string> &input, int j, std::string prev,
			  std::vector<Article> &outputs ) const;
};

/* Comparator struct used by set */
bool operator<(const Article &lhs, const Article &rhs);
