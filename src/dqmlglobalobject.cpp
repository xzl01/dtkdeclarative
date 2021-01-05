/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dqmlglobalobject.h"
#include "private/dqmlglobalobject_p.h"

#include <DObjectPrivate>
#include <DObject>
#include <DGuiApplicationHelper>
#include <DFontManager>

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

DQMLGlobalObjectPrivate::DQMLGlobalObjectPrivate(DQMLGlobalObject *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
}

void DQMLGlobalObjectPrivate::ensurePalette()
{
    if (paletteInit)
        return;

    paletteInit = true;
    palette = DGuiApplicationHelper::instance()->applicationPalette();
    inactivePalette = palette;

    QObject::connect(DGuiApplicationHelper::instance(), SIGNAL(applicationPaletteChanged()), q_func(), SLOT(_q_onPaletteChanged()));
}

void DQMLGlobalObjectPrivate::_q_onPaletteChanged()
{
    palette = DGuiApplicationHelper::instance()->applicationPalette();
    inactivePalette = palette;

    for (int i = 0; i < QPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);
        const QBrush &brush = palette.brush(QPalette::Disabled, role);
        inactivePalette.setBrush(QPalette::Active, role, brush);
    }

    for (int i = 0; i < DPalette::NColorTypes; ++i) {
        DPalette::ColorType type = static_cast<DPalette::ColorType>(i);
        const QBrush &brush = palette.brush(QPalette::Disabled, type);
        inactivePalette.setBrush(QPalette::Active, type, brush);
    }

    Q_EMIT q_func()->paletteChanged();
    Q_EMIT q_func()->inactivePaletteChanged();
}

DQMLGlobalObject::DQMLGlobalObject(QObject *parent)
    : QObject(parent)
    , DTK_CORE_NAMESPACE::DObject(*new DQMLGlobalObjectPrivate(this))
{
    auto pHelper = DWindowManagerHelper::instance();
    connect(pHelper, &DWindowManagerHelper::hasBlurWindowChanged, this, &DQMLGlobalObject::hasBlurWindowChanged);
    connect(pHelper, &DWindowManagerHelper::hasCompositeChanged, this, &DQMLGlobalObject::hasCompositeChanged);
    connect(pHelper, &DWindowManagerHelper::hasNoTitlebarChanged, this, &DQMLGlobalObject::hasNoTitlebarChanged);
}

DQMLGlobalObject::~DQMLGlobalObject()
{
}

bool DQMLGlobalObject::hasBlurWindow() const
{
    return DWindowManagerHelper::instance()->hasBlurWindow();
}

bool DQMLGlobalObject::hasComposite() const
{
    return DWindowManagerHelper::instance()->hasComposite();
}

bool DQMLGlobalObject::hasNoTitlebar() const
{
    return DWindowManagerHelper::instance()->hasNoTitlebar();
}

QString DQMLGlobalObject::windowManagerNameString() const
{
    return DWindowManagerHelper::instance()->windowManagerNameString();
}

DWindowManagerHelper::WMName DQMLGlobalObject::windowManagerName() const
{
    return DWindowManagerHelper::instance()->windowManagerName();
}

DPlatformThemeProxy *DQMLGlobalObject::platformTheme() const
{
    D_DC(DQMLGlobalObject);

    if (!d->platformTheme) {
        d->platformTheme = new DPlatformThemeProxy(DGuiApplicationHelper::instance()->applicationTheme(), const_cast<DQMLGlobalObject *>(this));
    }

    return d->platformTheme;
}

DFontManager *DQMLGlobalObject::fontManager() const
{
    return DFontManager::instance();
}

DPalette DQMLGlobalObject::palette() const
{
    D_DC(DQMLGlobalObject);
    const_cast<DQMLGlobalObjectPrivate*>(d)->ensurePalette();
    return d->palette;
}

DPalette DQMLGlobalObject::inactivePalette() const
{
    D_DC(DQMLGlobalObject);
    const_cast<DQMLGlobalObjectPrivate*>(d)->ensurePalette();
    return d->inactivePalette;
}

DQUICK_END_NAMESPACE

#include "moc_dqmlglobalobject.cpp"