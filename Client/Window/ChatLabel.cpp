#include "ChatLabel.h"

#include <QDebug>

void ChatLabel::fullWrap()
{
    if (!isWrapped) {
        originalText = QLabel::text();
        isWrapped = true;
    }
    QString text = originalText;
    //QStringList list = originalText.split();

    int width = QWidget::width();
    int charsPerLine = width / QWidget::font().pointSize();

    qDebug() << text << " " << text.size();
    qDebug() << charsPerLine;

    if (text.size() <= charsPerLine) {
        return;
    }

    for (int i = charsPerLine; i < text.size(); i += charsPerLine) {
        text.insert(i, ' ');
    }
    QLabel::setText(text);
}
