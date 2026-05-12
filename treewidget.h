#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPoint>

class BinaryTree;
struct Node;

class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void setTree(BinaryTree* tree);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    BinaryTree* m_tree;
    QMap<Node*, QPoint> m_pos;

    void updateLayout();
    void calculatePositions(Node* node, int x, int y, int offset);
    void drawEdges(QPainter& painter, Node* node);
    void drawNodes(QPainter& painter, Node* node);
};

#endif