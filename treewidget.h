#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPointF>
#include <QSet>

class BSTree;
struct Node;
struct StepInfo;

class TreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void setTree(BSTree* tree);
    void setDemoMode(bool enabled);
    void setCurrentStep(int step);
    int currentStep() const { return m_currentStep; }
    int totalSteps() const { return m_totalSteps; }

signals:
    void stepChanged(int step);
    void demoFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    BSTree* m_tree;
    QMap<const Node*, QPointF> m_pos;
    bool m_demoMode = false;
    int m_currentStep = -1;
    int m_totalSteps = 0;
    QSet<const Node*> m_highlightedNodes;
    QSet<const Node*> m_modifiedNodes;
    const Node* m_currentNode = nullptr;
    QString m_stepDescription;
    int m_operationType = 0;

    void updateLayout();
    void assignPos(const Node* n, int depth, int& idx, int stepX, int stepY, int margin);
    void drawEdges(QPainter& painter, const Node* node);
    void drawNodes(QPainter& painter, const Node* node);
    void updateFromStep();
};

#endif