#include "bstree.h"

#include <QByteArray>
#include <QQueue>
#include <algorithm>

BSTree::~BSTree() {
    clear();
}

void BSTree::clear() {
    destroy(m_root);
    m_root = nullptr;
}

char* BSTree::dupUtf8(const QString& s) {
    QByteArray arr = s.toUtf8();
    char* p = new char[arr.size() + 1];
    std::memcpy(p, arr.constData(), arr.size() + 1);
    return p;
}

QString BSTree::fromUtf8(const char* s) {
    return QString::fromUtf8(s ? s : "");
}

int BSTree::cmp(const char* a, const QString& b) {
    return QString::fromUtf8(a).compare(b, Qt::CaseSensitive);
}

void BSTree::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete[] n->data;
    delete n;
}

bool BSTree::insert(const QString& value) {
    if (value.trimmed().isEmpty()) return false;
    bool inserted = false;
    m_root = insert(m_root, value.trimmed(), inserted);
    return inserted;
}

Node* BSTree::insert(Node* n, const QString& value, bool& inserted) {
    if (!n) {
        inserted = true;
        return new Node(dupUtf8(value));
    }
    int c = cmp(n->data, value);
    if (c > 0) {
        n->left = insert(n->left, value, inserted);
    } else if (c < 0) {
        n->right = insert(n->right, value, inserted);
    } else {
        inserted = false;
    }
    return n;
}

bool BSTree::contains(const QString& value) const {
    return contains(m_root, value.trimmed());
}

bool BSTree::contains(Node* n, const QString& value) {
    if (!n) return false;
    int c = cmp(n->data, value);
    if (c == 0) return true;
    if (c > 0) return contains(n->left, value);
    return contains(n->right, value);
}

Node* BSTree::minNode(Node* n) {
    while (n && n->left) n = n->left;
    return n;
}

bool BSTree::remove(const QString& value) {
    bool removed = false;
    m_root = remove(m_root, value.trimmed(), removed);
    return removed;
}

Node* BSTree::remove(Node* n, const QString& value, bool& removed) {
    if (!n) return nullptr;

    int c = cmp(n->data, value);
    if (c > 0) {
        n->left = remove(n->left, value, removed);
    } else if (c < 0) {
        n->right = remove(n->right, value, removed);
    } else {
        removed = true;

        if (!n->left) {
            Node* r = n->right;
            delete[] n->data;
            delete n;
            return r;
        }
        if (!n->right) {
            Node* l = n->left;
            delete[] n->data;
            delete n;
            return l;
        }

        Node* s = minNode(n->right);
        std::swap(n->data, s->data);
        bool dummy = false;
        n->right = remove(n->right, fromUtf8(s->data), dummy);
    }
    return n;
}

int BSTree::countStartsWith(QChar ch) const {
    return countStartsWith(m_root, ch);
}

int BSTree::countStartsWith(Node* n, QChar ch) {
    if (!n) return 0;
    int cnt = 0;
    const QString s = fromUtf8(n->data);
    if (!s.isEmpty() && s.startsWith(ch, Qt::CaseSensitive)) cnt = 1;
    return cnt + countStartsWith(n->left, ch) + countStartsWith(n->right, ch);
}

int BSTree::height() const {
    return height(m_root);
}

int BSTree::height(Node* n) {
    if (!n) return 0;
    return 1 + std::max(height(n->left), height(n->right));
}

int BSTree::countNodes() const {
    return countNodes(m_root);
}

int BSTree::countNodes(Node* n) {
    if (!n) return 0;
    return 1 + countNodes(n->left) + countNodes(n->right);
}

void BSTree::preorder(Node* n, QStringList& out) {
    if (!n) return;
    out << fromUtf8(n->data);
    preorder(n->left, out);
    preorder(n->right, out);
}

void BSTree::inorder(Node* n, QStringList& out) {
    if (!n) return;
    inorder(n->left, out);
    out << fromUtf8(n->data);
    inorder(n->right, out);
}

void BSTree::postorder(Node* n, QStringList& out) {
    if (!n) return;
    postorder(n->left, out);
    postorder(n->right, out);
    out << fromUtf8(n->data);
}

QString BSTree::preorder() const {
    QStringList out;
    preorder(m_root, out);
    return out.join(" ");
}

QString BSTree::inorder() const {
    QStringList out;
    inorder(m_root, out);
    return out.join(" ");
}

QString BSTree::postorder() const {
    QStringList out;
    postorder(m_root, out);
    return out.join(" ");
}

QString BSTree::verticalPrint() const {
    if (!m_root) return "(пусто)";

    QStringList lines;
    QQueue<Node*> q;
    q.enqueue(m_root);

    while (!q.isEmpty()) {
        int levelSize = q.size();
        QStringList level;
        bool hasReal = false;

        for (int i = 0; i < levelSize; ++i) {
            Node* n = q.dequeue();
            if (n) {
                hasReal = true;
                level << fromUtf8(n->data);
                q.enqueue(n->left);
                q.enqueue(n->right);
            } else {
                level << ".";
            }
        }

        lines << level.join(" ");
        if (!hasReal) break;
    }

    return lines.join("\n");
}

void BSTree::horizontalPrint(Node* n, int depth, QStringList& out) {
    if (!n) return;
    horizontalPrint(n->right, depth + 1, out);
    out << QString(depth * 4, ' ') + fromUtf8(n->data);
    horizontalPrint(n->left, depth + 1, out);
}

QString BSTree::horizontalPrint() const {
    if (!m_root) return "(пусто)";
    QStringList out;
    horizontalPrint(m_root, 0, out);
    return out.join("\n");
}

void BSTree::collectInorder(Node* n, QVector<QString>& out) {
    if (!n) return;
    collectInorder(n->left, out);
    out.push_back(fromUtf8(n->data));
    collectInorder(n->right, out);
}

Node* BSTree::buildBalanced(const QVector<QString>& vals, int l, int r) {
    if (l > r) return nullptr;
    int m = (l + r) / 2;
    Node* n = new Node(dupUtf8(vals[m]));
    n->left = buildBalanced(vals, l, m - 1);
    n->right = buildBalanced(vals, m + 1, r);
    return n;
}

void BSTree::balance() {
    QVector<QString> vals;
    vals.reserve(countNodes());
    collectInorder(m_root, vals);
    clear();
    m_root = buildBalanced(vals, 0, vals.size() - 1);
}