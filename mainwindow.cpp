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
#include <QCheckBox>
#include <QSlider>
#include <QGroupBox>
#include <QTimerEvent>

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

    // Добавляем элементы управления для демо-режима
    setupDemoControls(leftLayout);

    m_output = new QTextEdit();
    m_output->setReadOnly(true);
    leftLayout->addWidget(new QLabel("Вывод:"));
    leftLayout->addWidget(m_output, 1);

    m_tree = new BSTree();
    m_treeWidget = new TreeWidget(splitter);
    m_treeWidget->setTree(m_tree);
    
    connect(m_treeWidget, &TreeWidget::stepChanged, this, [this](int step) {
        updateDemoControls();
    });

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    auto* mainLayout = new QHBoxLayout(central);
    mainLayout->addWidget(splitter);

    connect(btnInsert, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        m_tree->setDemoMode(m_demoModeCheck->isChecked());
        if (m_tree->insert(v)) {
            if (m_demoModeCheck->isChecked()) {
                log("Демо: Вставка элемента \"" + v + "\"");
                m_treeWidget->setCurrentStep(0);
                updateDemoControls();
            } else {
                log("Добавлен: " + v);
            }
        } else {
            log("Элемент уже существует или пустой.");
        }
        refresh();
    });

    connect(btnRemove, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        m_tree->setDemoMode(m_demoModeCheck->isChecked());
        if (m_tree->remove(v)) {
            if (m_demoModeCheck->isChecked()) {
                log("Демо: Удаление элемента \"" + v + "\"");
                m_treeWidget->setCurrentStep(0);
                updateDemoControls();
            } else {
                log("Удалён: " + v);
            }
        } else {
            log("Элемент не найден: " + v);
        }
        refresh();
    });

    connect(btnSearch, &QPushButton::clicked, this, [this] {
        const QString v = m_valueEdit->text().trimmed();
        if (v.isEmpty()) return;
        m_tree->setDemoMode(m_demoModeCheck->isChecked());
        bool found = m_tree->contains(v);
        if (m_demoModeCheck->isChecked()) {
            log("Демо: Поиск элемента \"" + v + "\"");
            m_treeWidget->setCurrentStep(0);
            updateDemoControls();
        } else {
            log(QString("Поиск \"%1\": %2").arg(v, found ? "найден" : "не найден"));
        }
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
        m_tree->setDemoMode(m_demoModeCheck->isChecked());
        m_tree->balance();
        if (m_demoModeCheck->isChecked()) {
            log("Демо: Балансировка дерева");
            m_treeWidget->setCurrentStep(0);
            updateDemoControls();
        } else {
            log("Дерево сбалансировано.");
        }
        refresh();
    });

    connect(btnClear, &QPushButton::clicked, this, [this] {
        m_tree->clear();
        m_treeWidget->setDemoMode(false);
        m_demoModeCheck->setChecked(false);
        log("Дерево очищено.");
        refresh();
    });

    connect(btnSample, &QPushButton::clicked, this, [this] {
        m_tree->clear();
        m_treeWidget->setDemoMode(false);
        m_demoModeCheck->setChecked(false);
        const QStringList sample = {
            "mango", "apple", "pear", "banana", "cherry",
            "kiwi", "plum", "grape", "orange", "lemon",
            "apricot", "date", "melon", "fig", "lime"
        };
        for (const auto& s : sample) m_tree->insert(s);
        log("Загружен пример дерева.");
        refresh();
    });

    setWindowTitle("ЛР1. Бинарные деревья. Вариант 25 (Qt6 + Демо режим)");
    resize(1400, 800);
}

