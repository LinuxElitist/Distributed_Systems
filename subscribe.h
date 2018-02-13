#include <string>
#include <set>

struct Article;

struct Subscriber {
    std::string ip;
    int port;
    mutable std::set<Article> articles;

    Subscriber(std::string ip, int port);

    //returns true if subscribed article is in art
    bool subscribed(const Article &art) const;

    /*  Remove the article */
    void unsubscribed(const Article &art) const;

    void print() const;
};

/* Comparator struct used by set */
bool operator<(const Subscriber &lhs, const Subscriber &rhs);
