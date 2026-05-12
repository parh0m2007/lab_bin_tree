#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPointF>

class BSTree;
struct Node;

class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void setTree(BSTree* tree);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    BSTree* m_tree;
    QMap<const Node*, QPointF> m_pos;

    void updateLayout();
    void assignPos(const Node* n, int depth, int& idx, int stepX, int stepY, int margin);
    void drawEdges(QPainter& painter, const Node* node);
    void drawNodes(QPainter& painter, const Node* node);
};

#endif