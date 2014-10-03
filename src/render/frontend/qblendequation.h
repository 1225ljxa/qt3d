/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3D_QBLENDEQUATION_H
#define QT3D_QBLENDEQUATION_H

#include <Qt3DRenderer/qrenderstate.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QBlendEquationPrivate;

class QT3DRENDERERSHARED_EXPORT QBlendEquation : public QRenderState
{
    Q_OBJECT
    Q_ENUMS(BlendMode)
    Q_PROPERTY(BlendMode mode READ mode WRITE setMode NOTIFY modeChanged)
public:

    enum BlendMode
    {
        FuncAdd = 0x8006,
        FuncSubstract = 0x800A,
        FuncReverseSubstract = 0x800B,
        Min = 0x8007,
        Max = 0x8008
    };

    explicit QBlendEquation(QNode *parent = 0);

    BlendMode mode() const;
    void setMode(BlendMode mode);

Q_SIGNALS:
    void modeChanged();

private:
    Q_DECLARE_PRIVATE(QBlendEquation)
    QNode *doClone() const Q_DECL_OVERRIDE;
};

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_QBLENDEQUATION_H
