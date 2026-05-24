#include "binarytree.h"

#include <QByteArray>
#include <QQueue>
#include <algorithm>
#include <cstring>

BSTree::~BSTree() {
    clear();
}

void BSTree::clear() {
    destroy(m_root);
    m_root = nullptr;
    clearSteps();
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
    
    if (m_demoMode) {
        clearSteps();
        bool inserted = false;
        m_root = insertDemo(m_root, value.trimmed(), inserted);
        return inserted;
    } else {
        bool inserted = false;
        m_root = insert(m_root, value.trimmed(), inserted);
        return inserted;
    }
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

Node* BSTree::insertDemo(Node* n, const QString& value, bool& inserted) {
    if (!n) {
        inserted = true;
        addStep(QString("Создан новый узел со значением \"%1\"").arg(value), 
                QVector<const Node*>(), QVector<const Node*>(), nullptr, 1);
        return new Node(dupUtf8(value));
    }
    
    int c = cmp(n->data, value);
    QVector<const Node*> highlighted = {n};
    
    if (c > 0) {
        addStep(QString("Сравниваем \"%1\" с \"%2\": идем влево").arg(QString::fromUtf8(n->data), value),
                highlighted, QVector<const Node*>(), n, 1);
        n->left = insertDemo(n->left, value, inserted);
    } else if (c < 0) {
        addStep(QString("Сравниваем \"%1\" с \"%2\": идем вправо").arg(QString::fromUtf8(n->data), value),
                highlighted, QVector<const Node*>(), n, 1);
        n->right = insertDemo(n->right, value, inserted);
    } else {
        inserted = false;
        addStep(QString("Элемент \"%1\" уже существует в дереве").arg(value),
                highlighted, QVector<const Node*>(), n, 1);
    }
    return n;
}

bool BSTree::contains(const QString& value) const {
    if (m_demoMode) {
        return const_cast<BSTree*>(this)->containsDemo(m_root, value.trimmed());
    }
    return contains(m_root, value.trimmed());
}

bool BSTree::contains(Node* n, const QString& value) {
    if (!n) return false;
    int c = cmp(n->data, value);
    if (c == 0) return true;
    if (c > 0) return contains(n->left, value);
    return contains(n->right, value);
}

bool BSTree::containsDemo(Node* n, const QString& value) {
    clearSteps();
    
    Node* current = n;
    int stepNum = 0;
    while (current) {
        QVector<const Node*> highlighted = {current};
        int c = cmp(current->data, value);
        
        if (c == 0) {
            addStep(QString("Найден элемент \"%1\"").arg(value),
                    highlighted, QVector<const Node*>(), current, 3);
            return true;
        }
        
        if (c > 0) {
            addStep(QString("Шаг %1: Сравниваем \"%2\" с \"%3\", идем влево")
                    .arg(++stepNum).arg(QString::fromUtf8(current->data), value),
                    highlighted, QVector<const Node*>(), current, 3);
            current = current->left;
        } else {
            addStep(QString("Шаг %1: Сравниваем \"%2\" с \"%3\", идем вправо")
                    .arg(++stepNum).arg(QString::fromUtf8(current->data), value),
                    highlighted, QVector<const Node*>(), current, 3);
            current = current->right;
        }
    }
    
    addStep(QString("Элемент \"%1\" не найден в дереве").arg(value),
            QVector<const Node*>(), QVector<const Node*>(), nullptr, 3);
    return false;
}

Node* BSTree::minNode(Node* n) {
    while (n && n->left) n = n->left;
    return n;
}

bool BSTree::remove(const QString& value) {
    if (m_demoMode) {
        clearSteps();
        bool removed = false;
        m_root = removeDemo(m_root, value.trimmed(), removed);
        return removed;
    } else {
        bool removed = false;
        m_root = remove(m_root, value.trimmed(), removed);
        return removed;
    }
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

Node* BSTree::removeDemo(Node* n, const QString& value, bool& removed) {
    if (!n) {
        addStep(QString("Узел \"%1\" не найден для удаления").arg(value),
                QVector<const Node*>(), QVector<const Node*>(), nullptr, 2);
        return nullptr;
    }

    int c = cmp(n->data, value);
    QVector<const Node*> highlighted = {n};

    if (c > 0) {
        addStep(QString("Ищем \"%1\": сравниваем с \"%2\", идем влево").arg(value, QString::fromUtf8(n->data)),
                highlighted, QVector<const Node*>(), n, 2);
        n->left = removeDemo(n->left, value, removed);
    } else if (c < 0) {
        addStep(QString("Ищем \"%1\": сравниваем с \"%2\", идем вправо").arg(value, QString::fromUtf8(n->data)),
                highlighted, QVector<const Node*>(), n, 2);
        n->right = removeDemo(n->right, value, removed);
    } else {
        removed = true;
        QVector<const Node*> modified = {n};
        addStep(QString("Найден узел для удаления: \"%1\"").arg(value),
                highlighted, modified, n, 2);

        if (!n->left) {
            addStep(QString("У узла нет левого потомка: заменяем правым поддеревом"),
                    highlighted, modified, n, 2);
            Node* r = n->right;
            delete[] n->data;
            delete n;
            return r;
        }
        if (!n->right) {
            addStep(QString("У узла нет правого потомка: заменяем левым поддеревом"),
                    highlighted, modified, n, 2);
            Node* l = n->left;
            delete[] n->data;
            delete n;
            return l;
        }

        Node* s = minNode(n->right);
        QVector<const Node*> modifiedTwo = {n, s};
        addStep(QString("Два потомка: находим минимальный в правом поддереве \"%1\"")
                .arg(QString::fromUtf8(s->data)),
                highlighted, modifiedTwo, s, 2);
        
        addStep(QString("Меняем данные: \"%1\" <-> \"%2\"")
                .arg(QString::fromUtf8(n->data), QString::fromUtf8(s->data)),
                highlighted, modifiedTwo, n, 2);
        
        std::swap(n->data, s->data);
        bool dummy = false;
        n->right = removeDemo(n->right, fromUtf8(s->data), dummy);
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
    if (m_demoMode) {
        clearSteps();
        balanceDemo();
    } else {
        QVector<QString> vals;
        vals.reserve(countNodes());
        collectInorder(m_root, vals);
        clear();
        m_root = buildBalanced(vals, 0, vals.size() - 1);
    }
}

void BSTree::balanceDemo() {
    addStep("Начинаем балансировку дерева", QVector<const Node*>(), QVector<const Node*>(), nullptr, 4);
    
    QVector<QString> vals;
    vals.reserve(countNodes());
    collectInorder(m_root, vals);
    
    addStep(QString("Получено %1 элементов при симметричном обходе").arg(vals.size()),
            QVector<const Node*>(), QVector<const Node*>(), nullptr, 4);
    
    clear();
    m_root = buildBalanced(vals, 0, vals.size() - 1);
    
    addStep("Дерево сбалансировано", QVector<const Node*>(), QVector<const Node*>(), nullptr, 4);
}

// Методы для демонстрационного режима
void BSTree::setDemoMode(bool enabled) {
    m_demoMode = enabled;
    if (!enabled) {
        clearSteps();
    }
}

void BSTree::setCurrentStep(int step) {
    m_currentStep = qBound(-1, step, m_steps.size() - 1);
}

const StepInfo& BSTree::getStep(int index) const {
    static StepInfo empty;
    if (index < 0 || index >= m_steps.size()) return empty;
    return m_steps[index];
}

void BSTree::clearSteps() {
    m_steps.clear();
    m_currentStep = -1;
}

void BSTree::addStep(const QString& desc, const QVector<const Node*>& highlighted,
                     const QVector<const Node*>& modified, const Node* current, int opType) {
    StepInfo step;
    step.description = desc;
    step.highlightedNodes = highlighted;
    step.modifiedNodes = modified;
    step.currentNode = current;
    step.operationType = opType;
    m_steps.append(step);
    m_currentStep = m_steps.size() - 1;
}