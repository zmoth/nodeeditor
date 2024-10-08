#pragma once

#include <unordered_set>

#include "Definitions.hpp"
#include "Export.hpp"

#include <QUndoCommand>
#include <QtCore/QJsonObject>
#include <QtCore/QPointF>

namespace QtNodes {

class BasicGraphicsScene;

/// @brief 创建节点命令
class NODE_EDITOR_PUBLIC CreateCommand : public QUndoCommand
{
public:
    CreateCommand(BasicGraphicsScene *scene, QString name, QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene *_scene;
    NodeId _nodeId;
    QJsonObject _sceneJson;
};

/**
 * Selected scene objects are serialized and then removed from the scene.
 * The deleted elements could be restored in `undo`.
 */
class NODE_EDITOR_PUBLIC DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(BasicGraphicsScene *scene);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene *_scene;
    QJsonObject _sceneJson;
};

class NODE_EDITOR_PUBLIC CopyCommand : public QUndoCommand
{
public:
    explicit CopyCommand(BasicGraphicsScene *scene);
};

class NODE_EDITOR_PUBLIC PasteCommand : public QUndoCommand
{
public:
    PasteCommand(BasicGraphicsScene *scene, QPointF const &mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QJsonObject takeSceneJsonFromClipboard();
    QJsonObject makeNewNodeIdsInScene(QJsonObject const &sceneJson);

private:
    BasicGraphicsScene *_scene;
    QPointF const &_mouseScenePos;
    QJsonObject _newSceneJson;
};

/// @brief 删除连接命令
class NODE_EDITOR_PUBLIC DisconnectCommand : public QUndoCommand
{
public:
    DisconnectCommand(BasicGraphicsScene *scene, ConnectionId);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene *_scene;

    ConnectionId _connId;
};

/// @brief 连接命令
class NODE_EDITOR_PUBLIC ConnectCommand : public QUndoCommand
{
public:
    ConnectCommand(BasicGraphicsScene *scene, ConnectionId);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene *_scene;

    ConnectionId _connId;
};

class NODE_EDITOR_PUBLIC MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(BasicGraphicsScene *scene, QPointF const &diff);

    void undo() override;
    void redo() override;

    /**
     * A command ID is used in command compression. It must be an integer unique to
     * this command's class, or -1 if the command doesn't support compression.
     */
    int id() const override;

    /**
     * Several sequential movements could be merged into one command.
     */
    bool mergeWith(QUndoCommand const *c) override;

private:
    BasicGraphicsScene *_scene;
    std::unordered_set<NodeId> _selectedNodes;
    QPointF _diff;
};

} // namespace QtNodes
