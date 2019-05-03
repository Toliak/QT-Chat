#pragma once

#include <QObject>
#include <QLabel>

class ChatLabel: public QLabel
{
Q_OBJECT

public:
    enum ChatLabelType
    {
        ChatLabelMessage,
        ChatLabelError,
    };

    explicit ChatLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QLabel(parent, f)
    {}

    explicit ChatLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QLabel(text, parent, f)
    {}

    /**
     * @brief Inserts spaces in very long words
     */
    void fullWrap();

private:
    ChatLabelType labelType = ChatLabelType::ChatLabelMessage;
    QString originalText;                   ///< Label text before wrapping
    bool isWrapped = false;                 ///< Is label wrapped by fullWrap
};