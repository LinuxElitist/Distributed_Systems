#include "subscribe.h"
#include "article.h"
#include <tuple>

using std::string;
using std::tie;
using std::set;

Subscriber::Subscriber(string ip, int port) {
    this->ip = ip;
    this->port = port;
}

bool Subscriber::subscribed(const Article &art) const {
    if (articles.find(art) != articles.end()) {
        return true;
    }

    auto cats = art.getCategory();

    for (int i = 0; i < cats.size(); i++) {
        if (articles.find(Article(cats[i])) != articles.end()) {
            return true;
        }
    }

    return false;
}

void Subscriber::unsubscribed(const Article &art) const {
    auto art_it = articles.find(art);
    if (art_it != articles.end()) {
        articles.erase(art_it);
    }
}


bool operator<(const Subscriber &lhs, const Subscriber &rhs) {
    return tie(lhs.ip, lhs.port) < tie(rhs.ip, rhs.port);
}

void Subscriber::print() const {
    for (auto it = articles.begin(); it != articles.end(); ++it) {
        (*it).print();
    }
}
