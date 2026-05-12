#include "mainwindow.h"
#include "binarytree.h"
#include "treewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QLabel>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* splitter = new QSplitter(this);

    auto* left = new QWidget(splitter);
    auto* leftLayout = new QVBoxLayout(left);

    auto* form = new QFormLayout();
    m_valueEdit = new QLineEdit();
    m_prefixEdit = new QLineEdit();
    m_valueEdit->setPlaceholderText("например: apple");
    m_prefixEdit->setPlaceholderText("например: a");
    form->addRow("Элемент:", m_valueEdit);
    form->addRow("Символ:", m_prefixEdit);
    leftLayout->addLayout(form);

    auto* btnInsert = new QPushButton("Вставить");
    auto* btnRemove = new QPushButton("Удалить");
    auto* btnSearch = new QPushButton("Поиск");
    auto* btnCount = new QPushButton("Посчитать по символу");
    auto* btnPre = new QPushButton("Прямой обход");
    auto* btnIn = new QPushButton("Симметричный обход");
    auto* btnPost = new QPushButton("Обратный обход");
    auto* btnVert = new QPushButton("Вертикальная печать");
    auto* btnHoriz = new QPushButton("Горизонтальная печать");
    auto* btnBalance = new QPushButton("Балансировка");
    auto* btnSample = new QPushButton("Пример дерева");
    auto* btnClear = new QPushButton("Очистить");

    leftLayout->addWidget(btnInsert);
    leftLayout->addWidget(btnRemove);
    leftLayout->addWidget(btnSearch);
    leftLayout->addWidget(btnCount);
    leftLayout->addWidget(btnPre);
    leftLayout->addWidget(btnIn);
    leftLayout->addWidget(btnPost);
    leftLayout->addWidget(btnVert);
    leftLayout->addWidget(btnHoriz);
    leftLayout->addWidget(btnBalance);
    leftLayout->addWidget(btnSample);
    leftLayout->addWidget(btnClear);

    m_output = new QTextEdit();
    m_output->setReadOnly(true);
    leftLayout->addWidget(new QLabel("Вывод:"));
    leftLayout->addWidget(m_output, 1);

    m_tree = new BSTree();
    m_treeWidget = new TreeWidget(splitter);
    m_treeWidget->setTree(m_tree);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    auto* mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(splitter);

    connect(btnInsert, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        if (m_tree->insert(v)) log("Добавлен: " + v);
        else log("Элемент уже существует или пустой.");
        refresh();
    });

    connect(btnRemove, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        if (m_tree->remove(v)) log("Удалён: " + v);
        else log("Элемент не найден: " + v);
        refresh();
    });

    connect(btnSearch, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        log(QString("Поиск \"%1\": %2").arg(v, m_tree->contains(v) ? "найден" : "не найден"));
    });

    connect(btnCount, &QPushButton::clicked, this, [this] {
        const QString p = m_prefixEdit->text().trimmed();
        if (p.isEmpty()) return;
        int cnt = m_tree->countStartsWith(p[0]);
        log(QString("Элементов, начинающихся с \"%1\": %2").arg(p[0]).arg(cnt));
    });

    connect(btnPre, &QPushButton::clicked, this, [this] {
        log("Прямой обход: " + m_tree->preorder());
    });

    connect(btnIn, &QPushButton::clicked, this, [this] {
        log("Симметричный обход: " + m_tree->inorder());
    });

    connect(btnPost, &QPushButton::clicked, this, [this] {
        log("Обратный обход: " + m_tree->postorder());
    });

    connect(btnVert, &QPushButton::clicked, this, [this] {
        log("Вертикальная печать:\n" + m_tree->verticalPrint());
    });

    connect(btnHoriz, &QPushButton::clicked, this, [this] {
        log("Горизонтальная печать:\n" + m_tree->horizontalPrint());
    });

    connect(btnBalance, &QPushButton::clicked, this, [this] {
        m_tree->balance();
        log("Дерево сбалансировано.");
        refresh();
    });

    connect(btnClear, &QPushButton::clicked, this, [this] {
        m_tree->clear();
        log("Дерево очищено.");
        refresh();
    });

    connect(btnSample, &QPushButton::clicked, this, [this] {
        m_tree->clear();
        const QStringList sample = {
            "mango", "apple", "pear", "banana", "cherry",
            "kiwi", "plum", "grape", "orange", "lemon",
            "apricot", "date", "melon", "fig", "lime"
        };
        for (const auto& s : sample) m_tree->insert(s);
        log("Загружен пример дерева.");
        refresh();
    });

    setWindowTitle("ЛР1. Бинарные деревья. Вариант 25");
    resize(1200, 700);
}

void MainWindow::log(const QString& s) {
    m_output->append(s);
}

void MainWindow::refresh() {
    m_treeWidget->update();
}

MainWindow::~MainWindow() {
    delete m_tree;
}