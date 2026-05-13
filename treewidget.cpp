#include "treewidget.h"
#include "binarytree.h"
#include <QPainter>
#include <QResizeEvent>
#include <QMouseEvent>
#include <algorithm>

TreeWidget::TreeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(500, 400);
    setAutoFillBackground(true);
}

void TreeWidget::setTree(BSTree* tree) {
    m_tree = tree;
    if (!m_demoMode) {
        updateLayout();
    }
    update();
}

void TreeWidget::setDemoMode(bool enabled) {
    m_demoMode = enabled;
    if (!enabled) {
        m_currentStep = -1;
        m_totalSteps = 0;
        m_highlightedNodes.clear();
        m_modifiedNodes.clear();
        m_currentNode = nullptr;
        m_stepDescription.clear();
        updateLayout();
    }
    update();
}

void TreeWidget::setCurrentStep(int step) {
    if (!m_demoMode || !m_tree) return;
    
    m_currentStep = qBound(-1, step, m_tree->totalSteps() - 1);
    m_totalSteps = m_tree->totalSteps();
    
    if (m_currentStep >= 0 && m_currentStep < m_tree->totalSteps()) {
        const StepInfo& info = m_tree->getStep(m_currentStep);
        m_stepDescription = info.description;
        m_operationType = info.operationType;
        m_currentNode = info.currentNode;
        
        m_highlightedNodes.clear();
        for (const Node* n : info.highlightedNodes) {
            m_highlightedNodes.insert(n);
        }
        
        m_modifiedNodes.clear();
        for (const Node* n : info.modifiedNodes) {
            m_modifiedNodes.insert(n);
        }
    } else {
        m_highlightedNodes.clear();
        m_modifiedNodes.clear();
        m_currentNode = nullptr;
        m_stepDescription.clear();
    }
    
    emit stepChanged(m_currentStep);
    
    if (m_currentStep >= m_tree->totalSteps() - 1) {
        emit demoFinished();
    }
    
    update();
}

void TreeWidget::updateFromStep() {
    if (m_demoMode && m_tree && m_currentStep >= 0) {
        setCurrentStep(m_currentStep);
    }
}

void TreeWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (!m_demoMode) {
        updateLayout();
    }
}

void TreeWidget::mousePressEvent(QMouseEvent* event) {
    if (m_demoMode && event->button() == Qt::LeftButton) {
        // Клик для перехода к следующему шагу
        if (m_currentStep < m_totalSteps - 1) {
            setCurrentStep(m_currentStep + 1);
        }
    }
    QWidget::mousePressEvent(event);
}

void TreeWidget::assignPos(const Node* n, int depth, int& idx, int stepX, int stepY, int margin) {
    if (!n) return;
    assignPos(n->left, depth + 1, idx, stepX, stepY, margin);
    m_pos[n] = QPointF(margin + idx * stepX, margin + depth * stepY);
    ++idx;
    assignPos(n->right, depth + 1, idx, stepX, stepY, margin);
}

void TreeWidget::updateLayout() {
    m_pos.clear();
    if (!m_tree || !m_tree->root()) return;

    int count = std::max(1, m_tree->countNodes());
    int stepX = std::max(80, width() / count);
    int stepY = 70;
    int margin = 35;
    int idx = 0;
    assignPos(m_tree->root(), 0, idx, stepX, stepY, margin);
}

void TreeWidget::drawEdges(QPainter& p, const Node* n) {
    if (!n) return;
    const QPointF a = m_pos.value(n);
    if (n->left) {
        const QPointF b = m_pos.value(n->left);
        p.drawLine(a, b);
        drawEdges(p, n->left);
    }
    if (n->right) {
        const QPointF b = m_pos.value(n->right);
        p.drawLine(a, b);
        drawEdges(p, n->right);
    }
}

void TreeWidget::drawNodes(QPainter& p, const Node* n) {
    if (!n) return;
    const QPointF c = m_pos.value(n);
    QRectF r(c.x() - 20, c.y() - 20, 40, 40);
    
    // Определяем цвет узла
    QColor fillColor = Qt::white;
    QColor borderColor = Qt::black;
    int penWidth = 2;
    
    if (m_demoMode) {
        if (m_modifiedNodes.contains(n)) {
            fillColor = QColor(255, 200, 200); // Светло-красный для измененных
            borderColor = QColor(255, 0, 0);
            penWidth = 3;
        } else if (m_highlightedNodes.contains(n)) {
            fillColor = QColor(200, 255, 200); // Светло-зеленый для текущих
            borderColor = QColor(0, 200, 0);
            penWidth = 3;
        }
        
        if (n == m_currentNode) {
            borderColor = QColor(0, 0, 255);
            penWidth = 4;
        }
    }
    
    p.setBrush(fillColor);
    p.setPen(QPen(borderColor, penWidth));
    p.drawEllipse(r);
    
    p.setPen(Qt::black);
    p.drawText(r, Qt::AlignCenter, QString::fromUtf8(n->data));
    
    drawNodes(p, n->left);
    drawNodes(p, n->right);
}

void TreeWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    if (!m_tree || !m_tree->root()) {
        p.setPen(Qt::black);
        p.drawText(rect(), Qt::AlignCenter, "Дерево пусто");
        return;
    }

    if (!m_demoMode) {
        updateLayout();
    }
    
    p.setPen(QPen(Qt::black, 1.5));
    drawEdges(p, m_tree->root());
    drawNodes(p, m_tree->root());
    
    // Отображение информации о текущем шаге в демо-режиме
    if (m_demoMode && !m_stepDescription.isEmpty()) {
        QRect textRect = rect();
        textRect.setHeight(40);
        textRect.moveTop(height() - 45);
        
        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 11));
        p.drawText(textRect, Qt::AlignCenter, m_stepDescription);
        
        // Индикатор шага
        QString stepInfo = QString("Шаг %1 из %2").arg(m_currentStep + 1).arg(m_totalSteps);
        p.setFont(QFont("Arial", 10));
        p.drawText(10, height() - 10, stepInfo);
    }
}