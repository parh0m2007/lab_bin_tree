#include "treewidget.h"
#include "binarytree.h"
#include <QPainter>
#include <QResizeEvent>
#include <algorithm>

TreeWidget::TreeWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(500, 400);
    setAutoFillBackground(true);
}

void TreeWidget::setTree(BSTree* tree) {
    m_tree = tree;
    updateLayout();
    update();
}

void TreeWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateLayout();
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
    QRectF r(c.x() - 18, c.y() - 18, 36, 36);
    p.setBrush(Qt::white);
    p.drawEllipse(r);
    p.drawText(r, Qt::AlignCenter, QString::fromUtf8(n->data));
    drawNodes(p, n->left);
    drawNodes(p, n->right);
}

void TreeWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QPen(Qt::black, 1.5));

    if (!m_tree || !m_tree->root()) {
        p.drawText(rect(), Qt::AlignCenter, "Дерево пусто");
        return;
    }

    updateLayout();
    drawEdges(p, m_tree->root());
    drawNodes(p, m_tree->root());
}