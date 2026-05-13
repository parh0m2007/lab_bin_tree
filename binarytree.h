#pragma once

#include <QString>
#include <QStringList>
#include <QVector>
#include <QPointer>

struct Node {
    char* data = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(char* s) : data(s) {}
};

struct StepInfo {
    QString description;
    QVector<const Node*> highlightedNodes;
    QVector<const Node*> modifiedNodes;
    const Node* currentNode = nullptr;
    int operationType = 0; // 0=none, 1=insert, 2=remove, 3=search, 4=balance
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

    // Методы для демонстрационного режима
    void setDemoMode(bool enabled);
    bool isDemoMode() const { return m_demoMode; }
    void setCurrentStep(int step);
    int currentStep() const { return m_currentStep; }
    int totalSteps() const { return m_steps.size(); }
    const StepInfo& getStep(int index) const;
    void clearSteps();

private:
    Node* m_root = nullptr;
    bool m_demoMode = false;
    int m_currentStep = -1;
    QVector<StepInfo> m_steps;

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

    // Вспомогательные методы для демо-режима
    void addStep(const QString& desc, const QVector<const Node*>& highlighted, 
                 const QVector<const Node*>& modified, const Node* current, int opType);
    
    // Версии методов для демо-режима
    Node* insertDemo(Node* n, const QString& value, bool& inserted);
    Node* removeDemo(Node* n, const QString& value, bool& removed);
    bool containsDemo(Node* n, const QString& value);
    void balanceDemo();
};