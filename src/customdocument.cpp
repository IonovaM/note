#include "customDocument.h"
#include <QDebug>
#include <QGuiApplication>
#include <QTextCursor>
#include <QMessageBox>

CustomDocument::CustomDocument(QWidget *parent) : QTextEdit(parent)
{
    installEventFilter(this);
    viewport()->installEventFilter(this);
    setMouseTracking(true);
    setAttribute(Qt::WidgetAttribute::WA_Hover, true);
}

void CustomDocument::setDocumentPadding(int left, int top, int right, int bottom)
{
    setViewportMargins(left, top, right, bottom);
}

void CustomDocument::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    emit resized();
}

void CustomDocument::mouseMoveEvent(QMouseEvent *event)
{
    QTextEdit::mouseMoveEvent(event);
    emit mouseMoved();
}

bool CustomDocument::eventFilter(QObject *obj, QEvent *event)
{
    // qDebug() << event->type();

    if (event->type() == QEvent::HoverMove) {
        // if hovering and the control key is active, check whether the mouse is over a link
        if (QGuiApplication::keyboardModifiers() == Qt::ExtraButton24
            && getUrlUnderMouse().isValid()) {
            viewport()->setCursor(Qt::PointingHandCursor);
        } else {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    } else if (event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Control) {
            // check if mouse is over a link
            auto url = getUrlUnderMouse();
            viewport()->setCursor(url.isValid() ? Qt::PointingHandCursor : Qt::IBeamCursor);
        } else if (keyEvent->modifiers().testFlag(Qt::AltModifier)) {
            // alt + arrow up/down
            if (keyEvent->key() == Qt::Key_Up) {
                moveBlockUp();
                return true;
            } else if (keyEvent->key() == Qt::Key_Down) {
                moveBlockDown();
                return true;
            }
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {

        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if ((obj == viewport()) && (mouseEvent->button() == Qt::LeftButton)
            && (QGuiApplication::keyboardModifiers() == Qt::ExtraButton24)) {

            viewport()->setCursor(Qt::IBeamCursor);

            openLinkAtCursorPosition();

            return true;
        }
    } else if (event->type() == QEvent::KeyRelease) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Control) {
            viewport()->setCursor(Qt::IBeamCursor);
        }
    }

    return QTextEdit::eventFilter(obj, event);
}

QString CustomDocument::getMarkdownUrlAtPosition(const QString &text, int position)
{
    QString url;
    const QMap<QString, QString> urlMap = parseMarkdownUrlsFromText(text);
    QMap<QString, QString>::const_iterator i = urlMap.constBegin();
    for (; i != urlMap.constEnd(); ++i) {
        const QString &linkText = i.key();
        const QString &urlString = i.value();

        const int foundPositionStart = text.indexOf(linkText);

        if (foundPositionStart >= 0) {
            const int foundPositionEnd = foundPositionStart + linkText.size();
            if ((position >= foundPositionStart) && (position < foundPositionEnd)) {
                url = urlString;
                break;
            }
        }
    }

    return url;
}

QUrl CustomDocument::getUrlUnderMouse()
{
    auto pos = viewport()->mapFromGlobal(QCursor::pos());
    QTextCursor cursor = cursorForPosition(pos);
    const int cursorPosition = cursor.position();

    cursor.movePosition(QTextCursor::StartOfBlock);
    const int indexInBlock = cursorPosition - cursor.position();
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    return QUrl(getMarkdownUrlAtPosition(cursor.selectedText(), indexInBlock));
}