void MainWindow::setupDemoControls(QVBoxLayout* layout) {
    auto* demoGroup = new QGroupBox("Демонстрационный режим");
    auto* demoLayout = new QVBoxLayout(demoGroup);
    
    m_demoModeCheck = new QCheckBox("Включить пошаговую демонстрацию");
    m_demoModeCheck->setChecked(false);
    connect(m_demoModeCheck, &QCheckBox::toggled, this, [this](bool checked) {
        m_tree->setDemoMode(checked);
        m_treeWidget->setDemoMode(checked);
        if (!checked) {
            stopTimer();
        }
        updateDemoControls();
        log(checked ? "Включен режим пошаговой демонстрации" : "Выключен режим демонстрации");
    });
    demoLayout->addWidget(m_demoModeCheck);
    
    // Панель управления шагами
    auto* stepControlLayout = new QHBoxLayout();
    
    m_prevStepBtn = new QPushButton("◀ Пред.");
    m_prevStepBtn->setEnabled(false);
    connect(m_prevStepBtn, &QPushButton::clicked, this, &MainWindow::goToPrevStep);
    stepControlLayout->addWidget(m_prevStepBtn);
    
    m_playBtn = new QPushButton("▶ Старт");
    m_playBtn->setEnabled(false);
    connect(m_playBtn, &QPushButton::clicked, this, &MainWindow::togglePlayPause);
    stepControlLayout->addWidget(m_playBtn);
    
    m_nextStepBtn = new QPushButton("След. ▶");
    m_nextStepBtn->setEnabled(false);
    connect(m_nextStepBtn, &QPushButton::clicked, this, &MainWindow::goToNextStep);
    stepControlLayout->addWidget(m_nextStepBtn);
    
    demoLayout->addLayout(stepControlLayout);
    
    // Слайдер для навигации по шагам
    m_stepSlider = new QSlider(Qt::Horizontal);
    m_stepSlider->setEnabled(false);
    m_stepSlider->setMinimum(0);
    m_stepSlider->setMaximum(0);
    m_stepSlider->setValue(0);
    connect(m_stepSlider, &QSlider::valueChanged, this, [this](int value) {
        if (m_isPlaying) {
            togglePlayPause();
        }
        m_treeWidget->setCurrentStep(value);
    });
    demoLayout->addWidget(m_stepSlider);
    
    // Метка текущего шага
    m_stepLabel = new QLabel("Шаг: 0 / 0");
    m_stepLabel->setAlignment(Qt::AlignCenter);
    demoLayout->addWidget(m_stepLabel);
    
    layout->addWidget(demoGroup);
}

void MainWindow::updateDemoControls() {
    bool hasSteps = m_tree->totalSteps() > 0;
    int currentStep = m_treeWidget->currentStep();
    int totalSteps = m_treeWidget->totalSteps();
    
    m_prevStepBtn->setEnabled(hasSteps && currentStep > 0);
    m_nextStepBtn->setEnabled(hasSteps && currentStep < totalSteps - 1);
    m_playBtn->setEnabled(hasSteps);
    m_stepSlider->setEnabled(hasSteps);
    
    m_stepSlider->setMaximum(qMax(0, totalSteps - 1));
    m_stepSlider->setValue(qBound(0, currentStep + 1, qMax(0, totalSteps - 1)));
    
    m_stepLabel->setText(QString("Шаг: %1 / %2").arg(currentStep + 1).arg(totalSteps));
    
    if (currentStep >= totalSteps - 1 && m_isPlaying) {
        togglePlayPause();
    }
    
    m_playBtn->setText(m_isPlaying ? "⏸ Пауза" : "▶ Старт");
}

void MainWindow::goToNextStep() {
    int nextStep = m_treeWidget->currentStep() + 1;
    if (nextStep < m_tree->totalSteps()) {
        m_treeWidget->setCurrentStep(nextStep);
        updateDemoControls();
    }
}

void MainWindow::goToPrevStep() {
    int prevStep = m_treeWidget->currentStep() - 1;
    if (prevStep >= 0) {
        m_treeWidget->setCurrentStep(prevStep);
        updateDemoControls();
    }
}

void MainWindow::togglePlayPause() {
    if (m_isPlaying) {
        stopTimer();
    } else {
        m_isPlaying = true;
        m_playBtn->setText("⏸ Пауза");
        m_playTimerId = startTimer(800); // 800 мс между шагами
        goToNextStep();
    }
}

void MainWindow::stopTimer() {
    if (m_playTimerId != -1) {
        killTimer(m_playTimerId);
        m_playTimerId = -1;
    }
    m_isPlaying = false;
    m_playBtn->setText("▶ Старт");
}

void MainWindow::timerEvent(QTimerEvent* event) {
    if (event->timerId() == m_playTimerId) {
        if (m_treeWidget->currentStep() >= m_tree->totalSteps() - 1) {
            stopTimer();
        } else {
            goToNextStep();
        }
    }
    QMainWindow::timerEvent(event);
}

void MainWindow::log(const QString& s) {
    m_output->append(s);
}

void MainWindow::refresh() {
    m_treeWidget->update();
    updateDemoControls();
}

MainWindow::~MainWindow() {
    stopTimer();
    delete m_tree;
}