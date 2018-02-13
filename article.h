/* Needed so that include header is included only once during compilation*/
#pragma once

#include <vector>
#include <string>
#include <cstdio>

struct Article {
public:
    std::string type, orig, org, content;

    /* Splitting based on ';'in 4 parts */
    Article(std::string art);

    void print() const;

    /* Returns all possible unique combinations of super categories */
    std::vector<Article> getCategory() const;

    std::string fullString() const;

private:

    void stringsplit(const std::vector<std::string> &in, int i, std::string prev,
                     std::vector<Article> &outs) const;
};

/* Comparator struct used by set */
bool operator<(const Article &lhs, const Article &rhs);