bool CustomDocument::openLinkAtCursorPosition()
{
    QUrl const url = getUrlUnderMouse();
    QString const urlString = url.toString();

    const bool isFileUrl = urlString.startsWith(QLatin1String("file://"));

    const bool isLegacyAttachmentUrl = urlString.startsWith(QLatin1String("file://attachments"));
    const bool isLocalFilePath = urlString.startsWith(QLatin1String("/"));

    const bool convertLocalFilepathsToURLs = true;

    if ((url.isValid() && isValidUrl(urlString)) || isFileUrl || isLocalFilePath
        || isLegacyAttachmentUrl) {

        if (_ignoredClickUrlSchemata.contains(url.scheme())) {
            qDebug() << __func__ << "ignored URL scheme:" << urlString;
            return false;
        }

        if (isFileUrl) {
            QString trimmed = urlString.mid(7);
            if (!QFile::exists(trimmed)) {
                qDebug() << __func__ << ": File does not exist:" << urlString;
                QMessageBox::warning(
                        nullptr, tr("File not found"),
                        tr("The file <strong>%1</strong> does not exist.").arg(trimmed));
                return false;
            }
        }

        if (isLocalFilePath && !QFile::exists(urlString)) {
            qDebug() << __func__ << ": File does not exist:" << urlString;
            QMessageBox::warning(nullptr, tr("File not found"),
                                 tr("The file <strong>%1</strong> does not exist.").arg(urlString));
            return false;
        }

        if (isLocalFilePath && convertLocalFilepathsToURLs) {
            openUrl(QString("file://") + urlString);
        } else {
            openUrl(urlString);
        }

        return true;
    }

    return false;
}

bool CustomDocument::isValidUrl(const QString &urlString)
{
    const QRegularExpressionMatch match = QRegularExpression(R"(^\w+:\/\/.+)").match(urlString);
    return match.hasMatch();
}

void CustomDocument::openUrl(const QString &urlString)
{
    qDebug() << "CustomDocument " << __func__ << " - 'urlString': " << urlString;

    QDesktopServices::openUrl(QUrl(urlString));
}

QMap<QString, QString> CustomDocument::parseMarkdownUrlsFromText(const QString &text)
{
    QMap<QString, QString> urlMap;
    QRegularExpression regex;
    QRegularExpressionMatchIterator iterator;
    regex = QRegularExpression(QStringLiteral("(<(.+?)>)"));
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString url = match.captured(2);
        urlMap[linkText] = url;
    }

    regex = QRegularExpression(R"((\[.*?\]\((.+?)\)))");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString url = match.captured(2);
        urlMap[linkText] = url;
    }

    regex = QRegularExpression(R"(\b\w+?:\/\/[^\s]+[^\s>\)])");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString url = match.captured(0);
        urlMap[url] = url;
    }

    regex = QRegularExpression(R"(\bwww\.[^\s]+\.[^\s]+\b)");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString url = match.captured(0);
        urlMap[url] = QStringLiteral("http://") + url;
    }

    regex = QRegularExpression(R"((\[.*?\]\[(.+?)\]))");
    iterator = regex.globalMatch(text);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        QString linkText = match.captured(1);
        QString referenceId = match.captured(2);

        QRegularExpression refRegExp(QStringLiteral("\\[") + QRegularExpression::escape(referenceId)
                                     + QStringLiteral("\\]: (.+)"));
        QRegularExpressionMatch urlMatch = refRegExp.match(toPlainText());

        if (urlMatch.hasMatch()) {
            QString url = urlMatch.captured(1);
            urlMap[linkText] = url;
        }
    }

    return urlMap;
}

void CustomDocument::moveBlockUp()
{
    QTextCursor cursor = textCursor();

    if (cursor.blockNumber() > 0) {
        QString currentBlock = cursor.block().text();
        const int currentHorizontalPosition = cursor.positionInBlock();

        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.deletePreviousChar();

        if (!cursor.movePosition(QTextCursor::StartOfBlock)) {
        }
        cursor.insertText(currentBlock);
        cursor.insertBlock();
        if (!cursor.movePosition(QTextCursor::PreviousBlock)) {
            qDebug() << "Could not move to previous block";
        }
        const int startPosition = cursor.position();
        cursor.setPosition(startPosition + currentHorizontalPosition);

        setTextCursor(cursor);
    }
}

void CustomDocument::moveBlockDown()
{
    QTextCursor cursor = textCursor();

    QString currentBlock = cursor.block().text();
    const int currentHorizontalPosition = cursor.positionInBlock();

    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.deleteChar();

    if (!cursor.movePosition(QTextCursor::EndOfBlock)) {
    }
    cursor.insertBlock();
    cursor.insertText(currentBlock);
    if (!cursor.movePosition(QTextCursor::StartOfBlock)) {
        qDebug() << "Could not move to start of next block";
    }
    const int startPosition = cursor.position();
    cursor.setPosition(startPosition + currentHorizontalPosition);

    setTextCursor(cursor);
}
