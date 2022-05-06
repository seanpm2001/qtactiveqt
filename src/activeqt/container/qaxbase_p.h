/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the ActiveQt framework of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#ifndef QAXBASE_P_H
#define QAXBASE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>
#include <QtCore/qmap.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/quuid.h>
#include <QtCore/qt_windows.h>

struct tagEXCEPINFO;

QT_BEGIN_NAMESPACE

class QAxBase;
class QAxEventSink;

class QAxBasePrivate
{
    Q_DISABLE_COPY_MOVE(QAxBasePrivate)
public:
    using UuidEventSinkHash = QHash<QUuid, QAxEventSink*>;

    explicit QAxBasePrivate();
    ~QAxBasePrivate();

    virtual QObject *qObject() const = 0;
    virtual const char *className() const = 0;
    virtual const QMetaObject *fallbackMetaObject() const = 0;
    virtual const QMetaObject *parentMetaObject() const = 0;

    virtual void emitException(int code, const QString &source, const QString &desc,
                               const QString &help) = 0;
    virtual void emitPropertyChanged(const QString &name) = 0;
    virtual void emitSignal(const QString &name, int argc, void *argv) = 0;

    int qtMetaCall(QMetaObject::Call, int, void **);
    static int qtStaticMetaCall(QAxBase *, QMetaObject::Call, int, void **);

    IDispatch *dispatch() const
    {
        if (disp)
            return disp;

        if (ptr)
            ptr->QueryInterface(IID_IDispatch, reinterpret_cast<void **>(&disp));
        return disp;
    }

    bool checkHRESULT(long hres, tagEXCEPINFO *exc, const QString &name, uint argerr);
    void handleException(tagEXCEPINFO *exc, const QString &name);

    static QVariant VARIANTToQVariant(const VARIANT &arg, const QByteArray &typeName, uint type = 0);

    QAxBase *q = nullptr;
    QString ctrl;
    UuidEventSinkHash eventSink;
    uint useEventSink       :1;
    uint useMetaObject      :1;
    uint useClassInfo       :1;
    uint cachedMetaObject   :1;
    uint initialized        :1;
    uint tryCache           :1;
    unsigned long classContext = CLSCTX_SERVER;

    IUnknown *ptr = nullptr;
    mutable IDispatch *disp = nullptr;

    QMap<QByteArray, bool> propWritable;

    QMetaObject *metaObject()
    {
        return metaobj;
    }

    mutable QMap<QString, LONG> verbs;

    QMetaObject *metaobj = nullptr;
};

QT_END_NAMESPACE

#endif // QAXBASE_P_H
