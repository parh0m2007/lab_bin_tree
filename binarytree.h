#pragma once

#include <QString>
#include <QStringList>
#include <QVector>

struct Node {
    char* data = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(char* s) : data(s) {}
};

class BSTree {
public:
    BSTree() = default;
    ~BSTree();

    Node* root() const { return m_root; }

    void clear();
    bool insert(const QString& value);
    bool remove(const QString& value);
    bool contains(const QString& value) const;

    int countStartsWith(QChar ch) const;
    int height() const;
    int countNodes() const;

    QString preorder() const;
    QString inorder() const;
    QString postorder() const;

    QString verticalPrint() const;
    QString horizontalPrint() const;

    void balance();

private:
    Node* m_root = nullptr;

    static char* dupUtf8(const QString& s);
    static QString fromUtf8(const char* s);
    static int cmp(const char* a, const QString& b);

    static void destroy(Node* n);
    static Node* insert(Node* n, const QString& value, bool& inserted);
    static Node* remove(Node* n, const QString& value, bool& removed);
    static Node* minNode(Node* n);
    static bool contains(Node* n, const QString& value);

    static void preorder(Node* n, QStringList& out);
    static void inorder(Node* n, QStringList& out);
    static void postorder(Node* n, QStringList& out);

    static int height(Node* n);
    static int countNodes(Node* n);
    static int countStartsWith(Node* n, QChar ch);

    static void collectInorder(Node* n, QVector<QString>& out);
    static Node* buildBalanced(const QVector<QString>& vals, int l, int r);

    static void horizontalPrint(Node* n, int depth, QStringList& out);
};